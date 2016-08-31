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
	//初始化棋盘
	void initChess();

	//初始化数据
	void initData();

	//初始化道具
	void initProps();

	//开始下落棋子
	void startDrop();

	//道具处理
	bool propProcess(int id,pair<int,int> pos=make_pair(0,0));
	
	//位置转换
	Vec2 posToposition(pair<int,int> pos,int key=1);
	pair<int,int> positionTopos(Vec2 vec);

	//方块消除
	void boxEliminate(vector<pair<int,int>> &vec);

	//方块提示
	void boxTip(pair<int,int> pos,vector<pair<int,int>> &vec);

	//得到周围相同颜色的方块
	vector<pair<int,int>> boxGet(pair<int,int> pos);
	void boxAdd(pair<int,int> pos,vector<pair<int,int>> &vec);

	void boxSequence(vector<pair<int,int>> &vec,bool sx=true);

	//方块下落移动
	bool chessMove();

	//棋盘是否还可消除判断
	bool chessCanEliminate();

	//棋盘清空
	void chessClear();

	void spriteShow(const string& str,bool pos=true);

	//分数动画
	void scoreMotion(int score,pair<int,int> pt);

	//关卡升级动画
	void LevelUpMotion();

	//最后得分动画
	void retainScoreMotion(int number,int score,float time);

	//最后得分
	int getRetainScore(int number);

	int getTargetScoreByRound(int round);

	//失败判断
	void faildJudge();

	int getScoreByNum(int num);

	inline int getChessInfo(pair<int,int> pos){return _chessInfo[pos.first*CHESS_SIZE_H_START+pos.second];}
	inline void setChessInfo(pair<int,int>pos,int k){_chessInfo[pos.first*CHESS_SIZE_H_START+pos.second]=k;}

	void addParticle(int x,int y,int shapeid,int type);
	void removeBox(pair<int,int> pos,int key=1);

	inline int getnum(pair<int,int> i){return i.first*CHESS_SIZE_H_START+i.second;}
	inline int getrenum(pair<int,int> i){return i.first*CHESS_SIZE_H_START+(CHESS_SIZE_H_START-i.second);}

	bool singelCanEliminate(pair<int,int> pos);

	//新手指引
	void startGuide();
	void setGuide1();
	void setGuide2();
	void setGuide3();
	void setGuide4();
	void setGuideProp(int id);

	Sprite* getProp(int id);

	//检测道具点击
	int propTouch(Vec2 pos);

	void propMotion(int key);

	void useProps(pair<int,int> pos);

	//使用道具4
	void useProp4(pair<int,int> pos);

	//点击处理
	void touchHandle();

	void handleHandle();

	//红包刷新
	void hongbaoFlush(float ft);

	//流星雨
	void startRain();

	//送炸弹
	void getBoom();

	//棋子出现方式
	float chessAppear(int key);

	//红包
	void hongbaoPage();

	//红包按钮点击事件
	void buttonClick(Ref* pSender, ui::Widget::TouchEventType touchType);

	//红包礼物
	void hongbaoReward();

	//人物站立动画
	void peopleStand();

	//方块收集
	void cubeCollect(pair<int,int> pos);

	//方块礼包
	void cubeGift();

	void cubeReward(int num,int prop);

	//道具个数显示
	void propNumShow(int propid,int key);

	//小人收集方块
	float smallPeopleCollect(vector<pair<int,int>> vec);

	void update(float ft);

	//点击提示
	void clickTip();
	void clickTipUpdate(float ft);
	void beganTip();
	void endTip();

	//弧形进度条update
	void huptUpdate(float ft);

	//公告栏
	void postBoard();
	void setPostShow(bool key);
	void postEvent();

	//跳过指引
	void ignoreGuide();

public:
	CC_SYNTHESIZE(int,_diamondnumber,Diamond);	//钻石数


	//分数
	void setmScore(int score) { _score = score; }
	int getmScore() { return _score; }
	//等级
	void setmLevel(int level) { _level = level; }
	int getmLevel() { return _level; }
	//目标
	void setmTarget(int target) { _target = target; }
	int getmTarget() { return _target; }
	//游戏状态
	void setGameState(GameState state) { _gamestate = state; }
	GameState getGameState() { return _gamestate; }

	void start();	//继续
	void pause();	//暂停
	void run();		//开始
	void Resurrection();	//复活

	//记录保存
	void dataSave(bool tt=true);

private:
	ClippingNode *_clip;

	std::map<int,int> _chessInfo;

	vector<pair<int,int>> _selectbox;

	int _lastLevelScore;	//上一关的的得分数

	int _selectprop;	//选择的道具

	int _propbox;	//道具消灭的方块

	//
	bool _guide;
	int _guidenumber;

	int _prop1motionnumber;

	bool _firstSaveIn;	//第一次保存进入
	_startdata sdata;	//保存的数据

	int _eliminateNum;	//消除的个数
	vector<pair<int,int>> _eliminatePos;	//消除的位置

	//道具个数
	Label *_propnum[4];

	LabelTTF *_hongbaotip;

	Node *_backnode;

	int heightoffest;

	int _shandianid;	//闪电声音id

	bool _hongbaoclick;

	int _smallpeoplenum;	//小人的显示id

	bool _candealwith;	//是否处理

	int _hunum;
	int _hutempnum;

	ProgressTimer *_huptimer;	//弧形进度条

	int _clicktiptime;	//点击提示时间
	vector<pair<int, int>> _clicktipvec;	//点击提示的方块

	//公告栏
	ScrollLabel *_postboard;
	LayerColor *_postlayer;
};

#endif