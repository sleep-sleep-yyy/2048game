#ifndef __LITTLE_MENU_H__
#define __LITTLE_MENU_H__
#include "cocos2d.h"
USING_NS_CC;

/*-----ʵ���Ͼ���һ����ϴ����źŵ�СС��ɫ����-----*/
class LittleMenu :public LayerColor
{
public:
	static LittleMenu* create(const Color4B& color);
	virtual bool initWithColor(const Color4B& color);
	//��������
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);
};
#endif