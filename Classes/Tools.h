#ifndef _TOOLS_H_
#define _TOOLS_H_

#include "cocos2d.h"

class Tools
{
public:
	static cocos2d::CCRect CCRectInset(cocos2d::Rect& rect, float dx, float dy);

	static cocos2d::CCRect CCRectOffset(cocos2d::CCRect& rect, float dx, float dy);
	
	static cocos2d::CCRect getIntersectsRect(cocos2d::CCRect& playerRect, cocos2d::CCRect& tileRect);

	//static const wchar_t* Tools::GetWC(const char *c);
};
#endif // !_TOOLS_H_
#pragma once
