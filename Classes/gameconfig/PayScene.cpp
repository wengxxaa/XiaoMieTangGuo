#include"PayScene.h"
#include"../cj/cjconfig.h"
#include"../cj/GameData.h"
#include"vigame_pay.h"
#include"MobClickCpp.h"
#include"vigame_ad.h"
#include "BaseConfig.h"
#include"../cj/Toast.h"
#include "cocostudio/CocoStudio.h"
#include "ui/UIText.h"
using namespace cocostudio;

static int tablenum=1;

void PayScene::payPage1()
{

	auto colorlay=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING),WINSIZE.width,WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay,100,100);
	colorlay->setPosition(WINORIGIN);

	diamondRain(colorlay);

	auto backcsb=CSLoader::createNode("animation/LimitFavorable.csb");
	colorlay->addChild(backcsb);
	backcsb->setAnchorPoint(Vec2(0.5f,0.5f));
	backcsb->setPosition(WINSIZE/2.0f);

	auto action=CSLoader::createTimeline("animation/LimitFavorable.csb");
	backcsb->runAction(action);
	action->play("loop",true);

	string num="29";
#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS|| IOSMODE)
    num="30";
#endif
	
	auto price=dynamic_cast<ui::TextBMFont*>(backcsb->getChildByName("bg")->getChildByName("Label_Price"));
	char name[30];
	sprintf(name,price->getString().c_str(),num.c_str());
	price->setString(name);

	auto cancel=dynamic_cast<ui::Button*>(backcsb->getChildByName("bg")->getChildByName("Button_Cancel"));
	cancel->addTouchEventListener([colorlay](Ref*,ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");
			colorlay->removeFromParent();
		}
	});

	auto sure=dynamic_cast<ui::Button*>(backcsb->getChildByName("bg")->getChildByName("sure"));
	sure->addTouchEventListener([=](Ref *,ui::Widget::TouchEventType type){
		if(type==ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
			//umeng
			umeng::eventDict dict;
			dict["pay"]="buy";
			umeng::MobClickCpp::event("BuyClick",&dict);
#endif

			PayScene::getInstance()->paysuccess(1);

			colorlay->removeFromParent();
		}
	});

	auto hand=handNode::createHand(0.6f);
	backcsb->getChildByName("bg")->addChild(hand,2);
	hand->setPosition(sure->getPosition()+Vec2(140,20));

	backcsb->setScale(0.0f);
	backcsb->runAction(Sequence::create(ScaleTo::create(0.1f,1.1f),ScaleTo::create(0.1f,1.0f),NULL));

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);
}

//商城
void PayScene::payPage2()
{
	auto colorlay=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING),WINSIZE.width,WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay,100,100);
	colorlay->setPosition(WINORIGIN);

	diamondRain(colorlay);

	auto animationlay=Layer::create();
	colorlay->addChild(animationlay);

	bool haveshop=false;
	string csbstr="animation/shop02.csb";
	//if(true)

	if(GameDataInstance()->d_PayType&&vigame::pay::PayManager::getDefaultFeeInfo()&&vigame::pay::PayManager::getDefaultFeeInfo()->getFeePriceByID(2))
	{
		haveshop=true;
		csbstr="animation/shop01.csb";
	}
	
	auto backcsb=CSLoader::createNode(csbstr);
	colorlay->addChild(backcsb);
	backcsb->setAnchorPoint(Vec2(0.5f,0.5f));
	backcsb->setPosition(WINSIZE/2.0f);

	int chiyoukey=0;

	if(haveshop)
	{
		chiyoukey=200;

		//

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS|| IOSMODE)
		int znum[4]={30,60,120,200};
		int znum2[4]={25,60,100,200};
		int zprice[4]={6,12,18,30};
#else
		int znum[4]={30,50,100,200};
		int znum2[4]={25,50,80,200};
		int zprice[4]={6,10,15,29};
#endif
		for(int i=0;i<4;i++)
		{
			char name1[30];
			sprintf(name1,"num%d-1",i+1);
			auto font1=dynamic_cast<ui::TextBMFont*>(backcsb->getChildByName(name1));
			font1->setString(cjTTFLabel::getNameByInt("x%d",znum[i]));
			
			char name2[30];
			sprintf(name2,"num%d-2",i+1);
			auto font2=dynamic_cast<ui::TextBMFont*>(backcsb->getChildByName(name2));
			font2->setString(cjTTFLabel::getNameByInt("%d",znum2[i]));

			char name3[30];
			sprintf(name3,"money%d",i+1);
			auto font3=dynamic_cast<ui::TextBMFont*>(backcsb->getChildByName(name3));
			font3->setString(cjTTFLabel::getNameByInt("%d.00",zprice[i]));

			auto anniu=dynamic_cast<ui::Button*>(backcsb->getChildByName(cjTTFLabel::getNameByInt("pay_%d",i+1)));
			anniu->addTouchEventListener([i](Ref*,ui::Widget::TouchEventType type){

				if(type==ui::Widget::TouchEventType::ENDED)
				{
					cjMusic::playEffect("video/tap.mp3");
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
					//umeng
					umeng::eventDict dict;
					dict["pay"]="buy";
					umeng::MobClickCpp::event("BuyClick",&dict);
#endif

					PayScene::getInstance()->paysuccess(i+2);
				}
			});
		}
		//
	}

	auto diamond=dynamic_cast<ui::TextBMFont*>(backcsb->getChildByName("diamondnum"));
	diamond->setString(cjTTFLabel::getNameByInt("%d",GameData::getSaveData()->_diamondNumber));
	auto time=DelayTime::create(0.5f);
	auto call=CCCallFunc::create([diamond](){
		diamond->setString(cjTTFLabel::getNameByInt("%d",GameData::getSaveData()->_diamondNumber));
	});
	diamond->runAction(RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(time,call)));

	auto cancel=dynamic_cast<ui::Button*>(backcsb->getChildByName("cha"));
	cancel->addTouchEventListener([colorlay](Ref *,ui::Widget::TouchEventType type){
		if(type==ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");
			colorlay->removeFromParent();
		}
	});

	backcsb->setScale(0.0f);
	backcsb->runAction(ScaleTo::create(0.2f,1.0f));

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);
}

