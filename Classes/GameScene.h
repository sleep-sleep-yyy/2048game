#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__
#include "cocos2d.h"
#include "CardSprite.h"
#include "LittleMenu.h"
#include <vector>

USING_NS_CC;
//int mode = 4;//游戏模式
class GameScene :public Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameScene);
	//触摸监听
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	//上下左右滑动动作
	bool moveLeft();
	bool moveRight();
	bool moveUp();
	bool moveDown();
	//创建4*4卡片矩阵
	void createCardArr();
	void randomCreateCard();
	//判断游戏赢输
	void checkGameWin();
	void checkGameOver();
	void showMenu(std::string result);//菜单显示
	void showRanking(Ref* sender);//显示排名
	void restart(Ref* sender); //重新开始游戏菜单项
	void pauseMenu(Ref* sender);//暂停后显示的界面
	void resume(Ref* sender);//重写恢复功能
	void modeChange(Ref* sender);//模式转换
	void createRanking(const int &score);//补充每次的成绩
	void getNoSort();//得到vector不排序
	void toggleSound();//切换声音开关
private:
	int score;  //当前分数
	int bestScore; //最好分数
	int mode=4;//游戏模式
	Label* scoreLabel;
	Sprite* soundButton;//声音按钮
	bool isSoundOn=true;
	CardSprite* cardArr[5][5];  //数字卡片矩阵
	Point startPt; //触摸开始点
	int offsetX, offsetY;  //触摸水平和竖直方向偏移量
	MenuItemLabel* menuItemNew1;//按钮放在这里方便对其宏观调控
	MenuItemLabel* menuItemNew2;
	MenuItemLabel* menuItemNew3;
	MenuItemLabel* menuItemNew4;
	MenuItemLabel* menuItemNew5;
	LittleMenu* littleMenu; //菜单层
	timeval tv; //当前时间
	struct Score
	{
		int score;
		int year;
		int month;
		int day;
		int hour;
		int minute;
	};
	struct IntString {
		int number;
		std::string newString;
	};
	std::vector<IntString>rankingList;//记录成绩
};

#endif