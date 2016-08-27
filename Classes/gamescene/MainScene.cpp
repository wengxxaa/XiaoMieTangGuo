#include"MainScene.h"
#include "MobClickCpp.h"
#include "vigame_ad.h"
#include "vigame_pay.h"
#include "vigame_core.h"
#include "../gameconfig/PayScene.h"
#include "../gameconfig/PayDhm.h"

#include "cocostudio/CocoStudio.h"

using namespace cocostudio;

#include"../cj/Toast.h"


#define Pay

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS|| IOSMODE)
void MainScene::checkVideoState(float dt) {
	auto pADItem = vigame::ad::ADManager::isAdReady("home_mfzs","video");
	auto zuanshi=this->getChildByName("csb")->getChildByName("Button_mfzs");
	auto hand = this->getChildByName("mfzshand");
	if (pADItem&&GameData::getSaveData()->_freediamondnum<10) {
		if (zuanshi->isVisible() == false) {
			zuanshi->setVisible(true);
			hand->setVisible(true);
		}
	}
	else {
		zuanshi->setVisible(false);
		hand->setVisible(false);
	}
}
#endif

bool MainScene::init()
{
	if(!Layer::init())
		return false;

    vigame::ad::ADManager::closeAd("banner");

	cjMusic::playBackgroundMusic("video/music.mp3", true);

	_xieyi=true;
	GameDataInstance()->d_PayType=0;
	GameDataInstance()->d_PayType= vigame::pay::PayManager::getDefaultPayType();
	GameDataInstance()->_guide=false;

	string csbname;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS|| IOSMODE)
	csbname = "animation/mainpage_ios.csb";
#else
	csbname = "animation/mainpage.csb";
#endif

	auto bbbsp = Sprite::create("animation/Mainpage/background_01.png");
	this->addChild(bbbsp);
	bbbsp->setAnchorPoint(Vec2(0.5f, 0.5f));
	bbbsp->setPosition(WINSIZE / 2.0f);

	auto backcsb=CSLoader::createNode(csbname);
	this->addChild(backcsb,1,"csb");
	backcsb->setAnchorPoint(Vec2(0.5f,0.5f));
	backcsb->setPosition(WINSIZE/2.0f);

	auto backsp = backcsb->getChildByName("background");
	backsp->retain();
	backsp->removeFromParent();

	_gridnode = NodeGrid::create();
	this->addChild(_gridnode);
	_gridnode->setPosition(WINORIGIN);
	_gridnode->addChild(backsp);

	string namestr[4]={
		"Button_dj",
		"Button_lj",
		"Button_td",
		"Button_ts"
	};

	for(int i=0;i<4;i++)
	{
		auto menu=dynamic_cast<ui::Button*>(backcsb->getChildByName(namestr[i]));
		menu->addTouchEventListener([=](Ref*,ui::Widget::TouchEventType type){
			if(type==ui::Widget::TouchEventType::BEGAN)
			{
				cjMusic::playEffect("video/tap.mp3");
				menu->setScale(MENU_SCALE*1.1f);
			}
			else if(type==ui::Widget::TouchEventType::MOVED)
				menu->setScale(1.1f);
			else if(type==ui::Widget::TouchEventType::ENDED)
			{
				menu->setScale(1.1f);

				if(i==0)
					GameData::getInstance()->d_Project=GAME_START;
				else if(i==1)
					GameData::getInstance()->d_Project=GAME_ANGLE;
				else if(i==2)
					GameData::getInstance()->d_Project=GAME_1010;
				else if(i==3)
				{
					GameData::getInstance()->d_Model=MODEL_POINT;
					GameData::getInstance()->d_Project=GAME_TETRIS;
				}
				if(_xieyi)
				{
					if(GameData::getSaveData()->_xieyi)
					{
						GameData::getSaveData()->_xieyi=false;
						GameData::getInstance()->dataSave();
					}
	
					//进入游戏
					GameData::getInstance()->d_time=time(NULL);
					Director::getInstance()->replaceScene(GameScene::createScene());
				}
				else
				{
					Toast::makeText("xieyi")->show();
				}
			}
		});
	}

