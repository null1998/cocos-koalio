#include "FirstScene.h"
#include"Player.h"
#include "GameLayer.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include"Tools.h"
#include"TileRect.h"
#include"HelloWorldScene.h"

USING_NS_CC;

using namespace ui;
using namespace CocosDenshion;



Scene* FirstScene::createScene()
{
	return FirstScene::create();
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}



bool FirstScene::init()
{
	if (!Scene::init()) {
		return false;
	}
	isGameOver = false;
	//获取整个手机可视屏幕尺寸
	auto visibleSize = Director::getInstance()->getVisibleSize();

	//获取手机可视屏原点的坐标
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//music
	SimpleAudioEngine* audio = SimpleAudioEngine::sharedEngine();

	audio->playBackgroundMusic("level1.mp3");

	CCLayerColor* blueSky = CCLayerColor::create(ccc4(100,100,250,255));

	addChild(blueSky);


	map = CCTMXTiledMap::create("level1.tmx");

	auto layer = GameLayer::createInstance();

	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = [](cocos2d::Touch* touch, cocos2d::Event* event) {

		CCPoint touchLocation = touch->getLocation();

		if (touchLocation.x > 240&&Player::getInstance()->_onGround) {

			Player::getInstance()->_mightAsWellJump = true;

			Player::getInstance()->velocity = ccp(Player::getInstance()->velocity.x, 230.f);

			Player::getInstance()->_onGround = false;

			SimpleAudioEngine::sharedEngine()->playEffect("jump.wav");

		}
		if(touchLocation.x <= 240){

			Player::getInstance()->_forwardMarch = true;
			
		}
		
		return true;
	};

	listener->onTouchEnded = [](cocos2d::Touch*touch, cocos2d::Event*event) {

		Player::getInstance()->_mightAsWellJump = false;

		Player::getInstance()->_forwardMarch = false;

		return true;
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, layer);
	
	 layer->setTouchEnabled(true);

	 this->addChild(layer);

	 layer->addChild(map);

	 map->addChild(Player::getInstance());
	
     walls = map->layerNamed("walls");

	 hazards = map->layerNamed("hazards");

	 this->scheduleUpdate();
	
	 return true;
}
void FirstScene::update(float delta)
{
	Player::getInstance()->update(delta);

	if (isGameOver) {

	    return;

    }
	this->checkHazardCollisions(Player::getInstance());

	this->getWin();

	this->checkForAndResolveCollisions(Player::getInstance());

	this->setViewpointCenter(Player::getInstance()->getPosition());
	
}

CCPoint FirstScene::tileCoordForPosition(cocos2d::CCPoint position)
{
	float x = floor(position.x / map->getTileSize().width); //位置x值/地图一块tile的宽度即可得到x坐标

	float levelHeightInPixels = map->getMapSize().height * map->getTileSize().height; //地图的实际高度

	float y = floor((levelHeightInPixels - position.y) / map->getTileSize().height);  //瓦片地图的原点在左上角，与cocos2d-x是不同的(2dx原点在左下角)

	return ccp(x, y);
}

TileRect FirstScene::tileRectFromTileCoords(cocos2d::CCPoint tileCoords)
{
	float levelHeightInPixels = map->getMapSize().height * map->getTileSize().height; //地图的实际高度

    //把地图坐标tileCoords转化为实际游戏中的坐标
	CCPoint origin = ccp(tileCoords.x * map->getTileSize().width, levelHeightInPixels - ((tileCoords.y + 1)*map->getTileSize().height));

	return TileRect(origin.x, origin.y, map->getTileSize().width, map->getTileSize().height);
}

