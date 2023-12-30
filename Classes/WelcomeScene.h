#ifndef __WELCOMESCENE_H__
#define __WELCOMESCENE_H__

#include "cocos2d.h"
USING_NS_CC;
class WelcomeScene : public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	void jumpToGame(Ref* sender);
	CREATE_FUNC(WelcomeScene);
};

#endif