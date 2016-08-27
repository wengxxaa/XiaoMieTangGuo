#include "GameScene.h"
#include "../cj/cjconfig.h"
#include "vigame_ad.h"
#include "MobClickCpp.h"
#include "../gameconfig/PayScene.h"
#include"../gameconfig/AngleManage.h"
#include "../gameconfig/GameManage.h"
#include "../gameconfig/SceneManage.h"
#include"../gameconfig/StartManage.h"
#include"../gameconfig//BaseManage.h"
#include "../cj/Toast.h"
#include "vigame_pay.h"
#include "cocostudio/CocoStudio.h"
using namespace cocostudio;


const int uph=783;
const int downh=710;
const float movetime=0.1f;
const int nextboxoffest=110;
const int heightoffest=73;
const int selectboxoffest=136;	//ø…π©—°‘Ò∑ΩøÈµƒ∏ﬂ∂»
const float selectboxscale=0.5f;

GameScene::GameScene(){

}

GameScene::~GameScene(){

}

bool GameScene::init()
{
	if(!Layer::init())
		return false;

	this->setTag(99);

	cjMusic::stopBackgroundMusck();

#if(CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	vigame::ad::ADManager::openAd("banner");
#endif

	string csbstr;
	if(GameDataInstance()->d_Project==GAME_START)
		csbstr="animation/background_star.csb";
	else if(GameDataInstance()->d_Project==GAME_ANGLE)
		csbstr="animation/background_angle.csb";
	else
		csbstr="animation/background_1010.csb";
	_backnode=CSLoader::createNode(csbstr);
	this->addChild(_backnode,1);
	_backnode->setPosition(WINORIGIN);

	//≤®Œ∆
	if (GameDataInstance()->d_Project == GAME_START)
	{
		auto bbbsp = Sprite::create("animation/Mainpage/background.png");
		this->addChild(bbbsp);
		bbbsp->setAnchorPoint(Vec2(0.5f, 0.5f));
		bbbsp->setPosition(WINSIZE / 2.0f);

		auto backsp = _backnode->getChildByName("background_4");
		backsp->retain();
		backsp->removeFromParent();

		_gridnode = NodeGrid::create();
		this->addChild(_gridnode);
		_gridnode->setPosition(WINORIGIN);
		_gridnode->addChild(backsp);
	}
	//

	auto pause=dynamic_cast<ui::Button*>(_backnode->getChildByName("pause"));
	pause->setGlobalZOrder(5);
	pause->addTouchEventListener([=](Ref* pSender, ui::Widget::TouchEventType touchType){
		if(touchType==ui::Widget::TouchEventType::ENDED)
		{
			auto lay=Director::getInstance()->getRunningScene()->getChildByTag(100);
			if(!lay)
				pausePage();
					
		}
	});

	auto diamondadd=dynamic_cast<ui::Button*>(_backnode->getChildByName("zuanshiadd"));
	if(diamondadd)
	{
		diamondadd->setGlobalZOrder(5);
		diamondadd->addTouchEventListener([=](Ref* pSender, ui::Widget::TouchEventType touchType){
			if(touchType==ui::Widget::TouchEventType::ENDED)
			{
				if (!GameDataInstance()->_guide)
				{
					auto lay = Director::getInstance()->getRunningScene()->getChildByTag(100);
					if (!lay)
					{
						cjMusic::playEffect("video/tap.mp3");
#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS|| IOSMODE)
						PayScene::getInstance()->freeDiamond();
#else
						PayScene::getInstance()->payPage1();
#endif
					}
				}
			}
		});
	}

	//≥ı ºªØ∑÷ ˝£¨µ»º∂£¨ƒø±Íœ‘ æ
	setgScore(0);
	setgLevel(0);
	setgTarget(0);
	setgDiamond(0);

	_levelfont=cjAtlasLabel::createWithInt(0,ATLAS_PATH);
	this->addChild(_levelfont,1,"levelfont");
	_levelfont->setPosition(_backnode->getChildByName("level")->getPosition());
	_levelfont->setScale(0.9f);

	_targetfont=cjAtlasLabel::createWithInt(0,ATLAS_PATH);
	this->addChild(_targetfont,1,"targetfont");
	_targetfont->setPosition(_backnode->getChildByName("mubiao")->getPosition()+Vec2(17,0));
	_targetfont->setScale(0.8f);

	if(_backnode->getChildByName("zuigao"))
	{
		int highsc=0;
		if(GameDataInstance()->d_Project==GAME_START)
			highsc=GameData::getSaveData()->_start._highestscore;
		else if(GameDataInstance()->d_Project==GAME_ANGLE)
			highsc=GameData::getSaveData()->_angle._highestscore;
		_highfont=cjAtlasLabel::createWithInt(highsc,ATLAS_PATH);
		this->addChild(_highfont,1,"highfont");
		_highfont->setPosition(_backnode->getChildByName("zuigao")->getPosition()+Vec2(17,0));
		_highfont->setScale(0.8f);
	}

	_scorefont=cjAtlasLabel::createWithInt(0,ATLAS_PATH);
	this->addChild(_scorefont,1,"scorefont");
	_scorefont->setPosition(_backnode->getChildByName("fenshu")->getPosition());
	if(GameDataInstance()->d_Project==GAME_ANGLE||GameDataInstance()->d_Project==GAME_START)
		_scorefont->setScale(1.3f);
	else
		_scorefont->setScale(0.9f);

	if(_backnode->getChildByName("zuanshi"))
	{
		_diamondfont=cjAtlasLabel::createWithInt(0,ATLAS_PATH);
		this->addChild(_diamondfont,1,"diamondfont");
		_diamondfont->setPosition(_backnode->getChildByName("zuanshi")->getPosition()+Vec2(12,0));
	}

	//≥ı ºªØnextbox savebox
	if(GameDataInstance()->d_Project==GAME_TETRIS)
	{
		setgStep(0);
		for(int i=0;i<3;i++)
		{
			_nextBoxPosition[i]=Vec2(WINORIGIN+Vec2(60+nextboxoffest*i,downh));
		}
	}
	rotate=cjMenu::createWithImage("button_1.png",[this](Ref *rf){
		if(GameDataInstance()->d_Project==GAME_TETRIS)
		{
			auto manage=(SceneManage*)this->getChildByName("manage");
			manage->Rotate();
		}
		else if(GameDataInstance()->d_Project==GAME_1010)
		{
			auto manage=(GameManage*)this->getChildByName("manage");
			manage->Rotate();
		}
	});
	this->addChild(rotate,5,"rotate");

	startGame();

	schedule(schedule_selector(GameScene::dateUpdate),0.1f);
	
	auto listener=EventListenerKeyboard::create();
	listener->onKeyReleased=CC_CALLBACK_2(GameScene::onKeyReleased,this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);

	if (GameDataInstance()->d_Project == GAME_START)
	{
		Director::getInstance()->setDepthTest(false);
		_wavenum = 0;

		auto listener2 = EventListenerTouchOneByOne::create();
		listener2->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
		listener2->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
		listener2->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener2, this);
	}
	return true;
}

