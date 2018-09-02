#include"Player.h"
#include "Tools.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

Player::Player(void) 
{
	_onGround = false;
	_forwardMarch = false;
	_mightAsWellJump = false;
	_status = ANIMATION_STATUS::STAND;
	//this->autorelease();
};
Player::~Player(void) {};



bool Player::setStatus(ANIMATION_STATUS status)
{
	CCAssert(status == NULL, "Invalid status for Player");

	bool bRet;
	switch (status) {
	case JUMP:
		break;
	case RUN:
		break;
	case STAND:
	    bRet = CCSprite::initWithFile("koalio_stand.png");
		velocity = ccp(0.f, 0.f);
		break;
	case FALL:
		break;
	default:
		break;
	}
	return false;
}
Player* Player::getInstance() {
	if (mInstance == nullptr) {
		mInstance = new Player();
	}
	return mInstance;
}

void Player::update(float delta) {

	CCPoint gravity = ccp(0.f, -350.f);

	CCPoint gravityStep = ccpMult(gravity,delta);

	CCPoint forwardMove = ccp(800.f, 0.f); //前进速度，每秒前进8.f

	CCPoint forwardStep = ccpMult(forwardMove, delta);

	this->velocity = ccpAdd(this->velocity, gravityStep);

	this->velocity = ccp(this->velocity.x *0.9f, this->velocity.y);//模拟摩擦力

	//CCPoint jumpForce = ccp(0.f, -30.f);

	float jumpMax = 150.f;

	if (this->_mightAsWellJump)
	{
		//this->velocity = ccpAdd(this->velocity, gravity);

		SimpleAudioEngine::sharedEngine()->playEffect("jump.wav");
	}
	else if (!this->_mightAsWellJump&&this->velocity.y > jumpMax)
	{
		this->velocity = ccp(this->velocity.x, jumpMax);
	}

	if (this->_forwardMarch)
	{
		this->velocity = ccpAdd(this->velocity, forwardStep); //加上向前的速度矢量
	}

	CCPoint s = ccpMult(this->velocity, delta);

	this->_desiredPosition = ccpAdd(this->getPosition(), s);

}
cocos2d::CCRect Player::collisionBoundingBox()
{   //修改图片边缘空白的误差

    cocos2d::Rect box = this->boundingBox();
	cocos2d::CCRect collisionBox = Tools::CCRectInset(box,3.f,0.f);

	CCPoint diff = ccpSub(this->_desiredPosition, this->getPosition()); //玩家当前距离与目的地的差距

	CCRect desireBoundingBox = Tools::CCRectOffset(collisionBox, diff.x, diff.y);

	return desireBoundingBox;
}

Player* Player::mInstance = nullptr;