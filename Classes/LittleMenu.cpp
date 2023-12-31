#include "LittleMenu.h"
USING_NS_CC;
LittleMenu* LittleMenu::create(const Color4B& color)
{
	LittleMenu* layer = new LittleMenu();
	if (layer && layer->initWithColor(color))//初始化成功
	{
		layer->autorelease();//自动回收
		return layer;
	}
	else//初始化失败
	{
		delete layer;
		layer = nullptr;
		return nullptr;
	}
}
bool LittleMenu::initWithColor(const Color4B& color)
{
	if (!LayerColor::initWithColor(color))
		return false;
	//添加空的触摸监听，防止触摸响应传递到底层
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(LittleMenu::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(LittleMenu::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	//吞没触摸事件
	listener->setSwallowTouches(true);
	return true;
}
bool LittleMenu::onTouchBegan(Touch* touch, Event* event)
{
	return true;
}
void LittleMenu::onTouchEnded(Touch* touch, Event* event)
{

}

