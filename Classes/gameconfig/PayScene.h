#ifndef PayScene_H_
#define PayScene_H_

#include"stdio.h"
#include"cocos2d.h"
USING_NS_CC;
#include"cocos-ext.h"
USING_NS_CC_EXT;

#include "vigame_ad.h"
#include "ui/UITextBMFont.h"

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
    
	//��ʯ���Ϸ�
	void turnAddDiamond(Layer *colorlay, int num, ui::TextBMFont *diamondnum, Vec2 position);
	void turnAddDiamond(Layer *colorlay, int num,Vec2 position);

	//���֧��
	void paysuccess(int key);

	//��ʯ��
	void diamondRain(Layer *lay);

	//�̻�
	void yanhua(Layer *lay);

	//�õ���Ʒ��Ч��
	void parShow(Layer *lay, int level, Vec2 pos);
    
    void addDiamondPlist(Layer *lay,int level);
    
    void openTip();
    
    void closeTip();
    
private:
    
    Menu* zuanshi;

	int _libakey;

	bool _canTouch;

	int _rewardid;

	Sprite *_sp[8];		//ת�̵�8��sp
};

#endif