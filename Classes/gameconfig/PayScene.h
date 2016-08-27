#ifndef PayScene_H_
#define PayScene_H_

#include"cocos2d.h"
USING_NS_CC;
#include"cocos-ext.h"
USING_NS_CC_EXT;

#include "vigame_ad.h"

class PayScene
{
public:
	static PayScene* getInstance()
	{
		static PayScene one;
		return &one;
	}

public:

	//��ʱ�Ż�1
	void payPage1();

	//�̳�
	void payPage2();

	//��ʱ�����
	void payPage3();

	//�������
	void payPage4();

	//ÿ��ǩ��
	void dailyClick();

	//����
	void help();

	//��������
	void aboutus();

	//ÿ��ǩ����ʾ
	void dailyClickTip(int number,int key=1);

	//���ؽ���
	void levelReward();

	//�����ʯ��ʾ
	void freeDiamond();

	//��ת��
	void turnTable();

	void tableCallBack(Node* csb);

	void tableReward(int key);

	void menuchange();

	void turnMotion(float ft);

	void turnTip();
    
    void turnAddDiamond(int num);

	//���֧��
	void paysuccess(int key);

	//��ʯ��
	void diamondRain(Layer *lay);

	//�õ���Ʒ��Ч��
	void parShow(Layer *lay, int level, Vec2 pos);
    
    void addDiamondPlist(Layer *lay,int level);


private:
    
    Menu* zuanshi;

	int _libakey;

	bool _canTouch;

	int _rewardid;

	Sprite *_sp[8];		//ת�̵�8��sp
};

#endif