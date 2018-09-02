#include"Tools.h"
#include<algorithm>
USING_NS_CC;
using namespace std;

cocos2d::CCRect Tools::CCRectInset(cocos2d::Rect& rect,float dx,float dy){
	 rect.origin.x += dx;
	 rect.size.width -= dx * 2;

	 rect.origin.y += dy;
	 rect.size.height -= dy * 2;

	 return rect;
 }
//移动
cocos2d::CCRect Tools::CCRectOffset(cocos2d::Rect& rect, float dx, float dy) {
	rect.origin.x += dx;
	
	rect.origin.y += dy;

	return rect;
}
cocos2d::CCRect Tools::getIntersectsRect(cocos2d::CCRect& playerRect, cocos2d::CCRect& tileRect)
{  
	//两个矩形的四个横坐标与四个纵坐标分别分组
	float hor[4] = { playerRect.origin.x,playerRect.origin.x + playerRect.size.width,tileRect.origin.x,tileRect.origin.x + tileRect.size.width };

	float ver[4] = { playerRect.origin.y,playerRect.origin.y + playerRect.size.height,tileRect.origin.y,tileRect.origin.y + tileRect.size.height };

	sort(hor,hor+4);

	sort(ver,ver+ 4);
	
	return CCRectMake(hor[1],ver[1],hor[2]-hor[1],ver[2]-ver[1]);

}

/*const wchar_t* Tools::GetWC(const char *c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}*/