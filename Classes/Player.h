#ifndef _PLAYER_H_
#define _PLAYER_H_
#define kVelocityYMax 500
#include "cocos2d.h"

enum ANIMATION_STATUS{
	STAND, // վ 
	JUMP, // ��
	RUN, // ��
	FALL // ����
};

class Player : public cocos2d::CCSprite
{
public:
	cocos2d::Vec2 velocity;

	//cocos2d::CCPoint _nowPosition;

	cocos2d::CCPoint _desiredPosition;
	
	ANIMATION_STATUS _status;

	bool _onGround;

	bool _forwardMarch;  //�Ƿ���ǰ��

	bool _backMarch;

	bool _mightAsWellJump; //������Ծ��
public:
	Player(void);

	~Player(void);

	virtual bool setStatus(ANIMATION_STATUS status);

	static Player* getInstance();

	void update(float delta);
	
	cocos2d::CCRect collisionBoundingBox();
private:
	static Player* mInstance;

};
#endif // !_FIRSTSCENE_H_
#pragma once
