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
//初始化卡片，数字，尺寸，坐标
void CardSprite::cardInit(int num, float p_x, float p_y)
{
	number = num;
	cardBgColour = LayerColor::create(Color4B(191, 191, 191, 255), TileWidth, TileWidth);
	cardBgColour->setPosition(Point(p_x, p_y));
	if (number > 0) //如果有数字，则显示数字
	{
		cardLabel = LabelTTF::create(String::createWithFormat("%d", number)->getCString(), "Arial", 50);
		//因为是加入到父节点中，所以此处用相对坐标
		cardLabel->setPosition(cardBgColour->getContentSize().width / 2, cardBgColour->getContentSize().height / 2);
		cardBgColour->addChild(cardLabel); //将文字标签加入到卡片层

	}
	else    //否则则是空卡片
	{
		cardLabel = LabelTTF::create("", "Arial", 50);
		cardLabel->setPosition(cardBgColour->getContentSize().width / 2, cardBgColour->getContentSize().height / 2);
		cardBgColour->addChild(cardLabel);
	}
	this->addChild(cardBgColour);  //将卡片层加入到卡片精灵
}
//返回一个卡片精灵的指针
CardSprite* CardSprite::createCard(int num, float p_x, float p_y)
{
	CardSprite* card = new CardSprite();
	if (!card->init())
	{
		CC_SAFE_RELEASE(card);
		return NULL;
	}
	card->autorelease();  //设置为自动回收
	card->cardInit(num, p_x, p_y);
	return card;
}
//设置卡片的数字
void CardSprite::setNumber(int num)
{
	number = num;
	//根据不同的数字范围显示不同的颜色
	if (number >= 0 && number < 16)
		cardLabel->setFontSize(50);
	if (number >= 16 && number < 128)
		cardLabel->setFontSize(40);
	if (number >= 128 && number < 1024)
		cardLabel->setFontSize(30);
	if (number >= 1024)
		cardLabel->setFontSize(20);
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
		cardBgColour->setColor(Color3B(230, 70, 60));
	if (number == 128)
		cardBgColour->setColor(Color3B(230, 190, 60));
	if (number == 256)
		cardBgColour->setColor(Color3B(153, 204, 51));
	if (number == 512)
		cardBgColour->setColor(Color3B(255, 102, 102));
	if (number == 1024)
		cardBgColour->setColor(Color3B(204, 204, 255));
	if (number == 2048)
		cardBgColour->setColor(Color3B(255, 204, 00));

	if (number > 0)  //当数字大于0时才显示数字
		cardLabel->setString(String::createWithFormat("%d", number)->getCString());
	else  //否则显示空
		cardLabel->setString("");
}

//获得卡片数字
int CardSprite::getNumber()
{
	return number;
}

//获得卡片层

LayerColor* CardSprite::getCardLayer()
{
	return cardBgColour;
}

