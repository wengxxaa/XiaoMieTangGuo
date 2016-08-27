#ifndef FIRSTSCENE_H_
#define FIRSTSCENE_H_

#include"cocos2d.h"
#include"../cj/cjconfig.h"
USING_NS_CC;

class FirstScene:public Layer
{
public:

	bool init();

	CREATE_FUNC(FirstScene);
	CREATE_SCENE(FirstScene);

	void preLoad();

	void startUpdate(float ft);

	int _time;
};

#endif