void GameScene::dateUpdate(float ft)
{
	bool temp=false;
	int score=0;
	int level=1;
	int target=500;
	int stepnumber=0;
	ShapeIndex nbox[3];
	GameState gstate;

	if(_backnode->getChildByName("zuanshi")&&GameData::getSaveData()->_diamondNumber!=getgDiamond())
	{
		_diamondfont->setString(cjTTFLabel::getNameByInt("%d", GameData::getSaveData()->_diamondNumber));
		setgDiamond(GameData::getSaveData()->_diamondNumber);
	}

	BaseManage *manage =dynamic_cast<BaseManage*>(getChildByName("manage"));
	if (manage)
	{
		temp = true;
		score = manage->getmScore();
		level = manage->getmLevel();
		target = manage->getmTarget();
		stepnumber = manage->getmStepNumber();
		gstate = manage->getGameState();
		for (int i = 0; i<3; i++)
			nbox[i] = manage->getNextBox(i);
	}

	if(temp)
	{
		if(getgScore()!=score)
		{
			int frontsc = 0;
			int endsc = 0;
			if (getgScore() < score)
			{
				frontsc = getgScore();
				endsc = score;
			}
			setgScore(score);
			float tt=0.6f;
			if(GameDataInstance()->d_Project==GAME_START||GameDataInstance()->d_Project==GAME_ANGLE)
				tt=0.8f;
			runAction(Sequence::create(DelayTime::create(tt),CallFunc::create([=](){
				if (endsc == 0)
					_scorefont->setString(cjTTFLabel::getNameByInt("%d", score));
				else
					scoreChange(frontsc, endsc);
			}),nullptr));
		}
		if(getgLevel()!=level)
		{
			setgLevel(level);
			_levelfont->setString(cjTTFLabel::getNameByInt("%d",getgLevel()));
		}
		if(getgTarget()!=target)
		{
			setgTarget(target);
			_targetfont->setString(cjTTFLabel::getNameByInt("%d",getgTarget()));
		}

		if(GameDataInstance()->d_Project==GAME_TETRIS&&getgStep()!=stepnumber&&stepnumber>0)
		{
			setgStep(stepnumber);
			if(getgStep()==1)
			{
				for(int i=0;i<3;i++)
				{
					_nextbox[i]=nbox[i];
					auto nextbox=BoxNode::createBox(_nextbox[i],int(_nextbox[i]));
					nextbox->setScale(selectboxscale);
					this->addChild(nextbox,3,15+i);
					nextbox->setPosition(nextbox->getCenterAnPoint(_nextBoxPosition[i]));
				}
			}
			else
			{
				this->removeChildByTag(15);
				for(int i=0;i<2;i++)
				{
					_nextbox[i]=nbox[i];
					auto box=this->getChildByTag(16+i);
					box->setTag(15+i);
					box->runAction(MoveBy::create(movetime,Vec2(-nextboxoffest,0)));
				}
				_nextbox[2]=nbox[2];
				auto nextbox=BoxNode::createBox(_nextbox[2],int(_nextbox[2]));
				nextbox->setScale(selectboxscale);
				this->addChild(nextbox,4,17);
				nextbox->setVisible(false);
				nextbox->setPosition(nextbox->getCenterAnPoint(_nextBoxPosition[2]));
				nextbox->runAction(Sequence::create(CCDelayTime::create(movetime),Show::create(),nullptr));
			}
		}

		if(STATE_FAILD==gstate)
		{
			runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]() {
				faildPage();
			}), nullptr));
			unschedule(schedule_selector(GameScene::dateUpdate));
		}
	}
}

