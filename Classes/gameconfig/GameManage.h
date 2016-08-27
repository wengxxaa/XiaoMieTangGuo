#ifndef GAMEMANAGE_H_
#define GAMEMANAGE_H_

#include"../cj/cjconfig.h"
#include"BoxNode.h"
#include"BoxLine.h"
#include"ConfigState.h"
#include"cocos2d.h"
#include"BaseManage.h"
USING_NS_CC;

class GameManage:public Layer,public BaseManage
{
public:
	virtual bool init();

	CREATE_FUNC(GameManage);

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);

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
	void Resurrection();

private:
	bool _selectExit[3];

	ClippingNode *_clip;	//裁剪区域

	int _touchselect;	//选中的方块

	vector<bool> _chessInfo;	//棋盘信息
	vector<int> _chessSaveInfo;	//棋盘储存信息

	pair<int,int> _chesstippos;	//棋盘提示的方块的位置

	bool faildJudge();

	void gameFaild();

	//记录保存
	void dataSave(bool tt=true);
	//
	bool _firstSaveIn;	//第一次保存进入
	_onezerodata sdata;	//保存的数据

	float _scoretime;	//得分时间
	int  _scorewav;	//得分音效

	//新手指引
	bool _guide;

	int selectboxoffest;
private:
	//初始化棋盘
	void initChess();

	void startNewRound(int key=1);	//出现新的一轮选项

	float chessCheck();	//棋盘消除检测

	float eliminateAnimation(const std::vector<int> &vx,const std::vector<int> &vy);	//消除动画

	vector<pair<int,int>> getFillBlack(BoxNode* box,Vec2 pt);

	Vec2 positionByPos(pair<int,int> pos);	//由坐标得位置
	pair<int,int> posByPosition(Vec2 position);	//由位置得坐标

	void chessFill(ShapeIndex shape,vector<pair<int,int>> vec);	//填充方块

	bool chessFailedJudge();	//失败判断

	void spriteShow(const string& str);

	void addParticle(int x,int y,int shapeid,int type);

	void removeBox(pair<int,int> pos);

	void showTipBox(BoxNode *box);	//显示提示的方块

	void removeTipBox();	//移除提示的方块

	//分数动画
	void scoreMotion(int score,pair<int,int> pt);

	//计时
	void timeUpdate(float ft);

	bool selectBoxExit(ShapeIndex shape,AngleIndex angle=ANGLE_0);	//可供选项里是否存在XX

public:
	CC_SYNTHESIZE(TouchState,_touchState,touchState);

	void Rotate(int key=1);	//重现给出选择box

private:
	//关卡升级
	bool levelUp();

	//关卡升级动画
	void LevelUpMotion();

	//关卡升级消除动画
	void LevelUpEliminate();

	//得到升级消除的棋子
	vector<pair<int,int>> getLevelUpBox();

	//方块可填充判断
	void BoxFillJudge();

	//单独判断
	bool BoxJudge(BoxNode *node);
	
	//拖动模式指引
	void dragGuide();

};

#endif