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
	//����
	void setmStepNumber(int stepnumber) { _stepnumber = stepnumber; }
	int getmStepNumber() { return _stepnumber; }
	//���Ʒ���
	ShapeIndex getNextBox(int i) { return _nextBox[i]; }
	//run
	void run();
	void pause();
	void start();
	void Resurrection();	//����
public:
	//һ�����鿪ʼ����
	void startDropBox();

	//���䱣��ķ���
	void startSaveBox();

	//���ɽ�Ҫ���ֵķ���
	void initNextBox();
	void createNextBox();

	//���������жϻص�
	void boxDropUpdate(float ft);

	//���������˶�һ��
	bool boxDropOneLine();

	//����λ��ȷ�����Ƴ�
	bool boxSure();

	//�����Ƿ�����ƶ��ж�
	bool boxMoveJudge(DirectionIndex dir,Vec2 vec=Vec2(0.0f,0.0f));

	//����ʱ��
	void timeUpdate(float ft);

	//�������
	void gameFiled();
	bool checkChess();

	//����任
	bool boxChange(int key=1);//1.˳ʱ����ת 3.��ʱ����ת

	//��������λ�õ���ʾ
	void boxDropTip(float ft);
	//�㴥ģʽ����λ�õ���ʾ
	void boxDropTip();

	//���淽��
	void boxSave();

	void Rotate();	//����ڷű任


	//ȡ��ǰ����ĸ���λ��
	vector<std::pair<int,int>> getBoxFill(Vec2 vec=Vec2(0.f,0.f));

	vector<std::pair<int,int>> getBoxFill(PointTip tip);

	//�Ż�����λ��
	Vec2 chessPos(int x,int y);

	//ָ��λ���ж�
	int PointTipJudge(PointTip pt);

	void PointTipRemove();	//���ݵ�ǰ���������λ��ɾ����ʾ��

	//�õ��޷����Ŀո���
	int getBlankOfUnFill();
	//����һ��ƫ����
	int getBlankOfUnFill(vector<pair<int,int>> map);

	//�õ���ǰ�����������������һ��ƫ������
	int getRemoveRow(PointTip pt);

	bool isBlankOfUnFill(pair<int,int> vec);

	bool tempUnFillJudeg(pair<int,int> vec);

	bool canPutIn(PointTip tip);

	//���Ѵ��ڵ�·���в��ң������ٴεݹ�����
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

	vector<int> _moveh;//����ĸ߶�

	CC_SYNTHESIZE(TouchState,_touchstate,TouchState);	//�Ƿ���ܻ���
	//CC_SYNTHESIZE(ChessDropMove,_movestate,MoveState);	//�Ƿ��������
	CC_SYNTHESIZE(Vec2,_lastPos,LostTouchPos);	//�ϴε����λ��

	CC_SYNTHESIZE(int, _time,mTime);	//ʱ��
	CC_SYNTHESIZE(bool,_boxsave,mBoxSave);	//�����Ƿ���Դ���

	inline int getTipNumber(){return _recentPointNumber;};

	//��¼����
	void dataSave(bool tt=true);

private:
	ShapeIndex _recentBox;    //��ǰ����
	int _saveBox;		//����ķ���

	bool _firstSaveIn;	//��һ�α������
	_tetrisdata sdata;	//���������

	std::pair<int,int> _recentPos;	//��ǰ�����λ��

	int _chessFilInfo[CHESS_SIZE_W][CHESS_SIZE_H];	//���̵������Ϣ
	int _chessSaveInfo[CHESS_SIZE_W][CHESS_SIZE_H];	//����Ҫ�洢����Ϣ

	map<int,vector<PointTip>> _pointPath;

	//�Ƿ��ƶ���
	bool _touchbool;
	//�Ƿ�����ƶ�
	bool _touchcan;

	ClippingNode *_clip;	//�ü�����

	GameModel _model;	//��Ϸģʽ

	EventListenerTouchOneByOne *listener;

	//PointģʽTip����
	vector<PointTip> _pointTipVec;
	//PointģʽTip��������
	map<int,vector<int>> _pointTipPair;
	//Pointģʽ��������д洢��ʱλ��
	vector<pair<int,int>> _pointTipPairTemp;
	vector<int> _pointTipPriority;

	vector<pair<int,int>> _tempUnFillVec;	//�����ո���ʱ����ʱ����

	//��ǰPointģʽ��ʾ������
	int _recentPointNumber;

	Layer *_PointTipLayer;	//Pointģʽ��ʾ��

	//�㴥����ķ���
	int _touchbox;

	std::map<int,int> _toph;
	set<pair<int,int>> _BlankOfUnFillVec;

	float _touchtime;

	vector<PointTip> _noCanPutIn;//��Ų�������ķ���
	bool getVecFromNoPutIn(PointTip tip);

	//
	float _scoretime;	//�÷�ʱ��
	int  _scorewav;	//�÷���Ч

	//��������ʱ��
	float _upboxtime;	//�������ķ���ʱ��
	bool _canup;	//�Ƿ����������


	int heightoffest;
private:
	//��ʼ������
	void initChess();

	void timeSchedule(float ft);

	//������������ʾ
	void textShow(const string& str,int size,Vec2 pos=WINSIZE/2.0f);

	//������ʾ
	void spriteShow(const string& str);

	//������
	void addParticle(int x,int y,int shapeid,int type=1);

	//�Ƴ�����
	void removeBox(pair<int,int> pos);

	//�ؿ�����
	bool levelUp();

	//�ؿ���������
	void LevelUpMotion();

	//�ؿ�������������
	void LevelUpEliminate();

	//��������
	void scoreMotion(int score,pair<int,int> pt);

	//�õ��������������ĵ�
	vector<pair<int,int>> getLevelUpBox();

	float rollEliminate(vector<int> vec);	//������

	//��ʾģʽ
	bool _guide;
	int _guidenumber;

	//����ģʽָ��
	void classesGuide();


	//��սģʽָ��
	void challengeGuide();

public:
	//�ж���ʾ��λ�úͼ��
	bool pointTipPairTempJudge(int pairid);

	//��ʾPointģʽ��ʾ
	void showTipWithId(int pairid);
	void showTipWithBox(PointTip tip);
	void moveBoxToId(int boxid);

	//inline int getChessSaveInfo(int x,int y){return _chessSaveInfo[x*CHESS_SIZE_H+y];}
	inline int getRecentBox(){return int(_recentBox);}
	inline std::pair<int,int> getRecentPos(){return _recentPos;};
};

#endif