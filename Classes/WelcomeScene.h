#ifndef __WELCOME_SCENE_H__
#define __WELCOME_SCENE_H__

#include "cocos2d.h"

class WelcomeScene : public cocos2d::Layer
{
public:
	//��ó�������cocos2dx�ڲ�ͨ���þ�̬���������ض���ָ��
	static cocos2d::Scene* createScene();
	//��дLayer���init()������ʼ��������
	virtual bool init();
	void JumpToGame(float t);
	//�ڲ�����һ���������create()����
	CREATE_FUNC(WelcomeScene);
};

#endif