//限时大礼包
void PayScene::payPage3()
{
	auto colorlay=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING),WINSIZE.width,WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay,100,100);
	colorlay->setPosition(WINORIGIN);

	diamondRain(colorlay);

	auto backcsd=CSLoader::createNode("animation/LimitGift.csb");
	colorlay->addChild(backcsd);
	backcsd->setAnchorPoint(Vec2(0.5f,0.5f));
	backcsd->setPosition(WINSIZE/2.0f);

	auto action1=CSLoader::createTimeline("animation/LimitGift.csb");
	backcsd->runAction(action1);
	action1->play("loop",true);

	auto node=dynamic_cast<Node*>(backcsd->getChildByName("GiftBox"));
	auto action2=CSLoader::createTimeline("animation/LimitGiftNode.csb");
	node->runAction(action2);
	action2->play("lihe",false);
	action2->setLastFrameCallFunc([action2](){
		action2->play("lihe_jihe",true);
	});

	int price=29;    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || IOSMODE)
	price=30;
#endif
	
	auto pricefont=dynamic_cast<ui::Text*>(backcsd->getChildByName("Label_Price"));
	char name[30];
	sprintf(name,pricefont->getString().c_str(),price);
	pricefont->setString(name);
	//

	auto cancel=dynamic_cast<ui::Button*>(backcsd->getChildByName("Button_Cancel"));
	cancel->addTouchEventListener([colorlay](Ref *,ui::Widget::TouchEventType type){
		if(type==ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");
			colorlay->removeFromParent();
		}

	});

	auto sure=dynamic_cast<ui::Button*>(backcsd->getChildByName("Button_Lingqu"));
	sure->addTouchEventListener([=](Ref *,ui::Widget::TouchEventType type){
		if(type==ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
			//umeng
			umeng::eventDict dict;
			dict["pay"]="buy";
			umeng::MobClickCpp::event("BuyClick",&dict);
#endif

			PayScene::getInstance()->paysuccess(6);
			colorlay->removeFromParent();
		}
	});

	auto hand=handNode::createHand(0.6f);
	colorlay->addChild(hand);
	hand->setPosition(sure->getPosition()+Vec2(120,20));

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);
}

//新手礼包
void PayScene::payPage4()
{
	auto colorlay=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING),WINSIZE.width,WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay,100,100);
	colorlay->setPosition(WINORIGIN);

	diamondRain(colorlay);

	auto backcsb=CSLoader::createNode("animation/newhand.csb");
	colorlay->addChild(backcsb);
	backcsb->setAnchorPoint(Vec2(0.5f,0.5f));
	backcsb->setPosition(WINSIZE/2.0f);

	auto action=CSLoader::createTimeline("animation/newhand.csb");
	backcsb->runAction(action);
	action->play("suremotion",true);

	int k=20;
	string s="0.1";
	if(vigame::pay::PayManager::getDefaultPayType()==2)
	{
		k=10;
		s="0.01";
	}

	char name[30];
	auto pricefont=dynamic_cast<ui::TextBMFont*>(backcsb->getChildByName("pricefont"));
	pricefont->setString(s);
	auto numfont=dynamic_cast<ui::TextBMFont*>(backcsb->getChildByName("numfont"));
	sprintf(name,numfont->getString().c_str(),k);
	numfont->setString(name);

	auto cancel=dynamic_cast<ui::Button*>(backcsb->getChildByName("cha"));
	cancel->addTouchEventListener([colorlay](Ref *,ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");
			colorlay->removeFromParent();
		}
	});

	auto sure=dynamic_cast<ui::Button*>(backcsb->getChildByName("sure"));
	sure->addTouchEventListener([=](Ref *,ui::Widget::TouchEventType type){
		if(type==ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
			//umeng
			umeng::eventDict dict;
			dict["pay"]="buy";
			umeng::MobClickCpp::event("BuyClick",&dict);
#endif

			if(vigame::pay::PayManager::getDefaultPayType()==2)
				PayScene::getInstance()->paysuccess(7);
			else
				PayScene::getInstance()->paysuccess(0);

			colorlay->removeFromParent();
		}
	});

	auto hand=handNode::createHand(0.6f);
	backcsb->addChild(hand,2);
	hand->setPosition(sure->getPosition()+Vec2(120,20));

	backcsb->setScale(0.0f);
	backcsb->runAction(ScaleTo::create(0.2f,1.0f));

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);
}

