#ifndef POINTMANAGE_H_
#define POINTMANAGE_H_

#include "BoxNode.h"
#include "cocos2d.h"
#include "ConfigState.h"
USING_NS_CC;

struct BoxMovePath{
	PointTip FrontTip;
	PointTip NowTip;
	int step;

	BoxMovePath(PointTip a,PointTip b,int c)
	{
		FrontTip=a;
		NowTip=b;
		step=c;
	};
};

struct BoxMoveResult{
	bool result;
	PointTip newTip;

	BoxMoveResult(bool re,PointTip nt)
	{
		result=re;
		newTip=nt;
	};
};

class PointManage{
public:
	PointManage(){};
	~PointManage(){};

	static PointManage* getInstance()
	{
		static PointManage one;
		return &one;
	}
public:
	////是否可以从A到B
	//bool getAToB(PointTip begintip,PointTip endtip,std::map<int,int> chessinfo);

	//从A到B的路径
	const PointManage* pathAToB(PointTip begintip,PointTip endtip,int chessinfo[CHESS_SIZE_W][CHESS_SIZE_H]);

	inline bool getJudge() const {return _canGet;}

	inline vector<PointTip> getData() const {return _BoxMoveVec;}

private:
	//数据初始化
	void dataInit(PointTip begintip,PointTip endtip,int chessinfo[CHESS_SIZE_W][CHESS_SIZE_H]);

	//A到B的运算
	void dataProces();

	//
//	bool boxMove(PointTip recenttip,int step);

	bool boxMoveProcessOne(PointTip recenttip,int step=1);

	bool isTheTop(PointTip recenttip);
	//旋转90度,180,270
//	BoxMoveResult BoxRotate(PointTip recenttip,int rotate);
	//反向旋转
	BoxMoveResult ReBoxRotate(PointTip recenttip,int key=1);//1.顺时针旋转，3.逆时针旋转
	//移动
	BoxMoveResult BoxMove(PointTip recenttip,pair<int,int> move);
	
	//路径是否重复判断
	bool BoxPathVecJudge(PointTip fronttip,PointTip nowtip,int step);

	//从路径集合中提取最终路径
//	void getPathByVec();
	void getPathByVecOne();

	bool topData();

	vector<std::pair<int,int>> BoxFillOffest(vector<std::pair<int,int>> vec,pair<int,int> offest);

private:
	//方块移动的过程
	vector<PointTip> _BoxMoveVec;

	//是否可以成功移到B点
	bool _canGet;

	//方块移动经过的路径
	vector<BoxMovePath> _BoxPathVec;

	//初始点
	PointTip _beginTip;
	//目标点
	PointTip _endTip;
	//当前地图的状态
	std::map<int,int> _chessFilInfo;

	//顶点
	vector<pair<int,PointTip>> _theTopTipVec;

	//
	PointTip _theTopTip;
	int _theTopStep;
	std::map<int,int> _toph;

};

#endif