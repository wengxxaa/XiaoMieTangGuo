#ifndef GameData_H_
#define GameData_H_

#include"fstream"
#include"../gameconfig/ConfigState.h"
#include"cocos2d.h"
USING_NS_CC;

struct _tetrisdata{
	bool _istip;	//是否保存记录

	int _chessInfo[CHESS_SIZE_W][CHESS_SIZE_H];	//棋盘信息
	int _level;	//关卡
	int _score;	//分数
	int _targetscore;
	int _recentbox;	//当前方块
	int _recentposx;
	int _recentposy;	//当前位置
	int _nextbox[3];	//下一个方块

	bool _guider;	//新手指引

	_tetrisdata(){};
	_tetrisdata& operator=(const _tetrisdata &one)
	{
		_istip=one._istip;
		for(int i=0;i<CHESS_SIZE_W;i++)
			for(int j=0;j<CHESS_SIZE_H;j++)
				_chessInfo[i][j]=one._chessInfo[i][j];
		_level=one._level;
		_score=one._score;
		_targetscore=one._targetscore;
		_recentbox=one._recentbox;
		_recentposx=one._recentposx;
		_recentposy=one._recentposy;
		for(int i=0;i<3;i++)
			_nextbox[i]=one._nextbox[i];
		_guider=one._guider;
		return (*this);
	}
};

struct _onezerodata{
	bool _istip;	//是否保存记录

	int _chessInfo[CHESS_SIZE_W_1010][CHESS_SIZE_H_1010];	//棋盘信息
	int _level;	//关卡
	int _score;	//分数
	int _target;	//目标
	int _boxshape[3];	//选择方块的形状
	int _boxangle[3];	//可选方块的角度

	bool _guider;	//新手指引

	_onezerodata(){};
	_onezerodata& operator=(const _onezerodata& one){
		_istip=one._istip;
		for(int i=0;i<CHESS_SIZE_W_1010;i++)
			for(int j=0;j<CHESS_SIZE_H_1010;j++)
				_chessInfo[i][j]=one._chessInfo[i][j];
		_level=one._level;
		_score=one._score;
		_target=one._target;
		for(int i=0;i<3;i++)
		{
			_boxshape[i]=one._boxshape[i];
			_boxangle[i]=one._boxangle[i];
		}
		_guider=one._guider;
		return (*this);
	}
};

struct _startdata{
	bool _istip;	//是否保存记录

	int _chessInfo[CHESS_SIZE_W_START][CHESS_SIZE_H_START];	//棋盘信息
	int _level;	//关卡
	int _score;	//分数
	int _target;	//目标
	int _lastlevelscore;	//上一关分数

	int _highestscore;	//最高分
	bool _guider;	//新手指引

	_startdata(){};
	_startdata& operator=(const _startdata &one)
	{
		_istip=one._istip;
		for(int i=0;i<CHESS_SIZE_W_START;i++)
			for(int j=0;j<CHESS_SIZE_H_START;j++)
				_chessInfo[i][j]=one._chessInfo[i][j];
		_level=one._level;
		_score=one._score;
		_target=one._target;
		_lastlevelscore=one._lastlevelscore;
		_highestscore=one._highestscore;
		_guider=one._guider;

		return (*this);
	}
};

struct _angledate{
	bool _istip;	//是否保存记录
	int _chessInfo[CHESS_SIZE_W_ANGLE][CHESS_SIZE_H_ANGLE];	//棋盘信息
	int _level;	//关卡
	int _score;	//分数
	int _target;	//目标
	int _boxshape[3];	//选择方块的形状
	int _highestscore;	//最高分

	bool _guider;	//新手指引

	_angledate(){};
	_angledate& operator=(const _angledate& one){
		_istip=one._istip;
		for(int i=0;i<CHESS_SIZE_W_ANGLE;i++)
			for(int j=0;j<CHESS_SIZE_H_ANGLE;j++)
				_chessInfo[i][j]=one._chessInfo[i][j];
		_level=one._level;
		_score=one._score;
		_target=one._target;
		for(int i=0;i<3;i++)
		{
			_boxshape[i]=one._boxshape[i];
		}
		_guider=one._guider;
		return (*this);
	}
};

struct _data{
	_tetrisdata _challengesstetris;
	_onezerodata _onezero;
	_startdata _start;
	_angledate _angle;

	bool _xinshou;
	int _diamondNumber;		//钻石个数
	int _propnum[4];	//道具个数
	int _hongbaotime;	//红包时间
	int _hongbaonum;	//领取红包次数
	int _starthongbaonum;	//星星模式红包次数
	int _anglehongbaonum;	//六角模式红包次数
	bool _music;	//声音
	bool _xieyi;	//用户协议
	int _qd_day;	//签到的天数
	int _qd_number;	//连续签到的天数

	int _gg_guanshu;	//当天过关的关数

	int _levelreward_num;	//过关奖励的领取次数
	int _day_num;	//上一次登录的天数时间
	bool _freeround;	//免费启动转盘
	int _freeroundday;	//上次免费启动转盘的天数
	int _freeroundnum;	//免费转盘玩的次数
	int _freediamondnum;	//免费钻石点击的次数
	//
	int _huhightnum;	//进度条的最高个数
	int _hunum;	//进度条的当前个数
	int _hupropid;	//奖励的道具
};

class SceneManage;

class GameData:public Ref
{
public:
	GameData():_saving(false){
	};
	~GameData(){
	};

	static GameData* getInstance(){
		static GameData one;
		return &one;
	}

private:
	void dataRead();

public:
	void dataSave();

	void loadSaveFile();

	void dataReset();

	inline static _data* getSaveData()
	{
		return &getInstance()->d_saveData;
	}

public:
	GameProject d_Project;

	//俄罗斯方块
	GameModel d_Model;

	//umeng计时
	time_t d_time;

	//指引模式
	bool _guide;

	//
	int BANER_HEIGHT;

	//现在的时间天数
	int d_daytime;

	//过关奖励
	bool d_levelReward;

	int d_PayType;
private:
	_data d_saveData;

	bool _saving;
};

#define GameDataInstance() GameData::getInstance()

#endif