//每日签到
void PayScene::dailyClick()
{
	bool canClick=true;
	if(GameData::getSaveData()->_qd_day==GameDataInstance()->d_daytime)
		canClick=false;

	auto colorlay=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING),WINSIZE.width,WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay,100,100);
	colorlay->setPosition(WINORIGIN);

	diamondRain(colorlay);

	auto backcsb=CSLoader::createNode("animation/dailygift.csb");
	colorlay->addChild(backcsb);
	backcsb->setAnchorPoint(Vec2(0.5f,0.5f));
	backcsb->setPosition(WINSIZE/2.0f);

	auto cancel=dynamic_cast<ui::Button*>(backcsb->getChildByName("cha"));
	cancel->addTouchEventListener([colorlay](Ref *,ui::Widget::TouchEventType type){
		if(type==ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");
			colorlay->removeFromParent();
		}
	});

	for(int i=0; i<7;i++)
	{
		if(i>=GameData::getSaveData()->_qd_number)
		{
			auto right=backcsb->getChildByName(cjTTFLabel::getNameByInt("day_%d",i+1))->getChildByName("right");
			right->setVisible(false);
		}
	}

	auto sure=dynamic_cast<ui::Button*>(backcsb->getChildByName("sure"));
	auto mflq=sure->getChildByName("fnt_mflq");
	auto ylq=sure->getChildByName("fnt_ylq");
	if(canClick)
	{
		ylq->setVisible(false);
		auto hand=handNode::createHand(0.6f);
		backcsb->addChild(hand,1,"hand");
		hand->setPosition(sure->getPosition()+Vec2(120,20));
	}
	else
		mflq->setVisible(false);

	sure->addTouchEventListener([=](Ref *,ui::Widget::TouchEventType type){
		if(type==ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");
			if(GameData::getSaveData()->_qd_day!=GameDataInstance()->d_daytime&&canClick)
			{
				GameData::getSaveData()->_qd_day=GameDataInstance()->d_daytime;
				GameData::getSaveData()->_qd_number++;
				assert(GameData::getSaveData()->_qd_number<8);
				//GameData::getSaveData()->_diamondNumber+=DAILY_REWARD[GameData::getSaveData()->_qd_number-1];
				//GameData::getInstance()->dataSave();

				dailyClickTip(DAILY_REWARD[GameData::getSaveData()->_qd_number-1]);

				auto right=backcsb->getChildByName(cjTTFLabel::getNameByInt("day_%d",GameData::getSaveData()->_qd_number))->getChildByName("right");
				right->setVisible(true);

				mflq->setVisible(false);
				ylq->setVisible(true);
				backcsb->removeChildByName("hand");
			}
			else
			{
				colorlay->removeFromParent();
			}
		}
	});
	 
	backcsb->setScale(0.0f);
	backcsb->runAction(ScaleTo::create(0.2f,1.0f));

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);

	return;
}

//帮助
void PayScene::help()
{
	auto colorlay=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING),WINSIZE.width,WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay,100,100);
	colorlay->setPosition(WINORIGIN);

	auto backcsb=CSLoader::createNode("animation/help.csb");
	colorlay->addChild(backcsb);
	backcsb->setAnchorPoint(Vec2(0.5f,0.5f));
	backcsb->setPosition(WINSIZE/2.0f);

	auto cancel=dynamic_cast<ui::Button*>(backcsb->getChildByName("cha"));
	cancel->addTouchEventListener([colorlay](Ref*,ui::Widget::TouchEventType type){
		if(type==ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");
			colorlay->removeFromParent();
		}
	});

	backcsb->setScale(0.0f);
	backcsb->runAction(ScaleTo::create(0.2f,1.0f));

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);
}

//每日签到提示
void PayScene::dailyClickTip(int number,int key)
{
	if(key==2)
	{
		auto downsp=Scale9Sprite::createWithSpriteFrameName("tsk.png");
		Director::getInstance()->getRunningScene()->addChild(downsp,100);
		downsp->setPreferredSize(CCSizeMake(250,60));
		downsp->setOpacity(200);
		downsp->setPosition(Vec2(WINSIZE.width/2.0f,400));

		auto tip=LabelTTF::create(cjTTFLabel::getNameByIntL(cjTTFLabel::getStringByKey("hb3").c_str(),number),TTF_PATH,25);
		downsp->addChild(tip);
		tip->setColor(Color3B::WHITE);
		tip->setPosition(downsp->getContentSize()/2.0f);

		downsp->runAction(Sequence::create(MoveBy::create(0.3f,Vec2(0,150)),MoveBy::create(0.2f,Vec2(0,-10)),DelayTime::create(2.0f),
			CallFunc::create([downsp](){
				downsp->removeFromParent();
		}),nullptr));
	}
	else
	{
		auto colorlay=dynamic_cast<Layer*>(Director::getInstance()->getRunningScene()->getChildByTag(100));

		auto secondlayer=LayerColor::create(Color4B(0,0,0,255*0.7f),WINSIZE.width,WINSIZE.height);
		colorlay->addChild(secondlayer,100,100);
		secondlayer->setPosition(WINORIGIN);

		auto gx=cjSprite::createWithImage("gxn.png");
		secondlayer->addChild(gx);
		gx->setPosition(Vec2(WINSIZE.width*0.5f,WINSIZE.height*0.8f));
		gx->setScale(0.0f);
		gx->runAction(ScaleTo::create(0.2f,1.0f));

		auto guangnode=CSLoader::createNode("animation/guang.csb");
		secondlayer->addChild(guangnode);
		guangnode->setScale(0.8f);
		guangnode->setPosition(Vec2(WINSIZE.width*0.5f,WINSIZE.height*0.6f));

		auto guangaction=CSLoader::createTimeline("animation/guang.csb");
		guangnode->runAction(guangaction);
		guangaction->play("guang",true);

		auto backsp=Sprite::create("animation/guoguan/34.png");
		secondlayer->addChild(backsp);
		backsp->setPosition(guangnode->getPosition());

		string str;
		int num=1;

		if(number<50)
		{
			str="zs.png";
			num=number;
			GameData::getSaveData()->_diamondNumber+=num;

            addDiamondPlist(secondlayer,2);
		}
		else if(number>50)
		{
			str=cjTTFLabel::getNameByInt("prop%02d.png",number-50);
			GameData::getSaveData()->_propnum[number-51]++;
		}

		GameDataInstance()->dataSave();

		auto sp=cjSprite::createWithImage(str);
		secondlayer->addChild(sp);
		if(key==53||key==54)
			sp->setScale(1.0f);
		else
			sp->setScale(1.2f);
		sp->setPosition(guangnode->getPosition()+Vec2(0,10));

		auto font=cjAtlasLabel::createWithInt(num,ATLAS_PATH3);
		secondlayer->addChild(font);
		font->setScale(1.1f);
		font->setAnchorPoint(Vec2(1.0f,0.5f));
		font->setPosition(guangnode->getPosition()-Vec2(2,50));

		auto ge=cjSprite::createWithImage("zi_ge.png");
		secondlayer->addChild(ge);
		ge->setAnchorPoint(Vec2(0.0f,0.5f));
		ge->setPosition(font->getPosition()+Vec2(5,0));

		auto touchListener = EventListenerTouchOneByOne::create();
		touchListener->setSwallowTouches(true);
		touchListener->onTouchBegan = [secondlayer,this](Touch* touch, Event* event){
			return true;
		};
		touchListener->onTouchEnded = [secondlayer,this](Touch* touch, Event* event){
			secondlayer->removeFromParent();
		};
		secondlayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, secondlayer);
	}
}

