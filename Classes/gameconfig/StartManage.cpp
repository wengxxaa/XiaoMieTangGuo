#include"StartManage.h"
#include"ConfigState.h"
#include"BaseConfig.h"
#include"../cj/cjconfig.h"
#include "vigame_ad.h"
#include "vigame_pay.h"
#include "../cj/GameData.h"
#include "../cj/Toast.h"
#include "cocostudio/CocoStudio.h"
#include "PayScene.h"
#include "cocostudio/ActionTimeline/CCTimeline.h"
#include "../gamescene/GameScene.h"
#include "vigame_core.h"
#include "OffersHelper.h"
#include "core/MMChnlManager.h"
#include "old/Wb_Utils_Static.h"
using namespace cocostudio;

const float chessmovetime=0.2f;
const float boxeliminatetime=0.5f;
const float boxremovesingletime=0.12f;
const int downh=730;
const float boxscale=0.735f;
const float cleardelaytime=0.1f;	//«Â≥˝Õ£∂Ÿ ±º‰
const float tipdelaytime=0.6f;	//Ã· æµ»¥˝ ±º‰
const Vec2 diamondpos=Vec2(370,712);
const float propscale = 0.9f;	//µ¿æﬂÀı∑≈

const int diamondnum[4]={10,10,10,50};

const Color3B propcolor=Color3B(255,255,255);

static int kkk=0;

void StartManage::update(float ft)
{
	kkk++;
	if (kkk == 500)
		this->unscheduleUpdate();

	addParticle(5, 5, 1, 1);
}

void StartManage::start()
{
	setGameState(STATE_RUN);
}

void StartManage::pause()
{
	setGameState(STATE_PAUSE);
}

void StartManage::run()
{
	setGameState(STATE_RUN);
}

bool StartManage::init()
{
	if(!Layer::init())
		return false;

	kkk=0;
//	scheduleUpdate();

	heightoffest=GameDataInstance()->BANER_HEIGHT;

	_selectprop=0;
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS|| IOSMODE)
//	postBoard();
#endif
	initChess();
	initData();
	initProps();

	LevelUpMotion();
	runAction(Sequence::create(DelayTime::create(1.0f),CallFunc::create([this](){
		startDrop();
	}),nullptr));

	auto listener=EventListenerTouchOneByOne::create();
	listener->onTouchBegan=CC_CALLBACK_2(StartManage::onTouchBegan,this);
	listener->onTouchMoved=CC_CALLBACK_2(StartManage::onTouchMoved,this);
	listener->onTouchEnded=CC_CALLBACK_2(StartManage::onTouchEnded,this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
	listener->setSwallowTouches(true);

	return true;
}

void StartManage::initData()
{
	bool tempbool=false;
	_firstSaveIn=false;
	_candealwith=true;
	if(GameData::getSaveData()->_start._istip)
	{
		sdata=GameData::getSaveData()->_start;
		tempbool=true;
	}
	if(tempbool&&sdata._level>=1)
	{
		_firstSaveIn=true;
		sdata=GameData::getSaveData()->_start;
		setmLevel(sdata._level);
		setmScore(sdata._score);
		setmTarget(sdata._target);
		_lastLevelScore=sdata._lastlevelscore;

		//for(int i=0;i<CHESS_SIZE_W_START;i++)
		//	for(int j=0;j<CHESS_SIZE_H_START;j++)
		//		_chessInfo[i*CHESS_SIZE_H_START+j]=sdata._chessInfo[i][j];
	}
	else
	{
		setmLevel(1);
		setmScore(0);
		setmTarget(getTargetScoreByRound(1));
		_lastLevelScore=0;

		for(int i=0;i<CHESS_SIZE_W_START;i++)
			for(int j=0;j<CHESS_SIZE_H_START;j++)
				_chessInfo[i*CHESS_SIZE_H_START+j]=0;
	}

	_selectbox.clear();
	_guide=false;
	_guidenumber=-1;
	_eliminateNum=0;
	_eliminatePos.clear();

	if(!GameData::getSaveData()->_start._guider)
	{
		GameDataInstance()->_guide=true;
		_guide=true;
	}
}

void StartManage::initChess()
{
	Vec2 vec[4];	
	vec[0]=WINORIGIN+Vec2(WINSIZE.width/2.0f-CHESSMAN_SIZE_START*CHESS_SIZE_W_START/2,0+heightoffest);
	vec[1]=WINORIGIN+Vec2(WINSIZE.width/2.0f+CHESSMAN_SIZE_START*CHESS_SIZE_W_START/2,0+heightoffest);
	vec[2]=WINORIGIN+Vec2(WINSIZE.width/2.0f+CHESSMAN_SIZE_START*CHESS_SIZE_W_START/2,(CHESS_SIZE_H_START)*CHESSMAN_SIZE_START+heightoffest);
	vec[3]=WINORIGIN+Vec2(WINSIZE.width/2.0f-CHESSMAN_SIZE_START*CHESS_SIZE_W_START/2,(CHESS_SIZE_H_START)*CHESSMAN_SIZE_START+heightoffest);

	auto node=DrawNode::create();
	this->addChild(node,10);
	node->drawSolidPoly(vec,4,Color4F(1.0f,1.0f,1.0f,0.0f));

	_clip=ClippingNode::create(node);
	this->addChild(_clip,10);
	_clip->setInverted(false);

	//for(int i=0;i<CHESS_SIZE_W_START;i++)
	//	for(int j=0;j<CHESS_SIZE_H_START;j++)
	//	{
	//		auto sp=cjSprite::createWithImage("fangkuai_hs.png");
	//		this->addChild(sp);
	//		sp->setScale(CHESSMAN_SCALE_START);
	//		sp->setPosition(posToposition(make_pair(i,j)));
	//	}
}

void StartManage::initProps()
{
	_selectprop=0;

	ArmatureDataManager::getInstance()->addArmatureFileInfo("yellowman/yellowman.ExportJson");
	auto armature=Armature::create("yellowman");
	this->addChild(armature,3,"people");
	armature->setScale(0.22f);
	armature->setPosition(Vec2(54,680));
	peopleStand();

	auto parnode =cjSprite::createWithImage("ppp.png");
	this->addChild(parnode, 4, "par");
	parnode->setPosition(Vec2(53.3f, 733.0f));
	parnode->setAnchorPoint(Vec2(0.5f, 7.5f));
	parnode->setRotation(45.0f);
	parnode->setOpacity(0);

	auto par = ParticleSystemQuad::create("lizi/Particle_follow_2.plist");
	par->setAutoRemoveOnFinish(true);
	par->setPositionType(cocos2d::ParticleSystem::PositionType::RELATIVE);
	parnode->addChild(par);
	par->setRotation(-100.0f);
	//par->setScale(1.2f);
	par->setPosition(Vec2(5.5f,5.5f));

	auto hongbao=cjMenu::createWithImage("wf.png",[this](Ref *mu){
		if((getGameState()==STATE_RUN)&&(!_guide||(_guidenumber==3)))
		{
			if (_guidenumber == 3 || (!(Label *)getChildByName("hongbao")->isVisible()) && (GameData::getSaveData()->_hongbaotime == 0))
			{
				hongbaoPage();
				endTip();
			}
			else
			{
				_hongbaotip->stopAllActions();
				_hongbaotip->setVisible(true);
				_hongbaotip->runAction(Sequence::create(DelayTime::create(2.0f),CallFunc::create([=](){
					_hongbaotip->setVisible(false);
				}),nullptr));
			}
		}
	},false);
	this->addChild(hongbao,1,"hongbaomenu");
	hongbao->setAnchorPoint(Vec2(0.0f,0.0f));
	hongbao->setPosition(Vec2(160,650));
	hongbao->setScale(0.9f);

	char hbname[50];
	if(GameData::getSaveData()->_hongbaotime!=0)
		sprintf(hbname,"%02d:%02d",GameData::getSaveData()->_hongbaotime/60,GameData::getSaveData()->_hongbaotime%60);
	else
		sprintf(hbname,"00:00");
	auto _hongbaofont=cjTTFLabel::createWithString(hbname,20);
	this->addChild(_hongbaofont,1,"hongbao");
	_hongbaofont->setColor(Color3B::YELLOW);
	_hongbaofont->setPosition(hongbao->getPosition()-Vec2(0,50));
	if(_guide)
	{
		_hongbaofont->setVisible(false);
	}
	else
	{
		if(GameData::getSaveData()->_hongbaotime==0)
		{
			_hongbaofont->setVisible(false);
			auto hand=handNode::createHand(0.4f);
			this->addChild(hand,2,"hbhand");
			hand->setPosition(_hongbaofont->getPosition()+Vec2(0,50));
		}

		schedule(schedule_selector(StartManage::hongbaoFlush),1.0f);
	}

	_hongbaotip=LabelTTF::create(cjTTFLabel::getStringByKey("hongbaotip"),TTF_PATH,20);
	this->addChild(_hongbaotip,1);
	_hongbaotip->setPosition(Vec2(WINSIZE.width-150,propsheight-75));
	_hongbaotip->setVisible(false);
	_hongbaotip->setColor(Color3B::YELLOW);

	for(int i=0;i<4;i++)
	{
		auto sp=getProp(i+1);
		this->addChild(sp,1,cjTTFLabel::getNameByInt("prop%d",i+1));
		sp->setPosition(Vec2(WINSIZE.width-40-65*(3-i),propsheight));

		sp->setScale(propscale);

		if(i==0||i==1||i==3)
			sp->getChildByName("node")->setVisible(false);

		if(!_guide)
		{
			string str = "zuan_10.png";
			if (i == 3)
				str = "zuan_50.png";
			auto price = cjSprite::createWithImage(str);
			this->addChild(price,1);
			price->setPosition(Vec2(sp->getPosition().x,propsheight-40));

			if(GameData::getSaveData()->_propnum[i]>0)
			{
				_propnum[i]=cjTTFLabel::createWithString(cjTTFLabel::getNameByInt("x%d",GameData::getSaveData()->_propnum[i]));
				this->addChild(_propnum[i],2);
				_propnum[i]->setColor(propcolor);
				_propnum[i]->setPosition(sp->getPosition()+Vec2(20,-15));
			}
		}
	}

	//ª°–ŒΩ¯∂»Ãı
	auto husp = cjSprite::createWithImage("image3.png");
	_huptimer = ProgressTimer::create(husp);
	this->addChild(_huptimer, 2);
	_huptimer->setScale(0.75f);
	_huptimer->setPosition(Vec2(53, 734));
	_huptimer->setType(ProgressTimer::Type::RADIAL);
	_huptimer->setReverseDirection(true);

	if(GameData::getSaveData()->_hunum>=GameData::getSaveData()->_huhightnum||_guide)
		GameData::getSaveData()->_hunum=0;
	_hutempnum = 0;
	_hunum=GameData::getSaveData()->_hunum;

	parnode->setRotation(45.0f - (_hunum*1.0f / GameData::getSaveData()->_huhightnum)*200.0f);

	this->schedule(schedule_selector(StartManage::huptUpdate));
}

//π´∏Ê¿∏
void StartManage::postBoard()
{
	_postlayer = LayerColor::create(Color4B(26,21,91,255), 490, 30);
	this->addChild(_postlayer, 1);
	_postlayer->setAnchorPoint(Vec2(0.0f, 0.5f));
	_postlayer->setPosition(Vec2(-5, 560));

    //公告
//    OffersHelper::openHeadlineDialog();
    
/*    auto re=CCRect(0, 560, 480, 30);
    _postboard = ScrollLabel::createWithRect(re, TTF_PATH, 20);
	this->addChild(_postboard,1);
	_postboard->setFontColor(Color3B::YELLOW);

    auto image = vigame::notice::getUrl();
	if (vigame::notice::open())
	{

		_postboard->addStringToQueue(image);
	}

    std::string url = vigame::MMChnlManager::getInstance()->getMMChnl()->headline;
    cocos2d::log("openHeadlineDialog:url=%s",url.c_str());
    if (url.size() == 0){
        return;
    }*/
   // Wb_Utils_Static::openURL(image);
    
	//for (int i = 0; i < 10; i++)
	//{
	//	_postboard->addStringToQueue(cjTTFLabel::getNameByInt("wo shi yi ge gong gao lan %d",i));
	//}

	//_postboard->setFinishCallfunc(this, (SEL_CallFunc)(&StartManage::postEvent));

}

void StartManage::postEvent()
{
	for (int i = 0; i < 10; i++)
	{
		_postboard->addStringToQueue(cjTTFLabel::getNameByInt("wo shi yi ge gong gao lan %d", i));
	}
}

void StartManage::setPostShow(bool key)
{
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS|| IOSMODE)
//	_postlayer->setVisible(key);
//	_postboard->setVisible(key);
#endif
}

void StartManage::peopleStand()
{
	auto people=dynamic_cast<Armature*>(getChildByName("people"));
	auto key=rand()%4+1;
	people->getAnimation()->play(cjTTFLabel::getNameByInt("stand%d",key),-1,1);
}

void StartManage::hongbaoFlush(float ft)
{
	auto hongbaofont=(Label *)getChildByName("hongbao");
	char hbname[30];
	if(GameData::getSaveData()->_hongbaotime==0&&hongbaofont->isVisible())
	{
		auto menu=this->getChildByName("hongbaomenu");
		Vector<FiniteTimeAction*> action;
		for(int i=4;i>0;i--)
		{
			action.pushBack(RotateTo::create(0.05f*i,5.0f*i));
			action.pushBack(RotateTo::create(0.05f*i,-5.0f*i));
		}
		action.pushBack(RotateTo::create(0.05f,0.0f));
		menu->runAction(Sequence::create(action));

		int k=GameData::getSaveData()->_hongbaonum;
		if(k<5)
			k++;

		hongbaofont->setVisible(false);
		sprintf(hbname,"%02d:%02d",TIME_HONGBAO[k]/60,TIME_HONGBAO[k]%60);
		hongbaofont->setString(hbname);

		auto hand=handNode::createHand(0.4f);
		this->addChild(hand,2,"hbhand");
		hand->setPosition(hongbaofont->getPosition()+Vec2(0,50));
	}
	else if(GameData::getSaveData()->_hongbaotime>0)
	{
		GameData::getSaveData()->_hongbaotime--;
		sprintf(hbname,"%02d:%02d",GameData::getSaveData()->_hongbaotime/60,GameData::getSaveData()->_hongbaotime%60);
		hongbaofont->setString(hbname);
	}
}

