#ifndef SceneManage_H_
#define SceneManage_H_

#include "cocos2d.h"
#include"../cj/cjconfig.h"
#include "BoxNode.h"
#include "BoxLine.h"
#include "../cj/GameData.h"
#include "BaseManage.h"
USING_NS_CC;

class SceneManage:public Layer,public BaseManage
{
public:
	SceneManage();
	~SceneManage();

	bool init();

	CREATE_FUNC(SceneManage);

public:
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
	//步数
	void setmStepNumber(int stepnumber) { _stepnumber = stepnumber; }
	int getmStepNumber() { return _stepnumber; }
	//下移方块
	ShapeIndex getNextBox(int i) { return _nextBox[i]; }
	//run
	void run();
	void pause();
	void start();
	void Resurrection();	//复活
public:
	//一个方块开始下落
	void startDropBox();

	//下落保存的方块
	void startSaveBox();

	//生成将要出现的方块
	void initNextBox();
	void createNextBox();

	//方块下落判断回调
	void boxDropUpdate(float ft);

	//方块向下运动一步
	bool boxDropOneLine();

	//方块位置确定和移除
	bool boxSure();

	//方块是否可以移动判断
	bool boxMoveJudge(DirectionIndex dir,Vec2 vec=Vec2(0.0f,0.0f));

	//下落时间
	void timeUpdate(float ft);

	//清空棋盘
	void gameFiled();
	bool checkChess();

	//方块变换
	bool boxChange(int key=1);//1.顺时针旋转 3.逆时针旋转

	//方块下落位置的提示
	void boxDropTip(float ft);
	//点触模式下落位置的提示
	void boxDropTip();

	//储存方块
	void boxSave();

	void Rotate();	//方块摆放变换


	//取当前方块的覆盖位置
	vector<std::pair<int,int>> getBoxFill(Vec2 vec=Vec2(0.f,0.f));

	vector<std::pair<int,int>> getBoxFill(PointTip tip);

	//放回棋盘位置
	Vec2 chessPos(int x,int y);

	//指引位置判断
	int PointTipJudge(PointTip pt);

	void PointTipRemove();	//根据当前方块下落的位置删除提示点

	//得到无法填充的空格数
	int getBlankOfUnFill();
	//增加一个偏移量
	int getBlankOfUnFill(vector<pair<int,int>> map);

	//得到当前的消除行数（添加了一个偏移量）
	int getRemoveRow(PointTip pt);

	bool isBlankOfUnFill(pair<int,int> vec);

	bool tempUnFillJudeg(pair<int,int> vec);

	bool canPutIn(PointTip tip);

	//从已存在的路径中查找，不用再次递归运算
	bool getVecFromPutIn(PointTip tip);
	bool getByLine(PointTip fronttip,PointTip targettip);
	bool getByAngle(PointTip fronttip,PointTip targettip);

	int getButtonPosition(PointTip tip);

	pair<int,int> positionToPos(Vec2 pt);

	std::map<int,int> getTopHeight();

	void PointMove(float ft);
	int _pointMoveNumber;
	vector<PointTip> _pointMoveP;

	inline int getSaveBox(){return _saveBox;}

	vector<int> _moveh;//下落的高度

	CC_SYNTHESIZE(TouchState,_touchstate,TouchState);	//是否接受滑动
	//CC_SYNTHESIZE(ChessDropMove,_movestate,MoveState);	//是否可以下落
	CC_SYNTHESIZE(Vec2,_lastPos,LostTouchPos);	//上次点击的位置

	CC_SYNTHESIZE(int, _time,mTime);	//时间
	CC_SYNTHESIZE(bool,_boxsave,mBoxSave);	//方块是否可以储存

	inline int getTipNumber(){return _recentPointNumber;};

	//记录保存
	void dataSave(bool tt=true);

private:
	ShapeIndex _recentBox;    //当前方块
	int _saveBox;		//保存的方块

	bool _firstSaveIn;	//第一次保存进入
	_tetrisdata sdata;	//保存的数据

	std::pair<int,int> _recentPos;	//当前方块的位置

	int _chessFilInfo[CHESS_SIZE_W][CHESS_SIZE_H];	//棋盘的填充信息
	int _chessSaveInfo[CHESS_SIZE_W][CHESS_SIZE_H];	//棋盘要存储的信息

	map<int,vector<PointTip>> _pointPath;

	//是否移动过
	bool _touchbool;
	//是否可以移动
	bool _touchcan;

	ClippingNode *_clip;	//裁剪区域

	GameModel _model;	//游戏模式

	EventListenerTouchOneByOne *listener;

	//Point模式Tip储存
	vector<PointTip> _pointTipVec;
	//Point模式Tip储存分组后
	map<int,vector<int>> _pointTipPair;
	//Point模式分组过程中存储临时位置
	vector<pair<int,int>> _pointTipPairTemp;
	vector<int> _pointTipPriority;

	vector<pair<int,int>> _tempUnFillVec;	//存放算空格数时的临时变量

	//当前Point模式演示的组数
	int _recentPointNumber;

	Layer *_PointTipLayer;	//Point模式提示层

	//点触下落的方块
	int _touchbox;

	std::map<int,int> _toph;
	set<pair<int,int>> _BlankOfUnFillVec;

	float _touchtime;

	vector<PointTip> _noCanPutIn;//存放不能下落的方块
	bool getVecFromNoPutIn(PointTip tip);

	//
	float _scoretime;	//得分时间
	int  _scorewav;	//得分音效

	//向上升快时间
	float _upboxtime;	//向上升的方块时间
	bool _canup;	//是否可以向上升


	int heightoffest;
private:
	//初始化棋盘
	void initChess();

	void timeSchedule(float ft);

	//界面上字体显示
	void textShow(const string& str,int size,Vec2 pos=WINSIZE/2.0f);

	//精灵显示
	void spriteShow(const string& str);

	//加粒子
	void addParticle(int x,int y,int shapeid,int type=1);

	//移除方块
	void removeBox(pair<int,int> pos);

	//关卡升级
	bool levelUp();

	//关卡升级动画
	void LevelUpMotion();

	//关卡升级消除动画
	void LevelUpEliminate();

	//分数动画
	void scoreMotion(int score,pair<int,int> pt);

	//得到升级可以消除的点
	vector<pair<int,int>> getLevelUpBox();

	float rollEliminate(vector<int> vec);	//行消除

	//提示模式
	bool _guide;
	int _guidenumber;

	//经典模式指引
	void classesGuide();


	//挑战模式指引
	void challengeGuide();

public:
	//判断提示的位置和间隔
	bool pointTipPairTempJudge(int pairid);

	//显示Point模式提示
	void showTipWithId(int pairid);
	void showTipWithBox(PointTip tip);
	void moveBoxToId(int boxid);

	//inline int getChessSaveInfo(int x,int y){return _chessSaveInfo[x*CHESS_SIZE_H+y];}
	inline int getRecentBox(){return int(_recentBox);}
	inline std::pair<int,int> getRecentPos(){return _recentPos;};
};

#endif