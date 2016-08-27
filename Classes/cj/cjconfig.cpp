#include "cjconfig.h"
#include "GameData.h"

//CJMENU
Menu* cjMenu::createWithImage(const std::string &str1,const std::string &str2,const std::string &str3,const ccMenuCallback &call,bool tt)
{
	auto normalSp=cjSprite::createWithImage(str1);
	auto selectSp=cjSprite::createWithImage(str2);
	auto disableSp=cjSprite::createWithImage(str3);
	auto menuItem=MenuItemSprite::create(normalSp,selectSp,disableSp,call);
	auto select=menuItem->getSelectedImage();
	select->setAnchorPoint(Vec2(0.5f,0.5f));
	if(tt)
		select->setScale(1.1f);
	select->setPosition(select->getPosition()+Vec2(select->getContentSize()/2.0f));
	auto menu=Menu::create(menuItem,nullptr);
	return menu;
}

void cjMenu::addChild(Node * child, int zOrder)
{
	this->addChild(child,zOrder);
}

Menu* cjMenu::createWithImage2(const std::string &str,const ccMenuCallback &call,bool tt)
{
	auto normalSp=Sprite::create(str);
	auto selectSp=Sprite::create(str);
	auto disableSp=Sprite::create(str);
	auto menuItem=MenuItemSprite::create(normalSp,selectSp,disableSp,call);
	auto select=menuItem->getSelectedImage();
	select->setAnchorPoint(Vec2(0.5f,0.5f));
	if(tt)
		select->setScale(1.1f);
	else
		select->setScale(1.0f);
	select->setPosition(select->getPosition()+Vec2(select->getContentSize()/2.0f));
	auto menu=Menu::create(menuItem,nullptr);
	return menu;
}

Menu* cjMenu::createWithImage(const std::string &str1,const ccMenuCallback &call,bool tt)
{
	return createWithImage(str1,str1,str1,call,tt);
}

Menu* cjMenu::createWithFont(const std::string &fontstring,const ccMenuCallback &call)
{
	auto menuItem=MenuItemFont::create(fontstring,call);
	auto menu=Menu::create(menuItem,nullptr);
	return menu;
}

Menu* cjMenu::createWithSprite(Sprite* normalSp,Sprite* selectedSp, const ccMenuCallback &call)
{
	auto menuItem=MenuItemSprite::create(normalSp,selectedSp,call);
	auto menu=Menu::create(menuItem,nullptr);
	return menu;
}

//CJATLASLABEL
LabelAtlas* cjAtlasLabel::createWithInt(const int number,std::string textpath/* =ATLAS_PATH */)
{
	char name[10];
	sprintf(name,"%d",number);
	if(textpath==ATLAS_PATH)
	{
		auto diceCount = LabelAtlas::create(name, textpath, 13, 18, '0');
		diceCount->setAnchorPoint(Vec2(0.5f,0.5f));
		return diceCount; 
	}
	else if(textpath==ATLAS_PATH2)
	{
		auto diceCount = LabelAtlas::create(name, textpath, 21, 31, '0');
		diceCount->setAnchorPoint(Vec2(0.5f,0.5f));
		return diceCount; 
	}
	else if(textpath==ATLAS_PATH3)
	{
		auto diceCount = LabelAtlas::create(name, textpath, 17, 23, '0');
		diceCount->setAnchorPoint(Vec2(0.5f,0.5f));
		return diceCount; 
	}
	else if(textpath==ATLAS_PATH4)
	{
		auto diceCount = LabelAtlas::create(name, textpath, 13, 22, '0');
		diceCount->setAnchorPoint(Vec2(0.5f,0.5f));
		return diceCount; 
	}
	else if (textpath == ATLAS_PATH5)
	{
		auto diceCount = LabelAtlas::create(name, textpath, 22, 26, '0');
		diceCount->setAnchorPoint(Vec2(0.5f, 0.5f));
		return diceCount;
	}
	return nullptr;
}

//CJTTFLABEL
Label* cjTTFLabel::createWithString(const std::string &text,int size/* =20 */,const std::string textpath/* =TTF_PATH */)
{
	TTFConfig ttfconfig;
	ttfconfig.fontSize=size;
	ttfconfig.fontFilePath=textpath;
	auto label=Label::createWithTTF(ttfconfig,text);
	label->setAnchorPoint(Vec2(0.5f+FntXOff,0.5f+FntYOff));
	return label;
}