Sprite* StartManage::getProp(int id)
{
	Sprite *sp=cjSprite::createWithImage(cjTTFLabel::getNameByInt("prop%02d.png",id));
	if(id==1)
	{
		auto node=Node::create();
		sp->addChild(node,1,"node");
		for(int i=1;i<4;i++)
		{
			auto tt=cjSprite::createWithImage(cjTTFLabel::getNameByInt("prop1_%02d.png",i));
			node->addChild(tt,1,cjTTFLabel::getNameByInt("prop1-%02d",i));
			tt->setPosition(Vec2(65,50));
			tt->setScale(0.6f);
			if(i!=1)
				tt->setVisible(false);

			Vector<FiniteTimeAction*> vec;
			if(i==1)
			{
				vec.pushBack(DelayTime::create(0.1f));
				vec.pushBack(Hide::create());
				vec.pushBack(DelayTime::create(0.2f));
				vec.pushBack(Show::create());
			}
			else if(i==2)
			{
				vec.pushBack(DelayTime::create(0.1f));
				vec.pushBack(Show::create());
				vec.pushBack(DelayTime::create(0.1f));
				vec.pushBack(Hide::create());
				vec.pushBack(DelayTime::create(0.1f));
			}
			else if(i==3)
			{
				vec.pushBack(DelayTime::create(0.2f));
				vec.pushBack(Show::create());
				vec.pushBack(DelayTime::create(0.1f));
				vec.pushBack(Hide::create());
			}
			auto rep=RepeatForever::create((ActionInterval*)Sequence::create(vec));

			tt->runAction(rep);
		}
	}
	else if(id==4)
	{
		auto node=Node::create();
		sp->addChild(node,1,"node");
		for(int i=1;i<3;i++)
		{
			auto tt=cjSprite::createWithImage(cjTTFLabel::getNameByInt("prop4_%02d.png",i));
			node->addChild(tt,1,cjTTFLabel::getNameByInt("prop4-%02d",i));
			tt->setPosition(sp->getContentSize()/2.0f);
			if(i!=1)
				tt->setVisible(false);

			Vector<FiniteTimeAction*> vec;
			if(i==1)
			{
				vec.pushBack(DelayTime::create(0.1f));
				vec.pushBack(Hide::create());
				vec.pushBack(DelayTime::create(0.1f));
				vec.pushBack(Show::create());
			}
			else if(i==2)
			{
				vec.pushBack(DelayTime::create(0.1f));
				vec.pushBack(Show::create());
				vec.pushBack(DelayTime::create(0.1f));
				vec.pushBack(Hide::create());
			}
			auto rep=RepeatForever::create((ActionInterval*)Sequence::create(vec));

			tt->runAction(rep);
		}
	}
	else if(id==2)
	{
		sp->setScale(0.95f);
		auto light=cjSprite::createWithImage("prop02-01.png");
		sp->addChild(light,-1,"node");
		light->setPosition(Vec2(sp->getContentSize()/2.0f)-Vec2(0,5));
		light->runAction(RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(ScaleTo::create(0.5f,1.2f),ScaleTo::create(0.5f,1.05f))));
	}
	return sp;
}

bool StartManage::propProcess(int id,pair<int,int> pos)
{
	if(_selectprop>0)
	{
		auto lastmu=getChildByName(cjTTFLabel::getNameByInt("prop%d",_selectprop));
		if(_selectprop==1)
		{
			lastmu->stopAllActions();
			lastmu->setRotation(0);
			lastmu->getChildByName("node")->setVisible(false);
		}
		else if (_selectprop == 2)
		{
			lastmu->getChildByName("node")->setVisible(false);
		}
		else if(_selectprop==4)
		{
			lastmu->stopAllActions();
			lastmu->setRotation(0);
			lastmu->setOpacity(255);
			lastmu->getChildByName("node")->setVisible(false);
		}
	}

	if(!_guide)
	{
		if(diamondnum[id]>GameData::getSaveData()->_diamondNumber&&GameData::getSaveData()->_propnum[id]==0)
		{
			_selectprop=0;
#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS|| IOSMODE)
			PayScene::getInstance()->freeDiamond();
#else
			if(GameDataInstance()->d_PayType&&vigame::pay::PayManager::getDefaultFeeInfo()&&vigame::pay::PayManager::getDefaultFeeInfo()->getFeePriceByID(1))
				PayScene::getInstance()->payPage1();
			else
				Toast::makeText("PAY_RESULT_FAIL")->show();
#endif
			return false;
		}
		else
		{
			if (id != 1)
			{
				if (GameData::getSaveData()->_propnum[id] == 0)
					GameData::getSaveData()->_diamondNumber -= diamondnum[id];
				else
				{
					GameData::getSaveData()->_propnum[id]--;
					if (GameData::getSaveData()->_propnum[id] == 0)
						_propnum[id]->setVisible(false);
					else
					{
						_propnum[id]->setString(cjTTFLabel::getNameByInt("x%d", GameData::getSaveData()->_propnum[id]));
					}

					if ((id + 1) == GameData::getSaveData()->_hupropid)
					{
						GameData::getSaveData()->_hupropid = 0;
					}
				}
				GameData::getInstance()->dataSave();
			}
		}
	}

	//ªªŒª
	if(id==2)
	{
		cjMusic::playEffect("video/Props_Rainbow.mp3");

		auto prop=getChildByName("prop3");
		prop->setVisible(false);
		auto bombcsb=CSLoader::createNode("animation/prop_status_reset.csb");
		this->addChild(bombcsb,10);
		bombcsb->setPosition(getChildByName("prop3")->getPosition());
		auto action=CSLoader::createTimeline("animation/prop_status_reset.csb");
		action->gotoFrameAndPlay(0,false);
		bombcsb->runAction(action);
		bombcsb->runAction(Sequence::create(DelayTime::create(0.4f),CallFunc::create([bombcsb,prop](){
			bombcsb->removeFromParent();
			prop->setVisible(true);
		}),nullptr));

		vector<pair<int,int>> selectbox;
		do 
		{
			selectbox.clear();
			for(int i=0;i<CHESS_SIZE_W_START;i++)
				for(int j=0;j<CHESS_SIZE_H_START;j++)
				{
					if(getChessInfo(make_pair(i,j)))
					{
						setChessInfo(make_pair(i,j),rand()%START_INDEX_NUMBER+1);
						selectbox.push_back(make_pair(i,j));
					}
				}
		} while (!chessCanEliminate());

		random_shuffle(selectbox.begin(),selectbox.end());

		setGameState(STATE_STOP);

		Vector<FiniteTimeAction*> boxaction;
		for(auto &i:selectbox)
		{
			boxaction.pushBack(DelayTime::create(1.0f/60.0f));
			boxaction.pushBack(CallFunc::create([i,this](){
				auto pp=_clip->getChildByTag(i.first*CHESS_SIZE_H_START+i.second);
				pp->runAction(Sequence::create(ScaleTo::create(0.1f,0.0f),CallFunc::create([this,i](){
					_clip->removeChildByTag(i.first*CHESS_SIZE_H_START+i.second);
					auto newbox=StartBox::createBox(getChessInfo(i));
					_clip->addChild(newbox,CHESSMAN_LEVEL,i.first*CHESS_SIZE_H_START+i.second);
					newbox->setPosition(posToposition(i));
					newbox->setScale(0.0f);
					newbox->runAction(Sequence::create(ScaleTo::create(0.2f,boxscale*1.1f),
						ScaleTo::create(0.2f,boxscale*0.9f),
						ScaleTo::create(0.2f,boxscale),nullptr));
				}),nullptr));

				//À¢–¬¡£◊”
				auto resetcsb=CSLoader::createNode("animation/prop_skill_reset.csb");
				_clip->addChild(resetcsb,8);
				resetcsb->setPosition(pp->getPosition());

				auto resetcsbaction=CSLoader::createTimeline("animation/prop_skill_reset.csb");
				resetcsb->runAction(resetcsbaction);
				resetcsbaction->play("red",false);
				resetcsbaction->setLastFrameCallFunc([=](){
					resetcsb->removeFromParent();	
				});
			}));
		}

		boxaction.pushBack(CallFunc::create([this](){
			if(!_guide)
			{
				if(getGameState()==STATE_STOP)
					setGameState(STATE_RUN);
			}
			else
			{
				_guidenumber++;
				runAction(Sequence::create(DelayTime::create(1.0f),CallFunc::create([this](){
					setGameState(STATE_RUN);
					setGuideProp(4);
				}),nullptr));
			}
		}));
		runAction(Sequence::create(boxaction));
	}
	else if(id==3)
	{
		int key=getChessInfo(pos);
		for(int i=0;i<CHESS_SIZE_W_START;i++)
			for(int j=0;j<CHESS_SIZE_H_START;j++)
			{
				if(key==getChessInfo(make_pair(i,j)))
				{
					_selectbox.push_back(make_pair(i,j));
					auto box=(StartBox*)_clip->getChildByTag(i*CHESS_SIZE_H_START+j);
					box->setBox2();
					//box->runAction(Sequence::create(MoveBy::create(0.05f,Vec2(0,2)),MoveBy::create(0.05f,Vec2(0,-2)),nullptr));
				}
			}
	}
	//’®µØ
	else if(id==0)
	{
		auto ssp=getProp(1);
		this->addChild(ssp,10);
		ssp->setPosition(getChildByName(cjTTFLabel::getNameByInt("prop%d",1))->getPosition());

		auto v1=ssp->getPosition();
		auto v2=posToposition(pos)+Vec2(10,-10);
		//auto v3=Vec2((v1.x-v1.y+v2.x+v2.y)/2.0f,(v1.x+v1.y-v2.x+v2.y)/2.0f);
		auto v3=ssp->getPosition();
		if(v2.x<WINSIZE.width/2.0f)
			v3-=Vec2(200,-100);
		else
			v3+=Vec2(200,100);

		ccBezierConfig bezier;
		bezier.controlPoint_1 = v1;//≤®π»∆´œÚ÷µ
		bezier.controlPoint_2 =v3;//≤®∑Â∆´œÚ÷µ
		bezier.endPosition = v2;//∂Ø◊˜÷’µ„

		cjMusic::playEffect("video/boom.mp3");

		float boomtime = 0.7f;

		BezierTo * bezierto = BezierTo::create(boomtime, bezier);
		ssp->runAction(Sequence::create(bezierto,CallFunc::create([ssp,pos,this](){
			ssp->removeFromParent();

		}),nullptr));
		ssp->runAction(Sequence::create(ScaleTo::create(0.15f,1.5f),nullptr));
		runAction(Sequence::create(DelayTime::create(boomtime),
			CallFunc::create([pos,this](){

				//auto sp=Sprite::create("animation/daoju_images/bigbang_boom.png");
				//_clip->addChild(sp,CHESSMAN_LEVEL+1);
				//sp->setPosition(posToposition(pos));
				//sp->setScale(0.0f);
				//sp->runAction(Sequence::create(ScaleTo::create(0.3f,1.2f),CallFunc::create([sp](){
				//	sp->removeFromParent();
				//}),nullptr));

				auto bombcsb=CSLoader::createNode("animation/prop_skill_bomb.csb");
				this->addChild(bombcsb,10);
				bombcsb->setPosition(posToposition(pos));
				auto action=CSLoader::createTimeline("animation/prop_skill_bomb.csb");
				action->gotoFrameAndPlay(0,false);
				bombcsb->runAction(action);

				cjSchedule::getInstance()->runOnNextFrame([this,pos](float ft){
					int k=0;
					for(int i=pos.first-1;i<=pos.first+1;i++)
						for(int j=pos.second-1;j<=pos.second+1;j++)
						{
							if(i>=0&&i<CHESS_SIZE_W_START&&j>=0&&j<CHESS_SIZE_W_START&&getChessInfo(make_pair(i,j)))
							{

								k++;
								addParticle(i,j,getChessInfo(make_pair(i,j)),3);
								removeBox(make_pair(i,j),2);
							}
						}
				});
		}),nullptr));
	}
	return true;
}

void StartManage::startDrop()
{
	if(!_firstSaveIn&&GameData::getSaveData()->_hupropid>0)
	{
		GameData::getSaveData()->_propnum[GameData::getSaveData()->_hupropid-1]--;
		propNumShow(GameData::getSaveData()->_hupropid-1,-1);
		GameData::getSaveData()->_hupropid=0;
		GameData::getInstance()->dataSave();
	}

	setGameState(STATE_STOP);
	for(int i=0;i<CHESS_SIZE_W_START;i++)
		for(int j=0;j<CHESS_SIZE_H_START;j++)
		{
			int color=rand()%START_INDEX_NUMBER+1;
			if(_firstSaveIn)
				color=sdata._chessInfo[i][j];

			if(_guide)
			{
				vector<pair<int,int>> vec;
				vec.push_back(make_pair(4,9));
				vec.push_back(make_pair(5,9));
				vec.push_back(make_pair(6,9));
				vec.push_back(make_pair(7,9));
				vec.push_back(make_pair(8,9));
				vector<pair<int,int>> vec2;
				vec2.push_back(make_pair(1,9));
				vec2.push_back(make_pair(9,9));
				vec2.push_back(make_pair(2,8));
				vec2.push_back(make_pair(3,8));
				vec2.push_back(make_pair(4,8));
				vec2.push_back(make_pair(5,8));
				vec2.push_back(make_pair(6,8));
				vec2.push_back(make_pair(7,8));
				vec2.push_back(make_pair(8,8));
				if((i==2&&j==9)||(i==3&&j==9))
					color=1;
				else if(std::find(vec.begin(),vec.end(),make_pair(i,j))!=vec.end())
					color=2;
				else if(std::find(vec2.begin(),vec2.end(),make_pair(i,j))!=vec2.end())
					color=rand()%(START_INDEX_NUMBER-2)+3;
				else
					color=rand()%(START_INDEX_NUMBER)+1;			
			}

			if(color>0)
			{
				_chessInfo[i*CHESS_SIZE_H_START+j]=color;
				//auto box=StartBox::createBox(color);
				//box->setScale(boxscale);
				//_clip->addChild(box,CHESSMAN_LEVEL,i*CHESS_SIZE_H_START+j);
				//box->setPosition(posToposition(make_pair(i,j),0)+Vec2(0,CHESS_SIZE_H_START*CHESSMAN_SIZE_START));
				//box->runAction(Sequence::create(DelayTime::create(j*0.1f+rand()%10*0.01f),MoveBy::create(0.2f,Vec2(0,-CHESS_SIZE_H_START*CHESSMAN_SIZE_START)),nullptr));
			}		
		}

		float time=chessAppear((rand()%6)+1);

		if(_firstSaveIn)
			_firstSaveIn=false;
		if(!_guide)
			dataSave();
		runAction(Sequence::create(DelayTime::create(time),CallFunc::create([this](){
			//if(getmScore()==0&&!GameData::getSaveData()->_start._guider)
			//{
			//	setGameState(STATE_RUN);
			//	noviceGuider();
			//}
			//else
			{
				setGameState(STATE_RUN);
				if(_score>=_target)
				{
					spriteShow("guoguan.png");
				}

				if(_guide)
				{
					runAction(Sequence::create(DelayTime::create(0.5f),CallFunc::create([this](){
						_guidenumber=0;
						startGuide();
					}),nullptr));
				}
				else
				{
					beganTip();
				}
			}
		}),nullptr));
}

