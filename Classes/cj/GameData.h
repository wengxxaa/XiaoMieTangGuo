#ifndef GameData_H_
#define GameData_H_

#include"fstream"
#include"../gameconfig/ConfigState.h"
#include"cocos2d.h"
USING_NS_CC;

struct _tetrisdata{
	bool _istip;	//�Ƿ񱣴��¼

	int _chessInfo[CHESS_SIZE_W][CHESS_SIZE_H];	//������Ϣ
	int _level;	//�ؿ�
	int _score;	//����
	int _targetscore;
	int _recentbox;	//��ǰ����
	int _recentposx;
	int _recentposy;	//��ǰλ��
	int _nextbox[3];	//��һ������

	bool _guider;	//����ָ��

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
	bool _istip;	//�Ƿ񱣴��¼

	int _chessInfo[CHESS_SIZE_W_1010][CHESS_SIZE_H_1010];	//������Ϣ
	int _level;	//�ؿ�
	int _score;	//����
	int _target;	//Ŀ��
	int _boxshape[3];	//ѡ�񷽿����״
	int _boxangle[3];	//��ѡ����ĽǶ�

	bool _guider;	//����ָ��

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
	bool _istip;	//�Ƿ񱣴��¼

	int _chessInfo[CHESS_SIZE_W_START][CHESS_SIZE_H_START];	//������Ϣ
	int _level;	//�ؿ�
	int _score;	//����
	int _target;	//Ŀ��
	int _lastlevelscore;	//��һ�ط���

	int _highestscore;	//��߷�
	bool _guider;	//����ָ��

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
	bool _istip;	//�Ƿ񱣴��¼
	int _chessInfo[CHESS_SIZE_W_ANGLE][CHESS_SIZE_H_ANGLE];	//������Ϣ
	int _level;	//�ؿ�
	int _score;	//����
	int _target;	//Ŀ��
	int _boxshape[3];	//ѡ�񷽿����״
	int _highestscore;	//��߷�

	bool _guider;	//����ָ��

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
	int _diamondNumber;		//��ʯ����
	int _propnum[4];	//���߸���
	int _hongbaotime;	//���ʱ��
	int _hongbaonum;	//��ȡ�������
	int _starthongbaonum;	//����ģʽ�������
	int _anglehongbaonum;	//����ģʽ�������
	bool _music;	//����
	bool _xieyi;	//�û�Э��
	int _qd_day;	//ǩ��������
	int _qd_number;	//����ǩ��������

	int _gg_guanshu;	//������صĹ���

	int _levelreward_num;	//���ؽ�������ȡ����
	int _day_num;	//��һ�ε�¼������ʱ��
	bool _freeround;	//�������ת��
	int _freeroundday;	//�ϴ��������ת�̵�����
	int _freeroundnum;	//���ת����Ĵ���
	int _freediamondnum;	//�����ʯ����Ĵ���
	//
	int _huhightnum;	//����������߸���
	int _hunum;	//�������ĵ�ǰ����
	int _hupropid;	//�����ĵ���
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

	//����˹����
	GameModel d_Model;

	//umeng��ʱ
	time_t d_time;

	//ָ��ģʽ
	bool _guide;

	//
	int BANER_HEIGHT;

	//���ڵ�ʱ������
	int d_daytime;

	//���ؽ���
	bool d_levelReward;

	int d_PayType;
private:
	_data d_saveData;

	bool _saving;
};

#define GameDataInstance() GameData::getInstance()

#endif
