#ifndef MAINSCENE_H_
#define MAINSCENE_H_

#include"GameScene.h"
#include "../cj/cjconfig.h"
#include"../cj/GameData.h"
#include "vigame_ad.h"

class MainScene:public Layer
{
public:
	
	virtual bool init();

	CREATE_FUNC(MainScene);
	CREATE_SCENE(MainScene);

	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchesMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);

	void text(int k);

private:
	void set();

	void xinshouUpdate(float ft);

	int _setstate;

	bool _xieyi;
    
    Menu* zuanshi;

	NodeGrid *_gridnode;

	int _wavenum;

	void setPackageName(const char* packageName)
	{
		CCLog("packageName:%s",packageName);
	}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS|| IOSMODE)
    void checkVideoState(float dt);
#endif
};

#endif