#ifndef CJCONFIG_H_
#define CJCONFIG_H_

#include "SimpleAudioEngine.h"
#include"../gameconfig/ConfigState.h"
#include "core/macros.h"
#include"core/base/log.h"
#include"cocos2d.h"
#include"stdio.h"
USING_NS_CC;

static const float FntXOff=0.0f;
static const float FntYOff=0.0f;

#define WINORIGIN (Director::getInstance()->getVisibleOrigin())	
#define WINSIZE (Director::getInstance()->getVisibleSize())

#define CREATE_SCENE(__TYPE__) \
	static Scene* createScene() \
{ \
	Scene *scene=Scene::create(); \
	__TYPE__ *layer=__TYPE__::create();\
	scene->addChild(layer,1,"mainlayer");\
	return scene;\
}

#define  LOGGAME(...)  log("GameLog", __VA_ARGS__);

class cjMenu:public Menu
{
public:
	//create with image
	static Menu* createWithImage(const std::string &str1,const std::string &str2,const std::string &str3,const ccMenuCallback &call,bool tt=true);

	static Menu* createWithImage(const std::string &str,const ccMenuCallback &call,bool tt=true);

	static Menu* createWithImage2(const std::string &str,const ccMenuCallback &call,bool tt=true);

	//create with sprite
	static Menu* createWithSprite(Sprite* normalSp,Sprite* selectedSp, const ccMenuCallback &call);

	//create with font
	static Menu* createWithFont(const std::string &fontstring,const ccMenuCallback &call);

public:
	virtual void addChild(Node * child, int zOrder);
};

class cjTTFLabel:public Label
{
public:
	cjTTFLabel(){};
	~cjTTFLabel(){};
public:
	//create with TTF
	static Label* createWithString(const std::string &text,int size=20,const std::string textpath=TTF_PATH);

	static Label* createWithInt(const int number,int size=20,std::string textpath=TTF_PATH);

	static string getStringByKey(const char *str);

	static string getNameByInt(const char*str,int id);
	static string getNameByIntL(const char*str,int id);
};

class cjAtlasLabel:public Label
{
public:
	cjAtlasLabel(){};
	~cjAtlasLabel(){};
public:
	static LabelAtlas* createWithInt(const int number,std::string textpath=ATLAS_PATH);
};

class cjSchedule
{
public:
	cjSchedule(){};
	~cjSchedule(){};

	static cjSchedule* getInstance()
	{
		static cjSchedule one;
		return &one;
	}

public:
	void runOnNextFrame(const ccSchedulerFunc &call);

	//ÆÁÄ»Õð¶¯
	void shockBegin(Layer *lay);
	void shockEnd(Layer *lay);

	//jnitext
	void jnitext(const string& str1,const string &str2);
};

class cjSprite:public Sprite{
public:
	static Sprite* createWithImage(const std::string &str1); 
};

class cjMusic
{
public:
	static void playBackgroundMusic(const char *str,bool loop=false);

	static void stopBackgroundMusck()
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	}

	static int playEffect(const char *str,bool loop=false);

	static void stopEffect(int id);

	static void stopAllEffect()
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
	}
};

class handNode:public Node
{
public:
	handNode(){};
	~handNode(){};

	static handNode* createHand(float ft)
	{
	 	handNode *one=new handNode();
		if(one&&one->init())
		{
			one->autorelease();
			one->setScale(ft);
			return one;
		}
		else 
		{
			CC_SAFE_DELETE(one);
			return nullptr;
		}
	}

	bool init();
};

#endif
