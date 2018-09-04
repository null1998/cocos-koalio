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
	//��ȡ�����ֻ�������Ļ�ߴ�
	auto visibleSize = Director::getInstance()->getVisibleSize();

	//��ȡ�ֻ�������ԭ�������
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//music
	SimpleAudioEngine* audio = SimpleAudioEngine::sharedEngine();

	//audio->playBackgroundMusic("level1.mp3");

	audio->playBackgroundMusic("Mario_BGM.mp3");

	CCLayerColor* blueSky = CCLayerColor::create(ccc4(100,100,250,255));

	addChild(blueSky);


	//map = CCTMXTiledMap::create("level1.tmx");

	map= CCTMXTiledMap::create("untitled.tmx");

	auto layer = GameLayer::createInstance();

	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = [](cocos2d::Touch* touch, cocos2d::Event* event) {

		CCPoint touchLocation = touch->getLocation();

		if (touchLocation.x > 640&&Player::getInstance()->_onGround) {

			SimpleAudioEngine::sharedEngine()->playEffect("Mario_jump.mp3");

			Player::getInstance()->_mightAsWellJump = true;

			Player::getInstance()->velocity = ccp(Player::getInstance()->velocity.x, 350.f);

			Player::getInstance()->_onGround = false;

			//SimpleAudioEngine::sharedEngine()->playEffect("jump.wav");


		}
		if(touchLocation.x <= 640){

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

	// hazards = map->layerNamed("hazards");

	 this->scheduleUpdate();
	
	 return true;
}
void FirstScene::update(float delta)
{
	Player::getInstance()->update(delta);

	if (isGameOver) {

	    return;

    }

	this->getWin();

	this->checkForAndResolveCollisions(Player::getInstance());

	//����Ƿ�������
	//this->checkHazardCollisions(Player::getInstance());

	this->setViewpointCenter(Player::getInstance()->getPosition());

}

CCPoint FirstScene::tileCoordForPosition(cocos2d::CCPoint position)
{
	float x = floor(position.x / map->getTileSize().width); //λ��xֵ/��ͼһ��tile�Ŀ�ȼ��ɵõ�x����

	float levelHeightInPixels = map->getMapSize().height * map->getTileSize().height; //��ͼ��ʵ�ʸ߶�

	float y = floor((levelHeightInPixels - position.y) / map->getTileSize().height);  //��Ƭ��ͼ��ԭ�������Ͻǣ���cocos2d-x�ǲ�ͬ��(2dxԭ�������½�)

	return ccp(x, y);
}

TileRect FirstScene::tileRectFromTileCoords(cocos2d::CCPoint tileCoords)
{
	float levelHeightInPixels = map->getMapSize().height * map->getTileSize().height; //��ͼ��ʵ�ʸ߶�

    //�ѵ�ͼ����tileCoordsת��Ϊʵ����Ϸ�е�����
	CCPoint origin = ccp(tileCoords.x * map->getTileSize().width, levelHeightInPixels - ((tileCoords.y + 1)*map->getTileSize().height));

	return TileRect(origin.x, origin.y, map->getTileSize().width, map->getTileSize().height);
}

TileRect* FirstScene::getSurroundingTilesAtPosition(cocos2d::CCPoint playerPosition, cocos2d::CCTMXLayer* layer)
{
	int indexes[8] = { 7, 1, 5, 3, 8, 6, 2, 0 };//�������������˳��

	TileRect *bounds = new TileRect[8];
	
	CCPoint plPos = this->tileCoordForPosition(playerPosition); //���ؾ������Ƭ��ͼ����

	auto layerSize = layer->getLayerSize();

	//ȡ�������ھ������ܵ�8��tile����������ÿ������
	for (int i = 0; i<8; i++)
	{
		auto index = indexes[i];

		int c = index % 3;   //�൱�ڵ�ǰi��������

		int r = (int)(index / 3); //�൱�ڵ�ǰi��������

		CCPoint tilePos = ccp(plPos.x + (c - 1), plPos.y + (r - 1));//�˸���Ƭ���Ե���Ƭ����

		//�ж�wall�ڴ˴��Ƿ���tile����û����Ϊ0
		int tgid = layer->tileGIDAt(tilePos);

		if (tilePos.x < 0 || tilePos.x >= layerSize.width) {

			bounds[i] = this->tileRectFromTileCoords(tilePos);
			//������ͼ��ʶ
			bounds[i].isBarrier = true;

			bounds[i].tgid = tgid;

			continue;
		}
		if (tilePos.y < 0 || tilePos.y >= layerSize.height) {

			bounds[i] = this->tileRectFromTileCoords(tilePos);
			//������ͼ��ʶ
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
	//TileRectָ������
	TileRect* tilesArray = this->getSurroundingTilesAtPosition(player->_desiredPosition, walls);//7, 1, 5, 3, 8, 6, 2, 0

	for (int i = 0; i < 8; i++) {

		TileRect tileRect = tilesArray[i];//��Χ�˸�tile����ײ����

		if (tilesArray[i].y <=0){

			this->gameOver(false);

		}
		//����ƬΪ�����������Ƿ���ײ
		if (tileRect.tgid) {
			auto isTileOnRight = i == 2 || i == 4 || i == 6;//�Ҳ�һ������
			auto isTileOnLeft = i == 3 || i == 5 || i == 7;
			auto isTileOnTop = i == 1 || i == 6 || i == 7;
			auto isTileOnBottom = i == 0 || i == 4 || i == 5;

			auto isMoveLeft = Player::getInstance()->velocity.x <= 0;
			auto isMoveRight = Player::getInstance()->velocity.x >= 0;

			auto isFall = Player::getInstance()->velocity.y <= 0;
			auto isJump = Player::getInstance()->velocity.y >= 0;
			CCRect playerRect = player->collisionBoundingBox();  //��Ҫ����ĵط��������ײ����

			//�Ƿ���ײ������ײ�����������ײ������δ�С
			auto isCollideWithPlayer = tileRect.intersectsRect(playerRect);
			if (!isCollideWithPlayer) {
				continue;
			}
			cocos2d::CCRect intersection = Tools::getIntersectsRect(playerRect, tileRect);// ȡ����ײ����

		
			if (intersection.size.height > 0) {
				if (isTileOnRight && isMoveRight) {
					//�ұߵ�tile2 5 8
					player->_desiredPosition = ccp(player->_desiredPosition.x - intersection.size.width, player->_desiredPosition.y);

					player->velocity = ccp(0.f, player->velocity.y);

					//char chInput[512];
					//sprintf(chInput, "right tgid:%d\n", tileRect.tgid);
					//const wchar_t* w = Tools::GetWC(chInput);
					//OutputDebugString(w);

				}
				else if (isTileOnLeft && isMoveLeft) {
					//��ߵ�tile0 3 6
					player->_desiredPosition = ccp(player->_desiredPosition.x + intersection.size.width, player->_desiredPosition.y);

					player->velocity = ccp(0.f, player->velocity.y);

					//char chInput[512];
					//sprintf(chInput, "left tgid:%d\n", tileRect.tgid);
					//const wchar_t* w = Tools::GetWC(chInput);
					//OutputDebugString(w);
				}
			}
			
			if (intersection.size.width > 0) {
				if (isTileOnBottom && isFall) {
					//tile��koala���·� �����䵽��tile�� 6 7 8
					player->_desiredPosition = ccp(player->_desiredPosition.x, player->_desiredPosition.y + intersection.size.height);
				
					player->velocity = ccp(player->velocity.x, 0.f);

					player->_onGround = true;

					//char chInput[512];
					//sprintf(chInput, "fall tgid:%d\n", tileRect.tgid);
					//const wchar_t* w = Tools::GetWC(chInput);
					//OutputDebugString(w);
				}
				else if (isTileOnTop && isJump) {
					//����ͷ����tile��Ҫ������������0 1 2
					player->_desiredPosition = ccp(player->_desiredPosition.x, player->_desiredPosition.y - intersection.size.height);
				
					player->velocity = ccp(player->velocity.x, 0.f);

					//char chInput[512];
					//sprintf(chInput, "jump tgid:%d\n", tileRect.tgid);
					//const wchar_t* w = Tools::GetWC(chInput);
					//OutputDebugString(w);
				}
			}

		}

	}
	player->setPosition(player->_desiredPosition); //������λ���趨��������ȥ�ĵط�*/
}

void FirstScene::checkHazardCollisions(Player* player) {

	TileRect* hazardTiles = this->getSurroundingTilesAtPosition(player->_desiredPosition, hazards);

	for (int i = 0; i < 8; i++) {

		TileRect hazardRect = hazardTiles[i];

		if (hazardRect.tgid) {

			CCRect playerRect = player->collisionBoundingBox();  //��Ҫ����ĵط��������ײ����

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

		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();

		SimpleAudioEngine::sharedEngine()->playEffect("Mario_died.mp3");
	}
	CCLabelTTF* label = CCLabelTTF::create(result->getCString(), "Marker Felt", 80);

	label->setPosition(Vec2(640.f,450.f));

	this->addChild(label);

	auto replayLabel = Label::createWithTTF("replay", "fonts/Marker Felt.ttf", 40);

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

	replayMenu->setPosition(Vec2(640.f,0.f));

	this->addChild(replayMenu);

	CCMoveBy *slideIn = CCMoveBy::create(1.f, ccp(0, 360.f));

	replayMenu->runAction(slideIn);


}

void FirstScene::restart() {

	Player::getInstance()->_onGround=true;

	Player::getInstance()->_forwardMarch=false;  //�Ƿ���ǰ��

	Player::getInstance()->_mightAsWellJump = false;

	Player::getInstance()->velocity = ccp(0.f, 0.f);

	Player::getInstance()->_desiredPosition = (ccp(250.f, 250.f));

	Player::getInstance()->setPosition(ccp(250.f,250.f));
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

	//�޶���ɫ���ܳ�������
	int x = MAX(pos.x, winSize.width / 2);
	int y = MAX(pos.y, winSize.height / 2);

	//�޶���ɫ�����ܳ���Ļ
	x = MIN(x, (map->getMapSize().width * map->getTileSize().width) - winSize.width / 2);
	y = MIN(y, (map->getMapSize().height * map->getTileSize().height) - winSize.height / 2);

	CCPoint actualPosition = ccp(x, y);

	CCPoint centerOfView = ccp(winSize.width / 2, winSize.height / 2);

	CCPoint viewPoint = ccpSub(centerOfView, actualPosition);

	//�趨һ�µ�ͼ��λ��
	map->setPosition(viewPoint);
}



	