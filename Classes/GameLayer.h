#ifndef _GAMELAYER_H_
#define _GAMELAYER_H_
#include "cocos2d.h"

class GameLayer : public cocos2d::CCLayer {


public:
	
	static cocos2d::CCLayer* createInstance();
	//void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	CREATE_FUNC(GameLayer);
};
#endif // !_GAMELAYER_H_
#pragma once