pair<int,int> StartManage::positionTopos(Vec2 position)
{
	assert(position.x>=0&&position.x<=WINSIZE.width&&position.y>=heightoffest&&position.y<=(heightoffest+CHESS_SIZE_H_START*CHESSMAN_SIZE_START));
	return make_pair((position.x-WINSIZE.width/2.0f)/CHESSMAN_SIZE_START+CHESS_SIZE_W_START/2.0f,(position.y-heightoffest)/CHESSMAN_SIZE_START);
}

Vec2 StartManage::posToposition(pair<int,int> pos,int key)
{
	key=1;
	assert(pos.first>=0&&pos.first<CHESS_SIZE_W_START&&pos.first>=0&&pos.second<CHESS_SIZE_H_START);
	return Vec2(WINSIZE.width/2.0f+(pos.first+0.5f-CHESS_SIZE_W_START/2.0f)*CHESSMAN_SIZE_START,heightoffest+(pos.second+key*0.5f)*CHESSMAN_SIZE_START);
}

//ºÏ≤‚µ¿æﬂµ„ª˜
int StartManage::propTouch(Vec2 pos)
{
	for(int i=0;i<4;i++)
	{
		auto po=getChildByName(cjTTFLabel::getNameByInt("prop%d",i+1))->getPosition();
		if(abs(pos.x-po.x)<=25&&abs(pos.y-po.y)<=25)
			return i+1;
	}
	return 0;
}

void StartManage::propMotion(int k)
{
	if(k==3)
	{
		propProcess(2);
	}
	else
	{
		if(_selectprop>0)
		{
			auto lastmu=getChildByName(cjTTFLabel::getNameByInt("prop%d",_selectprop));
			if(_selectprop==1)
			{
				lastmu->stopAllActions();
				lastmu->setRotation(0);
				lastmu->getChildByName("node")->setVisible(false);
			}
			else if(_selectprop==4)
			{
				lastmu->stopAllActions();
				lastmu->setRotation(0);
				lastmu->setOpacity(255);
				lastmu->getChildByName("node")->setVisible(false);
			}
			else if(_selectprop==2)
			{
				lastmu->getChildByName("node")->setVisible(false);
			}
		}

		if(_selectprop==k)
		{
			beganTip();
			_selectprop=0;
		}
		else
		{
			if (!_guide)
				endTip();
			_selectprop=k;
			auto mu=getChildByName(cjTTFLabel::getNameByInt("prop%d",_selectprop));
			if(k==1)
			{
				mu->getChildByName("node")->setVisible(true);
				mu->runAction(RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(RotateTo::create(0.2f,-10),RotateTo::create(0.2f,10))));
			}
			else if(k==4)
			{
				mu->setOpacity(0);
				mu->getChildByName("node")->setVisible(true);
			}
			else if(k==2)
			{
				mu->getChildByName("node")->setVisible(true);
			}
		}
	}
}

bool StartManage::onTouchBegan(Touch *touch, Event *unused_event)
{
	if(getGameState()!=STATE_RUN)
		return false;

	auto position=touch->getLocation();

	if(_guide)
	{
		if(_guidenumber>3)
		{
			int k=propTouch(position);
			if(k==(_guidenumber-3))
			{
				if(_selectprop==0&&_eliminateNum==0)
				{
					//propMotion(k-3);
					this->removeChildByName("guideproplayer");
					//Director::getInstance()->getRunningScene()->removeChildByName("guidepropseclayer");
					this->removeChildByName("hand");
					this->getChildByName(cjTTFLabel::getNameByInt("prop%d",k))->setZOrder(1);
					if(k==1||k==2||k==4)
					{
						auto ff=LabelTTF::create(cjTTFLabel::getStringByKey("dianjitip5"),TTF_PATH,28);
						this->addChild(ff,20,"tipfont");
						ff->setPosition(posToposition(make_pair(4,9))+Vec2(0,60));
					}
				}
				else
					return false;
			}
			else if(k>0)
				return false;
		}
		else if(position.x>=0&&position.x<WINSIZE.width&&position.y>=heightoffest&&position.y<=(heightoffest+CHESSMAN_SIZE_START*CHESS_SIZE_H_START))
		{
			auto ps=positionTopos(position);
			if(_guidenumber==0)
			{
				if(ps==make_pair(2,9)||ps==make_pair(3,9))
				{
					{
						auto colorlay=Director::getInstance()->getRunningScene()->getChildByTag(100);
						for(int i=0;i<2;i++)
							colorlay->removeChildByTag(1+i);
						colorlay->setVisible(false);
						auto secondlay=this->getChildByName("secondlay");
						secondlay->setVisible(false);
					}
					setGameState(STATE_STOP);
				}
				else 
					return false;
			}
			else if(_guidenumber==1)
			{
				vector<pair<int,int>> vec;
				vec.push_back(make_pair(4,9));
				vec.push_back(make_pair(5,9));
				vec.push_back(make_pair(6,9));
				vec.push_back(make_pair(7,9));
				vec.push_back(make_pair(8,9));
				if(std::find(vec.begin(),vec.end(),ps)!=vec.end())
				{
					setGameState(STATE_STOP);
					{
						auto colorlay=Director::getInstance()->getRunningScene()->getChildByTag(100);
						for(int i=0;i<5;i++)
							colorlay->removeChildByTag(1+i);
						colorlay->setVisible(false);
						auto secondlay=this->getChildByName("secondlay");
						secondlay->setVisible(false);
					}
					auto hb=(Menu *)this->getChildByName("hongbaomenu");
					hb->setEnabled(false);
					//_guide=false;
					//_guidenumber=0;
					//GameDataInstance()->_guide=false;
					//GameData::getSaveData()->_start._guider=true;
					//GameData::getInstance()->dataSave();
				}
				else
					return false;
			}
			else
				return false;
		}
		else 
			return false;
	}

	int k=propTouch(position);
	if((k>0)&&(_eliminateNum==0)&&((!_guide)||(_selectprop==0)))
	{
		propMotion(k);
		return false;
	}

	if(_guide&&_guidenumber>3&&(_selectprop==0))
		return false;

	if(position.x>=0&&position.x<WINSIZE.width&&position.y>=heightoffest&&position.y<=(heightoffest+CHESSMAN_SIZE_START*CHESS_SIZE_H_START))
	{
		auto pos=positionTopos(position);
		if(_chessInfo[pos.first*CHESS_SIZE_H_START+pos.second]&&std::find(_eliminatePos.begin(),_eliminatePos.end(),pos)==_eliminatePos.end())
		{
			bool temp=true;
			vector<pair<int,int>> vec;
			if(_selectprop==1||_selectprop==2||_selectprop==4)
			{
				temp=propProcess(_selectprop-1,pos);

				if(_guide)
				{
					this->removeChildByName("tipfont");
					_guidenumber++;
				}
			}
			else if(_selectprop==0)
			{
				boxTip(pos,vec);
			}
			int selectnumber=vec.size();
			if(selectnumber>1||(_selectprop>0&&temp))
			{
				endTip();
				float ft=0.0f;
				if(_selectprop==0)
				{
					_eliminateNum++;
					boxEliminate(vec);
					ft=boxremovesingletime*selectnumber+cleardelaytime;
				}
				else if(_selectprop==1)
				{
					ft=boxremovesingletime+0.9f;
				}
				else if(_selectprop==4)
				{
					ft=_selectbox.size()*boxremovesingletime;
					useProps(pos);
				}
				else if(_selectprop==2)
				{
					useProp4(pos);
				}
				if(_selectprop==1||_selectprop==4)
				{
					setGameState(STATE_STOP);
					runAction(Sequence::create(DelayTime::create(ft),CallFunc::create([this](){
						touchHandle();
					}),nullptr));
					_selectprop=0;
					_selectbox.clear();
				}
			}
		}
	}
	return false;
}

void StartManage::handleHandle()
{
	if(chessCanEliminate())
	{
		setGameState(STATE_RUN);
		if(_guide)
		{
			if(_guidenumber==0)
			{
				setGameState(STATE_STOP);
				runAction(Sequence::create(DelayTime::create(0.5f),CallFunc::create([=](){
					_guidenumber=1;
					setGameState(STATE_RUN);
					setGuide1();
				}),nullptr));
			}
			else if(_guidenumber==1)
			{
				setGameState(STATE_STOP);
				runAction(Sequence::create(DelayTime::create(tipdelaytime),CallFunc::create([this](){
					_guidenumber=2;
					setGameState(STATE_RUN);
					setGuide2();
				}),nullptr));
			}
			else if(_guidenumber>3)
			{
				if(_guidenumber==8)
				{
					LevelUpMotion();

					setGameState(STATE_STOP);
					Vector<FiniteTimeAction*> action;
					action.pushBack(DelayTime::create(1.0f));
					action.pushBack(CallFunc::create([this](){
						vector<pair<int, int>> eliminatebox;
						for (int i = 0; i<CHESS_SIZE_W_START; i++)
							for (int j = 0; j<CHESS_SIZE_H_START; j++)
							{
								if (getChessInfo(make_pair(i, j)))
									eliminatebox.push_back(make_pair(i, j));
							}
						boxSequence(eliminatebox, false);
						float tt = smallPeopleCollect(eliminatebox);
						Vector<FiniteTimeAction*> action2;
						action2.pushBack(DelayTime::create(tt+0.5f));
						action2.pushBack(CallFunc::create([this]() {
							_guide = false;
							_guidenumber = 0;
							GameDataInstance()->_guide = false;
							GameData::getSaveData()->_start._guider = true;
							GameData::getInstance()->dataSave();
							auto manage = static_cast<GameScene*>(Director::getInstance()->getRunningScene()->getChildByTag(99));
							manage->restart();
						}));
						runAction(Sequence::create(action2));
					}));
					runAction(Sequence::create(action));
				}
				else
				{
					setGameState(STATE_STOP);
					runAction(Sequence::create(DelayTime::create(tipdelaytime),CallFunc::create([this](){
						setGameState(STATE_RUN);
						setGuideProp(_guidenumber-3);
					}),nullptr));
				}
			}
		}
		else
		{
			dataSave();
			beganTip();
		}
	}
	else
	{
		runAction(Sequence::create(DelayTime::create(0.5f),CallFunc::create([this](){
			chessClear();
		}),nullptr));
	}
}

void StartManage::touchHandle()
{
	float time=0.0f;
	if(chessMove())
	{
		time+=chessmovetime;
	}
	runAction(Sequence::create(DelayTime::create(time),CallFunc::create([this](){
		if(_candealwith)
		{
			handleHandle();
		}
	}),nullptr));
}

void StartManage::onTouchMoved(Touch *touch, Event *unused_event)
{
}

void StartManage::onTouchEnded(Touch *touch, Event *unused_event)
{
}

//∑ΩøÈœ˚≥˝
void StartManage::boxEliminate(vector<pair<int,int>> &vec)
{
	int score=getScoreByNum(vec.size());
	_score+=score;
	scoreMotion(score,vec[0]);

	std::function<void(Armature*,MovementEventType,const std::string&)> armatureFun=[=](Armature* armature, MovementEventType type, const std::string& id){
		if(type==MovementEventType::COMPLETE)
		{
			armature->getAnimation()->stop();
			peopleStand();
		}
	};

	if(_score>=_target&&(_score-score)<_target)
	{
		spriteShow("guoguan.png");
	}
	else
	{
		if(vec.size()==5)
		{
			spriteShow("zan.png");
			cjMusic::playEffect("video/zan.mp3");
			auto people=dynamic_cast<Armature*>(getChildByName("people"));
			people->getAnimation()->play("cheer2");
			people->getAnimation()->setMovementEventCallFunc(armatureFun);
		}
		else if(vec.size()>5)
		{
			spriteShow("great.png");
			cjMusic::playEffect("video/great.mp3");
			auto people=dynamic_cast<Armature*>(getChildByName("people"));
			people->getAnimation()->play("cheer3");
			people->getAnimation()->setMovementEventCallFunc(armatureFun);
		}
	}

	levelupmusic=0;
	int kk=0;
	boxSequence(vec);
	Vector<FiniteTimeAction*> arrayOfActions;

	if((GameData::getSaveData()->_hunum+vec.size())>=GameData::getSaveData()->_huhightnum)
		_candealwith=false;

	for(auto &i:vec)
	{
		if(kk!=0)
			arrayOfActions.pushBack(DelayTime::create(boxremovesingletime));
		arrayOfActions.pushBack(CallFunc::create([this,i](){
			//∑ΩøÈ ’ºØ
			cubeCollect(i);
			//

			addParticle(i.first,i.second,getChessInfo(i),11);
			removeBox(i);
		}));
		kk++;
	}
	arrayOfActions.pushBack(DelayTime::create(cleardelaytime));
	arrayOfActions.pushBack(CallFunc::create([this](){
		_eliminateNum--;
		if(_eliminateNum==0)
		{
			_eliminatePos.clear();
			setGameState(STATE_STOP);

			touchHandle();
		}		
	}));
	runAction(Sequence::create(arrayOfActions));
}

//∑ΩøÈÃ· æ
void StartManage::boxTip(pair<int,int> pos,vector<pair<int,int>> &vect)
{
	//if(!_selectbox.empty())
	//{
	//	for(auto &i:_selectbox)
	//	{
	//		auto box=(StartBox*)_clip->getChildByTag(i.first*CHESS_SIZE_H_START+i.second);
	//		box->setBox1();
	//	}
	//}
	auto vec=boxGet(pos);
	//_selectbox.clear();
	if(vec.size()>1)
	{
		for(auto &i:vec)
		{
			vect.push_back(i);
			_eliminatePos.push_back(i);
			//_selectbox.push_back(i);
			auto box=(StartBox*)_clip->getChildByTag(i.first*CHESS_SIZE_H_START+i.second);
			box->setBox2();
			//box->runAction(Sequence::create(MoveBy::create(0.05f,Vec2(0,2)),MoveBy::create(0.05f,Vec2(0,-2)),nullptr));
		}
	}
}

vector<pair<int,int>> StartManage::boxGet(pair<int,int> pos)
{
	vector<pair<int,int>> vec;
	boxAdd(pos,vec);
	return vec;
}