TileRect* FirstScene::getSurroundingTilesAtPosition(cocos2d::CCPoint playerPosition, cocos2d::CCTMXLayer* layer)
{
	int indexes[8] = { 7, 1, 5, 3, 8, 6, 2, 0 };//优先下上右左的顺序

	TileRect *bounds = new TileRect[8];
	
	CCPoint plPos = this->tileCoordForPosition(playerPosition); //返回精灵的瓦片地图坐标

	auto layerSize = layer->getLayerSize();

	//取出环绕在精灵四周的8个tile，从上至下每行三个
	for (int i = 0; i<8; i++)
	{
		auto index = indexes[i];

		int c = index % 3;   //相当于当前i所处的列

		int r = (int)(index / 3); //相当于当前i所处的行

		CCPoint tilePos = ccp(plPos.x + (c - 1), plPos.y + (r - 1));

		//判断wall在此处是否有tile
		int tgid = layer->tileGIDAt(tilePos);

		if (tilePos.x < 0 || tilePos.x >= layerSize.width) {

			bounds[i] = this->tileRectFromTileCoords(tilePos);

			bounds[i].isBarrier = true;

			bounds[i].tgid = tgid;

			continue;
		}
		if (tilePos.y < 0 || tilePos.y >= layerSize.height) {

			bounds[i] = this->tileRectFromTileCoords(tilePos);

			bounds[i].isBarrier = true;

			bounds[i].tgid = tgid;

			continue;
		} 

		bounds[i] = this->tileRectFromTileCoords(tilePos);

		bounds[i].isBarrier = false;

		bounds[i].tgid = tgid;
	}

	TileRect* ptr = bounds;
	
	return ptr;
}

void FirstScene::checkForAndResolveCollisions(Player* player)
{
	//TileRect指针数组
	TileRect* tilesArray = this->getSurroundingTilesAtPosition(player->_desiredPosition, walls);//7, 1, 5, 3, 8, 6, 2, 0

	for (int i = 0; i < 8; i++) {

		TileRect tileRect = tilesArray[i];//周围八个tile的碰撞矩形

		if (tilesArray[i].y <=0){

			this->gameOver(false);

		}

		if (tileRect.tgid) {
			auto isTileOnRight = i == 2 || i == 4 || i == 6;
			auto isTileOnLeft = i == 3 || i == 5 || i == 7;
			auto isTileOnTop = i == 1 || i == 6 || i == 7;
			auto isTileOnBottom = i == 0 || i == 4 || i == 5;

			auto isMoveLeft = Player::getInstance()->velocity.x <= 0;
			auto isMoveRight = Player::getInstance()->velocity.x >= 0;

			auto isFall = Player::getInstance()->velocity.y <= 0;
			auto isJump = Player::getInstance()->velocity.y >= 0;
			CCRect playerRect = player->collisionBoundingBox();  //将要到达的地方的玩家碰撞矩形

			//是否碰撞
			auto isCollideWithPlayer = tileRect.intersectsRect(playerRect);
			if (!isCollideWithPlayer) {
				continue;
			}
			cocos2d::CCRect intersection = Tools::getIntersectsRect(playerRect, tileRect);// 取得相撞矩形

			
			if (intersection.size.height > 0) {
				if (isTileOnRight && isMoveRight) {
					//右边的tile2 5 8
					player->_desiredPosition = ccp(player->_desiredPosition.x - intersection.size.width, player->_desiredPosition.y);

					player->velocity = ccp(0.f, player->velocity.y);

					char chInput[512];
					sprintf(chInput, "right tgid:%d\n", tileRect.tgid);
					const wchar_t* w = Tools::GetWC(chInput);
					OutputDebugString(w);

				}
				else if (isTileOnLeft && isMoveLeft) {
					//左边的tile0 3 6
					player->_desiredPosition = ccp(player->_desiredPosition.x + intersection.size.width, player->_desiredPosition.y);

					player->velocity = ccp(0.f, player->velocity.y);

					char chInput[512];
					sprintf(chInput, "left tgid:%d\n", tileRect.tgid);
					const wchar_t* w = Tools::GetWC(chInput);
					OutputDebugString(w);
				}
			}
			
			if (intersection.size.width > 0) {
				if (isTileOnBottom && isFall) {
					//tile在koala正下方 考拉落到了tile上 6 7 8
					player->_desiredPosition = ccp(player->_desiredPosition.x, player->_desiredPosition.y + intersection.size.height);
				
					player->velocity = ccp(player->velocity.x, 0.f);

					player->_onGround = true;

					char chInput[512];
					sprintf(chInput, "fall tgid:%d\n", tileRect.tgid);
					const wchar_t* w = Tools::GetWC(chInput);
					OutputDebugString(w);
				}
				else if (isTileOnTop && isJump) {
					//考拉头顶到tile，要让主角向下移0 1 2
					player->_desiredPosition = ccp(player->_desiredPosition.x, player->_desiredPosition.y - intersection.size.height);
				
					player->velocity = ccp(player->velocity.x, 0.f);

					char chInput[512];
					sprintf(chInput, "jump tgid:%d\n", tileRect.tgid);
					const wchar_t* w = Tools::GetWC(chInput);
					OutputDebugString(w);
				}
			}

		}

	}
	player->setPosition(player->_desiredPosition); //把主角位置设定到它期望去的地方*/
}

