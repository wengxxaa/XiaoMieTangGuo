
#ifndef GAMESCENE_H_
#define GAMESCENE_H_

#include "cocos2d.h"
#include "MainScene.h"
#include "../cj/cjconfig.h"

#include "vigame_ad.h"

USING_NS_CC;

class GameScene:public Layer
{
public:
	GameScene();
	~GameScene();

	virtual bool init();

	CREATE_FUNC(GameScene);
	CREATE_SCENE(GameScene);

	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchesMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);

public:
	ShapeIndex _nextbox[3];	//三个nextbox
	int _savebox;	//保存的box

	Vec2 rightpt;	
	Vec2 leftpt;

	Vec2 _nextBoxPosition[3];

	Menu *rotate;

private:
	//分数.等级.目标显示
	void imageShow(float);

	CC_SYNTHESIZE(int,_score,gScore);
	CC_SYNTHESIZE(int,_level,gLevel);
	CC_SYNTHESIZE(int,_target,gTarget);
	CC_SYNTHESIZE(int,_time,gTime);
	CC_SYNTHESIZE(int,_step,gStep);
	CC_SYNTHESIZE(int,_diamond,gDiamond);

	Node* _backnode;	//ui
	LabelAtlas *_levelfont;
	LabelAtlas *_targetfont;
	LabelAtlas *_highfont;
	LabelAtlas *_scorefont;
	LabelAtlas *_diamondfont;

public:
    
	//数据刷新
	void dateUpdate(float ft);

	//暂停
	void pausePage();

	//失败
	void faildPage();

	//UMENG记录
	void UMCount();

	void start();
	void restart();
	void resurrection();
	  
	//记录星星最高分
	void recordStartHighScore();

	void startGame();

	//分数变化
	void scoreChange(int frontsc,int endsc);

	//钻石变化
	void diamondChange(int frontsc, int endsc);
    
    Layer* faildlayer;

	NodeGrid *_gridnode;
	int _wavenum;
};

#endif