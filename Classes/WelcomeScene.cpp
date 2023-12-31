#include "WelcomeScene.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* WelcomeScene::createScene()
{
	auto scene = Scene::create();
	auto layer = WelcomeScene::create();
	scene->addChild(layer);
	return scene;
}
/*-----��ӭ�������-----*/
bool WelcomeScene::init()
{
	if (!Layer::init())
		return false;
	//��Ϸ��
	const auto size = Director::getInstance()->getWinSize();//(900,600)
	const TTFConfig config("fonts/Marker Felt.ttf", 150);//����
	const auto GameName = Label::createWithTTF(config, "2048");
	GameName->setPosition(Vec2(size.width / 2, size.height*8/ 15+50));
	this->addChild(GameName);
	//������
	const auto GameAuthor = Label::createWithTTF(config, "GAME BY MMY");
	GameAuthor->setPosition(Vec2(size.width / 2, size.height / 3+50));
	GameAuthor->setScale(0.3f);
	this->addChild(GameAuthor);
	//���ÿ�ʼ��ť
	const Size visibleSize = Director::getInstance()->getVisibleSize();
	Vector<MenuItem*> MenuItems;
	const auto label1 = Label::createWithTTF("Start Game", "fonts/arial.ttf", 40);
	const auto playingItem = MenuItemLabel::create(label1, CC_CALLBACK_1(WelcomeScene::jumpToGame, this));
	MenuItems.pushBack(playingItem);
	playingItem->setPosition(Vec2(visibleSize.width * 9 / 18, visibleSize.height / 8+50));
	playingItem->setColor(Color3B(245, 245, 245));
	//�����˳���ť
	const auto label2 = Label::createWithTTF("Exit", "fonts/Mouldy.ttf", 24);
	const auto menuItemNew2 = MenuItemLabel::create(label2, [](Ref* sender) {Director::getInstance()->end(); });
	MenuItems.pushBack(menuItemNew2);
	menuItemNew2->setPosition(Vec2(visibleSize.width * 17 / 18 + 25, visibleSize.height - 25));
	menuItemNew2->setColor(Color3B(245, 245, 245));

	const auto menu = Menu::createWithArray(MenuItems);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	return true;
}
/*-----����Ԥ���أ����볡��-----*/
void WelcomeScene::jumpToGame(Ref* sender)
{
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("BackGround.mp3"); 
	SimpleAudioEngine::getInstance()->preloadEffect("Move.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("MoveClear.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("GameOver.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("GameWin.mp3");
	SimpleAudioEngine::getInstance()->playBackgroundMusic("BackGround.mp3", true);//true ��ʾѭ������
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5);//�������
	UserDefault::getInstance()->setIntegerForKey("mode", 4);//������ʼ��Ĭ��4*4
	UserDefault::getInstance()->setBoolForKey("sound", true);//������ʼ��Ĭ��4*4
	const auto scene = GameScene::createScene();//�л��¸�����
	Director::getInstance()->replaceScene(TransitionFade::create(0.7f, scene));
}
