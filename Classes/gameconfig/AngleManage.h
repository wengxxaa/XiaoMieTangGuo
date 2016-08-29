#ifndef AngleManage_H_
#define AngleManage_H_

#include"cocos2d.h"
#include"ConfigState.h"
#include"AngleNode.h"
#include "../cj/GameData.h"
#include"ui/CocosGUI.h"
#include"BaseManage.h"
USING_NS_CC;

class AngleManage:public Layer,public BaseManage
{
public:
	virtual bool init();

	CREATE_FUNC(AngleManage);

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);

public:
	CC_SYNTHESIZE(TouchState, _touchState, touchState);
	CC_SYNTHESIZE(int, _diamondnumber, Diamond);	//钻石数

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

	void Rotate(int key=1);	//重现给出选择box
	void Resurrection();

private:
	void initChess();

	void initData();

	//初始化道具
	void initProps();

	Vec2 posToposition(pair<int,int> pos);
	pair<int,int> positionTopos(Vec2 position);

	void startNewRound(int key=1);	//出现新的一轮选项

	void showTipBox(AngleNode *box);	//显示提示的方块
	void removeTipBox();	//移除提示的方块

	vector<pair<int,int>> getFillBlack(AngleNode* box,Vec2 pt);

	//填充方块
	void chessFill(int shape,vector<pair<int,int>> vec);	

	//分数动画
	void scoreMotion(int score,pair<int,int> pt);

	//方块可填充判断
	void BoxFillJudge();

	//单独判断
	bool BoxJudge(AngleNode *node);

 //	//过关分数
	//int getTargetScoreByRound(int round);
	////消除棋子分数
	//int getBoxScoreByRound(int round);

	void eliminateAnimation(const std::vector<int> &vx,const std::vector<int> &vy,const std::vector<int> &vz);	//消除动画

	bool chessCheck();

	void addParticle(int x,int y,int shapeid,int type);

	void removeBox(pair<int,int> pos);

	bool faildJudge();

	bool chessFailedJudge();	//失败判断

	void gameFaild();

	//关卡升级
	bool levelUp();

	//关卡升级动画
	void LevelUpMotion();

	//关卡升级消除动画
	void LevelUpEliminate();

	void spriteShow(const string& str);

	//得到升级消除的棋子
	vector<pair<int,int>> getLevelUpBox();

	void propMotion(int id);

	void useProp2(pair<int,int> pos);

	void firstSet(int id);

	//指引
	void dragGuide();

	//记录保存
	void dataSave(bool tt=true);
	//
	bool _firstSaveIn;	//第一次保存进入
	_angledate sdata;	//保存的数据

	//解析csb文件的名字
	pair<int,int> analyzeName(const string &name);

	inline int hongBaoLevel(){return 6-((_level%5)?_level%5:5);}

	void hongbaoFlush(float ft);

	//红包
	void hongbaoPage();

	//红包按钮点击事件
	void buttonClick(Ref* pSender, ui::Widget::TouchEventType touchType);

	//红包礼物
	void hongbaoReward();

	//流星雨
	void startRain();

	//点击结束处理
	void endTouch();

private:
	bool _chessBlack[CHESS_SIZE_W_ANGLE][CHESS_SIZE_H_ANGLE];
	int _chessColor[CHESS_SIZE_W_ANGLE][CHESS_SIZE_H_ANGLE];

	bool _selectExit[3];//可选择的方块

	bool _guide;

	int _touchselect;

	pair<int,int> _chesstippos;	//棋盘提示的方块的位置

	int _selectprop;

	LabelAtlas *_diamondFont;

	////boxscore
	//int _boxscore;

	int selectboxoffest;

	int _shandianid;	//闪电音效id

	bool _hongbaoclick;

	//道具个数
	Label *_propnum[3];

	LabelTTF *_hongbaotip;	//红包提示
};

#endif