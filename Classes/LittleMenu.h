#pragma once
#include "cocos2d.h"
class LittleMenu :public cocos2d::LayerColor
{
public:
	static LittleMenu* create(const cocos2d::Color4B& color);
	virtual bool initWithColor(const cocos2d::Color4B& color);
	//´¥Ãþ¼àÌý
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
};