#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS|| IOSMODE)
	this->schedule(schedule_selector(MainScene::checkVideoState), 1.0f);

	auto Button_mfzs= dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_mfzs"));
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
                    
                    PayScene::getInstance()->dailyClickTip(10,2);
                    PayScene::getInstance()->addDiamondPlist(this, 10);
				}
			});
		}
	});

	Button_mfzs->runAction(RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(RotateTo::create(0.2f,5.0f),RotateTo::create(0.2f,-5.0f))));
	Button_mfzs->setVisible(false);

	auto hand = handNode::createHand(0.4f);
	this->addChild(hand, 2,"mfzshand");
	hand->setPosition(Button_mfzs->getPosition()+Vec2(30,-10));
	hand->setVisible(false);

#else
	auto shop=dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_shop"));
	shop->addTouchEventListener([=](Ref *,ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			cjMusic::playEffect("video/tap.mp3");
			shop->setScale(MENU_SCALE);
		}
		else if(type==ui::Widget::TouchEventType::ENDED)
		{
			shop->setScale(1.0f);
			PayScene::getInstance()->payPage2();
		}
	});

	auto dhm=dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_dhm"));
	dhm->addTouchEventListener([=](Ref *,ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			cjMusic::playEffect("video/tap.mp3");
			dhm->setScale(MENU_SCALE);
		}
		else if(type==ui::Widget::TouchEventType::ENDED)
		{
			dhm->setScale(1.0f);
			PayDhm::CreateLayer();
		}
	});

	auto xslb=dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_xslb"));
	xslb->addTouchEventListener([=](Ref *,ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			cjMusic::playEffect("video/tap.mp3");
			xslb->setScale(MENU_SCALE);
		}
		else if(type==ui::Widget::TouchEventType::ENDED)
		{
			xslb->setScale(1.0f);
			PayScene::getInstance()->payPage3();
		}
	});

	if(GameDataInstance()->d_PayType&&vigame::pay::PayManager::getDefaultFeeInfo()&& vigame::pay::PayManager::getDefaultFeeInfo()->getFeePriceByID(6))
		xslb->setVisible(true);
	else
		xslb->setVisible(false);

	auto lb=dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_lb"));
	lb->addTouchEventListener([=](Ref *,ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			cjMusic::playEffect("video/tap.mp3");
			lb->setScale(MENU_SCALE);
		}
		else if(type==ui::Widget::TouchEventType::ENDED)
		{
			lb->setScale(1.0f);
			PayScene::getInstance()->payPage4();
		}
	});

	auto lbmotion = CSLoader::createTimeline("animation/mainpage.csb");
	backcsb->runAction(lbmotion);
	lbmotion->play("xslbmotion", true);

	if(GameData::getSaveData()->_xinshou&&GameDataInstance()->d_PayType&&vigame::pay::PayManager::getDefaultFeeInfo()&&(vigame::pay::PayManager::getDefaultFeeInfo()->getFeePriceByID(0)||vigame::pay::PayManager::getDefaultFeeInfo()->getFeePriceByID(7)))
	{
		lb->setVisible(true);
	}
	else
		lb->setVisible(false);

	auto mfzs=dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_mfzs"));
	mfzs->addTouchEventListener([=](Ref *,ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			cjMusic::playEffect("video/tap.mp3");
			mfzs->setScale(MENU_SCALE);
		}
		else if(type==ui::Widget::TouchEventType::MOVED)
			mfzs->setScale(1.0f);
		else if(type==ui::Widget::TouchEventType::ENDED)
		{
			mfzs->setScale(1.0f);
			//vigame::ad::ADManager::openAd("home_mfzs",new ADOpenResultCallback<MainScene>(this, &MainScene::openAD_MFZS_Finish));
		}
	});
	mfzs->setVisible(false);

	this->schedule(schedule_selector(MainScene::xinshouUpdate), 0.1f);

	//用户协议
	auto xieyi=backcsb->getChildByName("XieYi");
	if(GameData::getSaveData()->_xieyi)
	{
		auto xiyi_3=dynamic_cast<ui::Button*>(xieyi->getChildByName("Button_xiyi_3"));
		xiyi_3->addTouchEventListener([](Ref *,ui::Widget::TouchEventType type){
			if (type == ui::Widget::TouchEventType::ENDED)
			{
				cjMusic::playEffect("video/tap.mp3");
				vigame::UserAgreement::open();
			}
		});

		auto xiyi_5=xieyi->getChildByName("xiyi_5");

		auto xiyi_4=dynamic_cast<ui::Button*>(xieyi->getChildByName("Button_xiyi_4"));
		xiyi_4->addTouchEventListener([xiyi_5,this](Ref *,ui::Widget::TouchEventType type){
			if(type==ui::Widget::TouchEventType::BEGAN)
			{
				cjMusic::playEffect("video/tap.mp3");
				if(xiyi_5->isVisible())
				{
					xiyi_5->setVisible(false);
					_xieyi=false;
				}
				else
				{
					xiyi_5->setVisible(true);
					_xieyi=true;
				}
			}
		});
	}
	else
		xieyi->setVisible(false);
	//

	//更多游戏
	auto gdyx=dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_gdyx"));
	gdyx->addTouchEventListener([=](Ref *,ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			cjMusic::playEffect("video/tap.mp3");
			gdyx->setScale(MENU_SCALE);
		}
		else if(type==ui::Widget::TouchEventType::ENDED)
		{
			gdyx->setScale(1.0f);
			vigame::pay::PayManager::openMoreGame();
		}
	});
	gdyx->setVisible(false);
