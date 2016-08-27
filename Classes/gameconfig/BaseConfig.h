#ifndef BASE_CONGIF_H_
#define BASE_CONGIF_H_

#include"iostream"
#include"ConfigState.h"
#include"cocos2d.h"
#include"../cj/cjconfig.h"
#include "../cj/GameData.h"
using namespace std;
USING_NS_CC;

const Color3B particleColor[10]={
	Color3B(153,51,255),
	Color3B(76,85,255),
	Color3B(255,206,38),
	Color3B(43,217,153),
	Color3B(89,183,255),
	Color3B(255,76,175),
	Color3B(255,63,69),
	Color3B(0,201,80),
	Color3B(255,153,64),
	Color3B(220,64,255),
};

const Color3B angleColor[9]={
	Color3B(217,87,91),
	Color3B(217,147,87),
	Color3B(217,87,160),
	Color3B(217,191,87),
	Color3B(85,211,110),
	Color3B(100,87,217),
	Color3B(87,217,169),
	Color3B(87,160,217),
	Color3B(163,87,217)
};

const string startstr1[5]={
	//"fangkuai_5.png",
	//"fangkuai_8.png",
	//"fangkuai_1.png",
	//"fangkuai_7.png",
	//"fangkuai_3.png",
	"box1-01.png",
	"box2-01.png",
	"box3-01.png",
	"box4-01.png",
	"box5-01.png"
};

const string startstr2[5]={
	//"fangkuai_5_01.png",
	//"fangkuai_8_01.png",
	//"fangkuai_1_01.png",
	//"fangkuai_7_01.png",
	//"fangkuai_3_01.png",
	"box1-02.png",
	"box2-02.png",
	"box3-02.png",
	"box4-02.png",
	"box5-02.png"
};

const float scorevideotime=2.0f;//得分音效容许的间断时间
const float sprintshowtime=1.5f;	//精灵动画时间
static int levelupmusic=0;	//升级音效
const float boxgraytime=0.03f;	//方块变灰速度

const float startnexttime=0.1f;	//填充方块的移动时间
const int nextboxoffest=150;
const int nextboxbegin=80;	
const int boxtofiger=60;	//方块距离手指的距离
const float propsheight=650;
const float liuxingtime=0.6f;	//流星雨下落时间
const float levelupscale = 1.0f;	//关数字体大小
//class Base

//过关动画
static void LevelUpMotion(int level, int target)
{
	if(level==1)
	{
		cjMusic::playEffect("video/ReadyGo.mp3");
	}

	char name1[30];
	sprintf(name1,cjTTFLabel::getStringByKey("dijiguan").c_str(),level);
	auto fnt1=LabelTTF::create(name1,TTF_PATH,60);
	Director::getInstance()->getRunningScene()->addChild(fnt1,3);
	fnt1->setPosition(Vec2(WINSIZE/2.0f)+Vec2(0,-100));
	fnt1->runAction(CCFadeIn::create(0.3f));
	fnt1->runAction(Sequence::create(CCMoveBy::create(0.3f,Vec2(0,200)),DelayTime::create(1.1f),FadeOut::create(0.3f),nullptr));

	char name2[60];
	sprintf(name2,cjTTFLabel::getStringByKey("mubiao").c_str(),target);
	auto fnt2=LabelTTF::create(name2,TTF_PATH,50);
	Director::getInstance()->getRunningScene()->addChild(fnt2,3);
	fnt2->setPosition(fnt1->getPosition()-Vec2(0,100));
	fnt2->runAction(CCFadeIn::create(0.3f));
	fnt2->runAction(Sequence::create(CCMoveBy::create(0.3f,Vec2(0,200)),CCDelayTime::create(0.2f),ScaleTo::create(0.2f,1.2f),ScaleTo::create(0.2f,1.0f),CCDelayTime::create(0.5f),CCFadeOut::create(0.3f),
		CallFunc::create([fnt1,fnt2](){
			fnt1->removeFromParent();
			fnt2->removeFromParent();
	}),nullptr));
}


typedef enum _BoxState_
{
	STATE_01,
	STATE_02,
}BoxState;

//星星棋子
class StartBox:public cocos2d::Node
{
public:
	StartBox(){};
	~StartBox(){};

	static StartBox* createBox(int color)
	{
		StartBox *box=new (std::nothrow)StartBox();
		if(box&&box->init(color))
		{
			box->autorelease();
			return box;
		}
		else
		{
			CC_SAFE_DELETE(box);
			return nullptr;
		}
	}

	virtual bool init(int color);
	void setBox2();
	void setBox1();

public:
	int _color;

	BoxState _state;
};
//


#endif