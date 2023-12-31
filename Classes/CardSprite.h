#ifndef __CARDSPRITE_H__
#define __CARDSPRITE_H__
#include "cocos2d.h"
USING_NS_CC;
class CardSprite :public Sprite
{
public:
	static CardSprite* createCard(int num,float p_x, float p_y,int mode);
	virtual bool init();
	CREATE_FUNC(CardSprite);

	void setNumber(int num);//设置卡片数字
	int getNumber();//获得卡片数字
	LayerColor* getCardLayer();
private:
	void cardInit(int num, float p_x, float p_y, int mode);
	int number;
	Label* cardLabel;
	//LayerColor创建具有指定背景色的图层
	LayerColor* cardBgColour;
};
#endif