#ifdef EGAME
	if(PayManager::getInstance()->isMoreGame())
	{
		gdyx->setVisible(true);
	}
#endif
	//
#endif

	//签到
	auto qd = dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_qd"));
	qd->addTouchEventListener([=](Ref *, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			cjMusic::playEffect("video/tap.mp3");
			qd->setScale(MENU_SCALE);
		}
		else if (type == ui::Widget::TouchEventType::ENDED)
		{
			qd->setScale(1.0f);
			PayScene::getInstance()->dailyClick();
		}
	});
	//

	//大转盘
	auto turn=dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_turn"));
	turn->addTouchEventListener([=](Ref *,ui::Widget::TouchEventType type){
		if(type==ui::Widget::TouchEventType::BEGAN)
		{
			cjMusic::playEffect("video/tap.mp3");
			turn->setScale(MENU_SCALE);
		}			
		else if(type==ui::Widget::TouchEventType::ENDED)
		{
			turn->setScale(1.0f);
			PayScene::getInstance()->turnTable();
		}
	});

	if(vigame::MMChnlManager::getInstance()->getMMChnl()->lottery<=0||(GameData::getSaveData()->_freeroundnum>=20))
		turn->setVisible(false);
	//

	//设置
	set();
	//

	auto listener=EventListenerKeyboard::create();
	listener->onKeyReleased=CC_CALLBACK_2(MainScene::onKeyReleased,this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
    
	Director::getInstance()->setDepthTest(false);
	_wavenum = 0;

	auto listener2 = EventListenerTouchOneByOne::create();
	listener2->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);
	listener2->onTouchMoved = CC_CALLBACK_2(MainScene::onTouchMoved, this);
	listener2->onTouchEnded = CC_CALLBACK_2(MainScene::onTouchEnded, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener2, this);

	return true;
}


void MainScene::text(int k)
{

}

void exitGame()
{
	//umeng
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	umeng::MobClickCpp().end();
#endif
	vigame::ad::ADManager::openAd("exit_game");
	Director::getInstance()->end();

#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS|| IOSMODE)
	exit(0);
#endif
}

void MainScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if(vigame::pay::PayManager::isExitGame() == 1)
	{
		vigame::pay::PayManager::setOnGameExit(exitGame);
		vigame::pay::PayManager::openExitGame();
	}
	else
	{
		exitGame();
	}
}