void GameScene::pausePage()
{
	if(GameDataInstance()->_guide)
		return;

	cjMusic::playEffect("video/tap.mp3");

	vigame::ad::ADManager::openAd("pause");

	BaseManage *manage = dynamic_cast<BaseManage*>(getChildByName("manage"));
	manage->pause();

	auto layer=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING),WINSIZE.width,WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(layer,10,100);
	layer->ignoreAnchorPointForPosition(false);
	layer->setAnchorPoint(Vec2(0.5f,0.5f));
	layer->setPosition(WINORIGIN+WINSIZE/2.0f);

	char name[30];
	for(int i=0;i<3;i++)
	{
		if(i==0)
			sprintf(name,"button_jx.png");
		else if(i==1)
			sprintf(name,"button_cx.png");
		else if(i==2)
			sprintf(name,"button_fh.png");

		auto menu=cjMenu::createWithImage("button_back.png",[i,layer,this](Ref *mu){
			cjMusic::playEffect("video/tap.mp3");
			layer->removeFromParent();
			if(i==0)
				start();
			else if(i==1)
			{
				restart();
			}
			else if(i==2)
			{
				//”Œœ∑¥Êµµ
				//if(GameData::getInstance()->d_Model==MODEL_POINT)
				//	GameData::getInstance()->dataSave(manage);
				//

				recordStartHighScore();

				vigame::ad::ADManager::closeAd("banner");
				UMCount();

				Director::getInstance()->replaceScene(MainScene::createScene());
			}
		},false);
		layer->addChild(menu,2);

		menu->setPosition(Vec2(WINSIZE.width/2.0f,WINSIZE.height*(0.7f-0.2f*i)));

		auto sp=cjSprite::createWithImage(name);
		layer->addChild(sp,2);
		sp->setPosition(menu->getPosition()+Vec2(0,5));
	}

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	layer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, layer);
}

void GameScene::faildPage()
{
	if(GameDataInstance()->_guide)
		return;

	cjMusic::playEffect("video/faild.mp3");

	vigame::ad::ADManager::openAd("level_fail");

	if(GameDataInstance()->d_Project==GAME_TETRIS)
	{
		for(int i=0;i<3;i++)
		{
			auto box=(BoxNode*)getChildByTag(15+i);
			box->setGray();
		}
	}

	faildlayer=Layer::create();
	Director::getInstance()->getRunningScene()->addChild(faildlayer,12,100);

	auto layer=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING),WINSIZE.width,WINSIZE.height);
	faildlayer->addChild(layer);
	layer->ignoreAnchorPointForPosition(false);
	layer->setAnchorPoint(Vec2(0.5f,0.5f));
	layer->setPosition(WINORIGIN+WINSIZE/2.0f);

	auto backcsb=CSLoader::createNode("animation/faildpage.csb");
	layer->addChild(backcsb);
	backcsb->setAnchorPoint(Vec2(0.5f,0.5f));
	backcsb->setPosition(WINSIZE/2.0f);