void StartManage::boxAdd(pair<int,int> pos,vector<pair<int,int>> &vec)
{
	vec.push_back(pos);
	//x-1
	if((pos.first-1)>=0&&(std::find(vec.begin(),vec.end(),make_pair(pos.first-1,pos.second))==vec.end())&&getChessInfo(pos)==getChessInfo(make_pair(pos.first-1,pos.second)))
	{
		boxAdd(make_pair(pos.first-1,pos.second),vec);
	}

	//x+1
	if((pos.first+1)<CHESS_SIZE_W_START&&(std::find(vec.begin(),vec.end(),make_pair(pos.first+1,pos.second))==vec.end())&&getChessInfo(pos)==getChessInfo(make_pair(pos.first+1,pos.second)))
	{
		boxAdd(make_pair(pos.first+1,pos.second),vec);
	}

	//y-1
	if((pos.second-1)>=0&&(std::find(vec.begin(),vec.end(),make_pair(pos.first,pos.second-1))==vec.end())&&getChessInfo(pos)==getChessInfo(make_pair(pos.first,pos.second-1)))
	{
		boxAdd(make_pair(pos.first,pos.second-1),vec);
	}

	//y+1
	if((pos.second+1)<CHESS_SIZE_H_START&&(std::find(vec.begin(),vec.end(),make_pair(pos.first,pos.second+1))==vec.end())&&getChessInfo(pos)==getChessInfo(make_pair(pos.first,pos.second+1)))
	{
		boxAdd(make_pair(pos.first,pos.second+1),vec);
	}
}

//void StartManage::addParticle(int x,int y,int shapeid,int type)
//{
//	string tname;
//	int number=50;
//	switch (shapeid)
//	{
//	case 1:
//		tname="lizi/blueboom";
//		break;
//	case 2:
//		tname="lizi/greenboom";
//		break;
//	case 3:
//		tname="lizi/purpleboom";
//		break;
//	case 4:
//		tname="lizi/redboom";
//		break;
//	case 5:
//		tname="lizi/yellowboom";
//		break;
//	default:
//		break;
//	}
//
//	string name;
////	if(type==1)
//		name=tname+".plist";
//	//else
//	//{
//	//	name=tname+"2.plist";
//	//}
//	auto par=CCParticleSystemQuad::create(name.c_str());
//	this->addChild(par,11);
//	par->setAutoRemoveOnFinish(true);
//	par->setPosition(posToposition(make_pair(x,y)));
//
//	//
//	//auto ddd=cjSprite::createWithImage("backsp.png");
//	//this->addChild(ddd,12);
//	//ddd->setScale(CHESSMAN_SCALE_1010);
//	//ddd->setPosition(posToposition(make_pair(x,y)));
//	//ddd->setOpacity(0);
//	//ddd->runAction(Sequence::create(FadeIn::create(0.15f),FadeOut::create(0.15f),CallFunc::create([ddd](){
//	//	ddd->removeFromParent();
//	//}),nullptr));
//	//
//} 

void StartManage::addParticle(int x,int y,int shapeid,int type)
{
	auto node=Node::create();
	this->addChild(node,12);
	node->setPosition(posToposition(make_pair(x,y)));

	string str;
	switch (shapeid)
	{
	case 1:
		str="lizi/blueboom";
		break;
	case 2:
		str="lizi/greenboom";
		break;
	case 3:
		str="lizi/purpleboom";
		break;
	case 4:
		str="lizi/redboom";
		break;
	case 5:
		str="lizi/yellowboom";
		break;
	default:
		str="lizi/blueboom";
		break;
	}
	str+=".plist";
	auto par=ParticleSystemQuad::create(str);
	node->addChild(par,12);
	par->setPosition(node->getContentSize()/2.0f);
	par->setAutoRemoveOnFinish(true);

	node->runAction(Sequence::create(DelayTime::create(1.0f),CallFunc::create([node](){
		node->removeFromParent();
	}),nullptr));

	if(type==11)
	{
		auto propSkillNode = CSLoader::createNode("animation/xiaochu.csb");
		node->addChild(propSkillNode, 11);
		//propSkillNode->setPosition(posToposition(make_pair(x,y)));
		propSkillNode->setPosition(node->getContentSize()/2.0f);
		auto propSkillAction=CSLoader::createTimeline("animation/xiaochu.csb");
		propSkillNode->runAction(propSkillAction);
		propSkillAction->play("red",false);
		propSkillAction->setLastFrameCallFunc([propSkillNode](){
			propSkillNode->removeFromParent();
		});
	}
}

void StartManage::removeBox(pair<int,int> pos,int key)
{
	_clip->removeChildByTag(pos.first*CHESS_SIZE_H_START+pos.second);
	setChessInfo(pos,0);
	if(key==1)
	{
		char name[30];
	//	sprintf(name,"video/%02d_ComboMatch.mp3",levelupmusic%10+1);
		sprintf(name, "video/pop_%d.wav", levelupmusic % 4 + 1);
		cjMusic::playEffect(name);
		levelupmusic++;
	}
}

bool StartManage::chessMove()
{
	bool temp=false;
	std::map<int,int> downstep;
	//µ⁄“ª≤Ω£¨œ¬“∆
	int step=0;
	for(int i=0;i<CHESS_SIZE_W_START;i++)
	{
		step=0;
		for(int j=0;j<CHESS_SIZE_H_START;j++)
		{
			//downstep[i*CHESS_SIZE_H_START+j]=step;
			if(getChessInfo(make_pair(i,j))==0)
				++step;
			else if(step>0)
			{
				temp=true;
				auto box=_clip->getChildByTag(i*CHESS_SIZE_H_START+j);
				box->runAction(Sequence::create(MoveTo::create(0.15f,posToposition(make_pair(i,j-step),0)),
					//ScaleTo::create(0.1f,boxscale,boxscale*0.85f),
					//ScaleTo::create(0.1f,boxscale,boxscale*1.1f),
					//ScaleTo::create(0.1f,boxscale,boxscale),
					nullptr));
				box->setTag(i*CHESS_SIZE_H_START+(j-step));
				setChessInfo(make_pair(i,j-step),getChessInfo(make_pair(i,j)));
				setChessInfo(make_pair(i,j),0);
			}
		}
	}

	//µ⁄∂˛≤Ω£¨◊Û“∆
	step=0;
	for(int i=0;i<CHESS_SIZE_W_START;i++)
	{
		if(getChessInfo(make_pair(i,0))==0)
			++step;
		else if(step>0)
		{
			for(int j=0;j<CHESS_SIZE_H_START;j++)
			{
				if(getChessInfo(make_pair(i,j)))
				{
					temp=true;
					auto box=_clip->getChildByTag(i*CHESS_SIZE_H_START+j);
					box->runAction(Sequence::create(DelayTime::create(0.1f),MoveBy::create(0.05f,Vec2(-step*CHESSMAN_SIZE_START,0)),nullptr));
					box->setTag((i-step)*CHESS_SIZE_H_START+j);
					setChessInfo(make_pair(i-step,j),getChessInfo(make_pair(i,j)));
					setChessInfo(make_pair(i,j),0);
				}
			}
		}
	}
	return temp;
}

bool StartManage::chessCanEliminate()
{
	for(int i=0;i<CHESS_SIZE_W_START;i++)
		for(int j=0;j<CHESS_SIZE_H_START;j++)
		{
			if(singelCanEliminate(make_pair(i,j)))
				return true;
		}
		return false;
}

void StartManage::chessClear()
{
	vector<pair<int,int>> eliminatebox;
	for(int i=0;i<CHESS_SIZE_W_START;i++)
		for(int j=0;j<CHESS_SIZE_H_START;j++)
		{
			if(getChessInfo(make_pair(i,j)))
				eliminatebox.push_back(make_pair(i,j));
		}

		if(eliminatebox.size()>0)
			random_shuffle(eliminatebox.begin(), eliminatebox.end());
		levelupmusic=0;
		boxSequence(eliminatebox,false);
		//Vector<FiniteTimeAction*> action;
		//for(auto &i:eliminatebox)
		//{
		//	if(i!=eliminatebox[0])
		//		action.pushBack(DelayTime::create(boxremovesingletime));
		//	action.pushBack(CallFunc::create([this,i](){
		//		addParticle(i.first,i.second,getChessInfo(i),10);
		//		removeBox(i);
		//	}));
		//}
		//if(action.size()>0)
		//	runAction(Sequence::create(action));

		float ftime = smallPeopleCollect(eliminatebox);
		float ft=MAX(ftime,3.0f);

		int num=getRetainScore(eliminatebox.size());
		_score+=num;
		scoreMotion(num,make_pair(CHESS_SIZE_W_START/2,CHESS_SIZE_H_START/2));

		retainScoreMotion(eliminatebox.size(),num,ft);

		if(_score>=_target&&(_score-num)<_target)
		{
			spriteShow("guoguan.png",false);
		}

		runAction(Sequence::create(DelayTime::create(ft),CallFunc::create([this](){
			faildJudge();
		}),nullptr));
}

void StartManage::faildJudge()
{
	if(_score<_target)
	{
		setGameState(STATE_FAILD);
		dataSave(false);
		auto people=dynamic_cast<Armature*>(getChildByName("people"));
		people->getAnimation()->play("upset1");
	}
	else
	{

		setmLevel(getmLevel()+1);
		setmTarget(getTargetScoreByRound(getmLevel()));
		_lastLevelScore=getmScore();

		std::function<void(Armature*,MovementEventType,const std::string&)> armatureFun=[=](Armature* armature, MovementEventType type, const std::string& id){
			if(type==MovementEventType::COMPLETE)
			{
				armature->getAnimation()->stop();
				peopleStand();
			}
		};

		auto people=dynamic_cast<Armature*>(getChildByName("people"));
		people->getAnimation()->play("win");
		people->getAnimation()->setMovementEventCallFunc(armatureFun);

//		if(GameData::getSaveData()->_gg_guanshu<=5)
			GameData::getSaveData()->_gg_guanshu++;

		if(getmLevel()==4)
			vigame::ad::ADManager::openAd("level_win");

		int temp=false;
		if(GameData::getSaveData()->_gg_guanshu>5 && ((GameData::getSaveData()->_gg_guanshu - 5) % 2 == 0) &&GameData::getSaveData()->_levelreward_num<LEVELREWARD)
		{
			temp=true;
			setGameState(STATE_STOP);
			GameData::getSaveData()->_levelreward_num++;
			PayScene::getInstance()->levelReward();
		}

		setPostShow(true);
		removeChildByTag(777);
		cjMusic::playEffect("video/NextGameRound2.mp3");
		LevelUpMotion();
		runAction(Sequence::create(DelayTime::create(1.0f),CallFunc::create([this](){
			startDrop();
		}),DelayTime::create(0.1f*CHESS_SIZE_H_START+0.3f),CallFunc::create([this,temp](){
			if(temp)
			{
				if(getGameState()==STATE_STOP)
					setGameState(STATE_RUN);
			}
		}),nullptr));
	}
}

void StartManage::LevelUpMotion()
{
	if(_guide)
	{
		string sst="xinshouzhiyin";
		if(_guidenumber==8)
			sst="zhiyinjieshu";
		auto fnt2=LabelTTF::create(cjTTFLabel::getStringByKey(sst.c_str()),TTF_PATH,70);
		_clip->addChild(fnt2,CHESSMAN_LEVEL+1);
		fnt2->setPosition(Vec2(WINSIZE/2.0f)-Vec2(0,150));
		fnt2->runAction(CCFadeIn::create(0.3f));
		fnt2->runAction(Sequence::create(CCMoveBy::create(0.3f,Vec2(0,200)),CCDelayTime::create(0.2f),ScaleTo::create(0.2f,1.2f),ScaleTo::create(0.2f,1.0f),CCDelayTime::create(1.0f),CCFadeOut::create(0.3f),
			CallFunc::create([fnt2,this](){
				fnt2->removeFromParent();
				//setGameState(STATE_RUN);
		}),nullptr));

		GameData::getSaveData()->_hunum=0;
		_hunum=0;

		return;
	}
	if(getmLevel()==1)
	{
		setGameState(STATE_STOP);
		cjMusic::playEffect("video/ReadyGo.mp3");
	}

	//auto par = ParticleSystemQuad::create("lizi/snow_effect2.plist");
	//this->addChild(par, 10);
	//par->setPosition(WINSIZE/2.0f);
	//par->setAutoRemoveOnFinish(true);

	setPostShow(true);

	auto fnt1 = cjSprite::createWithImage("level_guanshu.png");
	_clip->addChild(fnt1, CHESSMAN_LEVEL + 1);
	fnt1->setCascadeOpacityEnabled(true);
	fnt1->setPosition(Vec2(WINSIZE / 2.0f) + Vec2(0, heightoffest*0.5f - 140));
	fnt1->runAction(Sequence::create(CCMoveBy::create(0.3f, Vec2(0, 200)), DelayTime::create(1.1f), FadeOut::create(0.3f), nullptr));

	auto fnt1num = cjAtlasLabel::createWithInt(getmLevel(),ATLAS_PATH2);
	fnt1->addChild(fnt1num);
	fnt1num->setCascadeOpacityEnabled(true);
	fnt1num->setScale(levelupscale);
	fnt1num->setPosition(fnt1->getContentSize() / 2.0f);

	auto fnt2 = cjSprite::createWithImage("level_mubiao.png");
	_clip->addChild(fnt2, CHESSMAN_LEVEL + 1);
	fnt2->setCascadeOpacityEnabled(true);
	fnt2->setAnchorPoint(Vec2(0.5f, 1.0f));
	fnt2->setPosition(fnt1->getPosition() - Vec2(0, 50));

	auto fnt2num = cjAtlasLabel::createWithInt(getmTarget(), ATLAS_PATH2);
	fnt2->addChild(fnt2num);
	fnt2num->setCascadeOpacityEnabled(true);
	fnt2num->setAnchorPoint(Vec2(0.0f, 0.5f));
	fnt2num->setScale(levelupscale+0.1f);
	fnt2num->setPosition(fnt2->getContentSize().width+10, fnt2->getContentSize().height / 2.0f);

	fnt2->setPosition(fnt2->getPosition() - Vec2(fnt2num->getContentSize().width/2.0f+10,0));
	fnt2->runAction(Sequence::create(CCMoveBy::create(0.3f, Vec2(0, 200)), CCDelayTime::create(0.2f), ScaleTo::create(0.2f, 1.2f), ScaleTo::create(0.2f, 1.0f), CCDelayTime::create(0.5f), CCFadeOut::create(0.3f),
		CallFunc::create([fnt1, fnt2, this]() {
		fnt1->removeFromParent();
		fnt2->removeFromParent();
		//setGameState(STATE_RUN);
	}), nullptr));
}

void StartManage::retainScoreMotion(int number,int score,float ft)
{
	char name1[30];
	sprintf(name1, cjTTFLabel::getStringByKey("rewardscore").c_str(), score);
	auto fnt1=LabelTTF::create(name1,TTF_PATH,50);
	this->addChild(fnt1,12);
	fnt1->setPosition(Vec2(WINSIZE/2.0f)+Vec2((WINSIZE.width/2.0f+100),heightoffest*0.5f+60));
	fnt1->runAction(CCFadeIn::create(0.3f));
	fnt1->runAction(Sequence::create(CCMoveBy::create(0.2f,Vec2(-WINSIZE.width/2.0f-100,0)),DelayTime::create(ft-0.4f),CCMoveBy::create(0.2f,Vec2(-WINSIZE.width/2.0f-100,0)),nullptr));

	char name2[60];
	sprintf(name2, cjTTFLabel::getStringByKey("retaincandy").c_str(), number);
	auto fnt2=LabelTTF::create(name2,TTF_PATH,50);
	this->addChild(fnt2,21);
	fnt2->setPosition(fnt1->getPosition()-Vec2(0,100));
	fnt2->runAction(Sequence::create(CCMoveBy::create(0.2f,Vec2(-WINSIZE.width/2.0f-100,0)),DelayTime::create(ft-0.4f),CCMoveBy::create(0.2f,Vec2(-WINSIZE.width/2.0f-100,0)),CallFunc::create([fnt1,fnt2,this](){
		fnt1->removeFromParent();
		fnt2->removeFromParent();
	}),nullptr));
}

