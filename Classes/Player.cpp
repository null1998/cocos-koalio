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
	   // bRet = CCSprite::initWithFile("koalio_stand.png");
		bRet = CCSprite::initWithFile("Mario.png");
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

	CCPoint gravity = ccp(0.f, -350.f);//重力

	CCPoint gravityStep = ccpMult(gravity,delta);//重力加速度

	CCPoint forwardMove = ccp(800.f, 0.f); //前进的力

	CCPoint forwardStep = ccpMult(forwardMove, delta);//前进加速度

	this->velocity = ccpAdd(this->velocity, gravityStep);//竖直方向速度改变

	this->velocity = ccp(this->velocity.x *0.9f, this->velocity.y);//模拟摩擦力

	float jumpMax = 500.f;
	//y速度限制
	
	if (!this->_mightAsWellJump&&this->velocity.y > jumpMax)
	{
		this->velocity = ccp(this->velocity.x, jumpMax);
	}
	//前进时水平方向速度改变
	if (this->_forwardMarch)
	{
		this->velocity = ccpAdd(this->velocity, forwardStep); //加上向前的速度矢量
	}
	//相当于位移
	CCPoint s = ccpMult(this->velocity, delta);
	//下一帧将要去的位置
	this->_desiredPosition = ccpAdd(this->getPosition(), s);

}
cocos2d::CCRect Player::collisionBoundingBox()
{   //修改图片边缘空白的误差，计算碰撞矩形
	cocos2d::CCRect collisionBox = Tools::CCRectInset(this->boundingBox(),3.f,0.f);

	CCPoint diff = ccpSub(this->_desiredPosition, this->getPosition()); //玩家当前距离与目的地的差距
	//得到即将要去的位置的碰撞矩形
	CCRect desireBoundingBox = Tools::CCRectOffset(collisionBox, diff.x, diff.y);

	return desireBoundingBox;
}

Player* Player::mInstance = nullptr;