//	auto csbbox = backcsb->getChildByName("bg")->getChildByName("ren");
//	auto csbaction = CSLoader::createTimeline("animation/faildpagenode.csb");
//	csbbox->runAction(csbaction);
//	csbaction->play("UI_fail", true);

	auto bg = backcsb->getChildByName("bg");

#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS|| IOSMODE)
	auto jxtg = bg->getChildByName("Button_jxtg");
	jxtg->setVisible(false);

	auto jxtg2 = dynamic_cast<ui::Button*>(bg->getChildByName("Button_jxtg2"));
	jxtg2->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::stopAllEffect();
			cjMusic::playEffect("video/tap.mp3");
			if (GameData::getSaveData()->_diamondNumber >= PAY_2_DIAMOND)
			{
				GameData::getSaveData()->_diamondNumber -= PAY_2_DIAMOND;
				faildlayer->removeFromParent();
				resurrection();
			}
			else
			{
				PayScene::getInstance()->freeDiamond();
			}
		}
	});

	auto mffh = dynamic_cast<ui::Button*>(bg->getChildByName("Button_mffh"));
	mffh->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::stopAllEffect();
			cjMusic::playEffect("video/tap.mp3");
			vigame::ad::ADManager::openAd("level_fail_mfzs", [=](vigame::ad::ADSourceItem* pADItem,int result) {
				if (result == vigame::ad::ADSourceItem::open_result::OpenSuccess)
				{
					faildlayer->removeFromParent();
					resurrection();
				}
			});
		}
	});

	mffh->runAction(CCRepeatForever::create((CCActionInterval*)Sequence::createWithTwoActions(ScaleTo::create(0.5f, 0.83f), ScaleTo::create(0.5f, 0.77f))));

	auto hand = handNode::createHand(0.4f);
	layer->addChild(hand, 2);
	hand->setPosition(mffh->getPosition() + Vec2(50, -20));
	
#else
	auto mffh = bg->getChildByName("Button_mffh");
	mffh->setVisible(false);
	auto jxtg2 = bg->getChildByName("Button_jxtg2");
	jxtg2->setVisible(false);
	auto tip1 = bg->getChildByName("tips2");
	tip1->setVisible(false);

	auto jxtg = dynamic_cast<ui::Button*>(bg->getChildByName("Button_jxtg"));
	jxtg->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::stopAllEffect();
			cjMusic::playEffect("video/tap.mp3");
			if (GameData::getSaveData()->_diamondNumber >= PAY_2_DIAMOND)
			{
				GameData::getSaveData()->_diamondNumber -= PAY_2_DIAMOND;
				faildlayer->removeFromParent();
				resurrection();
			}
			else
			{
				if (GameDataInstance()->d_PayType&&vigame::pay::PayManager::getDefaultFeeInfo() && vigame::pay::PayManager::getDefaultFeeInfo()->getFeePriceByID(1))
					PayScene::getInstance()->payPage1();
				else
				{
					recordStartHighScore();
					faildlayer->removeFromParent();
					vigame::ad::ADManager::closeAd("banner");
					UMCount();
					Director::getInstance()->replaceScene(MainScene::createScene());
				}
			}
		}
	});

	auto hand = handNode::createHand(0.6f);
	layer->addChild(hand, 2);
	hand->setPosition(jxtg->getPosition() + Vec2(120, 20));

#endif
	////

	auto cancel=dynamic_cast<ui::Button*>(bg->getChildByName("cha"));
	cancel->addTouchEventListener([this](Ref*,ui::Widget::TouchEventType type){
		if(type==ui::Widget::TouchEventType::ENDED)		{
			cjMusic::stopAllEffect();
			cjMusic::playEffect("video/tap.mp3");
			recordStartHighScore();
			faildlayer->removeFromParent();
			vigame::ad::ADManager::closeAd("banner");
			UMCount();
			Director::getInstance()->replaceScene(MainScene::createScene());
		}
	});

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	layer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, layer);
}


