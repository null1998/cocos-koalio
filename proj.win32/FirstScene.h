#ifndef _FIRSTSCENE_H_
#define _FIRSTSCENE_H_
#include "Player.h"
#include "cocos2d.h"
#include "TileRect.h"


class FirstScene : public cocos2d::Scene{
public:
	cocos2d::TMXTiledMap* map;
	cocos2d::CCTMXLayer* walls;
	cocos2d::CCTMXLayer* hazards;
	bool isGameOver;
	
public:
	
	static cocos2d::Scene* createScene();

	virtual bool init();

	void update(float delta);

	cocos2d::CCPoint tileCoordForPosition(cocos2d::CCPoint position);

	TileRect tileRectFromTileCoords(cocos2d::CCPoint tileCoords);

	TileRect* FirstScene::getSurroundingTilesAtPosition(cocos2d::CCPoint position, cocos2d::CCTMXLayer* layer);

	void FirstScene::checkForAndResolveCollisions(Player* player);

	void setViewpointCenter(cocos2d::CCPoint pos);

	void checkHazardCollisions(Player* player);
	
	void gameOver(bool isWin);

	void getWin();

	void restart();

	CREATE_FUNC(FirstScene);
};
#endif // !_FIRSTSCENE_H_
