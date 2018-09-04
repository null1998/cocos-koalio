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

	CCPoint gravity = ccp(0.f, -350.f);//����

	CCPoint gravityStep = ccpMult(gravity,delta);//�������ٶ�

	CCPoint forwardMove = ccp(800.f, 0.f); //ǰ������

	CCPoint forwardStep = ccpMult(forwardMove, delta);//ǰ�����ٶ�

	this->velocity = ccpAdd(this->velocity, gravityStep);//��ֱ�����ٶȸı�

	this->velocity = ccp(this->velocity.x *0.9f, this->velocity.y);//ģ��Ħ����

	float jumpMax = 500.f;
	//y�ٶ�����
	
	if (!this->_mightAsWellJump&&this->velocity.y > jumpMax)
	{
		this->velocity = ccp(this->velocity.x, jumpMax);
	}
	//ǰ��ʱˮƽ�����ٶȸı�
	if (this->_forwardMarch)
	{
		this->velocity = ccpAdd(this->velocity, forwardStep); //������ǰ���ٶ�ʸ��
	}
	//�൱��λ��
	CCPoint s = ccpMult(this->velocity, delta);
	//��һ֡��Ҫȥ��λ��
	this->_desiredPosition = ccpAdd(this->getPosition(), s);

}
cocos2d::CCRect Player::collisionBoundingBox()
{   //�޸�ͼƬ��Ե�հ׵���������ײ����

	CCRect box = this->boundingBox();
	cocos2d::CCRect collisionBox = Tools::CCRectInset(box,3.f,0.f);

	CCPoint diff = ccpSub(this->_desiredPosition, this->getPosition()); //��ҵ�ǰ������Ŀ�ĵصĲ��
	//�õ�����Ҫȥ��λ�õ���ײ����
	CCRect desireBoundingBox = Tools::CCRectOffset(collisionBox, diff.x, diff.y);

	return desireBoundingBox;
}

Player* Player::mInstance = nullptr;