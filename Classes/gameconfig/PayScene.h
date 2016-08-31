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

	//限时优惠1
	void payPage1();

	//商城
	void payPage2();

	//限时大礼包
	void payPage3();

	//新手礼包
	void payPage4();

	//每日签到
	void dailyClick();

	//帮助
	void help();

	//关于我们
	void aboutus();

	//每日签到提示
	void dailyClickTip(int number,int key=1);

	//过关奖励
	void levelReward();

	//免费钻石提示
	void freeDiamond();

	//大转盘
	void turnTable();

	void tableCallBack(Node* csb);

	void tableReward(int key);


	void menuchange();

	void turnMotion(float ft);

	void turnTip();
    
	//钻石向上飞
	void turnAddDiamond(Layer *colorlay, int num, ui::TextBMFont *diamondnum, Vec2 position);
	void turnAddDiamond(Layer *colorlay, int num,Vec2 position);

	//免费支付
	void paysuccess(int key);

	//钻石雨
	void diamondRain(Layer *lay);

	//烟花
	void yanhua(Layer *lay);

	//得到物品的效果
	void parShow(Layer *lay, int level, Vec2 pos);
    
    void addDiamondPlist(Layer *lay,int level);
    
    void openTip();
    
    void closeTip();
    
private:
    
    Menu* zuanshi;

	int _libakey;

	bool _canTouch;

	int _rewardid;

	Sprite *_sp[8];		//转盘的8个sp
};

#endif