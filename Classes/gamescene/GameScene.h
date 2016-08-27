
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
	ShapeIndex _nextbox[3];	//����nextbox
	int _savebox;	//�����box

	Vec2 rightpt;	
	Vec2 leftpt;

	Vec2 _nextBoxPosition[3];

	Menu *rotate;

private:
	//����.�ȼ�.Ŀ����ʾ
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
    
	//����ˢ��
	void dateUpdate(float ft);

	//��ͣ
	void pausePage();

	//ʧ��
	void faildPage();

	//UMENG��¼
	void UMCount();

	void start();
	void restart();
	void resurrection();
	  
	//��¼������߷�
	void recordStartHighScore();

	void startGame();

	//�����仯
	void scoreChange(int frontsc,int endsc);

	//��ʯ�仯
	void diamondChange(int frontsc, int endsc);
    
    Layer* faildlayer;

	NodeGrid *_gridnode;
	int _wavenum;
};

#endif