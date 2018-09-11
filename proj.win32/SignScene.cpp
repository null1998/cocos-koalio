#include "SignScene.h"
#include "FirstScene.h"
#include "HelloWorldScene.h"
USING_NS_CC;


Scene* SignScene::createScene()
{
	return SignScene::create();
}

bool SignScene::init() 
{
	if (!Scene::init())
	{
		return false;
	}
	//屏幕尺寸
	auto visibleSize = Director::getInstance()->getVisibleSize();

	//创建词典类实例，将xml文件加载到词典中
	auto *chnStrings = Dictionary::createWithContentsOfFile("CHN_Strings.xml");
	//通过xml文件中的key获取value
	const char *str1 = ((String*)chnStrings->objectForKey("string1"))->getCString();

	auto sign = Label::createWithSystemFont(str1, "Arial", 60);

	sign->setPosition(Vec2(visibleSize.width / 2, 500.f));

	sign->enableGlow(Color4B::YELLOW);

	this->addChild(sign, 1);

	//playMenu

	const char *str2 = ((String*)chnStrings->objectForKey("string2"))->getCString();

	auto playLabel = Label::createWithSystemFont(str2, "Arial", 30);

	playLabel->enableGlow(Color4B::YELLOW);

	auto playMenuItem = MenuItemLabel::create(playLabel, [&](Ref* sender) {
		auto helloWorldScene = HelloWorld::createScene();
		Director::getInstance()->replaceScene(helloWorldScene);
	});

	auto playMenu = Menu::create(playMenuItem, NULL);

	playMenu->setPosition(Vec2(visibleSize.width / 2, 150.f));

	this->addChild(playMenu);


	return true;
}