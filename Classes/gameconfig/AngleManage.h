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
	CC_SYNTHESIZE(int, _diamondnumber, Diamond);	//��ʯ��

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

	void Rotate(int key=1);	//���ָ���ѡ��box
	void Resurrection();

private:
	void initChess();

	void initData();

	//��ʼ������
	void initProps();

	Vec2 posToposition(pair<int,int> pos);
	pair<int,int> positionTopos(Vec2 position);

	void startNewRound(int key=1);	//�����µ�һ��ѡ��

	void showTipBox(AngleNode *box);	//��ʾ��ʾ�ķ���
	void removeTipBox();	//�Ƴ���ʾ�ķ���

	vector<pair<int,int>> getFillBlack(AngleNode* box,Vec2 pt);

	//��䷽��
	void chessFill(int shape,vector<pair<int,int>> vec);	

	//��������
	void scoreMotion(int score,pair<int,int> pt);

	//���������ж�
	void BoxFillJudge();

	//�����ж�
	bool BoxJudge(AngleNode *node);

 //	//���ط���
	//int getTargetScoreByRound(int round);
	////�������ӷ���
	//int getBoxScoreByRound(int round);

	void eliminateAnimation(const std::vector<int> &vx,const std::vector<int> &vy,const std::vector<int> &vz);	//��������

	bool chessCheck();

	void addParticle(int x,int y,int shapeid,int type);

	void removeBox(pair<int,int> pos);

	bool faildJudge();

	bool chessFailedJudge();	//ʧ���ж�

	void gameFaild();

	//�ؿ�����
	bool levelUp();

	//�ؿ���������
	void LevelUpMotion();

	//�ؿ�������������
	void LevelUpEliminate();

	void spriteShow(const string& str);

	//�õ���������������
	vector<pair<int,int>> getLevelUpBox();

	void propMotion(int id);

	void useProp2(pair<int,int> pos);

	void firstSet(int id);

	//ָ��
	void dragGuide();

	//��¼����
	void dataSave(bool tt=true);
	//
	bool _firstSaveIn;	//��һ�α������
	_angledate sdata;	//���������

	//����csb�ļ�������
	pair<int,int> analyzeName(const string &name);

	inline int hongBaoLevel(){return 6-((_level%5)?_level%5:5);}

	void hongbaoFlush(float ft);

	//���
	void hongbaoPage();

	//�����ť����¼�
	void buttonClick(Ref* pSender, ui::Widget::TouchEventType touchType);

	//�������
	void hongbaoReward();

	//������
	void startRain();

	//�����������
	void endTouch();

private:
	bool _chessBlack[CHESS_SIZE_W_ANGLE][CHESS_SIZE_H_ANGLE];
	int _chessColor[CHESS_SIZE_W_ANGLE][CHESS_SIZE_H_ANGLE];

	bool _selectExit[3];//��ѡ��ķ���

	bool _guide;

	int _touchselect;

	pair<int,int> _chesstippos;	//������ʾ�ķ����λ��

	int _selectprop;

	LabelAtlas *_diamondFont;

	////boxscore
	//int _boxscore;

	int selectboxoffest;

	int _shandianid;	//������Чid

	bool _hongbaoclick;

	//���߸���
	Label *_propnum[3];

	LabelTTF *_hongbaotip;	//�����ʾ
};

#endif