void PayScene::addDiamondPlist(Layer *lay,int level)
{
    auto diamonds=CCParticleSystemQuad::create("lizi/diamonds.plist");
    lay->addChild(diamonds,level);
    diamonds->setPosition(Vec2(WINSIZE.width/2.0f,WINSIZE.height*DIAMOND_HIGH));
    diamonds->setAutoRemoveOnFinish(true);
}

//过关奖励
void PayScene::levelReward()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS|| IOSMODE)
	auto pADItem = vigame::ad::ADManager::isAdReady("home_mfzs", "video");
	if (pADItem&&GameData::getSaveData()->_freeroundnum < 20) {
		turnTable();
	}
	return;
#endif

	if (GameDataInstance()->d_PayType&&vigame::pay::PayManager::getDefaultFeeInfo() && vigame::pay::PayManager::getDefaultFeeInfo()->getFeePriceByID(8) == 0)
		return;

	auto colorlay=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING),WINSIZE.width,WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay,100,100);
	colorlay->setPosition(WINORIGIN);

	diamondRain(colorlay);

	auto backcsb=CSLoader::createNode("animation/guoguangift.csb");
	colorlay->addChild(backcsb);
	backcsb->setAnchorPoint(Vec2(0.5f,0.5f));
	backcsb->setPosition(WINSIZE/2.0f);

	auto action=CSLoader::createTimeline("animation/guoguangift.csb");
	backcsb->runAction(action);
	action->play("loop",true);

	auto bg=backcsb->getChildByName("bg");

	auto price=dynamic_cast<ui::TextBMFont*>(bg->getChildByName("Label_Price"));
	char name[30];
	sprintf(name,price->getString().c_str(),"29");
	price->setString(name);
	price->setVisible(false);

	auto out=bg->getChildByName("Out");
	auto In=bg->getChildByName("In");
	In->setVisible(false);

	_canTouch=true;

	for(int i=0;i<9;i++)
	{
		auto Card=bg->getChildByName(cjTTFLabel::getNameByInt("Card%d",i+1));
		auto cout=Card->getChildByName("Out");
		auto cin=Card->getChildByName("In");
		cin->setVisible(false);
		   
		auto button=dynamic_cast<ui::Button*>(cout->getChildByName("Button_out"));
		button->addTouchEventListener([=](Ref *,ui::Widget::TouchEventType type){
			if(type==ui::Widget::TouchEventType::ENDED&&_canTouch)
			{
				cjMusic::playEffect("video/tap.mp3");
				_canTouch=false;
				int ddd[9]={4,5,11,30,50,60,80,160};

				int num=(rand()%5)+1;
				ddd[2]-=num;
				GameData::getSaveData()->_diamondNumber+=num;
				GameData::getInstance()->dataSave();
				dailyClickTip(num,2);

				auto diamonds = CCParticleSystemQuad::create("lizi/diamonds.plist");
				colorlay->addChild(diamonds, 2);
				diamonds->setPosition(Vec2(WINSIZE.width / 2.0f, WINSIZE.height*DIAMOND_HIGH));
				diamonds->setAutoRemoveOnFinish(true);

				float ff=0.2f;
				int knum=0;
				for(int j=0;j<9;j++)
				{
					auto jcard=bg->getChildByName(cjTTFLabel::getNameByInt("Card%d",j+1));
					auto jout=jcard->getChildByName("Out");
					auto jin=jcard->getChildByName("In");
					Vector<FiniteTimeAction*> action;
					if(j!=i)
						action.pushBack(DelayTime::create(0.3f+2*ff));
					action.pushBack(ScaleTo::create(ff,0.0f,1.0f));
					action.pushBack(FadeOut::create(0.0f));
					int jnum=num;
					if(j!=i)
					{
						jnum=ddd[knum];
						knum++;
					}
					action.pushBack(CallFunc::create([=](){
						jin->setVisible(true);
						jin->setScaleX(0.0f);
						jin->runAction(ScaleTo::create(ff,1.0f));

						auto numfont=dynamic_cast<ui::TextBMFont*>(jin->getChildByName("Num"));
						numfont->setString(cjTTFLabel::getNameByInt("x%d",jnum));
					}));
					if(j==i)
					{
						action.pushBack(DelayTime::create(0.3f+3*ff));
						action.pushBack(CallFunc::create([=](){
							out->setVisible(false);
							In->setVisible(true);
							price->setVisible(true);
							auto hand=handNode::createHand(0.6f);
							colorlay->addChild(hand,1);
							hand->setPosition(In->getPosition()+Vec2(130,80));
							auto Inbutton=dynamic_cast<ui::Button*>(In->getChildByName("Button_Lingqu"));
							Inbutton->addTouchEventListener([=](Ref *,ui::Widget::TouchEventType type){
								if(type==ui::Widget::TouchEventType::ENDED)
								{
									cjMusic::playEffect("video/tap.mp3");
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
									//umeng
									umeng::eventDict dict;
									dict["pay"]="buy";
									umeng::MobClickCpp::event("BuyClick",&dict);
#endif

									PayScene::getInstance()->paysuccess(8);
									colorlay->removeFromParent();
								}
							});
						}));
					}
					jout->runAction(Sequence::create(action));
				}
			}
		});
	}

	auto cancel=dynamic_cast<ui::Button*>(bg->getChildByName("Button_Cancel"));
	cancel->addTouchEventListener([colorlay](Ref *,ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");
			cjSchedule::getInstance()->runOnNextFrame([=](float ft) {
				colorlay->removeFromParent();
			});
		}
	});

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);

	return;
}