int StartManage::getScoreByNum(int num)
{
	int k=0;
	int t=5;
	for(int i=0;i<num;i++)
	{
		k+=t;
		t+=10;
	}
	return k;
}

void StartManage::spriteShow(const string& str,bool pos)
{
	if(str=="guoguan.png")
	{
		if(_guide)
			return ;
		float dd=heightoffest*0.5f;
		if(!pos)
			dd=-100;

		cjMusic::playEffect("video/levelup.mp3");

		setPostShow(false);

		auto spr=cjSprite::createWithImage(str);
		this->addChild(spr,12,777); 
		spr->setPosition(Vec2(WINSIZE/2.0f)+Vec2(0,dd));
		spr->setScale(1.4f);
		spr->runAction(CCSequence::create(FadeOut::create(0.08f),FadeIn::create(0.08f),FadeOut::create(0.08f),FadeIn::create(0.08f),FadeOut::create(0.08f),FadeIn::create(0.08f),
			ScaleTo::create(0.2f,1.0f),
			DelayTime::create(0.52f),
			MoveTo::create(0.5f,posToposition(make_pair(1,9))+Vec2(0,60)),
			nullptr));
		spr->runAction(Sequence::create(DelayTime::create(1.2f),ScaleTo::create(0.5f,0.4f),CallFunc::create([spr](){
			spr->setZOrder(8);
		}),nullptr));
	}
	else
	{
		auto spr=cjSprite::createWithImage(str);
		Director::getInstance()->getRunningScene()->addChild(spr,12);
		spr->setPosition(Vec2(WINSIZE/2.0f)+Vec2(0,heightoffest*0.5f));
		spr->setScale(2.0f);
		spr->runAction(CCSequence::create(FadeOut::create(0.08f),FadeIn::create(0.08f),FadeOut::create(0.08f),FadeIn::create(0.08f),FadeOut::create(0.08f),FadeIn::create(0.08f),
			ScaleTo::create(0.2f,1.5f),
			DelayTime::create(0.2f),
			FadeOut::create(0.3f),
			CallFunc::create([spr](){
				spr->removeFromParent();
		}),nullptr));
	}
}

//∑÷ ˝∂Øª≠
void StartManage::scoreMotion(int score,pair<int,int> pt)
{
	auto scorefnt=cjAtlasLabel::createWithInt(score,ATLAS_PATH2);
	this->addChild(scorefnt,15);
	scorefnt->setPosition(posToposition(pt));
	scorefnt->runAction(Sequence::create(MoveTo::create(0.8f,START_SCORE_PT),CallFunc::create([scorefnt,score,this](){
		scorefnt->removeFromParent();
		//parShow(START_SCORE_PT);

	}),nullptr));
	scorefnt->runAction(ScaleTo::create(0.8f,0.7f));
}

//◊Ó∫Ûµ√∑÷
int StartManage::getRetainScore(int number)
{
	int score = 1500;

	for (int i = 1; i <= number; i++)
	{
		score -= 30 * i - 15;//Ω±¿¯∑÷ ˝ºı…Ÿ
	}
	if (score < 0)
	{
		score = 0;
	}

	return score;
}

int StartManage::getTargetScoreByRound(int round)
{
	int targetScore = 0;

	if (round == 1) targetScore = 1000;
	//else targetScore = 10 * round*round + 1950 * round - 1440;
	else targetScore = 10 * round*round + 1950 * round - 1440;
	return targetScore;
}

void StartManage::Resurrection()
{
	peopleStand();
	setmScore(_lastLevelScore);
	LevelUpMotion();
	runAction(Sequence::create(DelayTime::create(1.0f),CallFunc::create([this](){
		startDrop();
	}),nullptr));
}

//∑ΩøÈ≈≈–Ú
void StartManage::boxSequence(vector<pair<int,int>> &vec,bool sx)
{
	int k=vec.size();
	if(k>0)
	{
		for(int i=0;i<k;i++)
			for(int j=k-1;j>i;j--)
			{
				if(sx)
				{
					if(getnum(vec[j])<getnum(vec[j-1]))
					{
						std::swap(vec[j],vec[j-1]);
					}
				}
				else
				{
					if(getrenum(vec[j])<getrenum(vec[j-1]))
					{
						std::swap(vec[j],vec[j-1]);
					}
				}
			}
	}
}

bool StartManage::singelCanEliminate(pair<int,int> pos)
{
	//x-1
	int i=pos.first;
	int j=pos.second;
	int key=getChessInfo(pos);
	if(key)
	{
		if((i-1)>=0&&key==getChessInfo(make_pair(i-1,j)))
		{
			return true;
		}

		//x+1
		if((i+1)<CHESS_SIZE_W_START&&key==getChessInfo(make_pair(i+1,j)))
		{
			return true;
		}

		//y-1
		if((j-1)>=0&&key==getChessInfo(make_pair(i,j-1)))
		{
			return true;
		}

		//y+1
		if((j+1)<CHESS_SIZE_H_START&&key==getChessInfo(make_pair(i,j+1)))
		{
			return true;
		}
	}
	return false;
}

//–¬ ÷÷∏“˝
void StartManage::startGuide()
{
	setPostShow(false);

	auto colorlay=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING*0),WINSIZE.width,WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay,100,100);
	colorlay->setPosition(WINORIGIN);

	auto colorlaysecond=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING),WINSIZE.width,WINSIZE.height);
	this->addChild(colorlaysecond,100,"secondlay");
	colorlaysecond->setPosition(WINORIGIN);

	auto csbsecond=CSLoader::createNode("animation/background_star_two.csb");
	colorlaysecond->addChild(csbsecond);

	auto levelfont=cjAtlasLabel::createWithInt(1,ATLAS_PATH);
	colorlaysecond->addChild(levelfont,1,"levelfont");
	levelfont->setPosition(csbsecond->getChildByName("level")->getPosition());
	levelfont->setScale(0.9f);

	_huptimer->setZOrder(101);
	auto people=this->getChildByName("people");
	people->setZOrder(101);
	auto par = this->getChildByName("par");
	par->setZOrder(102);

	auto csb=CSLoader::createNode("animation/guider.csb");
	colorlay->addChild(csb,3,"csb");

	auto nodes=csb->getChildByName("zi")->getChildren();
	for(auto &i:nodes)
		i->setVisible(false);

	csb->getChildByName("zi")->getChildByName("zi_2")->setVisible(true);

	for(int i=0;i<2;i++)
	{
		auto sp=cjSprite::createWithImage("box1-01.png");
		colorlay->addChild(sp,2,i+1);
		sp->setScale(boxscale);
		sp->setPosition(posToposition(make_pair(2+i,9)));
	}

	auto handnode=handNode::createHand(0.4f);
	colorlay->addChild(handnode,4,"hand");
	handnode->setPosition(posToposition(make_pair(3,9)));
}

void StartManage::setGuide2()
{
	auto colorlayer=Director::getInstance()->getRunningScene()->getChildByTag(100);
	colorlayer->setVisible(true);
	auto secondlay=this->getChildByName("secondlay");
	secondlay->setVisible(true);
	auto csb=colorlayer->getChildByName("csb");
	csb->getChildByName("zi")->getChildByName("zi_3")->setVisible(false);
	csb->getChildByName("zi")->getChildByName("zi_1")->setVisible(true);
	_guidenumber=3;

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [=](Touch* touch, Event* event){
		setGuide3();
		colorlayer->removeFromParent();
		secondlay->removeFromParent();
		auto people=this->getChildByName("people");
		people->setZOrder(3);
		auto par = this->getChildByName("par");
		par->setZOrder(4);
		_huptimer->setZOrder(2);
		return true;
	};
	colorlayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlayer);

	auto handnode = colorlayer->getChildByName("hand");
	handnode->setPosition(Vec2(50,665));
}

void StartManage::setGuide1()
{
	auto colorlayer=Director::getInstance()->getRunningScene()->getChildByTag(100);
	colorlayer->setVisible(true);
	auto secondlay=this->getChildByName("secondlay");
	secondlay->setVisible(true);
	auto csb=colorlayer->getChildByName("csb");
	csb->getChildByName("zi")->getChildByName("zi_2")->setVisible(false);
	csb->getChildByName("zi")->getChildByName("zi_3")->setVisible(true);

	for(int i=0;i<5;i++)
	{
		auto sp=cjSprite::createWithImage("box2-01.png");
		colorlayer->addChild(sp,2,i+1);
		sp->setScale(boxscale);
		sp->setPosition(posToposition(make_pair(4+i,9)));
	}

	auto handnode=colorlayer->getChildByName("hand");
	handnode->setPosition(posToposition(make_pair(6,9)));
}

void StartManage::setGuide3()
{
	auto colorlay=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING),WINSIZE.width,WINSIZE.height);
	this->addChild(colorlay,100,"guide3layer");
	colorlay->setPosition(WINORIGIN);

	//auto seclay=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING),80,80);
	//Director::getInstance()->getRunningScene()->addChild(seclay,100,"guide3seclayer");
	//seclay->setPosition(Vec2(WINSIZE)-Vec2(40,40));

	auto hb=(Menu *)this->getChildByName("hongbaomenu");
	hb->setZOrder(101);
	hb->setEnabled(true);

	auto csb=CSLoader::createNode("animation/guider.csb");
	colorlay->addChild(csb,3);
	auto nodes=csb->getChildByName("zi")->getChildren();
	for(auto &i:nodes)
		i->setVisible(false);

	csb->getChildByName("zi")->getChildByName("zi_8")->setVisible(true);

	auto hand=handNode::createHand(0.4f);
	this->addChild(hand,102,"hand");
	hand->setRotation(-20.0f);
	hand->setPosition(hb->getPosition());
}

void StartManage::setGuide4()
{
	auto colorlay=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING),WINSIZE.width,WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay,100,100);
	colorlay->setPosition(WINORIGIN);

	auto csb=CSLoader::createNode("animation/guider.csb");
	colorlay->addChild(csb,3,"csb");

	auto nodes=csb->getChildByName("zi")->getChildren();
	for(auto &i:nodes)
		i->setVisible(false);

	csb->getChildByName("zi")->getChildByName("zi_9")->setVisible(true);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [=](Touch* touch, Event* event){

		colorlay->removeFromParent();
		startRain();

		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);
}

void StartManage::setGuideProp(int id)
{
	auto colorlay=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING),WINSIZE.width,WINSIZE.height);
	this->addChild(colorlay,100,"guideproplayer");
	colorlay->setPosition(WINORIGIN);

	//auto seclay=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING),80,80);
	//Director::getInstance()->getRunningScene()->addChild(seclay,100,"guidepropseclayer");
	//seclay->setPosition(Vec2(WINSIZE)-Vec2(40,40));

	auto prop=getChildByName(cjTTFLabel::getNameByInt("prop%d",id));
	prop->setZOrder(101);

	auto hand=handNode::createHand(0.4f);
	this->addChild(hand,102,"hand");
	hand->setRotation(-20.0f);
	hand->setPosition(prop->getPosition()+Vec2(0,0));

	auto csb=CSLoader::createNode("animation/guider.csb");
	colorlay->addChild(csb);
	auto nodes=csb->getChildByName("zi")->getChildren();
	for(auto &i:nodes)
		i->setVisible(false);

	int kkk = _guidenumber;
	if (_guidenumber == 5)
		kkk=7;
	else if (_guidenumber>5)
		kkk--;

	auto font = csb->getChildByName("zi")->getChildByName(cjTTFLabel::getNameByInt("zi_%d", kkk));
	font->setVisible(true);
	font->runAction(RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(ScaleTo::create(0.6f, 1.02f), ScaleTo::create(0.6f, 0.98f))));
}

