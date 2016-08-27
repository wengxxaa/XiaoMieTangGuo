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
	////�Ƿ���Դ�A��B
	//bool getAToB(PointTip begintip,PointTip endtip,std::map<int,int> chessinfo);

	//��A��B��·��
	const PointManage* pathAToB(PointTip begintip,PointTip endtip,int chessinfo[CHESS_SIZE_W][CHESS_SIZE_H]);

	inline bool getJudge() const {return _canGet;}

	inline vector<PointTip> getData() const {return _BoxMoveVec;}

private:
	//���ݳ�ʼ��
	void dataInit(PointTip begintip,PointTip endtip,int chessinfo[CHESS_SIZE_W][CHESS_SIZE_H]);

	//A��B������
	void dataProces();

	//
//	bool boxMove(PointTip recenttip,int step);

	bool boxMoveProcessOne(PointTip recenttip,int step=1);

	bool isTheTop(PointTip recenttip);
	//��ת90��,180,270
//	BoxMoveResult BoxRotate(PointTip recenttip,int rotate);
	//������ת
	BoxMoveResult ReBoxRotate(PointTip recenttip,int key=1);//1.˳ʱ����ת��3.��ʱ����ת
	//�ƶ�
	BoxMoveResult BoxMove(PointTip recenttip,pair<int,int> move);
	
	//·���Ƿ��ظ��ж�
	bool BoxPathVecJudge(PointTip fronttip,PointTip nowtip,int step);

	//��·����������ȡ����·��
//	void getPathByVec();
	void getPathByVecOne();

	bool topData();

	vector<std::pair<int,int>> BoxFillOffest(vector<std::pair<int,int>> vec,pair<int,int> offest);

private:
	//�����ƶ��Ĺ���
	vector<PointTip> _BoxMoveVec;

	//�Ƿ���Գɹ��Ƶ�B��
	bool _canGet;

	//�����ƶ�������·��
	vector<BoxMovePath> _BoxPathVec;

	//��ʼ��
	PointTip _beginTip;
	//Ŀ���
	PointTip _endTip;
	//��ǰ��ͼ��״̬
	std::map<int,int> _chessFilInfo;

	//����
	vector<pair<int,PointTip>> _theTopTipVec;

	//
	PointTip _theTopTip;
	int _theTopStep;
	std::map<int,int> _toph;

};

#endif