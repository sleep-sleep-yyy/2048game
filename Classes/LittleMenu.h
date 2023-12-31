#ifndef __LITTLE_MENU_H__
#define __LITTLE_MENU_H__
#include "cocos2d.h"
USING_NS_CC;

/*-----实际上就是一个阻断触摸信号的小小颜色层啦-----*/
class LittleMenu :public LayerColor
{
public:
	static LittleMenu* create(const Color4B& color);
	virtual bool initWithColor(const Color4B& color);
	//触摸监听
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);
};
#endif