//µ⁄4∏ˆµ¿æﬂ–ßπ˚
void StartManage::useProps(pair<int,int> pos)
{
	LayerColor* pPropChildLayer = LayerColor::create(Color4B(0, 0, 0, OPA_BEIJING*255));

	LayerColor* pPropChildLayer2 = LayerColor::create(Color4B(0, 0, 0, OPA_BEIJING*255));
	this->addChild(pPropChildLayer2,9,9);

	int color = getChessInfo(pos);
	std::string colorName;
	std::string actionName_Line;

	switch (color)
	{
	case 4:
		colorName = "red";
		break;
	case 5:
		colorName = "yellow";
		break;
	case 2:
		colorName = "blue";
		break;
	case 1:
		colorName = "green";
		break;
	case 3:
		colorName = "purple";
		break;
	}

	int cleanNo = 0;
	pair<int,int> pStar1;
	pair<int,int> pStar2;
	Vector<FiniteTimeAction*> arrayOfActions;

	if(_selectbox.size()>0)
		cjSchedule::getInstance()->shockBegin(this);

	for(auto &i:_selectbox)
	{
		auto node=_clip->getChildByTag(i.first*CHESS_SIZE_H_START+i.second);
		node->setZOrder(101);
	}

	_shandianid=cjMusic::playEffect("video/shandian.mp3",true);

	do
	{
		if (_selectbox.empty())
		{
			arrayOfActions.pushBack(DelayTime::create(0.1f));
			arrayOfActions.pushBack(CallFunc::create([=](){
				pPropChildLayer->removeFromParent();
				pPropChildLayer2->removeFromParent();
			}));
			break;
		}

		cleanNo++;
		if (cleanNo == 1)
		{
			pStar1 = pos;
		}
		else
		{
			pStar1 = pStar2;
		}

		_selectbox.erase(std::find(_selectbox.begin(),_selectbox.end(),pStar1));
		if (_selectbox.size() > 0)
		{
			pStar2 = _selectbox[rand() % _selectbox.size()];
		}
		else
		{
			pStar2 = make_pair(-1,-1);
		}

		Vec2 position1 = posToposition(pStar1);

		arrayOfActions.pushBack(DelayTime::create(0.1f));
		//arrayOfActions.pushBack(CallFunc::create([=](){
		//	//pGameStar->playScoreAction(pStar1, 0);
		//	//pGameStar->playCleanStarSound();
		//	//pGameStar->removeStar(x1, y1);
		//}));
		arrayOfActions.pushBack(CallFunc::create([=](){
			auto particleNodes = CCNode::create();
			_clip->addChild(particleNodes, 102);

			auto skeletonNode = CSLoader::createNode("animation/xiaochu.csb");
			skeletonNode->setPosition(position1);
			particleNodes->addChild(skeletonNode, 3);

			auto action = CSLoader::createTimeline("animation/xiaochu.csb");
			skeletonNode->runAction(action);

			action->play("red_line", false);
			action->setLastFrameCallFunc([skeletonNode](){
				skeletonNode->removeFromParent();
			});

			auto playParticle1 = CallFunc::create([=](){
				addParticle(pStar1.first,pStar1.second,getChessInfo(pStar1),1);
				removeBox(pStar1,2);
			});

			//CallFunc* playParticle2 = CallFunc::create([=](){
			//	float dis=2.0f;
			//	this->runAction(Sequence::create(MoveTo::create(0.025f,Vec2(dis,0)),MoveTo::create(0.05f,Vec2(-dis,0)),MoveTo::create(0.025f,Vec2(0.0f,0.0f)),nullptr));
			//	this->runAction(Sequence::create(MoveTo::create(0.025f,Vec2(0,dis)),MoveTo::create(0.05f,Vec2(0,-dis)),MoveTo::create(0.025f,Vec2(0.0f,0.0f)),nullptr));
			//});

			particleNodes->runAction(Sequence::create(
				DelayTime::create(1 / 30.f),
				playParticle1,
				//DelayTime::create(1 / 30.f),
				//playParticle2,
				nullptr));

		}));

		if (pStar2.first>=0)
		{
			Vec2 position2 = posToposition(pStar2);
			arrayOfActions.pushBack(CallFunc::create([=](){
				auto propSkillNode = CSLoader::createNode("animation/prop_skill_laser.csb");

				std::string lineNameArray[] = { "red", "yellow", "blue", "green", "purple" };
				int randLine = rand() % 5;
				auto colorSprite = static_cast<Sprite*>(propSkillNode->getChildByName("skill4_red"));
				colorSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(
					"animation/daoju_images/skill4_" + lineNameArray[randLine] + ".png"));

				propSkillNode->setPosition(position1);
				float angle = -1 * CC_RADIANS_TO_DEGREES((position2 - position1).getAngle());
				propSkillNode->setRotation(angle);
				propSkillNode->setScaleX(position1.getDistance(position2) / colorSprite->getContentSize().width);
				_clip->addChild(propSkillNode, 102);

				auto propSkillAction = CSLoader::createTimeline("animation/prop_skill_laser.csb");
				propSkillNode->runAction(propSkillAction);

				propSkillAction->play("red", false);

				propSkillAction->setLastFrameCallFunc([propSkillNode](){
					propSkillNode->removeFromParent();
				});
			}));
		}
	} while (true);
	arrayOfActions.pushBack(CallFunc::create([this](){
		cjSchedule::getInstance()->shockEnd(this);
		cjMusic::stopEffect(_shandianid);
	}));

	pPropChildLayer->runAction(Sequence::create(arrayOfActions));

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	pPropChildLayer2->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, pPropChildLayer2);

	//Director::getInstance()->getRunningScene()->addChild(pPropChildLayer, 10);
	_clip->addChild(pPropChildLayer,100);
}

//µ⁄2∏ˆµ¿æﬂ
void StartManage::useProp4(pair<int,int> pos)
{
	auto node=_clip->getChildByTag(pos.first*CHESS_SIZE_H_START+pos.second);

	LayerColor* pPropChildLayer2 = LayerColor::create(Color4B(0, 0, 0, OPA_BEIJING*255));
	this->addChild(pPropChildLayer2,9);

	LayerColor* pPropChildLayer = LayerColor::create(Color4B(0, 0, 0, OPA_BEIJING*255));
	_clip->addChild(pPropChildLayer,100);

	auto splayer=Layer::create();
	this->addChild(splayer,100);

	node->setZOrder(101);
	node->runAction(RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(ScaleTo::create(0.5f,0.9f*boxscale),ScaleTo::create(0.5f,1.0f*boxscale))));

	auto guang=cjSprite::createWithImage("skill2_squaguang.png");
	splayer->addChild(guang);
	guang->setZOrder(100);
	guang->setPosition(node->getPosition());

	auto jiantou=cjSprite::createWithImage("skill2_kuangjiantou.png");
	splayer->addChild(jiantou,102);
	jiantou->setAnchorPoint(Vec2(0.5f,0.0f));
	jiantou->setPosition(node->getPosition()+Vec2(0,CHESSMAN_SIZE_START/2.0f));

	auto kuang=cjSprite::createWithImage("skill2_kuang.png");
	splayer->addChild(kuang,101);
	if(pos.first>1&&pos.first<(CHESS_SIZE_W_START-2))
		kuang->setPosition(Vec2(WINSIZE.width/2.0f,node->getPosition().y+CHESSMAN_SIZE_START/2.0f+58));
	else if(pos.first<=1)
		kuang->setPosition(Vec2(kuang->getContentSize().width/2.0f,node->getPosition().y+CHESSMAN_SIZE_START/2.0f+58));
	else
		kuang->setPosition(Vec2(WINSIZE.width-kuang->getContentSize().width/2.0f,node->getPosition().y+CHESSMAN_SIZE_START/2.0f+58));

	auto cancel=cjMenu::createWithImage("skill2_back.png",[=](Ref *mu){
		splayer->removeFromParent();
		pPropChildLayer->removeFromParent();
		pPropChildLayer2->removeFromParent();
		node->stopAllActions();
		node->setScale(boxscale);
		node->setZOrder(CHESSMAN_LEVEL);
		_selectprop=0;
	});
	splayer->addChild(cancel,103);
	cancel->setPosition(kuang->getPosition()+Vec2(kuang->getContentSize().width/2.0f-35,0));
	if(_guide)
		cancel->setEnabled(false);

	int key=0;
	for(int i=1;i<=5;i++)
	{
		if(i!=_chessInfo[pos.first*CHESS_SIZE_H_START+pos.second])
		{
			Menu* sp=cjMenu::createWithImage(cjTTFLabel::getNameByInt("box%d-01.png",i),[=](Ref *mu){
				setGameState(STATE_STOP);

				auto anode=StartBox::createBox(i);
				_clip->addChild(anode,CHESSMAN_LEVEL);
				anode->setPosition(kuang->getPosition()+Vec2(-kuang->getContentSize().width/2.0f+35+key*70,0));
				anode->runAction(Sequence::create(MoveTo::create(0.5f,node->getPosition()),CallFunc::create([=](){
					auto csb=CSLoader::createNode("animation/prop_skill_paint.csb");
					this->addChild(csb,10);
					csb->setPosition(posToposition(pos));
					auto csbaction=CSLoader::createTimeline("animation/prop_skill_paint.csb");
					csb->runAction(csbaction);
					csbaction->play("change",false);
					csbaction->setLastFrameCallFunc([csb](){
						csb->removeFromParent();
					});

					cjMusic::playEffect("video/qiehuan.wav");

					anode->runAction(Sequence::create(ScaleTo::create(0.2f,boxscale),CallFunc::create([this](){
						if(getGameState()==STATE_STOP)
							setGameState(STATE_RUN);
						GameData::getInstance()->dataSave();
						touchHandle();
					}),nullptr));
					anode->setTag(pos.first*CHESS_SIZE_H_START+pos.second);
					_chessInfo[pos.first*CHESS_SIZE_H_START+pos.second]=i;
				}),nullptr));

				splayer->removeFromParent();
				pPropChildLayer->removeFromParent();
				pPropChildLayer2->removeFromParent();
				node->stopAllActions();
				node->setScale(boxscale);
				_selectprop=0;
				node->runAction(Sequence::create(ScaleTo::create(0.4f,0.0f),CallFunc::create([=](){
					_clip->removeChildByTag(pos.first*CHESS_SIZE_H_START+pos.second);
				}),nullptr));

				if(!_guide)
				{
					if (GameData::getSaveData()->_propnum[1] == 0)
						GameData::getSaveData()->_diamondNumber -= diamondnum[1];
					else
					{
						GameData::getSaveData()->_propnum[1]--;
						if (GameData::getSaveData()->_propnum[1] == 0)
							_propnum[1]->setVisible(false);
						else
						{
							_propnum[1]->setString(cjTTFLabel::getNameByInt("x%d", GameData::getSaveData()->_propnum[1]));
						}

						if (2 == GameData::getSaveData()->_hupropid)
						{
							GameData::getSaveData()->_hupropid = 0;
						}
					}
					GameData::getInstance()->dataSave();
				}
			});
			splayer->addChild(sp,103);
			sp->setAnchorPoint(Vec2(0.0f,0.0f));
			sp->setScale(boxscale);
			sp->setPosition(kuang->getPosition()+Vec2(-kuang->getContentSize().width/2.0f+35+key*70,0));
			key++;
		}
	}

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	pPropChildLayer2->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, pPropChildLayer2);
}

void StartManage::dataSave(bool tt)
{
	if(_guide)
		return;

	if(tt)
	{
		GameData::getSaveData()->_start._istip=true;

		for(int i=0;i<CHESS_SIZE_W_START;i++)
			for(int j=0;j<CHESS_SIZE_H_START;j++)
				GameData::getSaveData()->_start._chessInfo[i][j]=_chessInfo[i*CHESS_SIZE_H_START+j];
		GameData::getSaveData()->_start._level=getmLevel();
		GameData::getSaveData()->_start._score=getmScore();
		GameData::getSaveData()->_start._target=getmTarget();
		GameData::getSaveData()->_start._lastlevelscore=_lastLevelScore;
		if(getmScore()>GameData::getSaveData()->_start._highestscore)
		{
			GameData::getSaveData()->_start._highestscore=getmScore();
		}
	}
	else
		GameData::getSaveData()->_start._istip=false;
	GameData::getInstance()->dataSave();
}

//¡˜–«”Í
void StartManage::startRain()
{
	auto liupos=this->getChildByName("hongbaomenu")->getPosition();

	vector<pair<int,int>> selectvec;
	for(int i=0;i<CHESS_SIZE_W_START;i++)
		for(int j=0;j<(CHESS_SIZE_H_START-2);j++) 
		{
			if(_chessInfo[i*CHESS_SIZE_H_START+j])
				selectvec.push_back(make_pair(i,j));
		}

		vector<pair<int,int>> vec;
		int number=rand()%5+5;
		number=MIN(number,selectvec.size());
		for(int i=0;i<number;i++)
		{
			int num=rand()%selectvec.size();
			vec.push_back(selectvec[num]);
			selectvec.erase(selectvec.begin()+num);
		}

		vector<pair<int,int>> unselectvec;
		for(int i=0;i<20;i++)
		{
			bool temp=true;
			do 
			{
				int kx=rand()%CHESS_SIZE_W_START;
				int ky=rand()%(CHESS_SIZE_H_START-2);
				if(std::find(vec.begin(),vec.end(),make_pair(kx,ky))==vec.end())
				{
					temp=false;
					unselectvec.push_back(make_pair(kx,ky));
				}
			} while (temp);
		}

		levelupmusic=0;
		setGameState(STATE_STOP);
		Vector<FiniteTimeAction*> action;
		action.pushBack(CallFunc::create([this](){
			auto node=Node::create();
			this->addChild(node,10);
			node->setPosition(Vec2(WINSIZE/2.0f)-Vec2(0,100));

			auto lxy1=cjSprite::createWithImage("lxy_1.png");
			node->addChild(lxy1);

			auto lxy2=cjSprite::createWithImage("lxy_2.png");
			node->addChild(lxy2);

			node->runAction(Sequence::create(MoveBy::create(0.5f,Vec2(0,150)),MoveBy::create(0.1f,Vec2(0,-10)),DelayTime::create(0.4f),CallFunc::create([node](){
				node->removeFromParent();
			}),nullptr));
		}));
		action.pushBack(DelayTime::create(1.0f));
		for(auto &i:unselectvec)
		{
			action.pushBack(DelayTime::create(0.1f));
			action.pushBack(CallFunc::create([this,i,liupos](){
				//auto par=ParticleSystemQuad::create("lizi/jianglishike0.plist");
				//auto par=cocostudio::GUIReader::shareReader()->
				ArmatureDataManager::getInstance()->addArmatureFileInfo("lizi/liuxingyu/jianglishike0.png","lizi/liuxingyu/jianglishike0.plist","lizi/liuxingyu/jianglishike.ExportJson");
				auto armature=Armature::create("jianglishike");
				armature->setPosition(liupos);
				armature->getAnimation()->play("shexian");
				this->addChild(armature,100);
				armature->setAnchorPoint(Vec2(0.8f,0.5f));
				auto d1=armature->getPosition();
				auto d2=posToposition(i);
				auto dd=atan2((d1.y-d2.y),(d2.x-d1.x));
				int kk=dd/3.1415*180;
				armature->setRotation(kk);
				float flo=liuxingtime;
				armature->setScale(0.0f);
				armature->runAction(ScaleTo::create(flo*0.8f,1.0f));
				auto move1=MoveTo::create(flo,posToposition(i));
				auto call=CallFunc::create([armature,i,this](){
					armature->removeFromParent();
				});
				armature->runAction(Sequence::create(move1,call,nullptr));

				levelupmusic++;
				cjMusic::playEffect(cjTTFLabel::getNameByInt("video/lxy_%02d.mp3", (levelupmusic % 3 + 1)).c_str());
			}));
		}
		runAction(Sequence::create(action));

		Vector<FiniteTimeAction*> action2;
		action2.pushBack(DelayTime::create(1.0f));
		for(auto &i:vec)
		{
			action2.pushBack(DelayTime::create(0.3f));
			action2.pushBack(CallFunc::create([this,i,liupos](){
				ArmatureDataManager::getInstance()->addArmatureFileInfo("lizi/liuxingyu/jianglishike0.png","lizi/liuxingyu/jianglishike0.plist","lizi/liuxingyu/jianglishike.ExportJson");
				auto par=Armature::create("jianglishike");
				par->setPosition(liupos);
				par->getAnimation()->play("shexian");
				this->addChild(par,100);
				par->setAnchorPoint(Vec2(0.8f,0.5f));
				auto d1=par->getPosition();
				auto d2=posToposition(i);
				auto dd=atan2((d1.y-d2.y),(d2.x-d1.x));
				int kk=dd/3.1415*180;
				par->setRotation(kk);
				float flo=liuxingtime;
				par->setScale(0.0f);
				par->runAction(ScaleTo::create(flo/2.0,1.0f));

				levelupmusic++;
				cjMusic::playEffect(cjTTFLabel::getNameByInt("video/lxy_%02d.mp3",(levelupmusic %3+1)).c_str());

				auto move1=MoveTo::create(flo,posToposition(i));
				auto call0=CallFunc::create([i,this](){
					auto box=(StartBox*)_clip->getChildByTag(i.first*CHESS_SIZE_H_START+i.second);
					auto par=ParticleSystemQuad::create("lizi/liuxing2_hit.plist");
					box->addChild(par,10);
					par->setPosition(Vec2(par->getContentSize()/2.0f));
					par->setAutoRemoveOnFinish(true);

		//			cjMusic::playEffect("video/pop_1.wav");
				});
				auto call=CallFunc::create([par,i,this](){
					par->removeFromParent();
					auto box=(StartBox*)_clip->getChildByTag(i.first*CHESS_SIZE_H_START+i.second);
					if(box)
					{
						auto propSkillNode = CSLoader::createNode("animation/xiaochu.csb");
						this->addChild(propSkillNode, 12);
						propSkillNode->setPosition(box->getPosition()+Vec2(0,CHESSMAN_SIZE_START*0.5f));
						auto propSkillAction=CSLoader::createTimeline("animation/xiaochu.csb");
						propSkillNode->runAction(propSkillAction);
						propSkillAction->play("red_line",false);
						propSkillAction->setLastFrameCallFunc([=](){
							propSkillNode->removeFromParent();
						});

						box->setBox2();

						//char name[30];
						//sprintf(name,"video/pop%02d.mp3",levelupmusic%4+1);
						//cjMusic::playEffect(name);
						//levelupmusic++;
					}
				});
				par->runAction(Sequence::create(move1,call0,call,nullptr));
			}));
		}
		action2.pushBack(DelayTime::create(liuxingtime+0.6f));
		action2.pushBack(CallFunc::create([vec,this](){
			cjMusic::playEffect("video/pop_1.wav");
			for(auto &i:vec)
			{
				//addParticle(i.first,i.second,getChessInfo(i),11);
				auto par = ParticleSystemQuad::create("lizi/area_boom.plist");
				_clip->addChild(par, 10);
				par->setPosition(posToposition(i));
				par->setAutoRemoveOnFinish(true);
				par->setScale(0.5f);
				par->setColor(Color3B::WHITE);
				removeBox(i,2);
			}
			int num=getRetainScore(vec.size());
			_score+=num;
			scoreMotion(num,make_pair(CHESS_SIZE_W_START/2,CHESS_SIZE_H_START/2));
			if(_score>=_target&&(_score-num)<_target)
			{
				spriteShow("guoguan.png");
			}
		}));
		action2.pushBack(DelayTime::create(boxeliminatetime));
		action2.pushBack(CallFunc::create([this](){
			touchHandle();
		}));
		runAction(Sequence::create(action2));
}

