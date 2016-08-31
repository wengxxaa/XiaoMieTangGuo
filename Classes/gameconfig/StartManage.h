#ifndef STARTMANAGE_H_
#define STARTMANAGE_H_

#include"cocos2d.h"
#include"../cj/cjconfig.h"
#include"../cj/GameData.h"
#include "ui/CocosGUI.h"
#include "BaseManage.h"
#include "../gonggao/ScrollLabel.h"

class StartManage:public Layer,public BaseManage
{
public:
	virtual bool init();

	CREATE_FUNC(StartManage);
	CREATE_SCENE(StartManage);

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);

private:
	//��ʼ������
	void initChess();

	//��ʼ������
	void initData();

	//��ʼ������
	void initProps();

	//��ʼ��������
	void startDrop();

	//���ߴ���
	bool propProcess(int id,pair<int,int> pos=make_pair(0,0));
	
	//λ��ת��
	Vec2 posToposition(pair<int,int> pos,int key=1);
	pair<int,int> positionTopos(Vec2 vec);

	//��������
	void boxEliminate(vector<pair<int,int>> &vec);

	//������ʾ
	void boxTip(pair<int,int> pos,vector<pair<int,int>> &vec);

	//�õ���Χ��ͬ��ɫ�ķ���
	vector<pair<int,int>> boxGet(pair<int,int> pos);
	void boxAdd(pair<int,int> pos,vector<pair<int,int>> &vec);

	void boxSequence(vector<pair<int,int>> &vec,bool sx=true);

	//���������ƶ�
	bool chessMove();

	//�����Ƿ񻹿������ж�
	bool chessCanEliminate();

	//�������
	void chessClear();

	void spriteShow(const string& str,bool pos=true);

	//��������
	void scoreMotion(int score,pair<int,int> pt);

	//�ؿ���������
	void LevelUpMotion();

	//���÷ֶ���
	void retainScoreMotion(int number,int score,float time);

	//���÷�
	int getRetainScore(int number);

	int getTargetScoreByRound(int round);

	//ʧ���ж�
	void faildJudge();

	int getScoreByNum(int num);

	inline int getChessInfo(pair<int,int> pos){return _chessInfo[pos.first*CHESS_SIZE_H_START+pos.second];}
	inline void setChessInfo(pair<int,int>pos,int k){_chessInfo[pos.first*CHESS_SIZE_H_START+pos.second]=k;}

	void addParticle(int x,int y,int shapeid,int type);
	void removeBox(pair<int,int> pos,int key=1);

	inline int getnum(pair<int,int> i){return i.first*CHESS_SIZE_H_START+i.second;}
	inline int getrenum(pair<int,int> i){return i.first*CHESS_SIZE_H_START+(CHESS_SIZE_H_START-i.second);}

	bool singelCanEliminate(pair<int,int> pos);

	//����ָ��
	void startGuide();
	void setGuide1();
	void setGuide2();
	void setGuide3();
	void setGuide4();
	void setGuideProp(int id);

	Sprite* getProp(int id);

	//�����ߵ��
	int propTouch(Vec2 pos);

	void propMotion(int key);

	void useProps(pair<int,int> pos);

	//ʹ�õ���4
	void useProp4(pair<int,int> pos);

	//�������
	void touchHandle();

	void handleHandle();

	//���ˢ��
	void hongbaoFlush(float ft);

	//������
	void startRain();

	//��ը��
	void getBoom();

	//���ӳ��ַ�ʽ
	float chessAppear(int key);

	//���
	void hongbaoPage();

	//�����ť����¼�
	void buttonClick(Ref* pSender, ui::Widget::TouchEventType touchType);

	//�������
	void hongbaoReward();

	//����վ������
	void peopleStand();

	//�����ռ�
	void cubeCollect(pair<int,int> pos);

	//�������
	void cubeGift();

	void cubeReward(int num,int prop);

	//���߸�����ʾ
	void propNumShow(int propid,int key);

	//С���ռ�����
	float smallPeopleCollect(vector<pair<int,int>> vec);

	void update(float ft);

	//�����ʾ
	void clickTip();
	void clickTipUpdate(float ft);
	void beganTip();
	void endTip();

	//���ν�����update
	void huptUpdate(float ft);

	//������
	void postBoard();
	void setPostShow(bool key);
	void postEvent();

	//����ָ��
	void ignoreGuide();

public:
	CC_SYNTHESIZE(int,_diamondnumber,Diamond);	//��ʯ��


	//����
	void setmScore(int score) { _score = score; }
	int getmScore() { return _score; }
	//�ȼ�
	void setmLevel(int level) { _level = level; }
	int getmLevel() { return _level; }
	//Ŀ��
	void setmTarget(int target) { _target = target; }
	int getmTarget() { return _target; }
	//��Ϸ״̬
	void setGameState(GameState state) { _gamestate = state; }
	GameState getGameState() { return _gamestate; }

	void start();	//����
	void pause();	//��ͣ
	void run();		//��ʼ
	void Resurrection();	//����

	//��¼����
	void dataSave(bool tt=true);

private:
	ClippingNode *_clip;

	std::map<int,int> _chessInfo;

	vector<pair<int,int>> _selectbox;

	int _lastLevelScore;	//��һ�صĵĵ÷���

	int _selectprop;	//ѡ��ĵ���

	int _propbox;	//��������ķ���

	//
	bool _guide;
	int _guidenumber;

	int _prop1motionnumber;

	bool _firstSaveIn;	//��һ�α������
	_startdata sdata;	//���������

	int _eliminateNum;	//�����ĸ���
	vector<pair<int,int>> _eliminatePos;	//������λ��

	//���߸���
	Label *_propnum[4];

	LabelTTF *_hongbaotip;

	Node *_backnode;

	int heightoffest;

	int _shandianid;	//��������id

	bool _hongbaoclick;

	int _smallpeoplenum;	//С�˵���ʾid

	bool _candealwith;	//�Ƿ���

	int _hunum;
	int _hutempnum;

	ProgressTimer *_huptimer;	//���ν�����

	int _clicktiptime;	//�����ʾʱ��
	vector<pair<int, int>> _clicktipvec;	//�����ʾ�ķ���

	//������
	ScrollLabel *_postboard;
	LayerColor *_postlayer;
};

#endif