//关于我们
void PayScene::aboutus()
{
	auto colorlay=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING),WINSIZE.width,WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay,100,100);
	colorlay->setPosition(WINORIGIN);

	auto backcsb=CSLoader::createNode("animation/aboutus.csb");
	colorlay->addChild(backcsb);
	backcsb->setAnchorPoint(Vec2(0.5f,0.5f));
	backcsb->setPosition(WINSIZE/2.0f);

	auto gamename=dynamic_cast<ui::Text*>(backcsb->getChildByName("name"));
	char name[100];
	sprintf(name,gamename->getString().c_str(),cjTTFLabel::getStringByKey("gamename").c_str());
	gamename->setString(name);

	auto banben=dynamic_cast<ui::Text*>(backcsb->getChildByName("banben"));
	sprintf(name,banben->getString().c_str(), vigame::SysConfig::getInstance()->getAppVersion().c_str());
	banben->setString(name);

	auto cancel=dynamic_cast<ui::Button*>(backcsb->getChildByName("cha"));
	cancel->addTouchEventListener([=](Ref*,ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			cjMusic::playEffect("video/tap.mp3");
			colorlay->removeFromParent();
		}
	});

	backcsb->setScale(0.0f);
	backcsb->runAction(ScaleTo::create(0.2f,1.0f));

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);
}

//大转盘
void PayScene::turnTable()
{
	_canTouch=true;
	tablenum=0;
	auto colorlay=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING),WINSIZE.width,WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay,100,100);
	colorlay->setPosition(WINORIGIN);

	diamondRain(colorlay);

	string csbname="animation/Truntable_android.csb";
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS|| IOSMODE)
	csbname="animation/Truntable_ios.csb";
#endif

	auto csb=CSLoader::createNode(csbname);
	colorlay->addChild(csb,1,"csb");
	csb->setPosition(WINORIGIN);

	for(int i=0;i<8;i++)
		_sp[i]=dynamic_cast<Sprite*>(csb->getChildByName("tab_back")->getChildByName(cjTTFLabel::getNameByInt("reward_%d",i+1)));

	auto csbaction=CSLoader::createTimeline(csbname);
	csb->runAction(csbaction);
	csbaction->play("light",true);

	auto cancel=dynamic_cast<ui::Button*>(csb->getChildByName("cha"));
	cancel->addTouchEventListener([=](Ref *,ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");
			colorlay->removeFromParent();
		}
	});

#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS|| IOSMODE)
	auto mfcj = dynamic_cast<Node*>(csb->getChildByName("mfcj"));
	auto lottery = dynamic_cast<ui::Button*>(mfcj->getChildByName("menu_1"));
	auto diamondnum = dynamic_cast<ui::TextBMFont*>(csb->getChildByName("diamondnum")->getChildByName("num"));
	auto tip = csb->getChildByName("tipShow");

	auto cishunum = dynamic_cast<ui::Text*>(csb->getChildByName("tab_back")->getChildByName("turnshow")->getChildByName("num"));
	cishunum->setString(cjTTFLabel::getNameByInt("%d", (20 - GameData::getSaveData()->_freeroundnum)));

	diamondnum->setString(cjTTFLabel::getNameByInt("%d",MAX(0,GameData::getSaveData()->_diamondNumber)));
	auto menuno= dynamic_cast<ui::Button*>(mfcj->getChildByName("menu_2"));	
	//menuno->addTouchEventListener([=](Ref *, ui::Widget::TouchEventType type) {
	//	if(type == ui::Widget::TouchEventType::BEGAN)
	//		turnTip();
	//});

	lottery->addTouchEventListener([=](Ref *, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			cjMusic::playEffect("video/tap.mp3");
			colorlay->stopAllActions();
			tableCallBack(csb);

			lottery->setScale(0.9f);
		}
		else if (type == ui::Widget::TouchEventType::ENDED)
		{
			lottery->setScale(1.0f);
			lottery->setEnabled(false);
		}
	});
    
    auto pADItem = vigame::ad::ADManager::isAdReady("home_mfzs","video");
    if(pADItem&&GameData::getSaveData()->_freeroundnum < 20){
        menuno->setVisible(false);
		tip->setVisible(false);
    }
    else{
        lottery->setVisible(false);

		if (GameData::getSaveData()->_freeroundnum >= 20)
		{
			auto tip1 = tip->getChildByName("tip1");
			tip1->setVisible(false);
		}
		else
		{
			auto tip2 = tip->getChildByName("tip2");
			tip2->setVisible(false);

			auto time = DelayTime::create(1.0f);
			auto call = CallFunc::create([=]() {
				auto pADItem2 = vigame::ad::ADManager::isAdReady("home_mfzs", "video");
				if (pADItem2)
				{
					tip->setVisible(false);
					menuno->setVisible(false);
					lottery->setVisible(true);
					diamondnum->stopAllActions();
				}
			});
			diamondnum->runAction(RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(time,call)));
		}
    }
    
