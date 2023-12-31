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
	static Scene* createScene();//��������
	virtual bool init();//��ʼ������
	CREATE_FUNC(GameScene);//������create��init
	//��������
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch,Event* event);
	//���̼���
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	//�������һ�������
	bool moveLeft();
	bool moveRight();
	bool moveUp();
	bool moveDown();
	//����4*4��5*5��Ƭ����
	void createCardArr();
	//�������һ����Ƭ
	void randomCreateCard();
	//�ж���ϷӮ��
	void checkGameWin();
	void checkGameOver();
	//�˵���ʾ
	void showMenu(std::string result);
	//��ʾ����
	void showRanking(Ref* sender);
	//���¿�ʼ��Ϸ�˵���
	void restart(Ref* sender);
	//��ͣ����ʾ�Ľ���
	void pauseMenu(Ref* sender);
	//������Ϸ��ť
	void Continue(Ref* sender);
	//��д�ָ�����
	void resume(Ref* sender);
	//ģʽת��
	void modeChange(Ref* sender);
	//����ÿ�εĳɼ�
	void createRanking(const int &score);
	//�õ�vector������
	void getNoSort();
	//�л���������
	void toggleSound();
private:
	int score;  //��ǰ����
	int bestScore; //��÷���
	int mode=4;//��Ϸģʽ
	Label* scoreLabel;//������ǩ
	Sprite* soundButton;//������ť
	bool isSoundOn=true;//��������
	EventListenerKeyboard* keyboardListener;//���̼�����
	CardSprite* cardArr[5][5];  //���ֿ�Ƭ����
	Point startPt; //������ʼ��
	int moveX, moveY;  //����ˮƽ����ֱ����ƫ����
	MenuItemLabel* menuItemNew1;//��ť�������﷽������۵���
	MenuItemLabel* menuItemNew2;
	MenuItemLabel* menuItemNew3;
	MenuItemLabel* menuItemNew4;
	MenuItemLabel* menuItemNew5;
	LittleMenu* littleMenu; //�˵���
	struct Score{//���а���
		int score;
		int year;
		int month;
		int day;
		int hour;
		int minute;
	};
	struct IntString {//���а���
		int number;
		std::string newString;
	};
	std::vector<IntString>rankingList;//���а��¼�ɼ�
};

#endif