//ÀÕ’®µØ
void StartManage::getBoom()
{
	setGameState(STATE_STOP);

	auto boom=cjSprite::createWithImage("prop01.png");
	this->addChild(boom,11);
	boom->setScale(3.0f);
	boom->setPosition(Vec2(WINSIZE/2.0f)+Vec2(0,100));

	auto back=cjSprite::createWithImage("fsx.png");
	this->addChild(back,10);
	back->setPosition(boom->getPosition());
	back->runAction(RepeatForever::create((ActionInterval*)RotateBy::create(1.0f,100)));

	Vector<FiniteTimeAction*> action;
	action.pushBack(DelayTime::create(1.0f));
	action.pushBack(CallFunc::create([boom,back](){
		back->removeFromParent();
		boom->runAction(MoveTo::create(0.5f,Vec2(WINSIZE.width-200,propsheight)));
		boom->runAction(ScaleTo::create(0.5f,1.0f));
	}));
	action.pushBack(DelayTime::create(0.5f));
	action.pushBack(CallFunc::create([boom,this](){
		GameData::getSaveData()->_propnum[0]++;

		propNumShow(0,1);
		boom->removeFromParent();
		setGameState(STATE_RUN);
	}));
	runAction(Sequence::create(action));

}

//∆Â◊”≥ˆœ÷∑Ω Ω
float StartManage::chessAppear(int key)
{
	float time[8]={0.1f*CHESS_SIZE_H_START+0.3f,0.3f,0.3f,1.1f,0.2f,0.3f,0.6f,0.02f*CHESS_SIZE_H_START*CHESS_SIZE_W_START+0.25f};
	for(int i=0;i<CHESS_SIZE_W_START;i++)
		for(int j=0;j<CHESS_SIZE_H_START;j++)
		{
			if(_chessInfo[i*CHESS_SIZE_H_START+j])
			{
				auto box=StartBox::createBox(_chessInfo[i*CHESS_SIZE_H_START+j]);
				box->setScale(boxscale);
				_clip->addChild(box,CHESSMAN_LEVEL,i*CHESS_SIZE_H_START+j);
				if(key==0)
				{
					box->setPosition(posToposition(make_pair(i,j))+Vec2(0,CHESS_SIZE_H_START*CHESSMAN_SIZE_START));
					box->runAction(Sequence::create(DelayTime::create(j*0.1f+rand()%10*0.01f),MoveBy::create(0.2f,Vec2(0,-CHESS_SIZE_H_START*CHESSMAN_SIZE_START)),nullptr));
				}
				else if(key==1)
				{
					box->setPosition(posToposition(make_pair(i,j))+Vec2(-CHESS_SIZE_W_START*CHESSMAN_SIZE_START,0));
					box->runAction(MoveTo::create(0.2f,posToposition(make_pair(i,j),0)));
				}
				else if(key==2)
				{
					box->setPosition(posToposition(make_pair(i,j))+Vec2(CHESS_SIZE_W_START*CHESSMAN_SIZE_START,0));
					box->runAction(MoveTo::create(0.2f,posToposition(make_pair(i,j),0)));
				}
				else if(key==3)
				{
					box->setPosition(posToposition(make_pair(i,j)));
					box->setOpacity(0);
					box->runAction(FadeIn::create(1.0f));
				}
				else if(key==4)
				{
					if(i<CHESS_SIZE_W_START/2)
						box->setPosition(posToposition(make_pair(i,j))+Vec2(-(CHESS_SIZE_W_START*CHESSMAN_SIZE_START/2),0));
					else
						box->setPosition(posToposition(make_pair(i,j))+Vec2(((CHESS_SIZE_W_START-CHESS_SIZE_W_START/2)*CHESSMAN_SIZE_START),0));
					box->runAction(MoveTo::create(0.2f,posToposition(make_pair(i,j),0)));
				}
				else if(key==5)
				{
					if(j<CHESS_SIZE_H_START/2)
						box->setPosition(posToposition(make_pair(i,j))+Vec2(0,-(CHESS_SIZE_H_START*CHESSMAN_SIZE_START/2)));
					else
						box->setPosition(posToposition(make_pair(i,j))+Vec2(0,((CHESS_SIZE_H_START-CHESS_SIZE_H_START/2)*CHESSMAN_SIZE_START)));
					box->runAction(MoveTo::create(0.2f,posToposition(make_pair(i,j),0)));
				}
				else if(key==6)
				{
					int k=1;
					if((j%2)==0)
						k=-1;
					box->setPosition(posToposition(make_pair(i,j))+Vec2(k*CHESS_SIZE_W_START*CHESSMAN_SIZE_START,0));
					box->runAction(MoveTo::create(0.5f,posToposition(make_pair(i,j))));
				}
				//else if(key==7)
				//{
				//	int k=0;
				//	if((j%2)==0)
				//		k=j*CHESS_SIZE_W_START+i;
				//	else
				//		k=(j*CHESS_SIZE_W_START+CHESS_SIZE_W_START-i);
				//	box->setPosition(posToposition(make_pair(i,j))+Vec2(0,CHESS_SIZE_H_START*CHESSMAN_SIZE_START));
				//	box->runAction(Sequence::create(DelayTime::create(k*0.02f),MoveBy::create(0.15f,Vec2(0,-CHESS_SIZE_H_START*CHESSMAN_SIZE_START)),nullptr));
				//}
			}
		}
		return time[key];
}

//∫Ï∞¸
void StartManage::hongbaoPage()
{					
	this->removeChildByName("hbhand");

	if(_guidenumber==3)
	{
		this->removeChildByName("guide3layer");
		//Director::getInstance()->getRunningScene()->removeChildByName("guide3seclayer");
		this->getChildByName("hongbaomenu")->setZOrder(1);
		this->removeChildByName("hand");
	}

	auto colorlay=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING),WINSIZE.width,WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay,100,100);
	colorlay->setPosition(WINORIGIN);

	PayScene::getInstance()->diamondRain(colorlay);

	auto node=CSLoader::createNode("animation/hongbao.csb");
	colorlay->addChild(node,0,"hongbaonode");
	node->setPosition(Vec2(WINORIGIN)+Vec2(0,50));

	_hongbaoclick=false;
	auto motion=CSLoader::createTimeline("animation/hongbao.csb");
	node->runAction(motion);
	motion->play("hongbao",false);
	runAction(Sequence::create(DelayTime::create(0.7f),CallFunc::create([this,colorlay](){
		_hongbaoclick=true;

		auto shou=cjSprite::createWithImage("ybb.png");
		colorlay->addChild(shou,1,"shou");
		shou->setPosition(Vec2(100,300));
		shou->setScale(0.6f);
		shou->setRotation(-20.0f);
		auto seq=Sequence::createWithTwoActions(MoveTo::create(1.5f,Vec2(400,300)),DelayTime::create(0.5f));
		shou->runAction(RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(seq,MoveTo::create(0.0f,Vec2(100,300)))));

	}),nullptr));

	for(int i=1;i<6;i++)
	{
		auto mu1=dynamic_cast<ui::Button*>(node->getChildByName(cjTTFLabel::getNameByInt("button%d",i)));
		mu1->setTag(i);
		mu1->addTouchEventListener(CC_CALLBACK_2(StartManage::buttonClick,this));
	}

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);
}

//∫Ï∞¸∞¥≈•µ„ª˜ ¬º˛
void StartManage::buttonClick(Ref* pSender, ui::Widget::TouchEventType touchType)
{
	auto menu=dynamic_cast<ui::Button*>(pSender);
	int k=menu->getTag();
	if((touchType==ui::Widget::TouchEventType::ENDED)&&(k>0&&k<7)&&_hongbaoclick)
	{
		cjMusic::playEffect("video/tap.mp3");
		menu->setZOrder(100);
		_hongbaoclick=false;
		CCLOG(cjTTFLabel::getNameByInt("click %d",k).c_str());

		auto layer=Director::getInstance()->getRunningScene()->getChildByTag(100);
		auto node=layer->getChildByName("hongbaonode");
		node->getChildByName("chouqu")->setVisible(false);
		layer->removeChildByName("shou");

		auto call=CallFunc::create([menu](){
			menu->runAction(Sequence::create(MoveTo::create(0.2f,Vec2(240,100)),ScaleTo::create(0.2f,0.0f,1.0f),nullptr));
			menu->runAction(RotateTo::create(0.3f,0.0f));
		});
		auto time=DelayTime::create(0.4f);
		auto call1=CallFunc::create([this,node](){
			hongbaoReward();
			node->removeFromParent();
		});
		runAction(Sequence::create(call,time,call1,nullptr));
	}
}

//∫Ï∞¸¿ÒŒÔ
void StartManage::hongbaoReward()
{
	auto layer=(Layer*)Director::getInstance()->getRunningScene()->getChildByTag(100);

	int rewardid=1;

	if(_guidenumber==3)
	{
		rewardid=50;
		_guidenumber=4;
	}
	else if(GameData::getSaveData()->_hongbaotime==0)
	{
		if(GameData::getSaveData()->_hongbaonum<=5)
			GameData::getSaveData()->_hongbaonum++;

		if(GameData::getSaveData()->_starthongbaonum<=5)
			GameData::getSaveData()->_starthongbaonum++;

		if(GameData::getSaveData()->_starthongbaonum==1)
			rewardid=50;
		else if(GameData::getSaveData()->_starthongbaonum==2)
			rewardid=20;
		else if(GameData::getSaveData()->_starthongbaonum==3)
			rewardid=51;
		else if(GameData::getSaveData()->_starthongbaonum==4)
			rewardid=52;
		else if(GameData::getSaveData()->_starthongbaonum==5)
			rewardid=10;
		else
		{
			int ddd=rand()%10;
			if(ddd>=5)
				rewardid=45+ddd;
			else
				rewardid=rand()%7+2;
		}
	}

	auto node=CSLoader::createNode("animation/reward.csb");
	char name[30];
	if (rewardid < 30)
	{
		sprintf(name,"dimond-%d",rewardid);
		PayScene::getInstance()->diamondRain(layer);
	}
	else
	{
		sprintf(name,"prop");
		PayScene::getInstance()->yanhua(layer);
	}

	auto sp=node->getChildByName(name);
	sp->retain();
	sp->removeFromParent();
	layer->addChild(sp,1);
	sp->setAnchorPoint(Vec2(0.5f,0.5f));
	sp->setPosition(Vec2(240,530));

	if(rewardid>30)
	{
		int key=rewardid-50;
		auto secondsp=cjSprite::createWithImage(cjTTFLabel::getNameByInt("prop%02d.png",key));
		sp->addChild(secondsp,1,"secondsp");
		secondsp->setPosition(Vec2(sp->getContentSize()/2.0f)+Vec2(0,10));
		if(key==0)
		{
			secondsp->setScale(1.0f);
			secondsp->setPosition(secondsp->getPosition()-Vec2(5,10));
		}
		else
			secondsp->setScale(1.2f);
	}
	else
	{
		//auto diamonds=CCParticleSystemQuad::create("lizi/diamonds.plist");
		//layer->addChild(diamonds,2);
		//diamonds->setPosition(Vec2(WINSIZE.width/2.0f,WINSIZE.height*DIAMOND_HIGH));
		//diamonds->setAutoRemoveOnFinish(true);
	}

	sp->setScaleX(0.0f);
	sp->runAction(Sequence::create(ScaleTo::create(0.2f,1.0f),MoveTo::create(0.3f,Vec2(240,350)),CallFunc::create([layer,sp,rewardid,this](){
		auto gx=cjSprite::createWithImage("gxn.png");
		layer->addChild(gx);
		gx->setPosition(Vec2(240,520));
		gx->setScale(0.0f);
		gx->runAction(ScaleTo::create(0.2f,1.0f));

		auto guangnode=CSLoader::createNode("animation/guang.csb");
		layer->addChild(guangnode);
		guangnode->setPosition(sp->getPosition());

		auto guangaction=CSLoader::createTimeline("animation/guang.csb");
		guangnode->runAction(guangaction);
		guangaction->play("guang",true);

		layer->runAction(Sequence::create(DelayTime::create(2.0f),CallFunc::create([guangnode](){
			guangnode->removeFromParent();
		}),nullptr));

	}),DelayTime::create(2.0f),CallFunc::create([=](){
		if(rewardid<30)
		{
			PayScene::getInstance()->turnAddDiamond(this, rewardid, diamondpos);
		}
		else if(rewardid==50)
		{
			if(_guide)
			{
				setGuide4();
			}
			else
				startRain();
		}
		else if(rewardid>50)
		{
			int propid=rewardid-51;

			auto propsp=this->getChildByName(cjTTFLabel::getNameByInt("prop%d",propid+1));
			auto secondsp=sp->getChildByName("secondsp");
			secondsp->retain();
			secondsp->removeFromParent();
			this->addChild(secondsp,100);
			secondsp->setPosition(Vec2(240,420));
			secondsp->runAction(ScaleTo::create(0.7f, propscale*1.2f));
			secondsp->runAction(Sequence::create(MoveTo::create(0.7f,propsp->getPosition()),ScaleTo::create(0.3f, propscale),CallFunc::create([=](){
				GameData::getSaveData()->_propnum[propid]++;
				secondsp->removeFromParent();

				propNumShow(propid,1);

				PayScene::getInstance()->parShow(this, 10,propsp->getPosition());

				cjMusic::playEffect("video/coinsin.mp3");
				GameData::getInstance()->dataSave();

			}),nullptr));
			secondsp->runAction(ScaleTo::create(0.5f,1.0f));
		}

		GameData::getSaveData()->_hongbaotime=TIME_HONGBAO[GameData::getSaveData()->_hongbaonum];
		GameData::getInstance()->dataSave();
		if(!_guide)
		{
			auto font=this->getChildByName("hongbao");
			font->setVisible(true);
		}

		layer->removeFromParent();

	}),nullptr));

}

