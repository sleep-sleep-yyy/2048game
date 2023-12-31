#include "CardSprite.h"
#include "GameDefine.h"
USING_NS_CC;

bool CardSprite::init()
{
	if (!Sprite::init())
		return false;
	else
		return true;
}
/*---初始化卡片，数字，尺寸，坐标---*/
void CardSprite::cardInit(int num,float p_x, float p_y,int mode)
{
	number = num;
	if(mode==4)
	    cardBgColour = LayerColor::create(Color4B(191, 191, 191, 255), TileWidth, TileWidth);
	else//mode==5
		cardBgColour = LayerColor::create(Color4B(191, 191, 191, 255), TileWidth_2, TileWidth_2);
	cardBgColour->setPosition(Point(p_x, p_y));
	if (number > 0) //有数字，显示数字
	{
		cardLabel = Label::createWithTTF(String::createWithFormat("%d", number)->getCString(), "fonts/Arial.ttf",50);
		//cardBgColour->getContentSize()表示这个cardBgColour节点的尺寸
		cardLabel->setPosition(cardBgColour->getContentSize().width / 2, cardBgColour->getContentSize().height / 2);
		cardBgColour->addChild(cardLabel); //将文字标签加入到带颜色的层
	}
	else//否则空
	{
		cardLabel = Label::createWithTTF("", "fonts/Arial.ttf", 50);
		cardLabel->setPosition(Vec2(cardBgColour->getContentSize().width / 2, cardBgColour->getContentSize().height / 2));
		cardBgColour->addChild(cardLabel);
	}
	//cardLabel->cardBgColour->card(create函数里的）
	this->addChild(cardBgColour); 
}
/*---返回一个卡片精灵的指针---*/
CardSprite* CardSprite::createCard(int num,float p_x, float p_y,int mode)
{
	CardSprite* card = new CardSprite();
	if (!card->init())
	{
		CC_SAFE_RELEASE(card);
		return NULL;
	}
	card->autorelease();  //设置为自动回收，无需显式调用delete
	card->cardInit(num,p_x, p_y,mode);
	return card;
}
/*---设置卡片的数字---*/
void CardSprite::setNumber(int num)
{
	number = num;
	//设置数字颜色
	if (number >= 0 && number < 16) {
		TTFConfig ttfConfig("fonts/Arial.ttf", 50);
		cardLabel->setTTFConfig(ttfConfig);
	}
	else if (number >= 16 && number < 128) {
		TTFConfig ttfConfig("fonts/Arial.ttf", 40);
		cardLabel->setTTFConfig(ttfConfig);
	}
	else if (number >= 128 && number < 1024) {
		TTFConfig ttfConfig("fonts/Arial.ttf", 35);
		cardLabel->setTTFConfig(ttfConfig);
	}
	else if (number >= 1024) {
		TTFConfig ttfConfig("fonts/Arial.ttf", 25);
		cardLabel->setTTFConfig(ttfConfig);
	}
	if (number > 0)  //当数字大于0时才显示数字
		cardLabel->setString(String::createWithFormat("%d", number)->getCString());
	else  //否则显示空
		cardLabel->setString("");

	//设置卡片颜色
	if (number == 0)
		cardBgColour->setColor(Color3B(191, 191, 191));
	if (number == 2)
		cardBgColour->setColor(Color3B(230, 220, 210));
	if (number == 4)
		cardBgColour->setColor(Color3B(205, 190, 70));
	if (number == 8) 
		cardBgColour->setColor(Color3B(230, 150, 100));
	if (number == 16)
		cardBgColour->setColor(Color3B(230, 120, 80));
	if (number == 32)
		cardBgColour->setColor(Color3B(230, 100, 90));
	if (number == 64)
		cardBgColour->setColor(Color3B(205, 133, 63));
	if (number == 128)
		cardBgColour->setColor(Color3B(205, 125, 98));
	if (number == 256)
		cardBgColour->setColor(Color3B(205, 104, 57));
	if (number == 512)
		cardBgColour->setColor(Color3B(255, 102, 102));
	if (number == 1024)
		cardBgColour->setColor(Color3B(204, 204, 255));
	if (number == 2048)
		cardBgColour->setColor(Color3B(255, 204, 00));
}
/*---获得卡片数字---*/
int CardSprite::getNumber()
{
	return number;
}
/*---获得卡片层---*/
LayerColor* CardSprite::getCardLayer()
{
	return cardBgColour;
}

