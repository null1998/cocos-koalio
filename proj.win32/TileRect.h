#ifndef _TILERECT_H_
#define _TILERECT_H_

#include "cocos2d.h"

class TileRect : public cocos2d::CCRect {

public:
	//每个瓦片特有id，若此处无瓦片则为0
	int tgid;
	//顶点游戏坐标与长宽。
	float x, y, width, height;
public:
	TileRect() {

		this->isBarrier = false;

		int tgid = 0;
	};
	TileRect(float x, float y, float width, float height) : cocos2d::CCRect(x, y, width, height) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->isBarrier = false;
	}
	bool isBarrier;
	
};
#endif // !_TILERECT_H_
#pragma once