//∑ΩøÈ ’ºØΩ±¿¯
void StartManage::cubeCollect(pair<int,int> pos)
{
	if (GameData::getSaveData()->_hunum < GameData::getSaveData()->_huhightnum)
	{
		GameData::getSaveData()->_hunum++;
		if (GameData::getSaveData()->_hunum == GameData::getSaveData()->_huhightnum)
		{
			auto people = dynamic_cast<Armature*>(getChildByName("people"));
			people->setZOrder(100);
			people->getAnimation()->play("present");

			//Ω¯∂»ÃıÃÿ–ß
			auto txcsb = CSLoader::createNode("animation/ProcessNode.csb");
			this->addChild(txcsb,101);
			txcsb->setScale(0.75f);
			txcsb->setPosition(Vec2(54, 735));
			auto txanimation = CSLoader::createTimeline("animation/ProcessNode.csb");
			txcsb->runAction(txanimation);
			txanimation->play("jindutiao",false);
			txanimation->setLastFrameCallFunc([=]() {
				txcsb->removeFromParent();
			});
			//

			std::function<void(Armature*, MovementEventType, const std::string&)> armatureFun = [=](Armature* armature, MovementEventType type, const std::string& id) {
				if ((id=="present")&&(type == MovementEventType::COMPLETE))
				{
					cubeGift();

					armature->getAnimation()->stop();
					peopleStand();
					people->setZOrder(3);
				}
			};

			people->getAnimation()->setMovementEventCallFunc(armatureFun);
		}
	}

	auto cubesp=cjSprite::createWithImage(cjTTFLabel::getNameByInt("box%d-01.png",getChessInfo(pos)));
	this->addChild(cubesp,13);
	cubesp->setScale(0.5f);
	cubesp->setPosition(posToposition(pos));
	cubesp->runAction(Sequence::create(
		MoveTo::create(0.6f,Vec2(50,730)),
		CallFunc::create([=](){
			cubesp->removeFromParent();	

			if(_hunum<GameData::getSaveData()->_huhightnum)
				_hunum++;
			//auto husp=(Sprite*)getChildByName("husp");
			//husp->setRotation(90.0f*(GameData::getSaveData()->_huhightnum-_hunum)/GameData::getSaveData()->_huhightnum);
	}),nullptr));
}

void StartManage::cubeGift()
{
	//if (getGameState() == STATE_FAILD)
	//	return;

	LayerColor* pPropChildLayer = LayerColor::create(Color4B(0, 0, 0, OPA_BEIJING*255));

	auto csb=CSLoader::createNode("animation/hugift.csb");
	pPropChildLayer->addChild(csb);
	csb->setPosition(WINORIGIN);

	PayScene::getInstance()->diamondRain(pPropChildLayer);

	auto csbaction=CSLoader::createTimeline("animation/hugift.csb");
	csb->runAction(csbaction);
	csbaction->play("xingxinglibao",false);
	csbaction->setLastFrameCallFunc([=](){
		csbaction->play("xingxinglibao_loop",true);
	});

	auto bg=csb->getChildByName("node")->getChildByName("bg");

	int diamondnum=rand()%4+2;
	int prop=rand()%4;

	for(int i=0;i<2;i++)
	{
		auto back=Sprite::create("animation/Newhand/lb_pic_box.png");
		bg->addChild(back,1);
		back->setScale(0.6f);
		back->setPosition(Vec2(bg->getContentSize().width*(0.35f+i*0.3f),bg->getContentSize().height*0.45f));

		string str;
		if(i==0)
			str="zs.png";
		else
			str=cjTTFLabel::getNameByInt("prop%02d.png",prop+1);
		auto sp=cjSprite::createWithImage(str);
		bg->addChild(sp);
		sp->setPosition(back->getPosition());
		string numname="x1";
		if(i==0)
		{
			sp->setScale(1.2f);
			numname=cjTTFLabel::getNameByInt("x%d",diamondnum);
			sp->setRotation(30);
		}

		auto label=LabelBMFont::create(numname,FNT_PATH);
		bg->addChild(label);
		label->setPosition(back->getPosition()-Vec2(0,65));
	}

//	auto husp=getChildByName("husp");

	auto cancel=dynamic_cast<ui::Button*>(csb->getChildByName("node")->getChildByName("bg")->getChildByName("Button_Cancel"));
	cancel->addTouchEventListener([=](Ref *rf,ui::Widget::TouchEventType type){
		if(type==ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");
			pPropChildLayer->removeFromParent();
			GameData::getSaveData()->_huhightnum+=100;
			GameData::getSaveData()->_hunum=0;
			_hunum=0;
			GameDataInstance()->dataSave();


			if(!_candealwith)
			{
				_candealwith=true;
				handleHandle();
			}
		}
	});

	auto sure=dynamic_cast<ui::Button*>(csb->getChildByName("node")->getChildByName("bg")->getChildByName("Button_Lingqu"));
	sure->addTouchEventListener([=](Ref *,ui::Widget::TouchEventType type){
		if(type==ui::Widget::TouchEventType::ENDED)
		{
			cjMusic::playEffect("video/tap.mp3");
			GameData::getSaveData()->_huhightnum+=100;
			GameData::getSaveData()->_hunum=0;
			_hunum=0;
			GameData::getInstance()->dataSave();
			cubeReward(diamondnum,prop);
	//		husp->setRotation(0);
	//		husp->runAction(RotateTo::create(1.0f,90.0f));
			if(!_candealwith)
			{
				cjSchedule::getInstance()->runOnNextFrame([=](float ft){
					_candealwith=true;
					handleHandle();
				});
			}

			pPropChildLayer->removeFromParent();
		}
	});

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	pPropChildLayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, pPropChildLayer);

	this->addChild(pPropChildLayer,10);
	//Director::getInstance()->getRunningScene()->addChild(pPropChildLayer,100,100);
}

void StartManage::cubeReward(int num,int prop)
{
	auto diamondsp=cjSprite::createWithImage("zs.png");
	this->addChild(diamondsp,11);
	diamondsp->setPosition(Vec2(WINSIZE.width*0.35f,WINSIZE.height*0.46f));
	diamondsp->setRotation(28);
	diamondsp->setScale(1.2f);
	diamondsp->runAction(Sequence::create(MoveTo::create(0.7f,diamondpos),ScaleTo::create(0.3f,1.0f),CallFunc::create([=](){
		GameData::getSaveData()->_diamondNumber+=num;
		diamondsp->removeFromParent();
		PayScene::getInstance()->parShow(this, 10, diamondpos);

		cjMusic::playEffect("video/coinsin.mp3");
		GameData::getInstance()->dataSave();
	}),nullptr));
	diamondsp->runAction(ScaleTo::create(0.5f,1.0f));

	auto pp=getChildByName(cjTTFLabel::getNameByInt("prop%d",prop+1));
	auto propsp=cjSprite::createWithImage(cjTTFLabel::getNameByInt("prop%02d.png",prop+1));
	this->addChild(propsp,11);
	propsp->setPosition(Vec2(WINSIZE.width*0.65f,WINSIZE.height*0.46f));
	propsp->runAction(ScaleTo::create(0.7f, 1.2f*propscale));
	propsp->runAction(Sequence::create(MoveTo::create(0.7f,pp->getPosition()),ScaleTo::create(0.3f,propscale),CallFunc::create([=](){
		GameData::getSaveData()->_propnum[prop]++;
		propsp->removeFromParent();
		PayScene::getInstance()->parShow(this, 10,pp->getPosition());
		GameData::getSaveData()->_hupropid=prop+1;

		propNumShow(prop,1);
	}),nullptr));
}

//µ¿æﬂ∏ˆ ˝œ‘ æ
void StartManage::propNumShow(int prop,int key)
{
	if(key>0)
	{
		if(GameData::getSaveData()->_propnum[prop]==1)
		{
			_propnum[prop]=cjTTFLabel::createWithString(cjTTFLabel::getNameByInt("x%d",GameData::getSaveData()->_propnum[prop]));
			this->addChild(_propnum[prop],2);
			_propnum[prop]->setColor(propcolor);
			_propnum[prop]->setPosition(getChildByName(cjTTFLabel::getNameByInt("prop%d",prop+1))->getPosition()+Vec2(20,-15));
		}
		else
			_propnum[prop]->setString(cjTTFLabel::getNameByInt("x%d",GameData::getSaveData()->_propnum[prop]));
	}
	else if(key<0)
	{
		if(GameData::getSaveData()->_propnum[prop]>0)
			_propnum[prop]->setString(cjTTFLabel::getNameByInt("x%d",GameData::getSaveData()->_propnum[prop]));
		else
			_propnum[prop]->removeFromParent();
	}
}

//–°»À ’ºØ∑ΩøÈ --∑Ω∞∏“ª
//float StartManage::smallPeopleCollect(vector<pair<int,int>> vec)
//{
//	float tt=3.0f;
//	auto people=CSLoader::createNode("animation/smallpeople.csb");
//	_clip->addChild(people,21);
//	people->setPosition(-150,heightoffest-10);
//	people->runAction(Sequence::create(MoveBy::create(2.0f,Vec2(300+WINSIZE.width,0)),DelayTime::create(1.0f),CallFunc::create([=](){
//		people->removeFromParent();
//	}),nullptr));
//
//	auto peopleaction=CSLoader::createTimeline("animation/smallpeople.csb");
//	people->runAction(peopleaction);
//	peopleaction->play("action",true);
//	people->setScale(0.8f);
//
//	for(auto &j:vec)
//	{
//		auto time=DelayTime::create(0.5f+j.first*0.15f+0.03f*j.second);
//		auto call=CallFunc::create([=](){
//			auto node=_clip->getChildByTag(j.first*CHESS_SIZE_H_START+j.second);
//			node->setZOrder(20);
//			node->runAction(Sequence::create(MoveTo::create(0.55f,people->getPosition()+Vec2(160,50)),CallFunc::create([this,j](){
//				removeBox(j,2);
//			}),nullptr));
//			node->runAction(ScaleTo::create(0.55f,0.0f));
//			node->runAction(FadeOut::create(0.55f));
//		});
//		this->runAction(Sequence::create(time,call,nullptr));
//		if(tt<(0.5f+j.first*0.1f+0.03f*j.second+0.55f))
//			tt=0.5f+j.first*0.1f+0.03f*j.second+0.55f;
//	}
//
//	return tt;
//}

//“¿¥Œ±¨’® --∑Ω∞∏∂˛
float StartManage::smallPeopleCollect(vector<pair<int, int>> vec)
{
	float tt = cleardelaytime;
	Vector<FiniteTimeAction*> action;
	for (auto &pos : vec)
	{
		action.pushBack(CallFunc::create([=]() {
			addParticle(pos.first,pos.second, getChessInfo(pos), 11);
			removeBox(pos, 1);
		}));
		action.pushBack(DelayTime::create(boxremovesingletime));
		tt += boxremovesingletime;
	}
	runAction(Sequence::create(action));

	return tt;
}

//µ„ª˜Ã· æ
void StartManage::beganTip()
{
	auto hand = this->getChildByName("clickhand");
	if (hand)
	{
		hand->setVisible(false);
	}
	else
	{
		auto clickhand = handNode::createHand(0.4f);
		this->addChild(clickhand, 20, "clickhand");
		clickhand->setVisible(false);
	}

	_clicktiptime = 0;
	this->schedule(schedule_selector(StartManage::clickTipUpdate), 1.0f);
}

void StartManage::endTip()
{
	auto hand=this->getChildByName("clickhand");
	if (hand)
		hand->setVisible(false);
	this->unschedule(schedule_selector(StartManage::clickTipUpdate));
	_clicktiptime = 0;
	for (auto &i : _clicktipvec)
	{
		auto box = static_cast<StartBox*>(_clip->getChildByTag(i.first*CHESS_SIZE_H_START+i.second));
		box->setBox1();
		box->setScale(boxscale);
	}
	_clicktipvec.clear();
}

void StartManage::clickTip()
{
	_clicktipvec.clear();
	for(int i=0;i<CHESS_SIZE_W_START;i++)
		for (int j = 0; j < CHESS_SIZE_H_START; j++)
		{
			if (_chessInfo[i*CHESS_SIZE_H_START + j])
			{
				_clicktipvec = boxGet(make_pair(i,j));
				if (_clicktipvec.size() > 1)
				{
					for (auto &k : _clicktipvec)
					{
						auto box = static_cast<StartBox*>(_clip->getChildByTag(k.first*CHESS_SIZE_H_START + k.second));
						box->setBox2();
						auto hand = this->getChildByName("clickhand");
						if (hand)
						{
							hand->setPosition(posToposition(_clicktipvec[_clicktipvec.size()-1]));
							hand->setVisible(true);
						}
					}
					return;
				}
				else
					_clicktipvec.clear();
			}
		}
}

void StartManage::clickTipUpdate(float ft)
{
	_clicktiptime++;
	if (_clicktiptime == 5)
	{
		clickTip();
		this->unschedule(schedule_selector(StartManage::clickTipUpdate));
	}
}

//ª°–ŒΩ¯∂»Ãıupdate
void StartManage::huptUpdate(float ft)
{
	if (_hutempnum != _hunum)
	{
		int k = 1;
		if (abs(_hutempnum - _hunum) >= 2)
			k = 2;
		if (_hutempnum < _hunum)
			_hutempnum+=k;
		else
			_hutempnum-=k;

		_huptimer->setPercentage(35+(61* _hutempnum / GameData::getSaveData()->_huhightnum));
		auto par = this->getChildByName("par");
		par->setRotation(45.0f - (_hutempnum*1.0f / GameData::getSaveData()->_huhightnum)*200.0f);
	}
}