#else

	if (GameData::getSaveData()->_freeround)
		csb->getChildByName("label2")->setVisible(false);
	else
		csb->getChildByName("label1")->setVisible(false);

	auto lottery = dynamic_cast<ui::Button*>(csb->getChildByName("tab_back")->getChildByName("lottery"));
	lottery->addTouchEventListener([=](Ref *, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			cjMusic::playEffect("video/tap.mp3");
			if (GameData::getSaveData()->_freeround)
			{
				csb->getChildByName("label2")->setVisible(true);
				csb->getChildByName("label1")->setVisible(false);
				colorlay->stopAllActions();
				tableCallBack(csb);
			}
			else
			{
				if (GameData::getSaveData()->_diamondNumber<10)
				{
					Toast::makeText("nomoney")->show();
				}
				else
				{
					GameData::getSaveData()->_diamondNumber -= 10;
					GameData::getInstance()->dataSave();
					colorlay->stopAllActions();
					tableCallBack(csb);
				}
			}

			lottery->setScale(0.9f);
		}
		else if (type == ui::Widget::TouchEventType::ENDED)
		{
			lottery->setScale(1.0f);
			lottery->setEnabled(false);
		}
	});
#endif

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);

	turnMotion(1.0f);

	return ;
}
void PayScene::menuchange()
{
	_sp[_rewardid]->getChildByName("shade")->setVisible(true);
	_sp[(_rewardid+1)%8]->getChildByName("shade")->setVisible(false);
	_rewardid=(_rewardid+1)%8;
	cjMusic::playEffect("video/Wheel_move.mp3");
}

void PayScene::tableCallBack(Node *csb)
{	
	//_sp[_rewardid]->getChildByName("shade")->setVisible(true);

	//_rewardid=0;
	//_sp[0]->getChildByName("shade")->setVisible(false);

	Vector<FiniteTimeAction*> action;
	for(int i=0;i<6;i++)
	{
		action.pushBack(DelayTime::create(0.3f-(i*0.05f)));
		action.pushBack(CallFunc::create([this](){
			menuchange();
		}));
	}
	for(int i=0;i<(40+rand()%8);i++)
	{
		action.pushBack(DelayTime::create(0.05f));
		action.pushBack(CallFunc::create([this](){
			menuchange();
		}));
	}
	for(int i=0;i<6;i++)
	{
		action.pushBack(DelayTime::create(0.05f+(i*0.05f)));
		action.pushBack(CallFunc::create([this](){
			menuchange();
		}));
	}
	action.pushBack(DelayTime::create(0.5f));
	action.pushBack(CallFunc::create([=](){
		cjMusic::playEffect("video/Wheel_stop.mp3");
		tableReward(_rewardid);
		auto lottery=dynamic_cast<ui::Button*>(csb->getChildByName("mfcj")->getChildByName("menu_1"));
		lottery->setEnabled(true);
		//_sp[_rewardid]->getChildByName("shade")->setVisible(true);
	}));
	csb->runAction(Sequence::create(action));
}

void PayScene::tableReward(int key)
{
	cjMusic::playEffect("video/Wheel_get.mp3");

	GameData::getSaveData()->_freeround = false;
	GameData::getSaveData()->_freeroundday = GameData::getInstance()->d_daytime;
	GameData::getInstance()->dataSave();

	auto colorlay=dynamic_cast<Layer*>(Director::getInstance()->getRunningScene()->getChildByTag(100));
	auto csb = colorlay->getChildByName("csb");
	GameData::getSaveData()->_freeroundnum++;
	if (GameData::getSaveData()->_freeroundnum >= 20)
	{
		auto menuno = csb->getChildByName("mfcj")->getChildByName("menu_2");
		auto lottery = csb->getChildByName("mfcj")->getChildByName("menu_1");
		auto tip = csb->getChildByName("tipShow");
		auto tip1 = tip->getChildByName("tip1");
		auto tip2 = tip->getChildByName("tip2");
		menuno->setVisible(true);
		lottery->setVisible(false);
		tip->setVisible(true);
		tip1->setVisible(false);
		tip2->setVisible(true);
	}
	GameData::getInstance()->dataSave();
	auto cishunum = dynamic_cast<ui::Text*>(csb->getChildByName("tab_back")->getChildByName("turnshow")->getChildByName("num"));
	cishunum->setString(cjTTFLabel::getNameByInt("%d", MAX(0,(20 - GameData::getSaveData()->_freeroundnum))));

	auto secondlayer=LayerColor::create(Color4B(0,0,0,255*0.7f),WINSIZE.width,WINSIZE.height);
	colorlay->addChild(secondlayer,100,100);
	secondlayer->setPosition(WINORIGIN);

	auto gx=cjSprite::createWithImage("gxn.png");
	secondlayer->addChild(gx);
	gx->setPosition(Vec2(WINSIZE.width*0.5f,WINSIZE.height*0.7f));
	gx->setScale(0.0f);
	gx->runAction(ScaleTo::create(0.2f,1.0f));

	auto guangnode=CSLoader::createNode("animation/guang.csb");
	secondlayer->addChild(guangnode);
	guangnode->setScale(0.8f);
	guangnode->setPosition(Vec2(WINSIZE.width*0.5f,WINSIZE.height*0.5f));

	auto guangaction=CSLoader::createTimeline("animation/guang.csb");
	guangnode->runAction(guangaction);
	guangaction->play("guang",true);

	auto backsp = Sprite::create("animation/guoguan/34.png");
	secondlayer->addChild(backsp);
	backsp->setPosition(guangnode->getPosition());

	string str;
	int num=1;
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS|| IOSMODE)
	if (key == 0 || key == 6)
	{
		str = "zs.png";
		num = rand() % 3+1;
		GameData::getSaveData()->_diamondNumber += num;
	}
	else if (key == 1 || key == 4 || key == 7)
	{
		//视频广告

        vigame::ad::ADManager::openAd("home_mfzs", [=](vigame::ad::ADSourceItem* pADItem, int result) {
            if (result == vigame::ad::ADSourceItem::open_result::OpenSuccess)
            {
                int dnum=rand()%3+3;
                GameData::getSaveData()->_diamondNumber += dnum;
                GameData::getInstance()->dataSave();
				turnMotion(1.0f);
				auto diamondnum = dynamic_cast<ui::TextBMFont*>(colorlay->getChildByName("csb")->getChildByName("diamondnum")->getChildByName("num"));
                turnAddDiamond(colorlay,dnum,diamondnum,Vec2(28,772));
               //PayScene::getInstance()->dailyClickTip(10,2);
               // PayScene::getInstance()->addDiamondPlist(colorlay, 10);
            }
        });
        
		secondlayer->removeFromParent();	
		return;
	}
	else if (key == 2)
	{
		str = "prop01.png";
		GameData::getSaveData()->_propnum[0]++;
	}
	else if (key == 3)
	{
		str = "prop03.png";
		GameData::getSaveData()->_propnum[2]++;
	}
	else if (key == 5)
	{
		str = "prop04.png";
		GameData::getSaveData()->_propnum[3]++;
	}