void FirstScene::checkHazardCollisions(Player* player) {

	TileRect* hazardTiles = this->getSurroundingTilesAtPosition(player->_desiredPosition, hazards);

	for (int i = 0; i < 8; i++) {

		TileRect hazardRect = hazardTiles[i];

		if (hazardRect.tgid) {

			CCRect playerRect = player->collisionBoundingBox();  //将要到达的地方的玩家碰撞矩形

			if (hazardRect.intersectsRect(playerRect)) {

				this->gameOver(false);

			}
		}
	}
}

void FirstScene::gameOver(bool isWon) {

	isGameOver = true;

	CCString* result;
	if (isWon)
	{
		result = CCString::create("You Won!");
	}
	else {

		result = CCString::create("You Died!");

		SimpleAudioEngine::sharedEngine()->playEffect("hurt.wav");
	}
	CCLabelTTF* label = CCLabelTTF::create(result->getCString(), "Marker Felt", 40);

	label->setPosition(ccp(240, 200));

	this->addChild(label);

	auto replayLabel = Label::createWithTTF("replay", "fonts/Marker Felt.ttf", 24);

	replayLabel->enableGlow(Color4B::YELLOW);

	if (replayLabel == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}

	auto replayMenuItem = MenuItemLabel::create(replayLabel, [=](Ref* sender) {

		auto scene = HelloWorld::createScene();

		Director::getInstance()->replaceScene(scene);

		restart();

	});

	auto replayMenu = Menu::create(replayMenuItem, NULL);

	replayMenu->setPosition(Vec2(240, -100));

	this->addChild(replayMenu);

	CCMoveBy *slideIn = CCMoveBy::create(1.f, ccp(0, 250));

	replayMenu->runAction(slideIn);


}

void FirstScene::restart() {

	Player::getInstance()->_onGround=true;

	Player::getInstance()->_forwardMarch=false;  //是否向前走

	Player::getInstance()->_mightAsWellJump = false;

	Player::getInstance()->velocity = ccp(0.f, 0.f);

	Player::getInstance()->_desiredPosition = (ccp(50.f, 50.f));

	Player::getInstance()->setPosition(ccp(50.f,50.f));
}

void FirstScene::getWin()
{
	if (Player::getInstance()->getPositionX()>3300.f)
	{
		this->gameOver(true);
	}
}

void FirstScene::setViewpointCenter(cocos2d::CCPoint pos)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	//限定角色不能超过半屏
	int x = MAX(pos.x, winSize.width / 2);
	int y = MAX(pos.y, winSize.height / 2);

	//限定角色不能跑出屏幕
	x = MIN(x, (map->getMapSize().width * map->getTileSize().width) - winSize.width / 2);
	y = MIN(y, (map->getMapSize().height * map->getTileSize().height) - winSize.height / 2);

	CCPoint actualPosition = ccp(x, y);

	CCPoint centerOfView = ccp(winSize.width / 2, winSize.height / 2);

	CCPoint viewPoint = ccpSub(centerOfView, actualPosition);

	//设定一下地图的位置
	map->setPosition(viewPoint);
}



	