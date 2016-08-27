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
	void Resurrection();

private:
	bool _selectExit[3];

	ClippingNode *_clip;	//�ü�����

	int _touchselect;	//ѡ�еķ���

	vector<bool> _chessInfo;	//������Ϣ
	vector<int> _chessSaveInfo;	//���̴�����Ϣ

	pair<int,int> _chesstippos;	//������ʾ�ķ����λ��

	bool faildJudge();

	void gameFaild();

	//��¼����
	void dataSave(bool tt=true);
	//
	bool _firstSaveIn;	//��һ�α������
	_onezerodata sdata;	//���������

	float _scoretime;	//�÷�ʱ��
	int  _scorewav;	//�÷���Ч

	//����ָ��
	bool _guide;

	int selectboxoffest;
private:
	//��ʼ������
	void initChess();

	void startNewRound(int key=1);	//�����µ�һ��ѡ��

	float chessCheck();	//�����������

	float eliminateAnimation(const std::vector<int> &vx,const std::vector<int> &vy);	//��������

	vector<pair<int,int>> getFillBlack(BoxNode* box,Vec2 pt);

	Vec2 positionByPos(pair<int,int> pos);	//�������λ��
	pair<int,int> posByPosition(Vec2 position);	//��λ�õ�����

	void chessFill(ShapeIndex shape,vector<pair<int,int>> vec);	//��䷽��

	bool chessFailedJudge();	//ʧ���ж�

	void spriteShow(const string& str);

	void addParticle(int x,int y,int shapeid,int type);

	void removeBox(pair<int,int> pos);

	void showTipBox(BoxNode *box);	//��ʾ��ʾ�ķ���

	void removeTipBox();	//�Ƴ���ʾ�ķ���

	//��������
	void scoreMotion(int score,pair<int,int> pt);

	//��ʱ
	void timeUpdate(float ft);

	bool selectBoxExit(ShapeIndex shape,AngleIndex angle=ANGLE_0);	//�ɹ�ѡ�����Ƿ����XX

public:
	CC_SYNTHESIZE(TouchState,_touchState,touchState);

	void Rotate(int key=1);	//���ָ���ѡ��box

private:
	//�ؿ�����
	bool levelUp();

	//�ؿ���������
	void LevelUpMotion();

	//�ؿ�������������
	void LevelUpEliminate();

	//�õ���������������
	vector<pair<int,int>> getLevelUpBox();

	//���������ж�
	void BoxFillJudge();

	//�����ж�
	bool BoxJudge(BoxNode *node);
	
	//�϶�ģʽָ��
	void dragGuide();

};

#endif