void MainScene::set()
{
	_setstate=1;

	Menu* setbu=cjMenu::createWithImage("anniu_set.png",[this](Ref *mu){
		auto setbu=getChildByName("set");
		if (_setstate == 1)
		{
			setbu->setPosition(Vec2(35, 35));
			getChildByName("help")->setVisible(true);
			if (GameData::getSaveData()->_music)
			{
				getChildByName("sound2")->setVisible(true);
			}
			else
			{
				getChildByName("sound1")->setVisible(true);
			}

#ifdef EGAME
			getChildByName("aboutus")->setVisible(true);
#endif // EGAME

			_setstate=2;
		}
		else if (_setstate == 2)
		{
			setbu->setPosition(Vec2(50, 50));
			getChildByName("help")->setVisible(false);
			if (GameData::getSaveData()->_music)
				getChildByName("sound2")->setVisible(false);
			else
				getChildByName("sound1")->setVisible(false);

#ifdef EGAME
			getChildByName("aboutus")->setVisible(false);
#endif // EGAME

			_setstate=1;
		}
	},false);
	this->addChild(setbu,2,"set");
	setbu->setAnchorPoint(Vec2(0.0f,0.0f));
	setbu->setPosition(Vec2(50,50));

	auto help=cjMenu::createWithImage("anniu_help.png",[](Ref *rf){
		PayScene::getInstance()->help();
	},false);
	this->addChild(help,1,"help");
	help->setAnchorPoint(Vec2(0.0f,0.0f));
	help->setPosition(Vec2(45,95));
	help->setVisible(false);

	for(int i=0;i<2;i++)
	{
		string str;
		if(i==0)
			str="anniu_sound1.png";
		else if(i==1)
			str="anniu_sound2.png";
		auto sound=cjMenu::createWithImage(str,[i,this](Ref *rf){
			if(i==0)
			{
				GameData::getSaveData()->_music=true;
				getChildByName("sound1")->setVisible(false);
				getChildByName("sound2")->setVisible(true);
				cjMusic::playBackgroundMusic("video/music.mp3", true);
			}
			else if(i==1)
			{
				GameData::getSaveData()->_music=false;
				getChildByName("sound1")->setVisible(true);
				getChildByName("sound2")->setVisible(false);
				cjMusic::stopBackgroundMusck();
			}
			GameData::getInstance()->dataSave();
		},false);
		sound->setAnchorPoint(Vec2(0.0f,0.0f));
		this->addChild(sound,1,cjTTFLabel::getNameByInt("sound%d",i+1));

#ifdef EGAME
		sound->setScale(0.85f);
		sound->setPosition(Vec2(94,30));
#else
		sound->setPosition(Vec2(97,45));
#endif


		//if(GameData::getSaveData()->_music&&i==0)
		//	sound->setVisible(false);
		//else if(!GameData::getSaveData()->_music&&i==1)
		sound->setVisible(false);
	}

#ifdef EGAME
	help->setScale(0.9f);
	help->setPosition(Vec2(32,92));

	auto aboutus=cjMenu::createWithImage("anniu_about.png",[](Ref *mu){
		PayScene::getInstance()->aboutus();
	});
	this->addChild(aboutus,1,"aboutus");
	aboutus->setPosition(Vec2(77,75));
	aboutus->setVisible(false);
	aboutus->setAnchorPoint(Vec2(0.0f,0.0f));
	aboutus->setScale(0.9f);
#endif

}

void MainScene::xinshouUpdate(float ft)
{
	//
	auto csb=getChildByName("csb");

	auto xslb = dynamic_cast<ui::Button*>(csb->getChildByName("Button_xslb"));
	if (!xslb->isVisible())
	{
		if (GameDataInstance()->d_PayType&&vigame::pay::PayManager::getDefaultFeeInfo() && vigame::pay::PayManager::getDefaultFeeInfo()->getFeePriceByID(6))
			xslb->setVisible(true);
	}

	auto lb = dynamic_cast<ui::Button*>(csb->getChildByName("Button_lb"));
	if (!lb->isVisible()&&GameData::getSaveData()->_xinshou&&GameDataInstance()->d_PayType&&vigame::pay::PayManager::getDefaultFeeInfo() && (vigame::pay::PayManager::getDefaultFeeInfo()->getFeePriceByID(0) || vigame::pay::PayManager::getDefaultFeeInfo()->getFeePriceByID(7)))
	{
		lb->setVisible(true);
	}
	else if(lb->isVisible()&&!GameData::getSaveData()->_xinshou)
	{
		lb->setVisible(false);
	}
}

bool MainScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	_wavenum++;
	_gridnode->runAction(Sequence::create(Ripple3D::create(1.5f, CCSizeMake(32, 24), touch->getLocation(), 300, 4, 30),
		CallFunc::create([this]() {
		_wavenum--;
		if (_wavenum == 0)
			_gridnode->runAction(Ripple3D::create(0.1f, CCSizeMake(32, 24), Vec2(0,0), 10, 1, 0));
	}), nullptr));
	return false;
}

void MainScene::onTouchesMoved(Touch *touch, Event *unused_event)
{}

void MainScene::onTouchEnded(Touch *touch, Event *unused_event)
{}