#else
	if(key==0||key==4)
	{
		str="zs.png";
		num=rand()%10+1;
		GameData::getSaveData()->_diamondNumber+=num;
	}
	else if(key==2||key==6)
	{
		str="zs.png";
		num=rand()%10+10;
		GameData::getSaveData()->_diamondNumber+=num;
	}
	else if(key==1)
	{
		str="prop01.png";
		GameData::getSaveData()->_propnum[0]++;
	}
	else if(key==3)
	{
		str="prop03.png";
		GameData::getSaveData()->_propnum[2]++;
	}
	else if(key==5)
	{
		str="prop04.png";
		GameData::getSaveData()->_propnum[3]++;
	}
	else if(key==7)
	{
		str="prop02.png";
		GameData::getSaveData()->_propnum[1]++;
	}
#endif
	GameDataInstance()->dataSave();

	auto sp=cjSprite::createWithImage(str);
	secondlayer->addChild(sp);
	if(key==5||key==7)
		sp->setScale(1.0f);
	else
		sp->setScale(1.2f);
	sp->setPosition(guangnode->getPosition()+Vec2(0,10));

	auto font=cjAtlasLabel::createWithInt(num,ATLAS_PATH3);
	secondlayer->addChild(font);
	font->setScale(1.1f);
	font->setAnchorPoint(Vec2(1.0f,0.5f));
	font->setPosition(guangnode->getPosition()-Vec2(2,50));

	auto ge=cjSprite::createWithImage("zi_ge.png");
	secondlayer->addChild(ge);
	ge->setAnchorPoint(Vec2(0.0f,0.5f));
	ge->setPosition(font->getPosition()+Vec2(5,0));

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [=](Touch* touch, Event* event){
		return true;
	};
	touchListener->onTouchEnded = [=](Touch* touch, Event* event){
		secondlayer->removeFromParent();
		if (IOSMODE && (str == "zs.png"))
		{
			auto diamondnum = dynamic_cast<ui::TextBMFont*>(colorlay->getChildByName("csb")->getChildByName("diamondnum")->getChildByName("num"));
            turnAddDiamond(colorlay,num,diamondnum,Vec2(28,772));
		}
		turnMotion(1.0f);
	};
	secondlayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, secondlayer);
}

//钻石向上飞
void PayScene::turnAddDiamond(Layer *colorlay,int num,ui::TextBMFont *diamondnum,Vec2 position)
{
    int addnum=MAX(num/5,1);
    int round=num;
    if(round>5)
        round=5;

	auto downsp = Sprite::create("animation/Shop/diamond4.png");
	colorlay->addChild(downsp,21);
	downsp->setPosition(Vec2(WINSIZE / 2.0f) + Vec2(0, 10));

    for (int i = 0; i < round; i++)
    {
        Vector<FiniteTimeAction*> taction;
        auto diamond = cjSprite::createWithImage("zs.png");
        colorlay->addChild(diamond, 20);
        diamond->setScale(1.2f);
        diamond->setRotation(30);
        diamond->setPosition(Vec2(WINSIZE / 2.0f) + Vec2(0, 10));
        taction.pushBack(DelayTime::create(i*0.1f));
        taction.pushBack(CallFunc::create([=]() {
            diamond->runAction(Sequence::create(MoveTo::create(0.7f, position), ScaleTo::create(0.3f, 1.0f), CallFunc::create([=]() {
                parShow(colorlay, 10, position);
                diamond->removeFromParent();

				if (i == (round - 1))
				{
					diamondnum->setString(cjTTFLabel::getNameByInt("%d", GameData::getSaveData()->_diamondNumber));
					downsp->removeFromParent();
				}
                else
                    diamondnum->setString(cjTTFLabel::getNameByInt("%d", GameData::getSaveData()->_diamondNumber-num+addnum*(i+1)));
            }), nullptr));
        }));
        diamond->runAction(Sequence::create(taction));
    }
}

void PayScene::turnAddDiamond(Layer *colorlay, int num, Vec2 position)
{
	int addnum = MAX(num / 5, 1);
	int round = num;
	if (round>5)
		round = 5;

	auto downsp = Sprite::create("animation/Shop/diamond4.png");
	colorlay->addChild(downsp, 21);
	downsp->setPosition(Vec2(WINSIZE / 2.0f) + Vec2(0, 10));

	for (int i = 0; i < round; i++)
	{
		Vector<FiniteTimeAction*> taction;
		auto diamond = cjSprite::createWithImage("zs.png");
		colorlay->addChild(diamond, 20);
		diamond->setScale(1.2f);
		diamond->setRotation(30);
		diamond->setPosition(Vec2(WINSIZE / 2.0f) + Vec2(0, 10));
		taction.pushBack(DelayTime::create(i*0.1f));
		taction.pushBack(CallFunc::create([=]() {
			diamond->runAction(Sequence::create(MoveTo::create(0.7f, position), ScaleTo::create(0.3f, 1.0f), CallFunc::create([=]() {
				parShow(colorlay, 10, position);
				diamond->removeFromParent();

				if (i == (round - 1))
				{
					GameData::getSaveData()->_diamondNumber += num-addnum*(round-1);
					downsp->removeFromParent();
				}
				else
					GameData::getSaveData()->_diamondNumber += addnum;
			}), nullptr));
		}));
		diamond->runAction(Sequence::create(taction));
	}
	GameData::getInstance()->dataSave();
}

