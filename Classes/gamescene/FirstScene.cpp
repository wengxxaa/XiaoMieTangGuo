#include "FirstScene.h"
#include "MainScene.h"
#include "cocostudio/CocoStudio.h"
#include "vigame_pay.h"
using namespace cocostudio;

bool FirstScene::init()
{
	if(!Layer::init())
		return false;

	CCLOG("FirstScene Begin");

	string str="animation/firstpage.csb";
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS|| IOSMODE)
	str = "animation/firstpage_ios.csb";
#endif

	auto backcsb=CSLoader::createNode(str);
	this->addChild(backcsb);
	backcsb->setPosition(WINORIGIN);

	_time = 1;
	runAction(Sequence::create(DelayTime::create(3.0f),CallFunc::create([this](){
		preLoad();
		Director::getInstance()->replaceScene(MainScene::createScene());
	}),nullptr));

	return true;
}

void FirstScene::preLoad()
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

void FirstScene::startUpdate(float ft)
{
	_time++;
	if(_time==8|| vigame::pay::PayManager::getDefaultFeeInfo())
	{ 
		Director::getInstance()->replaceScene(MainScene::createScene());
		this->unschedule(schedule_selector(FirstScene::startUpdate));
	}
}