Label* cjTTFLabel::createWithInt(const int number,int size/* =20 */,std::string textpath/* =TTF_PATH */)
{
	char name[30];
	sprintf(name,"%d",number);
	auto label=createWithString(string(name),size,textpath);
	return label;
}

string cjTTFLabel::getNameByInt(const char*str,int id)
{
	char name[30];
	sprintf(name,str,id);
	return name;
}

string cjTTFLabel::getNameByIntL(const char*str,int id)
{
	char name[100];
	sprintf(name,str,id);
	return name;
}


//jbk2utf8
std::string cjTTFLabel::getStringByKey(const char *str)
{
	auto dic = Dictionary::createWithContentsOfFileThreadSafe(HANZI_XML);  
	__String* fnt1name=(__String*)dic->objectForKey(str); 
	
	return fnt1name->getCString();
}

//CJSCHEDULE
void cjSchedule::runOnNextFrame(const ccSchedulerFunc &call)
{
	Director::getInstance()->getRunningScene()->getScheduler()->schedule(call,this,0.1f,0,0.0f,false,"NextFrame");
}

void cjSchedule::shockBegin(Layer *lay)
{
	//ÆÁÄ»Õð¶¯
	float dis=1.0f;
	auto seq1=RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(MoveTo::create(0.03f,Vec2(dis,0)),MoveTo::create(0.03f,Vec2(-dis,0))));
	auto seq2=RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(MoveTo::create(0.04f,Vec2(0,dis)),MoveTo::create(0.04f,Vec2(0,-dis))));
	seq1->setTag(101);
	seq2->setTag(102);
	lay->runAction(seq1);
	lay->runAction(seq2);

	long long pattern[1]={100};
//#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
//	CocosDenshion::SimpleAudioEngine::getInstance()->vibrateWithPattern(pattern,true);
//#endif
}

void cjSchedule::shockEnd(Layer *lay)
{
	lay->stopActionByTag(101);
	lay->stopActionByTag(102);
	lay->setPosition(Vec2(0,0));
//#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
//	CocosDenshion::SimpleAudioEngine::getInstance()->cancelVibrate();
//#endif
}

void cjSchedule::jnitext(const string& str1,const string &str2)
{
//#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
//	cocos2d::JniMethodInfo t;
//	if(JniHelper::getStaticMethodInfo(t,EXTRA_FILE,"showTipDialog","()V"))
//	{
//		t.env->CallStaticVoidMethod(t.classID,t.methodID);
//	}
//#endif
}

//CJSPRITE
Sprite* cjSprite::createWithImage(const std::string &framename)
{
	auto frame=SpriteFrameCache::getInstance()->getSpriteFrameByName(framename);
	CCAssert(frame,"the framename not exit!");
	return Sprite::createWithSpriteFrame(frame);
}

//CJMUSCI
void cjMusic::playBackgroundMusic(const char *str,bool loop)
{
	if(GameData::getSaveData()->_music)
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(str,loop);
}

int cjMusic::playEffect(const char *str,bool loop)
{
	if(GameData::getSaveData()->_music)
		return CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(str,loop);
	else
		return 0;
}

void cjMusic::stopEffect(int id)
{
	if(GameData::getSaveData()->_music)
		return CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(id);
}

//handNode
bool handNode::init()
{
	if(!Node::init())
		return false;

	auto hand1=cjSprite::createWithImage("yba.png");
	addChild(hand1);
	hand1->setVisible(false);
	hand1->setAnchorPoint(Vec2(0.15f,0.95f));
	auto seq1=Sequence::createWithTwoActions(DelayTime::create(0.5f),Show::create());
	auto seq2=Sequence::createWithTwoActions(DelayTime::create(0.5f),Hide::create());
	hand1->runAction(RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(seq1,seq2)));
	auto hand2=cjSprite::createWithImage("ybb.png");
	auto seq3=Sequence::createWithTwoActions(DelayTime::create(0.5f),Hide::create());
	auto seq4=Sequence::createWithTwoActions(DelayTime::create(0.5f),Show::create());
	hand2->runAction(RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(seq3,seq4)));
	addChild(hand2);
	hand2->setAnchorPoint(Vec2(0.15f,0.95f));

	return true;
}