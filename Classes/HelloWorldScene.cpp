/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "FirstScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;


Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
	//ÆÁÄ»³ß´ç
    auto visibleSize = Director::getInstance()->getVisibleSize();
	//×ø±êÔ­µã
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto label = Label::createWithTTF("Super koalio", "fonts/Marker Felt.ttf", 80);

    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        
        label->setPosition(Vec2(visibleSize.width / 2 + origin.x,450.f));

		label->enableGlow(Color4B::YELLOW);
        
        this->addChild(label, 1);
    }

	//playMenu
	auto playLabel = Label::createWithTTF("play", "fonts/Marker Felt.ttf", 40);

	playLabel->enableGlow(Color4B::YELLOW);

	if (label == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}

	auto playMenuItem = MenuItemLabel::create(playLabel, [&](Ref* sender) {
		auto firstScene = FirstScene::createScene();
		Director::getInstance()->replaceScene(firstScene);
	});

	auto playMenu = Menu::create(playMenuItem, NULL);

	playMenu->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	this->addChild(playMenu);

	//helpMenu
	auto helpLabel = Label::createWithTTF("help", "fonts/Marker Felt.ttf", 40);

	helpLabel->enableGlow(Color4B::YELLOW);

	if (label == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}

	auto helpMenuItem = MenuItemLabel::create(helpLabel, [&](Ref* sender) {
		
		//code
	});

	auto helpMenu = Menu::create(helpMenuItem, NULL);

	helpMenu->setPosition(Vec2(visibleSize.width / 2 + origin.x, 260.f));

	this->addChild(helpMenu);


	//quitMenu
	auto quitLabel = Label::createWithTTF("quit", "fonts/Marker Felt.ttf", 40);

	quitLabel->enableGlow(Color4B::YELLOW);

	if (label == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}

	auto quitMenuItem = MenuItemLabel::create(quitLabel, [&](Ref* sender) {

		Director::getInstance()->end();

	});

	auto quitMenu = Menu::create(quitMenuItem, NULL);

	quitMenu->setPosition(Vec2(visibleSize.width / 2 + origin.x, 160.f));

	this->addChild(quitMenu);

    return true;
}

