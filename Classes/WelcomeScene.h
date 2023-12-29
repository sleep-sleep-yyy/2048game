#ifndef __WELCOMESCENE_H__
#define __WELCOMESCENE_H__

#include "cocos2d.h"
USING_NS_CC;
class WelcomeScene : public Layer
{
public:
	//获得场景对象，cocos2dx内部通常用静态方法来返回对象指针
	static cocos2d::Scene* createScene();
	virtual bool init();
	void jumpToGame(Ref* sender);
	CREATE_FUNC(WelcomeScene);
};

#endif