#ifndef __WELCOME_SCENE_H__
#define __WELCOME_SCENE_H__

#include "cocos2d.h"

class WelcomeScene : public cocos2d::Layer
{
public:
	//获得场景对象，cocos2dx内部通常用静态方法来返回对象指针
	static cocos2d::Scene* createScene();
	//重写Layer里的init()函数初始化布景层
	virtual bool init();
	void JumpToGame(float t);
	//内部生成一个布景层的create()函数
	CREATE_FUNC(WelcomeScene);
};

#endif
