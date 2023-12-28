#include "WelcomeScene.h"
#include "SimpleAudioEngine.h"
//#include "RankingListScene.h"
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

bool WelcomeScene::init()
{
	if (!Layer::init())
		return false;
	//背景音乐和音效预加载一下
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("BackGround.mp3"); //true 表示循环播放
	SimpleAudioEngine::getInstance()->preloadEffect("Move.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("MoveClear.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("GameOver.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("GameWin.mp3");
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("BackGround.mp3", true);

	auto size = Director::getInstance()->getWinSize();
	TTFConfig config("fonts/Marker Felt.ttf", 150);//字体
	auto GameName = Label::createWithTTF(config, "2048");
	GameName->setPosition(Vec2(size.width / 2, size.height * 8 / 15 + 50));
	this->addChild(GameName);
	auto GameAuthor = Label::createWithTTF(config, "GAME BY MMY");
	GameAuthor->setPosition(Vec2(size.width / 2, size.height / 3 + 50));
	GameAuthor->setScale(0.3);
	this->addChild(GameAuthor);
	//设置开始游戏按钮
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vector<MenuItem*> MenuItems;
	auto label1 = Label::createWithTTF("Start Game", "fonts/arial.ttf", 40);
	auto playingItem = MenuItemLabel::create(label1, CC_CALLBACK_1(WelcomeScene::jumpToGame, this));
	MenuItems.pushBack(playingItem);
	playingItem->setPosition(Vec2(visibleSize.width * 9 / 18, visibleSize.height / 8 + 50));
	playingItem->setColor(Color3B(245, 245, 245));
	//设置退出按钮
	auto label2 = Label::createWithTTF("Exit", "fonts/Mouldy.ttf", 24);
	auto menuItemNew2 = MenuItemLabel::create(label2, [](Ref* sender) {Director::getInstance()->end(); });
	MenuItems.pushBack(menuItemNew2);
	menuItemNew2->setPosition(Vec2(visibleSize.width * 17 / 18 + 25, visibleSize.height - 25));
	menuItemNew2->setColor(Color3B(245, 245, 245));


	/* repeat for as many menu items as needed */
	auto menu = Menu::createWithArray(MenuItems);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	return true;
}
/*void WelcomeScene::showRangking(Ref* sender)
{
	auto scene = RankingListScene::createScene();//切换下个场景
	Director::getInstance()->replaceScene(TransitionFadeDown::create(0.1f, scene));
}
*/

void WelcomeScene::jumpToGame(Ref* sender)
{

	auto scene = GameScene::createScene();//切换下个场景
	Director::getInstance()->replaceScene(TransitionFadeDown::create(0, scene));
}
