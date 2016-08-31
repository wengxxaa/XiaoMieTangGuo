#include "AppDelegate.h"
//umeng
#include "MobClickCpp.h"
#include "gameconfig/ConfigState.h"
#include "gameconfig/PayScene.h"
#include "cj/cjconfig.h"
#include "cj/GameData.h"
#include "cj/Toast.h"
#include "gamescene/MainScene.h"
#if(CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
//#include "vld.h"
#endif

#include "vigame_ad.h"
#include "vigame_core.h"
#include "vigame_pay.h"
#include "vigame_tj.h"


USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 800);
static cocos2d::Size smallResolutionSize = cocos2d::Size(320, 480);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(768, 1024);
static cocos2d::Size largeResolutionSize = cocos2d::Size(1536, 2048);
AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages()
{
    return 0; //flag for packages manager
}
bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("My Game", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("My Game");
#endif
        director->setOpenGLView(glview);
    }

	CCLOG("step01");

//	LOGGAME("begin");

	//
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    auto data=FileUtils::getInstance()->getValueMapFromFile("umeng_ios.plist");
#else
    auto data=FileUtils::getInstance()->getValueMapFromFile("umeng.plist");
#endif

	auto umengdata=data.at("umengNumber").asString();

	LOGGAME(umengdata.c_str());

	MOBCLICKCPP_START_WITH_APPKEY(umengdata.c_str());
#endif
	//
#if(CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
	GameDataInstance()->BANER_HEIGHT=68;
#else
	GameDataInstance()->BANER_HEIGHT=(Director::getInstance()->getWinSize().width/320.0f)*50*(800.0f/Director::getInstance()->getWinSize().height);	//baner的高�?
#endif

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // Set the design resolution
	glview->setDesignResolutionSize(480, 800, ResolutionPolicy::EXACT_FIT);
	glview->setFrameSize(480, 800);
	//auto frameSize = glview->getFrameSize();
    //// if the frame's height is larger than the height of medium size.
    //if (frameSize.height > mediumResolutionSize.height)
    //{        
    //    director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    //}
    //// if the frame's height is larger than the height of small size.
    //else if (frameSize.height > smallResolutionSize.height)
    //{        
    //    director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    //}
    //// if the frame's height is smaller than the height of medium size.
    //else
    //{        
    //    director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    //}

	srand(time(NULL));
    register_all_packages();

	//ǩ�����
	if (!vigame::signature::check())
	{
		return false;
	}

//	LOGGAME("loadfile");
	//
	GameData::getInstance()->loadSaveFile();
	//

#ifdef WIN32
	glview->setFrameZoomFactor(0.75f);
#endif

//	LOGGAME("beginRun");
	// run

	GameDataInstance()->main_num = 0;

	GameDataInstance()->d_daytime=(time(NULL)/(24*3600));

	//重新的一天
	if(GameDataInstance()->d_daytime!=GameData::getSaveData()->_day_num)
	{
		GameData::getSaveData()->_day_num=GameDataInstance()->d_daytime;
		GameData::getSaveData()->_levelreward_num=0;

		GameData::getSaveData()->_gg_guanshu=0;

		GameData::getSaveData()->_freeroundnum = 0;
		GameData::getSaveData()->_freediamondnum = 0;
	}

	if(GameDataInstance()->d_daytime!=GameData::getSaveData()->_freeroundday)
	{
		GameData::getSaveData()->_freeroundday=GameDataInstance()->d_daytime;
		GameData::getSaveData()->_freeround=true;
	}

	if(GameDataInstance()->d_daytime!=GameData::getSaveData()->_qd_day)
	{
		if((GameDataInstance()->d_daytime-GameData::getSaveData()->_qd_day)!=1||(GameData::getSaveData()->_qd_number==7))
		{
			GameData::getSaveData()->_qd_number=0;
			GameData::getInstance()->dataSave();
		}
	}

	//������Դ�����ļ�
//	FileUtils::getInstance()->addZpkFile("001.zpk");
//	FileUtils::getInstance()->addZpkFile("002.zpk");

	//���ûص�����Ϸ�������߳�
	vigame::Thread::setOnRequestAppMainThreadCallback([](std::function<void()> function2)
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]()
		{
			function2();
		});
	});

	//MMЭ������仯ʱ�ص�
	vigame::MMChnlManager::getInstance()->addMMChnlChangedListener([](vigame::MMChnl* mmChnl)
	{

	});

	//֧����Ϣ�仯�ص�
	vigame::pay::PayManager::setPayFeeInfoChangedCallback([]()
	{
	});

	//����������ӽ�һص�
	vigame::ad::ADManager::setAddGameCoinCallback([](vigame::ad::ADSourceItem* adSourceItem, int type, int num, std::string reson)
	{
	});

	//��ʼ��
	vigame::pay::PayManager::init();
	vigame::CoreManager::init();
	vigame::ad::ADManager::init();
	vigame::tj::DataTJManager::init();

	vigame::pay::PayManager::setOnPayFinishCallback([](vigame::pay::PayParams payParams) {
		int result = payParams.getPayResult();
//umeng
		umeng::eventDict dict;
		switch (result) {
		case vigame::pay::PAY_RESULT_CANCEL:
			Toast::makeText("PAY_RESULT_CANCEL")->show();
			break;
		case vigame::pay::PAY_RESULT_SUCCESS:
			GameData::getSaveData()->_diamondNumber += PAY_DIAMOND[payParams.getPayId()];

			char idname[10];
			sprintf(idname, "%d", payParams.getPayId());
			//umeng
			dict["level"] = idname;
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng
			umeng::MobClickCpp::event("PayId", &dict);
#endif

			if (payParams.getPayId() == 6)
			{
				for (int i = 0; i<4; i++)
					GameData::getSaveData()->_propnum[i] += 1;
			}
			else if ((payParams.getPayId() == 0) || (payParams.getPayId() == 7))
			{
				GameData::getSaveData()->_xinshou = false;
			}

			cjMusic::playEffect("video/coinsin.mp3");

		GameData::getInstance()->dataSave();
			Toast::makeText("PAY_RESULT_SUCCESS")->show();
			break;
		case vigame::pay::PAY_RESULT_FAIL:
			Toast::makeText("PAY_RESULT_FAIL")->show();
			break;
		case vigame::pay::PAY_RESULT_OTHER:
			Toast::makeText("Unknown")->show();
			break;
		}
	});

	vigame::ad::ADManager::openAd("splash");
	
	Device::setKeepScreenOn(true);

	preLoad();
	director->runWithScene(MainScene::createScene());

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng
	umeng::MobClickCpp::applicationDidEnterBackground();
#endif

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng
	umeng::MobClickCpp::applicationWillEnterForeground();
#endif

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

//
void AppDelegate::onPayFeeInfoChanged()
{
	//CCLOG("onPayFeeInfoChanged!");
}

void AppDelegate::onMMChnlChanged(int onChangedCount){

}

void AppDelegate::onAdReady()
{

}

void AppDelegate::preLoad()
{
    CCLOG("preLoad");
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("main/tupian.plist","main/tupian.png");
    
    std::string lineNameArray[] = { "red", "yellow", "blue", "green", "purple" };
    for(int i=0;i<5;i++)
    {
        auto path="animation/daoju_images/skill4_" + lineNameArray[i] + ".png";
        auto spriteframe=SpriteFrame::create(path,Rect(0,0,50,60));
        SpriteFrameCache::getInstance()->addSpriteFrame(spriteframe,path);
    }
    
    auto file=FileUtils::getInstance()->getDataFromFile("video/LIST.TXT");
    auto data=file.getBytes();
    auto size=file.getSize();
    vector<string> name;
    string na="video/";
    for(int i=0;i<size;i++)
    {
        if(data[i]=='\r')
        {
            name.push_back(na);
        }
        else if(data[i]=='\n')
        {
            na.clear();
            na+="video/";
        }
        else
        {
            na+=data[i];
        }
    }
    
    for(auto &i:name)
    {
        if (i== "video/music.mp3")
            CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(i.c_str());
        else
            CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(i.c_str());
    }
}