void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	//auto lay=Director::getInstance()->getRunningScene()->getChildByTag(100);
	//auto faildlay=Director::getInstance()->getRunningScene()->getChildByTag(99);
	//if(lay)
	//{
	//	lay->removeFromParent();
	//}
	//else if(!faildlay)
	//{
	//	pausePage();
	//}
}

void GameScene::UMCount()
{
	int ti=time(NULL)-GameData::getInstance()->d_time;
	assert(ti>=0);
	int kk=(ti/10+1)*10;
	char name1[30];
	char name2[30];
	char name3[30];
	sprintf(name1,"%d",getgLevel());
	sprintf(name2,"%d",kk);
	//umeng
	umeng::eventDict dict;
	dict["level"]=name1;
	dict["time"]=name2;
	if(GameData::getInstance()->d_Project==GAME_TETRIS)
	{
		GameModel mm=GameData::getInstance()->d_Model;
		if(mm==MODEL_POINT)
			sprintf(name3,"ChallengeModel");
		else if(mm==MODEL_SLIP)
			sprintf(name3,"ClassicModel");
	}
	else if(GameData::getInstance()->d_Project==GAME_1010)
	{
		sprintf(name3,"DropModel");
	}
	else if(GameData::getInstance()->d_Project==GAME_START)
	{
		sprintf(name3,"StartModel");
	}
	else if(GameData::getInstance()->d_Project==GAME_ANGLE)
	{
		sprintf(name3,"AngleModel");
	}

#if (CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	//umeng
	umeng::MobClickCpp::event(name3,&dict);
#endif
}

void GameScene::start()
{
	BaseManage *manage = dynamic_cast<BaseManage*>(getChildByName("manage"));
	manage->start();
}

void GameScene::resurrection()
{
	if (GameDataInstance()->d_Project == GAME_TETRIS)
	{
		for (int i = 0; i<3; i++)
		{
			auto box = (BoxNode*)getChildByTag(15 + i);
			box->setNormal();
		}
	}

	BaseManage *manage = dynamic_cast<BaseManage*>(getChildByName("manage"));
	manage->Resurrection();

	if(GameDataInstance()->d_Project==GAME_START)
	{
		int kk=manage->getmScore();
		if(getgScore()!=kk)
		{
			setgScore(kk);
			_scorefont->setString(cjTTFLabel::getNameByInt("%d",kk));
		}
	}
	runAction(Sequence::create(DelayTime::create(1.0f),CallFunc::create([this](){
		schedule(schedule_selector(GameScene::dateUpdate),0.1f);
	}),nullptr));
}

void GameScene::restart()
{
	recordStartHighScore();

	setgLevel(0);
	setgTarget(0);
	setgScore(-1);
	setgStep(0);
	if(GameDataInstance()->d_Project!=GAME_START)
		((LabelAtlas*)getChildByName("scorefont"))->setString("0");
	if(GameDataInstance()->d_Project==GAME_1010)
	{
		GameData::getSaveData()->_onezero._istip=false;
		removeChildByName("manage");
		auto manage=GameManage::create();
		this->addChild(manage,1,"manage");
		manage->run();
	}
	else if(GameDataInstance()->d_Project==GAME_START)
	{
		GameData::getSaveData()->_start._istip=false;
		removeChildByName("manage");
		auto manage=StartManage::create();
		this->addChild(manage,1,"manage");
		manage->run();
	}
	else if(GameDataInstance()->d_Project==GAME_TETRIS)
	{
		if(GameDataInstance()->d_Model==MODEL_POINT)
			GameData::getSaveData()->_challengesstetris._istip=false;
		removeChildByName("manage");
		auto manage=SceneManage::create();
		this->addChild(manage,1,"manage");
		manage->run();
		for(int i=0;i<3;i++)
			this->removeChildByTag(15+i);
	}
	else if(GameDataInstance()->d_Project==GAME_ANGLE)
	{
		GameData::getSaveData()->_angle._istip=false;
		removeChildByName("manage");
		auto manage=AngleManage::create();
		this->addChild(manage,1,"manage");
		manage->run();
	}
	GameData::getInstance()->dataSave();
}