void PayScene::turnMotion(float ft)
{
	auto colorlay=dynamic_cast<Layer*>(Director::getInstance()->getRunningScene()->getChildByTag(100));
	if(colorlay)
	{
		_sp[_rewardid]->getChildByName("shade")->setVisible(false);
		auto time=DelayTime::create(0.8);
		auto call=CallFunc::create([=](){
			menuchange();
		});
		colorlay->runAction(RepeatForever::create((ActionInterval*)(Sequence::createWithTwoActions(time,call))));
	}
}

void PayScene::turnTip()
{
	auto colorlay = dynamic_cast<Layer*>(Director::getInstance()->getRunningScene()->getChildByTag(100));
	auto secondlayer = LayerColor::create(Color4B(0, 0, 0, 255 * 0.7f), WINSIZE.width, WINSIZE.height);
	colorlay->addChild(secondlayer, 100, 100);
	secondlayer->setPosition(WINORIGIN);

	secondlayer->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=]() {
		secondlayer->removeFromParent();
	}), nullptr));

	auto tipnode = CSLoader::createNode("animation/diamondtip_get.csb");
	secondlayer->addChild(tipnode);
	tipnode->setPosition(WINORIGIN);

	auto Button_mfzs = dynamic_cast<ui::Button*>(tipnode->getChildByName("Button_mfzs"));
	Button_mfzs->addTouchEventListener([=](Ref *, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			cjMusic::playEffect("video/tap.mp3");
			vigame::ad::ADManager::openAd("home_mfzs", [=](vigame::ad::ADSourceItem* pADItem, int result) {
				if (result == vigame::ad::ADSourceItem::open_result::OpenSuccess)
				{
					GameData::getSaveData()->_diamondNumber += 10;
					GameData::getSaveData()->_freediamondnum++;
					GameData::getInstance()->dataSave();

					dailyClickTip(10, 2);
					addDiamondPlist(colorlay, 10);
				}
			});
		}
	});

	auto fntnum= dynamic_cast<ui::TextBMFont*>(tipnode->getChildByName("fntnum"));
	fntnum->setString(cjTTFLabel::getNameByInt("%d",MAX(1,10- GameData::getSaveData()->_freediamondnum)));

	auto cancel = dynamic_cast<ui::Button*>(tipnode->getChildByName("cha"));
	cancel->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			cjMusic::playEffect("video/tap.mp3");
			colorlay->removeFromParent();
		}
	});

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
		return true;
	};
	touchListener->onTouchEnded = [=](Touch* touch, Event* event) {
		secondlayer->removeFromParent();
	};
	secondlayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, secondlayer);
}

//免费支付
void PayScene::paysuccess(int key)
{
	vigame::pay::PayManager::orderPay(key);

	return ;

	GameData::getSaveData()->_diamondNumber+=PAY_DIAMOND[key];

	char idname[10];
	sprintf(idname,"%d",key);

	if(key==6)
	{
		for(int i=0;i<4;i++)
			GameData::getSaveData()->_propnum[i]+=1;
	}
	else if((key==0)||(key==7))
	{
		GameData::getSaveData()->_xinshou=false;
	}

	{
		auto diamonds=CCParticleSystemQuad::create("lizi/diamonds.plist");
		Director::getInstance()->getRunningScene()->addChild(diamonds,101);
		diamonds->setPosition(Vec2(WINSIZE.width/2.0f,WINSIZE.height*DIAMOND_HIGH));
		diamonds->setAutoRemoveOnFinish(true);
	}

	GameData::getInstance()->dataSave();
	Toast::makeText("PAY_RESULT_SUCCESS")->show();
}

//钻石雨
void PayScene::diamondRain(Layer *lay)
{
	auto rain = ParticleSystemQuad::create("lizi/zuanshiyu.plist");
	lay->addChild(rain, 100);
	rain->setPosition(Vec2(WINSIZE.width/2.0f,WINSIZE.height*1.0f));
	rain->setAutoRemoveOnFinish(true);
}

//得到物品的效果
void PayScene::parShow(Layer *lay, int level, Vec2 pos)
{
	auto csb = CSLoader::createNode("animation/prop_skill_paint.csb");
	lay->addChild(csb, level);
	csb->setScale(0.5f);
	csb->setPosition(pos);
	auto csbaction = CSLoader::createTimeline("animation/prop_skill_paint.csb");
	csb->runAction(csbaction);
	csbaction->play("change", false);
	csbaction->setLastFrameCallFunc([csb]() {
		csb->removeFromParent();
	});

	auto skill2_bai = csb->getChildByName("skill2_bai");
	skill2_bai->setVisible(false);
}

//免费钻石提示
void PayScene::freeDiamond()
{
	auto colorlay = LayerColor::create(Color4B(0, 0, 0, 255 * OPA_BEIJING), WINSIZE.width, WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay, 100, 100);
	colorlay->setPosition(WINORIGIN);

	auto backcsb = CSLoader::createNode("animation/diamondtip.csb");
	colorlay->addChild(backcsb);
	backcsb->setAnchorPoint(Vec2(0.5f, 0.5f));
	backcsb->setPosition(WINSIZE / 2.0f);

	auto cancel = dynamic_cast<ui::Button*>(backcsb->getChildByName("cha"));
	cancel->addTouchEventListener([colorlay](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");
			colorlay->removeFromParent();
		}
	});

	backcsb->setScale(0.0f);
	backcsb->runAction(ScaleTo::create(0.2f, 1.0f));

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event) {
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);
}