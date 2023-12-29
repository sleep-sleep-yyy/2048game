#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__
#include "cocos2d.h"
#include "CardSprite.h"
#include "LittleMenu.h"
#include <vector>

USING_NS_CC;
//int mode = 4;//��Ϸģʽ
class GameScene :public Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameScene);
	//��������
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	//�������һ�������
	bool moveLeft();
	bool moveRight();
	bool moveUp();
	bool moveDown();
	//����4*4��Ƭ����
	void createCardArr();
	void randomCreateCard();
	//�ж���ϷӮ��
	void checkGameWin();
	void checkGameOver();
	void showMenu(std::string result);//�˵���ʾ
	void showRanking(Ref* sender);//��ʾ����
	void restart(Ref* sender); //���¿�ʼ��Ϸ�˵���
	void pauseMenu(Ref* sender);//��ͣ����ʾ�Ľ���
	void resume(Ref* sender);//��д�ָ�����
	void modeChange(Ref* sender);//ģʽת��
	void createRanking(const int &score);//����ÿ�εĳɼ�
	void getNoSort();//�õ�vector������
	void toggleSound();//�л���������
private:
	int score;  //��ǰ����
	int bestScore; //��÷���
	int mode=4;//��Ϸģʽ
	Label* scoreLabel;
	Sprite* soundButton;//������ť
	bool isSoundOn=true;
	CardSprite* cardArr[5][5];  //���ֿ�Ƭ����
	Point startPt; //������ʼ��
	int offsetX, offsetY;  //����ˮƽ����ֱ����ƫ����
	MenuItemLabel* menuItemNew1;//��ť�������﷽������۵���
	MenuItemLabel* menuItemNew2;
	MenuItemLabel* menuItemNew3;
	MenuItemLabel* menuItemNew4;
	MenuItemLabel* menuItemNew5;
	LittleMenu* littleMenu; //�˵���
	timeval tv; //��ǰʱ��
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
	std::vector<IntString>rankingList;//��¼�ɼ�
};

#endif