//º«¬º–«–«◊Ó∏ﬂ∑÷
void GameScene::recordStartHighScore()
{
	if(GameDataInstance()->d_Project==GAME_START)
	{
		if(getgScore()>GameData::getSaveData()->_start._highestscore)
		{
			GameData::getSaveData()->_start._highestscore=getgScore();
			GameData::getInstance()->dataSave();
		}
	}
	else if(GameDataInstance()->d_Project==GAME_ANGLE)
	{
		if(getgScore()>GameData::getSaveData()->_angle._highestscore)
		{
			GameData::getSaveData()->_angle._highestscore=getgScore();
			GameData::getInstance()->dataSave();
		}
	}
}

void GameScene::startGame()
{
	if(GameDataInstance()->d_Project==GAME_TETRIS)
	{
		rotate->setPosition(WINORIGIN+Vec2(415,downh));

		auto manage=SceneManage::create();
		this->addChild(manage,4,"manage");
		manage->run();
	}
	else if(GameDataInstance()->d_Project==GAME_1010)
	{
		rotate->setVisible(false);

		auto manage=GameManage::create();
		this->addChild(manage,4,"manage");
		manage->run();
	}
	else if(GameDataInstance()->d_Project==GAME_START)
	{
		rotate->setVisible(false);

		auto manage=StartManage::create();
		this->addChild(manage,4,"manage");
		manage->run();
	}
	else if(GameDataInstance()->d_Project==GAME_ANGLE)
	{
		rotate->setVisible(false);

		auto manage=AngleManage::create();
		this->addChild(manage,4,"manage");
		manage->run();
	}
}

//∑÷ ˝±‰ªØ
void GameScene::scoreChange(int frontsc, int endsc)
{
	static int addnum = 1;
	addnum = 1;
	int addsc=1;
	int dissc = endsc - frontsc;
	int round = dissc;
	if (dissc >= 60)
	{
		addsc = (endsc - frontsc) / 60;
		round = 60;
	}

	Vector<FiniteTimeAction*> action;
	for (int i = 1; i <= round; i++)
	{
		action.pushBack(CallFunc::create([=]() {
			if (addnum == round)
				_scorefont->setString(cjTTFLabel::getNameByInt("%d", endsc));
			else
				_scorefont->setString(cjTTFLabel::getNameByInt("%d", frontsc + addnum*addsc));
			addnum++;
		}));
		action.pushBack(DelayTime::create(1 / 60.0f));
	}
	_scorefont->runAction(Sequence::create(action));

	float ff;
	if (GameDataInstance()->d_Project == GAME_ANGLE || GameDataInstance()->d_Project == GAME_START)
		ff = 1.3f;
	else
		ff = 0.9f;
	_scorefont->runAction(Sequence::create(ScaleTo::create(0.5f,ff*1.3f),ScaleTo::create(0.5f,ff),nullptr));
}

//◊Í Ø±‰ªØ
void GameScene::diamondChange(int frontsc, int endsc)
{
	static int addnum = 1;
	int addsc = 1;
	int dissc = endsc - frontsc;
	int round = dissc;
	if (dissc >= 60)
	{
		addsc = (endsc - frontsc) / 60;
		round = 60;
	}

	Vector<FiniteTimeAction*> action;
	for (int i = 1; i <= round; i++)
	{
		action.pushBack(CallFunc::create([=]() {
			if (addnum == round)
				_diamondfont->setString(cjTTFLabel::getNameByInt("%d", endsc));
			else
				_diamondfont->setString(cjTTFLabel::getNameByInt("%d", frontsc + addnum*addsc));
			addnum++;
		}));
		action.pushBack(DelayTime::create(1 / 60.0f));
	}
	_scorefont->runAction(Sequence::create(action));
}

bool GameScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	//	_gridnode->runAction(Ripple3D::create(1.5f, CCSizeMake(30, 30), touch->getLocation(), 300, 4, 60));
	_wavenum++;
	_gridnode->runAction(Sequence::create(Ripple3D::create(1.5f, CCSizeMake(32, 24), touch->getLocation(), 300, 4, 30),
		CallFunc::create([this]() {
		_wavenum--;
		if (_wavenum == 0)
			_gridnode->runAction(Ripple3D::create(0.1f, CCSizeMake(32, 24), Vec2(0, 0), 10, 1, 0));
	}), nullptr));
	return false;
}

void GameScene::onTouchesMoved(Touch *touch, Event *unused_event)
{}

void GameScene::onTouchEnded(Touch *touch, Event *unused_event)
{}