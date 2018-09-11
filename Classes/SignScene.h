#ifndef _SIGNSCENE_H_
#define _SIGNSCENE_H_
#include "cocos2d.h";

class SignScene :public cocos2d::Scene {
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(SignScene);
};
#endif // !_SIGNSCENE_H_

