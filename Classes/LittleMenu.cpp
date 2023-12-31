#include "LittleMenu.h"
USING_NS_CC;
LittleMenu* LittleMenu::create(const Color4B& color)
{
	LittleMenu* layer = new LittleMenu();
	if (layer && layer->initWithColor(color))//��ʼ���ɹ�
	{
		layer->autorelease();//�Զ�����
		return layer;
	}
	else//��ʼ��ʧ��
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
	//��ӿյĴ�����������ֹ������Ӧ���ݵ��ײ�
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(LittleMenu::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(LittleMenu::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	//��û�����¼�
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

