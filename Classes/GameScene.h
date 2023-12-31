#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__
#include "cocos2d.h"
#include "CardSprite.h"
#include "LittleMenu.h"
#include <vector>
USING_NS_CC;
class GameScene :public Layer
{
public:
	static Scene* createScene();//创建场景
	virtual bool init();//初始化场景
	CREATE_FUNC(GameScene);//关联起create和init
	//触摸监听
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch,Event* event);
	//键盘监听
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	//上下左右滑动动作
	bool moveLeft();
	bool moveRight();
	bool moveUp();
	bool moveDown();
	//创建4*4或5*5卡片矩阵
	void createCardArr();
	//随机产生一个卡片
	void randomCreateCard();
	//判断游戏赢输
	void checkGameWin();
	void checkGameOver();
	//菜单显示
	void showMenu(std::string result);
	//显示排名
	void showRanking(Ref* sender);
	//重新开始游戏菜单项
	void restart(Ref* sender);
	//暂停后显示的界面
	void pauseMenu(Ref* sender);
	//继续游戏按钮
	void Continue(Ref* sender);
	//重写恢复功能
	void resume(Ref* sender);
	//模式转换
	void modeChange(Ref* sender);
	//补充每次的成绩
	void createRanking(const int &score);
	//得到vector不排序
	void getNoSort();
	//切换声音开关
	void toggleSound();
private:
	int score;  //当前分数
	int bestScore; //最好分数
	int mode=4;//游戏模式
	Label* scoreLabel;//分数标签
	Sprite* soundButton;//声音按钮
	bool isSoundOn=true;//声音控制
	EventListenerKeyboard* keyboardListener;//键盘监听器
	CardSprite* cardArr[5][5];  //数字卡片矩阵
	Point startPt; //触摸开始点
	int moveX, moveY;  //触摸水平和竖直方向偏移量
	MenuItemLabel* menuItemNew1;//按钮放在这里方便对其宏观调控
	MenuItemLabel* menuItemNew2;
	MenuItemLabel* menuItemNew3;
	MenuItemLabel* menuItemNew4;
	MenuItemLabel* menuItemNew5;
	LittleMenu* littleMenu; //菜单层
	struct Score{//排行榜用
		int score;
		int year;
		int month;
		int day;
		int hour;
		int minute;
	};
	struct IntString {//排行榜用
		int number;
		std::string newString;
	};
	std::vector<IntString>rankingList;//排行榜记录成绩
};

#endif