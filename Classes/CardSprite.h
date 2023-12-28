#pragma once
#ifndef __CARDSPRITE_H__
#define __CARDSPRITE_H__
#include "cocos2d.h"
USING_NS_CC;
class CardSprite :public Sprite
{
public:
	static CardSprite* createCard(int num, float p_x, float p_y);
	virtual bool init();
	CREATE_FUNC(CardSprite);

	void setNumber(int num);//���ÿ�Ƭ����
	int getNumber();//��ÿ�Ƭ����
	LayerColor* getCardLayer();
private:
	int number;
	void cardInit(int num, float p_x, float p_y);
	LabelTTF* cardLabel;
	LayerColor* cardBgColour;
};
#endif