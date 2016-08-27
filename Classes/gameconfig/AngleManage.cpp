#include"AngleManage.h"
#include"ConfigState.h"
#include"BaseConfig.h"
#include"../cj/cjconfig.h"
#include"../cj/GameData.h"
#include "../cj/Toast.h"
#include"AngleNode.h"
#include "vigame_ad.h"
#include "vigame_pay.h"
#include "PayScene.h"
#include "cocostudio/CocoStudio.h"

using namespace cocostudio;

const int heightoffest=210;		//棋盘底的高度
const float selectboxsize=0.65f;	//选择的方块缩放
const float chessfilltime=0.1f;
const Vec2 diamondpos=Vec2(370,712);

const float eliminatetime=0.05f;

int diamondnum[2]={10,50};

const Color3B propcolor=Color3B(255,255,255);

void AngleManage::start()
{
	setGameState(STATE_RUN);
}
void AngleManage::pause()
{
	setGameState(STATE_PAUSE);
}
void AngleManage::run()
{
	setGameState(STATE_RUN);
}

bool AngleManage::init()
{
	if(!Layer::init())
		return false;

	selectboxoffest=GameDataInstance()->BANER_HEIGHT;	//可供选择方块的高度
	LOGGAME(cjTTFLabel::getNameByInt("banner: %d",selectboxoffest).c_str());

	initData();
	initChess();
	initProps();

	LevelUpMotion();

	auto listener=EventListenerTouchOneByOne::create();
	listener->onTouchBegan=CC_CALLBACK_2(AngleManage::onTouchBegan,this);
	listener->onTouchMoved=CC_CALLBACK_2(AngleManage::onTouchMoved,this);
	listener->onTouchEnded=CC_CALLBACK_2(AngleManage::onTouchEnded,this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
	listener->setSwallowTouches(true);

	return true;
}

void AngleManage::initData()
{
	for(int i=0;i<CHESS_SIZE_W_ANGLE;i++)
		for(int j=0;j<CHESS_SIZE_H_ANGLE;j++)
		{
			_chessBlack[i][j]=false;
			_chessColor[i][j]=0;
		}

	for(int i=0;i<3;i++)
		_selectExit[i]=false;
	_guide=false;
	_touchselect=0;
	_firstSaveIn=false;
	settouchState(TOUCH_TRUE);
	if(!GameData::getSaveData()->_angle._guider)
	{
		GameDataInstance()->_guide=true;
		_guide=true;
		setGameState(STATE_STOP);
	}
	bool tempbool=false;
	if(GameData::getSaveData()->_angle._istip)
		tempbool=true;

	if(tempbool)
	{
		sdata=GameData::getSaveData()->_angle;
		_firstSaveIn=true;
		setmLevel(sdata._level);
		setmScore(sdata._score);
		setmTarget(sdata._target);
	}
	else
	{
		setmLevel(1);
		setmScore(0);
		setmTarget(500);
	}
	startNewRound();
}

void AngleManage::initChess()
{
	//底
	for(int i=0;i<CHESS_SIZE_W_ANGLE;i++)
		for(int j=0;j<CHESS_SIZE_H_ANGLE;j++)
		{
			bool temp=false;
			if(i<5)
			{
				int k=4+i;
				if(j<=k)
					temp=true;
			}
			else
			{
				int k=i-4;
				if(j>=k)
					temp=true;
			}
			if(temp)
			{
				auto sp=cjSprite::createWithImage("angle_back.png");
				this->addChild(sp);
				sp->setPosition(posToposition(make_pair(i,j)));

				_chessBlack[i][j]=true;

				//char name[100];
				//auto position=posToposition(make_pair(i,j));
				//sprintf(name,"<%d,%d>,(%f,%f)",i,j,position.x,position.y);
				//CCLOG(name);
			}
		}
	//
	if(_firstSaveIn)
	{
		for(int i=0;i<CHESS_SIZE_W_ANGLE;i++)
			for(int j=0;j<CHESS_SIZE_H_ANGLE;j++)
			{
				int key=sdata._chessInfo[i][j];
				if(key>0)
				{
					char name[30];
					sprintf(name,"angle_%d.png",key/2+1);
					auto box=cjSprite::createWithImage(name);
					this->addChild(box,CHESSMAN_LEVEL,i*CHESS_SIZE_H_ANGLE+j);
					box->setPosition(posToposition(make_pair(i,j)));
					box->setScale(CHESSMAN_SCALE_ANGLE);
					_chessColor[i][j]=key;
				}
			}
		_firstSaveIn=false;
	}
	else
	{
		int k=rand()%10+1;
		if(_guide)
		{
			_chessColor[0][1]=3;
			_chessColor[4][1]=3;
			_chessColor[5][1]=3;
		}	
		else
			firstSet(k);
	
		
		for(int i=0;i<CHESS_SIZE_W_ANGLE;i++)
			for(int j=0;j<CHESS_SIZE_H_ANGLE;j++)
			{
				if(_chessColor[i][j])
				{
					char name[30];
					sprintf(name,"angle_%d.png",_chessColor[i][j]/2+1);
					auto box=cjSprite::createWithImage(name);
					this->addChild(box,CHESSMAN_LEVEL,i*CHESS_SIZE_H_ANGLE+j);
					box->setPosition(posToposition(make_pair(i,j)));
					box->setScale(CHESSMAN_SCALE_ANGLE);
				}
			}
	}
}

////过关分数
//int AngleManage::getTargetScoreByRound(int round)
//{
//	int ret=getmTarget();
//	if(round==1)
//		ret+=500;
//	else if(round<5)
//		ret+=1000;
//	else if(round<8)
//		ret+=1500;
//	else if(round<11)
//		ret+=2000;
//	else if(round<16)
//		ret+=2500;
//	else if(round<21)
//		ret+=3000;
//	else if(round<31)
//		ret+=3500;
//	else if(round<41)
//		ret+=4000;
//	else if(round<51)
//		ret+=4500;
//	else 
//		ret+=5000;
//}
//
////消除棋子分数
//int AngleManage::getBoxScoreByRound(int round)
//{
//	if(round==1)
//		return 10;
//	else if(round<5)
//		return 20;
//	else if(round<8)
//		return 30;
//	else if(round<11)
//		return 50;
//	else if(round<16)
//		return 100;
//	else if(round<21)
//		return 150;
//	else if(round<31)
//		return 200;
//	else if(round<41)
//		return 250;
//	else if(round<51)
//		return 300;
//	else 
//		return 350;
//}

//初始化道具
void AngleManage::initProps()
{
	_selectprop=0;

	//hongbao
	auto hongbao=cjMenu::createWithImage("wf.png",[this](Ref *mu){
		//PayScene::getInstance()->hongbao(false);
		if(getGameState()==STATE_RUN&&!_guide)
		{
			if((GameData::getSaveData()->_hongbaotime==0)&&(!(Label *)getChildByName("hongbaofnt")->isVisible()))
				hongbaoPage();
			else
			{
				_hongbaotip->stopAllActions();
				_hongbaotip->setVisible(true);
				_hongbaotip->runAction(Sequence::create(DelayTime::create(2.0f),CallFunc::create([=](){
					_hongbaotip->setVisible(false);
				}),nullptr));
			}
		}
	});
	this->addChild(hongbao,1,"hongbaomenu");
	hongbao->setAnchorPoint(Vec2(0.0f,0.0f));
	hongbao->setPosition(Vec2(60,700));

	char hbname[50];
	if(GameData::getSaveData()->_hongbaotime!=0)
		sprintf(hbname,"%02d:%02d",GameData::getSaveData()->_hongbaotime/60,GameData::getSaveData()->_hongbaotime%60);
	else
		sprintf(hbname,"00:00");
	auto hongbaofnt=cjTTFLabel::createWithString(hbname);
	this->addChild(hongbaofnt,1,"hongbaofnt");
	hongbaofnt->setColor(Color3B::YELLOW);
	hongbaofnt->setPosition(hongbao->getPosition()-Vec2(0,50));

	if(GameData::getSaveData()->_hongbaotime==0)
	{
		hongbaofnt->setVisible(false);
		auto hand=handNode::createHand(0.4f);
		this->addChild(hand,1,"hbhand");
		hand->setPosition(hongbaofnt->getPosition()+Vec2(30,70));
	}

	_hongbaotip=LabelTTF::create(cjTTFLabel::getStringByKey("hongbaotip"),TTF_PATH,20);
	this->addChild(_hongbaotip,1);
	_hongbaotip->setPosition(Vec2(WINSIZE.width/2,665));
	_hongbaotip->setVisible(false);
	_hongbaotip->setColor(Color3B::YELLOW);

	auto str=cjTTFLabel::getStringByKey("diamond");

	string name;
	for(int i=0;i<2;i++)
	{
		if(i==0)
			name="prop03.png";
		else if(i==1)
			name="prop04.png";
		auto sp=cjMenu::createWithImage(name.c_str(),[i,this](Ref *rf){
			if(_touchselect==0)
			{
				if((GameData::getSaveData()->_diamondNumber>=diamondnum[i])||(GameData::getSaveData()->_propnum[i+2]>0))
				{
					propMotion(i+1);
				}
				else
				{
#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS|| IOSMODE)
					PayScene::getInstance()->freeDiamond();
#else
					if (GameDataInstance()->d_PayType&&vigame::pay::PayManager::getDefaultFeeInfo() && vigame::pay::PayManager::getDefaultFeeInfo()->getFeePriceByID(1))
						PayScene::getInstance()->payPage1();
					else
						Toast::makeText("PAY_RESULT_FAIL")->show();
#endif
				}
			}
		});
		this->addChild(sp,1,cjTTFLabel::getNameByInt("prop%d",i+1));
		sp->setAnchorPoint(Vec2(0.0f,0.0f));

		int propheight=250;
		int propwidth=35;

		if(i==0)
			sp->setPosition(Vec2(propwidth,propheight));
		else if(i==1)
		{
			sp->setPosition(Vec2(WINSIZE.width-propwidth,propheight));

			auto node=Node::create();
			this->addChild(node,1,"node");
			node->setAnchorPoint(Vec2(0.5f,0.5f));
			node->setPosition(sp->getPosition());
			node->setVisible(false);
			for(int i=1;i<3;i++)
			{
				auto tt=cjSprite::createWithImage(cjTTFLabel::getNameByInt("prop4_%02d.png",i));
				node->addChild(tt,1,cjTTFLabel::getNameByInt("prop4-%02d",i));
				tt->setPosition(node->getContentSize()/2.0f);
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

		if(_guide)
			sp->setEnabled(false);

		string str = "zuan_10.png";
		if (i == 1)
			str = "zuan_50.png";
		auto price = cjSprite::createWithImage(str);
		this->addChild(price, 1);
		price->setPosition(sp->getPosition() - Vec2(0, 40));

		if(GameData::getSaveData()->_propnum[2+i]>0)
		{
			_propnum[i]=cjTTFLabel::createWithString(cjTTFLabel::getNameByInt("x%d",GameData::getSaveData()->_propnum[2+i]));
			this->addChild(_propnum[i],2);
			_propnum[i]->setColor(propcolor);
			_propnum[i]->setPosition(sp->getPosition()+Vec2(20,-15));
		}
	}
	schedule(schedule_selector(AngleManage::hongbaoFlush),1.0f);
}

void AngleManage::hongbaoFlush(float ft)
{
	auto hongbaofont=(Label *)getChildByName("hongbaofnt");
	char hbname[30];
	if(GameData::getSaveData()->_hongbaotime==0&&hongbaofont&&hongbaofont->isVisible())
	{
		auto menu=getChildByName("hongbaomenu");
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
		this->addChild(hand,1,"hbhand");
		hand->setPosition(hongbaofont->getPosition()+Vec2(30,70));
	}
	else if(GameData::getSaveData()->_hongbaotime>0)
	{
		GameData::getSaveData()->_hongbaotime--;
		sprintf(hbname,"%02d:%02d",GameData::getSaveData()->_hongbaotime/60,GameData::getSaveData()->_hongbaotime%60);
		hongbaofont->setString(hbname);
	}
}

void AngleManage::propMotion(int id)
{
	if(id==1)
	{
		cjMusic::playEffect("video/qiehuan.wav");

		auto prop = getChildByName("prop1");
		prop->setVisible(false);
		auto bombcsb = CSLoader::createNode("animation/prop_status_reset.csb");
		this->addChild(bombcsb, 10);
		bombcsb->setPosition(getChildByName("prop1")->getPosition());
		auto action = CSLoader::createTimeline("animation/prop_status_reset.csb");
		action->gotoFrameAndPlay(0, false);
		bombcsb->runAction(action);
		bombcsb->runAction(Sequence::create(DelayTime::create(0.4f), CallFunc::create([bombcsb, prop]() {
			bombcsb->removeFromParent();
			prop->setVisible(true);
		}), nullptr));

		Rotate(2);
		if(GameData::getSaveData()->_propnum[2]>0)
		{
			GameData::getSaveData()->_propnum[2]--;
			if(GameData::getSaveData()->_propnum[2]==0)
				_propnum[0]->setVisible(false);
			else
			{
				_propnum[0]->setString(cjTTFLabel::getNameByInt("x%d",GameData::getSaveData()->_propnum[2]));
			}
		}
		else
			GameData::getSaveData()->_diamondNumber-=diamondnum[0];
		GameData::getInstance()->dataSave();
	}
	else if(id==2)
	{
		auto mu=getChildByName("prop2");
		auto node=getChildByName("node");
		if(_selectprop==2)
		{
			_selectprop=0;
			mu->setOpacity(255);
			node->setVisible(false);
		}
		else
		{
			_selectprop=2;
			mu->setOpacity(0);
			node->setVisible(true);
		}
	}
}

void AngleManage::Rotate(int key)	//重现给出选择box
{
	if(key==1)
	{
		if(gettouchState()==TOUCH_FALSE||_touchselect>0)
			return;
	}
	for(int i=0;i<3;i++)
	{
		removeChildByTag(1000+i);
		_selectExit[i]=false;
	}

	startNewRound(key);
	if(key==1)
		BoxFillJudge();

	dataSave();
}

void AngleManage::Resurrection()
{
	for(int i=0;i<CHESS_SIZE_W_ANGLE;i++)
		for(int j=0;j<CHESS_SIZE_H_ANGLE;j++)
		{
			if(_chessColor[i][j])
			{
				auto box=getChildByTag(i*CHESS_SIZE_H_ANGLE+j);
				box->setVisible(true);

				removeChildByTag(1000+i*CHESS_SIZE_H_ANGLE+j);
			}
		}

		vector<pair<int,int>> selectvec;
		for(int i=0;i<CHESS_SIZE_W_ANGLE;i++)
			for(int j=0;j<CHESS_SIZE_H_ANGLE;j++)
			{
				if(_chessColor[i][j])
					selectvec.push_back(make_pair(i,j));
			}

			vector<pair<int,int>> eliminatebox;
			int number=MIN(6,selectvec.size());
			for(int i=0;i<number;i++)
			{
				int num=rand()%selectvec.size();
				eliminatebox.push_back(selectvec[num]);
				selectvec.erase(selectvec.begin()+num);
			}

			if(eliminatebox.size()>0)
			{
				cjSchedule::getInstance()->shockBegin(this);
				runAction(Sequence::create(DelayTime::create(0.25f*(eliminatebox.size()-1)),CallFunc::create([this](){
					cjSchedule::getInstance()->shockEnd(this);
				}),nullptr));
			}

			levelupmusic=0;
			int key=0;
			for(auto &i:eliminatebox)
			{
				auto pos=i;
				_chessColor[pos.first][pos.second]=0;
				runAction(Sequence::create(DelayTime::create(0.25f*MIN(3,key)),CallFunc::create([this,pos,key](){

					addParticle(pos.first,pos.second,_chessColor[pos.first][pos.second],2);
					removeBox(pos);
					if(key<=3)
					{
						char musicname[30];
						if(key<3)
						{
							sprintf(musicname,"video/levelupbegin%d.mp3",levelupmusic%4);
							++levelupmusic;
						}
						else if(key==3)
							sprintf(musicname,"video/levelupend.mp3");
						cjMusic::playEffect(musicname);
					}
				}),nullptr));
				key++;
			}

			setGameState(STATE_RUN);
			Rotate(2);
			runAction(Sequence::create(DelayTime::create(1.5f),CallFunc::create([this](){
				BoxFillJudge();
				settouchState(TOUCH_TRUE);
			}),nullptr));
}

Vec2 AngleManage::posToposition(pair<int,int> pos)
{
	int px=pos.first+(2-(pos.second+1)/2);
	int py=pos.second;

	float x=WINSIZE.width/2.0f+(px-(CHESS_SIZE_W_ANGLE/2.0f-0.5f))*CHESSMAN_SIZE_W_ANGLE;
	float y=(py+0.5f)*CHESSMAN_SIZE_H_ANGLE+heightoffest;
	if(pos.second%2>0)
		x+=CHESSMAN_SIZE_W_ANGLE/2.0f;
	return Vec2(x,y);
}

pair<int,int> AngleManage::positionTopos(Vec2 position)
{
	float y=float(position.y-heightoffest)/CHESSMAN_SIZE_H_ANGLE;
	float x=position.x;
	if(y<0)
		y=-1;
	int py=int(y);
	if(py%2>0)
		x-=CHESSMAN_SIZE_W_ANGLE/2.0f;
	float tx=(x-WINSIZE.width/2.0f)/CHESSMAN_SIZE_W_ANGLE+(CHESS_SIZE_W_ANGLE/2.0f-0.5f);
	int ttx=int(tx+0.5f);
	int px=ttx-(2-(py+1)/2);
	return make_pair(px,py);
}

void AngleManage::startNewRound(int key)
{
	for(int i=0;i<3;i++)
	{
		if(!_selectExit[i])
		{
			bool temp=false;
			for(int j=i+1;j<3;j++)
			{
				if(_selectExit[j])
				{
					temp=true;
					auto box=this->getChildByTag(1000+j);
					box->runAction(CCMoveBy::create(startnexttime,Vec2(-nextboxoffest*(j-i),0)));
					_selectExit[i]=true;
					_selectExit[j]=false;
					box->setTag(1000+i);
					temp=true;
					break;
				}
			}
			if(!temp)
			{
				int shape=1;
				if(getmScore()==0||key==2)
				{
					if(i==0)
						shape=1;
					else if(i==1)
						shape=rand()%2+2;
					else if(i==2)
						shape=rand()%3+4;
				}
				else if(getmLevel()==1)
					shape=rand()%6+1;
				else
					shape=rand()%17+1;

				if(_guide&&i==1)
				{
					shape=6;
				}
				else if(_firstSaveIn)
				{
					shape=(ShapeIndex)sdata._boxshape[i];
				}

				auto selectBox=AngleNode::createNode(shape);
				selectBox->setScale(selectboxsize);
				this->addChild(selectBox,11,1000+i);
				selectBox->setPosition(selectBox->getCenterAnPointX(Vec2(nextboxbegin+nextboxoffest*i,selectboxoffest))+Vec2(WINSIZE.width,0));
				//selectBox->setPosition(Vec2(nextboxbegin+nextboxoffest*i,selectboxoffest)+Vec2(WINSIZE.width,0));
				selectBox->runAction(CCMoveBy::create(startnexttime,Vec2(-WINSIZE.width,0)));
				_selectExit[i]=true;
			}
		}
	}

	if(_guide)
	{
		runAction(Sequence::create(DelayTime::create(2.0),CallFunc::create([this](){
			dragGuide();
		}),nullptr));
	}
}

bool AngleManage::onTouchBegan(Touch *touch, Event *unused_event)
{
	if(getGameState()!=STATE_RUN||gettouchState()==TOUCH_FALSE||_touchselect>0)
		return false;

	_touchselect=0;
	auto startpos=touch->getLocation(); 
	if(_selectprop>0)
	{
		auto pos=positionTopos(startpos);
		if(pos.first>=0&&pos.first<CHESS_SIZE_W_ANGLE&&pos.second>=0&&pos.second<CHESS_SIZE_H_ANGLE&&_chessColor[pos.first][pos.second])
		{
			useProp2(pos);
		}
		return false;
	}
	else if(startpos.y>0&&startpos.y<heightoffest&&startpos.x)
	{
		int k=((startpos.x-nextboxbegin)/nextboxoffest+0.5f);
		if(k>=0&&k<3&&_selectExit[k])
		{
			if(_guide)
			{
				if(k==1)
				{
					auto lay=Director::getInstance()->getRunningScene()->getChildByName("bclayer");
					if(lay)
						lay->setVisible(false);
					else
						return false;
				}
				else
					return false;
			}
			_touchselect=k+1;
			auto box=(AngleNode *)this->getChildByTag(1000+k);
			if(box->_boxcolor==BoxColor::BOX_NORMAL)
			{
				box->stopAllActions();
				box->setPosition(box->getCenterAnPointX(startpos)+Vec2(0,boxtofiger));
				box->setScale(1.07f);
				return true;
			}
			else
			{
				_touchselect=0;
				return false;
			}
		}
	}
	return false;
}
void AngleManage::onTouchMoved(Touch *touch, Event *unused_event)
{
	if(_touchselect<=0||gettouchState()==TOUCH_FALSE)
		return;

	auto recentpos=touch->getLocation();

	auto selectbox=(AngleNode*)this->getChildByTag(1000+_touchselect-1);
	selectbox->setPosition(selectbox->getCenterAnPointX(recentpos)+Vec2(0,boxtofiger));

	showTipBox(selectbox);
}
void AngleManage::onTouchEnded(Touch *touch, Event *unused_event)
{
	if(_touchselect<=0||gettouchState()==TOUCH_FALSE)
		return;
	removeTipBox();
	settouchState(TOUCH_FALSE);
	auto box=(AngleNode *)this->getChildByTag(1000+_touchselect-1);
	assert(box);
	auto fillvec=getFillBlack(box,box->getPosition());
	if(fillvec.empty())
	{
		if(_guide)
		{
			auto lay=Director::getInstance()->getRunningScene()->getChildByName("bclayer");
			lay->setVisible(true);
		}

		//放置失败
		cjMusic::playEffect("video/dropwrong-drag.mp3");
		box->setScale(selectboxsize);
		box->runAction(Sequence::create(CCMoveTo::create(0.1f,box->getCenterAnPointX(Vec2(nextboxbegin+nextboxoffest*(_touchselect-1),selectboxoffest))),CallFunc::create([this](){
			_touchselect=0;
			settouchState(TOUCH_TRUE);
		}),nullptr));
		//box->runAction(CCScaleTo::create(0.1f,0.6f));
	}
	else
	{
		if(_guide)
		{
			auto lay=Director::getInstance()->getRunningScene()->getChildByName("bclayer");
			lay->removeFromParent();
			_guide=false;
			GameDataInstance()->_guide=false;
			GameData::getSaveData()->_angle._guider=true;
			GameData::getInstance()->dataSave();

			auto mu1=(Menu*)getChildByName("prop1");
			mu1->setEnabled(true);
			auto mu2=(Menu*)getChildByName("prop2");
			mu2->setEnabled(true);
		}

		//放置成功
		cjMusic::playEffect("video/drop-drag.mp3");
		Vector<FiniteTimeAction*> action;
		action.pushBack(MoveTo::create(0.01f,posToposition(fillvec[0])));
		action.pushBack(CallFunc::create([this,fillvec,box](){
			box->setVisible(false);
			_selectExit[_touchselect-1]=false;
			startNewRound();
			_touchselect=0;
			chessFill(box->_id,fillvec);
		}));
		action.pushBack(DelayTime::create(chessfilltime));
		action.pushBack(CallFunc::create([this,box](){
			box->removeFromParent();
			float time=chessfilltime;
			if (!chessCheck())
			{
				runAction(Sequence::create(DelayTime::create(time), CallFunc::create([this]() {
					BoxFillJudge();
					faildJudge();
				}), nullptr));
			}
		}));
		box->runAction(Sequence::create(action));
	}
}

void AngleManage::useProp2(pair<int,int> pos)
{
	if(GameData::getSaveData()->_propnum[3]>0)
	{
		GameData::getSaveData()->_propnum[3]--;
		if(GameData::getSaveData()->_propnum[3]==0)
			_propnum[1]->setVisible(false);
		else
		{
			_propnum[1]->setString(cjTTFLabel::getNameByInt("x%d",GameData::getSaveData()->_propnum[3]));
		}
	}
	else
		GameData::getSaveData()->_diamondNumber-=diamondnum[1];


	_selectprop=0;
	setGameState(STATE_STOP);
	int color=_chessColor[pos.first][pos.second]/2+1;
	vector<pair<int,int>> vec;
	vec.push_back(pos);
	for(int i=0;i<CHESS_SIZE_W_ANGLE;i++)
		for(int j=0;j<CHESS_SIZE_H_ANGLE;j++)
		{
			if(_chessColor[i][j]>0&&(_chessColor[i][j]/2+1)==color&&(make_pair(i,j)!=pos))
				vec.push_back(make_pair(i,j));
		}

		int sco=vec.size()*CHESS_SCORE;
		_score+=sco;
		scoreMotion(sco,vec[0]);

		LayerColor* pPropChildLayer = LayerColor::create(Color4B(0, 0, 0, 255*OPA_BEIJING));

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

		if(vec.size()>0)
		{
			cjSchedule::getInstance()->shockBegin(this);
			for(auto &i:vec)
			{
				auto box=getChildByTag(i.first*CHESS_SIZE_H_ANGLE+i.second);
				box->setZOrder(100);
			}
		}

		_shandianid=cjMusic::playEffect("video/shandian.mp3",true);

		int cleanNo = 0;
		pair<int,int> pStar1;
		pair<int,int> pStar2;
		Vector<FiniteTimeAction*> arrayOfActions;
		do
		{
			if (vec.empty())
			{
				arrayOfActions.pushBack(DelayTime::create(0.1f));
				arrayOfActions.pushBack(CallFunc::create([=](){
					pPropChildLayer->removeFromParent();
					GameData::getInstance()->dataSave();
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

			vec.erase(std::find(vec.begin(),vec.end(),pStar1));
			if (vec.size() > 0)
			{
				pStar2 = vec[rand() % vec.size()];
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
				this->addChild(particleNodes, 101);

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
					//std::string particleName = "lizi/" + colorName + "boom.plist";
					//ParticleSystemQuad* m_emitter = ParticleSystemQuad::create(particleName);
					//m_emitter->setPosition(position1);
					//m_emitter->setAutoRemoveOnFinish(true);
					//particleNodes->addChild(m_emitter, 3);

					addParticle(pStar1.first,pStar1.second,_chessColor[pStar1.first][pStar1.second],1);
					removeBox(pStar1);
				});

				CallFunc* playParticle2 = CallFunc::create([=](){
					//auto propSkillNode = CSLoader::createNode("animation/xiaochu.csb");
					//pPropChildLayer->addChild(propSkillNode, 3);
					//propSkillNode->setPosition(posToposition(pStar1));
					//auto propSkillAction=CSLoader::createTimeline("animation/xiaochu.csb");
					//propSkillNode->runAction(propSkillAction);
					//propSkillAction->gotoFrameAndPlay(0,26,false);
					//propSkillNode->runAction(Sequence::create(DelayTime::create(0.5f),CallFunc::create([propSkillNode](){
					//	propSkillNode->removeFromParent();
					//}),nullptr));
				});

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
					//propSkillNode->setScaleY(1.2f);
					this->addChild(propSkillNode, 101);

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
			if(getGameState()==STATE_STOP)
				setGameState(STATE_RUN);

			cjMusic::stopEffect(_shandianid);

			cjSchedule::getInstance()->shockEnd(this);

			auto mu=getChildByName("prop2");
			mu->setOpacity(255);
			auto node=getChildByName("node");
			node->setVisible(false);
		}));

		pPropChildLayer->runAction(Sequence::create(arrayOfActions));

		auto touchListener = EventListenerTouchOneByOne::create();
		touchListener->setSwallowTouches(true);
		touchListener->onTouchBegan = [](Touch* touch, Event* event){
			return true;
		};
		pPropChildLayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, pPropChildLayer);

		this->addChild(pPropChildLayer, 12);
}

//填充方块
void AngleManage::chessFill(int shape,vector<pair<int,int>> vec)
{
	int score=_score;
	for(auto &i:vec)
	{
		char name[30];
		sprintf(name,"angle_%d.png",shape/2+1);
		auto box=cjSprite::createWithImage(name);
		this->addChild(box,CHESSMAN_LEVEL,i.first*CHESS_SIZE_H_ANGLE+i.second);
		box->setPosition(posToposition(i));
		box->setScale(CHESSMAN_SCALE_ANGLE);
		_chessColor[i.first][i.second]=shape;
		box->runAction(Sequence::create(ScaleTo::create(0.03f,0.97f*CHESSMAN_SCALE_ANGLE),ScaleTo::create(0.03f,1.03f*CHESSMAN_SCALE_ANGLE),ScaleTo::create(0.03f,1.0f*CHESSMAN_SCALE_ANGLE),nullptr));
		_score+=CHESS_SCORE;
	}
	this->runAction(Sequence::create(DelayTime::create(0.2f),CallFunc::create([score,this,vec](){
		scoreMotion(_score-score,vec[0]);
	}),nullptr));
}

void AngleManage::showTipBox(AngleNode *box)	//显示提示的方块
{
	auto fillvec=getFillBlack(box,box->getPosition());
	if(!fillvec.empty())
	{
		if(fillvec[0]!=_chesstippos)
		{
			removeChildByTag(2000);
			_chesstippos=fillvec[0];
			auto node=Node::create();
			addChild(node,CHESSMAN_LEVEL,2000);
			char name[30];
			sprintf(name,"angle_%d.png",box->_id/2+1);
			for(auto &k:fillvec)
			{
				auto sp=cjSprite::createWithImage(name);
				node->addChild(sp);
				sp->setOpacity(120);
				sp->setPosition(posToposition(k));
			}
		}
	}
	else
	{
		removeChildByTag(2000);
		_chesstippos=positionTopos(box->getPosition());
	}
}

//分数动画
void AngleManage::scoreMotion(int score,pair<int,int> pt)
{
	if(score<0)
		return;
	auto scorefnt=cjAtlasLabel::createWithInt(score);
	this->addChild(scorefnt,15);
	scorefnt->setScale(2.0f);
	scorefnt->setPosition(posToposition(pt)+Vec2(-scorefnt->getContentSize().width,-20));
	scorefnt->runAction(Sequence::create(MoveTo::create(0.6f,WINORIGIN+ANGLE_SCORE_PT),CallFunc::create([scorefnt,score,this](){
		scorefnt->removeFromParent();
	}),nullptr));
	scorefnt->runAction(ScaleTo::create(0.6f,1.2f));
}

void AngleManage::removeTipBox()	//移除提示的方块
{
	this->removeChildByTag(2000);
	_chesstippos=make_pair(-1,-1);
}

vector<pair<int,int>> AngleManage::getFillBlack(AngleNode* box,Vec2 pt)
{
	vector<pair<int,int>> vec;
	auto centerpos=positionTopos(pt);
	int centerx=centerpos.first;
	int centery=centerpos.second;
	if(centerx<0||centery<0||centerx>float(CHESS_SIZE_W_ANGLE)||centery>float(CHESS_SIZE_H_ANGLE))
	{
		//放置失败
	}
	else
	{
		auto fillvec=box->_boxfill;
		for(auto &i:fillvec)
		{
			i.first+=centerx;
			i.second+=centery;
			if(i.first<0||i.first>=CHESS_SIZE_W_ANGLE||i.second<0||i.second>=CHESS_SIZE_H_ANGLE||_chessColor[i.first][i.second]||!_chessBlack[i.first][i.second])
			{
				vec.clear();
				break;
			}
			else
			{
				vec.push_back(make_pair(i.first,i.second));
			}
		}
	}
	return vec;
}

//方块可填充判断
void AngleManage::BoxFillJudge()
{
	for(int i=0;i<3;i++)
	{
		if(_selectExit[i])
		{
			auto box=(AngleNode*)getChildByTag(1000+i);
			if(BoxJudge(box))
				box->setNormal();
			else
				box->setGray();
		}
	}
}

//单独判断
bool AngleManage::BoxJudge(AngleNode *node)
{
	auto fillvec=node->_boxfill;
	for(int i=0;i<CHESS_SIZE_W_ANGLE;i++)
		for(int j=0;j<CHESS_SIZE_H_ANGLE;j++)
		{
			bool temp=true;
			for(auto &k:fillvec)
			{
				int kx=k.first+i;
				int ky=k.second+j;
				if(kx>=0&&kx<CHESS_SIZE_W_ANGLE&&ky>=0&&ky<CHESS_SIZE_H_ANGLE)
				{
					if(_chessColor[kx][ky]||!_chessBlack[kx][ky])
					{
						temp=false;
						break;
					}
				}
				else
				{
					temp=false;
					break;
				}
			}
			if(temp)
				return true;
		}
		return false;
}

bool AngleManage::chessCheck()
{
	std::vector<int> eliminatex;
	std::vector<int> eliminatey;
	std::vector<int> eliminatez;
	for(int i=0;i<CHESS_SIZE_W_ANGLE;i++)
	{
		bool temp=true;
		for(int j=0;j<CHESS_SIZE_H_ANGLE;j++)
		{
			if((_chessColor[i][j]==0)&&_chessBlack[i][j])
				temp=false;
		}
		if(temp)
			eliminatex.push_back(i);
	}
	for(int j=0;j<CHESS_SIZE_H_ANGLE;j++)
	{
		bool temp=true;
		for(int i=0;i<CHESS_SIZE_W_ANGLE;i++)
		{
			if((_chessColor[i][j]==0)&&_chessBlack[i][j])
				temp=false;
		}
		if(temp)
			eliminatey.push_back(j);
	}
	for(int i=-4;i<=4;i++)
	{
		bool temp=true;
		for(int j=0;j<9;j++)
		{
			if((i+j>=0)&&(i+j<CHESS_SIZE_W_ANGLE)&&(_chessColor[i+j][j]==0)&&_chessBlack[i+j][j])
				temp=false;
		}
		if(temp)
			eliminatez.push_back(i);
	}

	if(!eliminatex.empty()||!eliminatey.empty()||!eliminatez.empty())
	{
		eliminateAnimation(eliminatex,eliminatey,eliminatez);
		return true;
	}
	else
		return false;
}

//消除动画
void AngleManage::eliminateAnimation(const std::vector<int> &vx, const std::vector<int> &vy, const std::vector<int> &vz)
{
	int orign = _score;
	Vector<FiniteTimeAction*> action;
	if (!vx.empty())
	{
		for (auto &i : vx)
		{
			for (int j = 0; j < CHESS_SIZE_H_ANGLE; j++)
			{
				if (_chessColor[i][j])
				{
					if (j != 0)
						action.pushBack(DelayTime::create(eliminatetime));
					action.pushBack(CallFunc::create([this, i, j]() {
						addParticle(i, j, _chessColor[i][j], 1);
						removeBox(make_pair(i, j));
					}));
					_score += CHESS_SCORE;
				}
			}
		}
	}
	if (!vy.empty())
	{
		for (auto &j : vy)
		{
			for (int i = 0; i < CHESS_SIZE_W_ANGLE; i++)
			{
				if (std::find(vx.begin(), vx.end(), i) == vx.end())
				{
					if (_chessColor[i][j])
					{
						if (i != 0)
							action.pushBack(DelayTime::create(eliminatetime));
						action.pushBack(CallFunc::create([this, i, j]() {
							addParticle(i, j, _chessColor[i][j], 1);
							removeBox(make_pair(i, j));
						}));
						_score += CHESS_SCORE;
					}
				}
			}
		}
	}
	if (!vz.empty())
	{
		for (auto &z : vz)
		{
			for (int k = 0; k < 9; k++)
			{
				if (std::find(vx.begin(), vx.end(), z + k) == vx.end() && std::find(vy.begin(), vy.end(), k) == vy.end())
				{
					if ((z + k) >= 0 && (z + k) < CHESS_SIZE_W_ANGLE&&_chessColor[z + k][k])
					{
						if (k != 0)
							action.pushBack(DelayTime::create(eliminatetime));
						action.pushBack(CallFunc::create([this, z, k]() {
							addParticle(z + k, k, _chessColor[z + k][k], 1);
							removeBox(make_pair(z + k, k));
						}));
						_score += CHESS_SCORE;
					}
				}
			}
		}
	}
	action.pushBack(CallFunc::create([=]() {
		runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create([this]() {
			BoxFillJudge();
			faildJudge();
		}), nullptr));
	}));
	if(action.size()>0)
		runAction(Sequence::create(action));
	int kkk=vx.size()+vy.size()+vz.size();
	if(kkk>0)
	{
		cjMusic::playEffect("video/eliminate.mp3");

		if(_score<_target||(_score-orign)>=_target)
		{
			if(kkk==2)
			{
				spriteShow("zan.png");
				cjMusic::playEffect("video/zan.mp3");
			}
			else if(kkk>2)
			{
				spriteShow("great.png");
				cjMusic::playEffect("video/great.mp3");
			}
		}
	}
}

void AngleManage::addParticle(int x,int y,int shapeid,int type)
{
	ArmatureDataManager::getInstance()->addArmatureFileInfo("lizi/liujiao/xiaochu.ExportJson");
	auto armature=Armature::create("xiaochu");
	if(type==2)
		armature->getAnimation()->play("single_boom");
	else
		armature->getAnimation()->play("lizi_boom");
	this->addChild(armature,12);
	armature->setPosition(posToposition(make_pair(x,y)));
	runAction(Sequence::create(DelayTime::create(1.0f),CallFunc::create([armature](){
		armature->removeFromParent();
		ArmatureDataManager::getInstance()->removeArmatureFileInfo("lizi/liujiao/xiaochu.ExportJson");
	}),nullptr));

	auto color=angleColor[shapeid/2];

	Bone* bone = armature->getBone("Layer24");
	DecorativeDisplay* pdd = dynamic_cast<DecorativeDisplay*>(bone->getDisplayManager()->getDecorativeDisplayByIndex(0));
	if (pdd){
		if (pdd->getDisplay()){
			ParticleSystem*pParticle = dynamic_cast<ParticleSystem*>(pdd->getDisplay());
			pParticle->setStartColor(Color4F(color.r/255.0f,color.g/255.0f,color.b/255.0f,1.0f));
			pParticle->setEndColor(Color4F(color.r/255.0f,color.g/255.0f,color.b/255.0f,1.0f));
			pParticle->setAutoRemoveOnFinish(true);
		}
	}
	Bone* bone1 = armature->getBone("Layer18");
	DecorativeDisplay* pdd1 = dynamic_cast<DecorativeDisplay*>(bone1->getDisplayManager()->getDecorativeDisplayByIndex(0));
	if (pdd1){
		if (pdd1->getDisplay()){
			ParticleSystem*pParticle = dynamic_cast<ParticleSystem*>(pdd1->getDisplay());
			pParticle->setStartColor(Color4F(color.r/255.0f,color.g/255.0f,color.b/255.0f,1.0f));
			pParticle->setEndColor(Color4F(color.r/255.0f,color.g/255.0f,color.b/255.0f,1.0f));
			pParticle->setAutoRemoveOnFinish(true);
		}
	}
}

void AngleManage::removeBox(pair<int,int> pos)
{
	removeChildByTag(pos.first*CHESS_SIZE_H_ANGLE+pos.second);
	_chessColor[pos.first][pos.second]=0;
}

bool AngleManage::faildJudge()
{
	if(chessFailedJudge())
	{
		runAction(Sequence::create(DelayTime::create(0.5f),CallFunc::create([this](){
			gameFaild();
		}),nullptr));
		dataSave(false);
		return true;
	}
	else
	{
		if(!levelUp())
		{
			dataSave();
			settouchState(TOUCH_TRUE);
		}
	}
	return false;
}

bool AngleManage::chessFailedJudge()	//失败判断
{
	for(int i=0;i<3;i++)
	{
		if(_selectExit[i])
		{
			auto box=(AngleNode*)this->getChildByTag(1000+i);
			if(box->_boxcolor==BoxColor::BOX_NORMAL)
				return false;
		}
	}
	return true;
}

void AngleManage::gameFaild()
{
	for(int i=0;i<CHESS_SIZE_W_ANGLE;i++)
		for(int j=0;j<CHESS_SIZE_H_ANGLE;j++)
		{
			if(_chessColor[i][j])
			{
				runAction(Sequence::create(DelayTime::create(j*boxgraytime),CallFunc::create([i,j,this](){
					auto box=getChildByTag(i*CHESS_SIZE_H_ANGLE+j);
					box->setVisible(false);

					auto tempbox=cjSprite::createWithImage("angle_hs.png");
					addChild(tempbox,CHESSMAN_LEVEL,1000+i*CHESS_SIZE_H_ANGLE+j);
					tempbox->setPosition(box->getPosition());
					tempbox->setScale(CHESSMAN_SCALE_ANGLE);
				}),nullptr));
			}
		}

		runAction(Sequence::create(DelayTime::create(CHESS_SIZE_H_ANGLE*boxgraytime),CallFunc::create([this](){
			setGameState(STATE_FAILD);
		}),nullptr));
		//setGameState(STATE_FAILD);
}

bool AngleManage::levelUp()
{
	if(getmScore()>=getmTarget()&&getmScore()>0)
	{
		setmLevel(getmLevel()+1);
		setmTarget(getmTarget()+1000);

		if(getmLevel()==4)
			vigame::ad::ADManager::openAd("level_win");

		if(GameData::getSaveData()->_gg_guanshu<=5)
			GameData::getSaveData()->_gg_guanshu++;

		settouchState(TOUCH_FALSE);

		cjMusic::playEffect("video/levelup.mp3");

		spriteShow("guoguan.png");
		runAction(CCSequence::create(CCDelayTime::create(sprintshowtime),CallFunc::create([this](){
			LevelUpEliminate();
		}),nullptr));
		return true;
	}
	return false;
}

void AngleManage::spriteShow(const string& str)
{
	float sck=1.0f;
	if(str=="guoguan.png")
		sck=0.7f;

	auto spr=cjSprite::createWithImage(str);
	addChild(spr,CHESSMAN_LEVEL+1);
	spr->setPosition(Vec2(WINSIZE/2.0f)+Vec2(0,heightoffest*0.5f));
	spr->setScale(2.0f*sck);
	spr->runAction(CCSequence::create(FadeOut::create(0.08f),FadeIn::create(0.08f),FadeOut::create(0.08f),FadeIn::create(0.08f),FadeOut::create(0.08f),FadeIn::create(0.08f),
		ScaleTo::create(0.2f,1.5f*sck),
		DelayTime::create(0.52f),
		FadeOut::create(0.3f),
		CallFunc::create([spr](){
			spr->removeFromParent();
	}),nullptr));
}

void AngleManage::LevelUpMotion()
{
	BoxFillJudge();

	int temp=0;
	if(GameData::getSaveData()->_gg_guanshu>5&&GameData::getSaveData()->_levelreward_num<LEVELREWARD&&getmLevel()!=sdata._level&&getmLevel()>1)
	{
		temp=1;
		settouchState(TOUCH_FALSE);
	}
	else
		settouchState(TOUCH_TRUE);

	dataSave();

	if(getmLevel()==1)
		cjMusic::playEffect("video/ReadyGo.mp3");

	auto fnt1 = cjSprite::createWithImage("level_guanshu.png");
	addChild(fnt1, CHESSMAN_LEVEL + 1);
	fnt1->setCascadeOpacityEnabled(true);
	fnt1->setPosition(Vec2(WINSIZE / 2.0f) + Vec2(0, heightoffest*0.5f - 140));
	fnt1->runAction(Sequence::create(CCMoveBy::create(0.3f, Vec2(0, 200)), DelayTime::create(1.1f), FadeOut::create(0.3f), nullptr));

	auto fnt1num = cjAtlasLabel::createWithInt(getmLevel(), ATLAS_PATH2);
	fnt1->addChild(fnt1num);
	fnt1num->setCascadeOpacityEnabled(true);
	fnt1num->setScale(levelupscale);
	fnt1num->setPosition(fnt1->getContentSize() / 2.0f);

	auto fnt2 = cjSprite::createWithImage("level_mubiao.png");
	addChild(fnt2, CHESSMAN_LEVEL + 1);
	fnt2->setCascadeOpacityEnabled(true);
	fnt2->setAnchorPoint(Vec2(0.5f, 1.0f));
	fnt2->setPosition(fnt1->getPosition() - Vec2(0, 50));

	auto fnt2num = cjAtlasLabel::createWithInt(getmTarget(), ATLAS_PATH2);
	fnt2->addChild(fnt2num);
	fnt2num->setCascadeOpacityEnabled(true);
	fnt2num->setAnchorPoint(Vec2(0.0f, 0.5f));
	fnt2num->setScale(levelupscale + 0.1f);
	fnt2num->setPosition(fnt2->getContentSize().width + 10, fnt2->getContentSize().height / 2.0f);

	fnt2->setPosition(fnt2->getPosition() - Vec2(fnt2num->getContentSize().width / 2.0f + 10, 0));
	fnt2->runAction(Sequence::create(CCMoveBy::create(0.3f, Vec2(0, 200)), CCDelayTime::create(0.2f), ScaleTo::create(0.2f, 1.2f), ScaleTo::create(0.2f, 1.0f), CCDelayTime::create(0.5f), CCFadeOut::create(0.3f),
		CallFunc::create([fnt1, fnt2, temp, this]() {
		fnt1->removeFromParent();
		fnt2->removeFromParent();
		//setGameState(STATE_RUN);

		if (temp == 1)
		{
			GameData::getSaveData()->_levelreward_num++;
			PayScene::getInstance()->levelReward();
			//GameDataInstance()->d_levelReward=false;
			settouchState(TOUCH_TRUE);
		}
	}), nullptr));
}

//得到升级消除的棋子
vector<pair<int,int>> AngleManage::getLevelUpBox()
{
	vector<pair<int,int>> selectvec;
	for(int i=0;i<CHESS_SIZE_W_ANGLE;i++)
		for(int j=0;j<CHESS_SIZE_H_ANGLE;j++)
		{
			if(_chessColor[i][j])
				selectvec.push_back(make_pair(i,j));
		}

		vector<pair<int,int>> vec;
		if(selectvec.size()>0)
		{
			auto pos=selectvec[rand()%(selectvec.size())];
			int type=rand()%3;
			if(type==0)	//横
			{
				for(int i=0;i<CHESS_SIZE_W_ANGLE;i++)
				{
					if(_chessBlack[i][pos.second]&&_chessColor[i][pos.second]!=0)
						vec.push_back(make_pair(i,pos.second));
				}
			}
			else if(type==1)	//竖
			{
				for(int i=0;i<CHESS_SIZE_H_ANGLE;i++)
				{
					if(_chessBlack[pos.first][i]&&_chessColor[pos.first][i]!=0)
						vec.push_back(make_pair(pos.first,i));
				}
			}
			else if(type==2)	//斜
			{
				int i=pos.first-pos.second;
				for(int j=0;j<9;j++)
				{
					if(((i+j)>=0)&&((i+j)<CHESS_SIZE_W_ANGLE)&&(_chessColor[i+j][j]!=0)&&_chessBlack[i+j][j])
						vec.push_back(make_pair(i+j,j));
				}
			}
		}
		return vec;
}

void AngleManage::LevelUpEliminate()
{
	auto eliminatebox=getLevelUpBox();

	levelupmusic=0;
	int key=0;

	if(eliminatebox.size()>0)
	{
		cjSchedule::getInstance()->shockBegin(this);
		runAction(Sequence::create(DelayTime::create(0.25f*eliminatebox.size()),CallFunc::create([this](){
			cjSchedule::getInstance()->shockEnd(this);
		}),nullptr));
	}

	for(auto &i:eliminatebox)
	{
		auto pos=i;
		_chessColor[pos.first][pos.second]=0;
		runAction(Sequence::create(DelayTime::create(0.25f*MIN(3,key)),CallFunc::create([this,pos,key](){
			addParticle(pos.first,pos.second,_chessColor[pos.first][pos.second],2);
			removeBox(pos);
			if(key<=3)
			{
				char musicname[30];
				if(key<3)
				{
					sprintf(musicname,"video/levelupbegin%d.mp3",levelupmusic%4);
					++levelupmusic;
				}
				else if(key==3)
					sprintf(musicname,"video/levelupend.mp3");
				cjMusic::playEffect(musicname);
			}
		}),nullptr));
		key++;
	}
	runAction(Sequence::create(DelayTime::create(1.5f),CallFunc::create([this](){
		LevelUpMotion();
		auto hongbaofnt=(LabelTTF*)getChildByName("hongbaofnt");
		hongbaofnt->setString(cjTTFLabel::getNameByInt(cjTTFLabel::getStringByKey("shengjiguan").c_str(),hongBaoLevel()));
	}),nullptr));
}

//解析csb文件的名字
pair<int,int> AngleManage::analyzeName(const string &name)
{
	vector<int> tt;
	string ss;
	for(int i=0;i<name.size();i++)
	{
		if(name[i]=='_')
		{
			int x=atoi(ss.c_str());
			tt.push_back(x);
			ss.clear();
		}
		else
		{
			ss+=name[i];
		}
	}
	int x=atoi(ss.c_str());
	tt.push_back(x);
	return make_pair(tt[0]*CHESS_SIZE_H_ANGLE+tt[1],tt[2]);
}

//初次地图设计
void AngleManage::firstSet(int id)
{
	string name=cjTTFLabel::getNameByInt("Angle/map%02d.csb",id);
	auto mapdata=CSLoader::createNode(name);
	auto sprs=mapdata->getChildren();
	for(auto &i:sprs)
	{
		auto name=i->getName();
		auto info=analyzeName(name);
		pair<int,int> pos=make_pair(info.first/CHESS_SIZE_H_ANGLE,info.first%CHESS_SIZE_H_ANGLE);
		_chessColor[pos.first][pos.second]=MAX(1,(info.second-1)*2);
	}
}

//六角模式指引
void AngleManage::dragGuide()
{
	auto bclayer=Layer::create();
	Director::getInstance()->getRunningScene()->addChild(bclayer,100,"bclayer");

	Vec2 v0[4]={
		Vec2(0,0),Vec2(WINSIZE.width,0),Vec2(WINSIZE.width,WINSIZE.height),Vec2(0,WINSIZE.height),
	};
	auto drawback=DrawNode::create();
	drawback->drawSolidPoly(v0,4,Color4F(0,0,0,0.7f));

	auto clipnode=ClippingNode::create();
	bclayer->addChild(clipnode);

	Vec2 v1[4]={
		Vec2(1,766),Vec2(119,766),Vec2(119,798),Vec2(1,798)
	};
	Vec2 v2[4]={
		Vec2(124,766),Vec2(279,766),Vec2(279,798),Vec2(124,798)
	};
	Vec2 v3[4]={
		Vec2(283,766),Vec2(439,766),Vec2(439,798),Vec2(283,798)
	};
	Vec2 v31[4]={
		Vec2(ANGLE_SCORE_PT.x-40,ANGLE_SCORE_PT.y-20),Vec2(ANGLE_SCORE_PT.x+40,ANGLE_SCORE_PT.y-20),Vec2(ANGLE_SCORE_PT.x+40,ANGLE_SCORE_PT.y+20),Vec2(ANGLE_SCORE_PT.x-40,ANGLE_SCORE_PT.y+20)
	};

	auto sten=Node::create();
	bclayer->addChild(sten);

	auto draw=DrawNode::create();
	draw->drawSolidPoly(v1,4,Color4F(1.0f,1.0f,1.0f,0.0f));
	draw->drawPoly(v1,4,true,Color4F(1.0f,1.0f,1.0f,1.0f));
	draw->drawSolidPoly(v2,4,Color4F(1.0f,1.0f,1.0f,0.0f));
	draw->drawPoly(v2,4,true,Color4F(1.0f,1.0f,1.0f,1.0f));
	draw->drawSolidPoly(v3,4,Color4F(1.0f,1.0f,1.0f,0.0f));
	draw->drawPoly(v3,4,true,Color4F(1.0f,1.0f,1.0f,1.0f));
	draw->drawSolidPoly(v31,4,Color4F(1.0f,1.0f,1.0f,0.0f));
	draw->drawPoly(v31,4,true,Color4F(1.0f,1.0f,1.0f,1.0f));

	draw->drawLine(Vec2(62,760),Vec2(62,680),Color4F(1,1,1,1));
	draw->drawLine(Vec2(187,760),Vec2(187,660),Color4F(1,1,1,1));
	draw->drawLine(Vec2(315,760),Vec2(315,680),Color4F(1,1,1,1));
	draw->drawLine(Vec2(ANGLE_SCORE_PT.x+30,ANGLE_SCORE_PT.y-20),Vec2(ANGLE_SCORE_PT.x+30,ANGLE_SCORE_PT.y-120),Color4F(1,1,1,1));

	sten->addChild(draw);
	//

	//font
	auto fnt1=LabelTTF::create(cjTTFLabel::getStringByKey("tiplevel"),TTF_PATH,32);
	fnt1->setAnchorPoint(Vec2(0.5f,1.0f));
	fnt1->setPosition(Vec2(62,680));
	bclayer->addChild(fnt1,2);

	auto fnt2=LabelTTF::create(cjTTFLabel::getStringByKey("tiptarget"),TTF_PATH,32);
	fnt2->setAnchorPoint(Vec2(0.5f,1.0f));
	fnt2->setPosition(Vec2(187,660));
	bclayer->addChild(fnt2,2);

	auto fnt3=LabelTTF::create(cjTTFLabel::getStringByKey("highscore"),TTF_PATH,32);
	fnt3->setAnchorPoint(Vec2(0.5f,1.0f));
	fnt3->setPosition(Vec2(315,680));
	bclayer->addChild(fnt3,2);

	auto fnt31=LabelTTF::create(cjTTFLabel::getStringByKey("tipscore"),TTF_PATH,32);
	fnt31->setAnchorPoint(Vec2(0.5f,1.0f));
	fnt31->setPosition(Vec2(ANGLE_SCORE_PT.x+30,ANGLE_SCORE_PT.y-120));
	bclayer->addChild(fnt31,2);

	auto fnt4=LabelTTF::create(cjTTFLabel::getStringByKey("tipdrag1"),TTF_PATH,35);
	fnt4->setAnchorPoint(Vec2(0.5f,1.0f));
	fnt4->setPosition(Vec2(WINSIZE.width/2.0f,480));
	bclayer->addChild(fnt4,2);

	auto fnt5=LabelTTF::create(cjTTFLabel::getStringByKey("tipdrag2"),TTF_PATH,35);
	fnt5->setAnchorPoint(Vec2(0.5f,1.0f));
	fnt5->setPosition(Vec2(WINSIZE.width/2.0f,420));
	bclayer->addChild(fnt5,2);
	//

	//方块
	auto node1=AngleNode::createNode(6);
	node1->setPosition(node1->getCenterAnPointX(Vec2(nextboxbegin+nextboxoffest,selectboxoffest)));
	node1->setScale(selectboxsize);
	sten->addChild(node1,1,"node1");

	//hand
	auto handnode=Node::create();
	bclayer->addChild(handnode,1);
	auto hand2=cjSprite::createWithImage("ybb.png");
	hand2->setAnchorPoint(Vec2(0.15f,0.9f));
	auto seq3=Sequence::createWithTwoActions(DelayTime::create(0.5f),MoveTo::create(1.0f,posToposition(make_pair(2,1))));
	auto seq4=Sequence::createWithTwoActions(DelayTime::create(0.5f),MoveTo::create(0.0f,node1->getPosition()));
	//hand2->runAction(RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(seq3,seq4)));
	handnode->addChild(hand2);
	handnode->setPosition(node1->getPosition());
	handnode->runAction(RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(seq3,seq4)));
	handnode->setScale(0.6f);
	//

	auto node2=AngleNode::createNode(6);
	node2->setPosition(posToposition(make_pair(2,1)));
	node2->setOpacity(0);
	//auto node3=BoxLine::createLine(ShapeIndex::T_SHAPE);
	//node3->setPosition(posToposition(make_pair(1,0)));
	//sten->addChild(node3);
	sten->addChild(node2);
	//

	clipnode->setStencil(sten);


	clipnode->addChild(drawback);
	clipnode->setInverted(true);
}

//记录保存
void AngleManage::dataSave(bool tt)
{
	if(tt)
	{
		GameData::getSaveData()->_angle._istip=true;
		for(int i=0;i<CHESS_SIZE_W_ANGLE;i++)
			for(int j=0;j<CHESS_SIZE_H_ANGLE;j++)
				GameData::getSaveData()->_angle._chessInfo[i][j]=_chessColor[i][j];
		GameData::getSaveData()->_angle._level=getmLevel();
		GameData::getSaveData()->_angle._score=getmScore();
		GameData::getSaveData()->_angle._target=getmTarget();
		for(int i=0;i<3;i++)
		{
			auto box=(AngleNode*)getChildByTag(1000+i);
			GameData::getSaveData()->_angle._boxshape[i]=box->_id;
		}
	}
	else
	{
		GameData::getSaveData()->_angle._istip=false;
	}
	GameData::getInstance()->dataSave();
}

//红包
void AngleManage::hongbaoPage()
{
	this->removeChildByName("hbhand");

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
		mu1->addTouchEventListener(CC_CALLBACK_2(AngleManage::buttonClick,this));
	}

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);
}

//红包按钮点击事件
void AngleManage::buttonClick(Ref* pSender, ui::Widget::TouchEventType touchType)
{
	auto menu=dynamic_cast<ui::Button*>(pSender);
	int k=menu->getTag();
	if((touchType==ui::Widget::TouchEventType::ENDED)&&(k>0&&k<7)&&_hongbaoclick)
	{
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

//红包礼物
void AngleManage::hongbaoReward()
{
	auto layer=(Layer*)Director::getInstance()->getRunningScene()->getChildByTag(100);

	int rewardid=1;
	if(GameData::getSaveData()->_hongbaotime==0)
	{
		if(GameData::getSaveData()->_hongbaonum<=5)
			GameData::getSaveData()->_hongbaonum++;
		if(GameData::getSaveData()->_anglehongbaonum<=5)
			GameData::getSaveData()->_anglehongbaonum++;

		int key=rand()%10;
		if(GameData::getSaveData()->_anglehongbaonum==1||key==9)
			rewardid=50;
		else if(key==8)
			rewardid=53;
		else if(key==0)
			rewardid=54;
		else
		{
			int ddd=rand()%10;
			if(ddd<4)
				rewardid=3;
			else if(ddd<6)
				rewardid=2;
			else if(ddd<8)
				rewardid=4;
			else if(ddd<9)
				rewardid=5;
			else 
				rewardid=rand()%3+6;
		}
	}

	auto node=CSLoader::createNode("animation/reward.csb");
	char name[30];
	if(rewardid<10)
		sprintf(name,"dimond-%d",rewardid);
	else 
		sprintf(name,"prop");
	auto sp=node->getChildByName(name);
	sp->retain();
	sp->removeFromParent();
	layer->addChild(sp,1);
	sp->setAnchorPoint(Vec2(0.5f,0.5f));
	sp->setPosition(Vec2(240,530));

	if(rewardid>30)
	{
		auto secondsp=cjSprite::createWithImage(cjTTFLabel::getNameByInt("prop%02d.png",rewardid-50));
		sp->addChild(secondsp,1,"secondsp");
		secondsp->setPosition(Vec2(sp->getContentSize()/2.0f)+Vec2(0,10));

		if(rewardid==50)
		{
			secondsp->setScale(1.0f);
			secondsp->setPosition(secondsp->getPosition()-Vec2(5,10));
		}
		else
			secondsp->setScale(1.5f);
	}
	else
	{
		auto diamonds=CCParticleSystemQuad::create("lizi/diamonds.plist");
		layer->addChild(diamonds,2);
		diamonds->setPosition(Vec2(WINSIZE.width/2.0f,WINSIZE.height*DIAMOND_HIGH));
		diamonds->setAutoRemoveOnFinish(true);
	}

	sp->setScaleX(0.0f);
	sp->runAction(Sequence::create(ScaleTo::create(0.2f,1.0f),MoveTo::create(0.3f,Vec2(240,420)),CallFunc::create([layer,sp,rewardid,this](){
		auto gx=cjSprite::createWithImage("gxn.png");
		layer->addChild(gx);
		gx->setPosition(Vec2(240,600));
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
			auto diamond=cjSprite::createWithImage("zs.png");
			this->addChild(diamond,10);
			diamond->setRotation(30);
			diamond->setPosition(Vec2(240,420));
			diamond->runAction(ScaleTo::create(0.7f, 1.2f));
			diamond->runAction(Sequence::create(MoveTo::create(0.7f,diamondpos), ScaleTo::create(0.3f, 1.0f), CallFunc::create([=](){
				GameData::getSaveData()->_diamondNumber+=rewardid;
				diamond->removeFromParent();
				cjMusic::playEffect("video/coinsin.mp3");
				GameData::getInstance()->dataSave();

				PayScene::getInstance()->parShow(this,10,diamondpos);
			}),nullptr));
			diamond->setScale(2.0f);
			diamond->runAction(ScaleTo::create(0.5f,1.0f));
		}
		else if(rewardid>50)
		{
			auto propsp=this->getChildByName(cjTTFLabel::getNameByInt("prop%d",rewardid-52));
			auto secondsp=sp->getChildByName("secondsp");
			secondsp->retain();
			secondsp->removeFromParent();
			this->addChild(secondsp,100);
			secondsp->setPosition(Vec2(240,420));
			secondsp->runAction(ScaleTo::create(0.7f,1.2f));
			secondsp->runAction(Sequence::create(MoveTo::create(0.7f,propsp->getPosition()), ScaleTo::create(0.3f, 1.0f), CallFunc::create([=](){
				GameData::getSaveData()->_propnum[rewardid-51]++;
				secondsp->removeFromParent();
				if(GameData::getSaveData()->_propnum[rewardid-51]==1)
				{
					_propnum[rewardid-53]=cjTTFLabel::createWithString(cjTTFLabel::getNameByInt("x%d",GameData::getSaveData()->_propnum[rewardid-51]));
					this->addChild(_propnum[rewardid-53],2);
					_propnum[rewardid-53]->setColor(propcolor);
					_propnum[rewardid-53]->setPosition(getChildByName(cjTTFLabel::getNameByInt("prop%d",rewardid-52))->getPosition()+Vec2(20,-15));
				}
				else
					_propnum[rewardid-53]->setString(cjTTFLabel::getNameByInt("x%d",GameData::getSaveData()->_propnum[rewardid-51]));
				cjMusic::playEffect("video/coinsin.mp3");
				GameData::getInstance()->dataSave();

				PayScene::getInstance()->parShow(this, 10, propsp->getPosition());
			}),nullptr));
			secondsp->runAction(ScaleTo::create(0.5f,1.0f));
		}
		else if(rewardid==50)
		{
			startRain();
		}

		GameData::getSaveData()->_hongbaotime=TIME_HONGBAO[GameData::getSaveData()->_hongbaonum];
		GameData::getInstance()->dataSave();
		if(!_guide)
		{
			auto font=this->getChildByName("hongbaofnt");
			font->setVisible(true);
		}

		layer->removeFromParent();

	}),nullptr));
}

//流星雨
void AngleManage::startRain()
{
	auto liupos=this->getChildByName("hongbaomenu")->getPosition();

	levelupmusic = 0;
	vector<pair<int,int>> selectvec;
	for(int i=0;i<CHESS_SIZE_W_ANGLE;i++)
		for(int j=0;j<CHESS_SIZE_H_ANGLE;j++) 
		{
			if(_chessBlack[i][j]&&_chessColor[i][j])
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
				int kx=rand()%CHESS_SIZE_W_ANGLE;
				int ky=rand()%CHESS_SIZE_H_ANGLE;
				if(_chessBlack[kx][ky]&&std::find(vec.begin(),vec.end(),make_pair(kx,ky))==vec.end()&&std::find(unselectvec.begin(),unselectvec.end(),make_pair(kx,ky))==unselectvec.end())
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
				cjMusic::playEffect(cjTTFLabel::getNameByInt("video/lxy_%02d.mp3", (levelupmusic % 3 + 1)).c_str());


				auto move1=MoveTo::create(flo,posToposition(i));
				auto call0=CallFunc::create([i,this](){
					auto box=this->getChildByTag(i.first*CHESS_SIZE_H_ANGLE+i.second);
					auto par=ParticleSystemQuad::create("lizi/liuxing2_hit.plist");
					box->addChild(par,100);
					par->setScale(0.6f);
					par->setPosition(box->getContentSize()/2.0f);
					par->setAutoRemoveOnFinish(true);
				});
				auto call=CallFunc::create([par,i,this](){
					par->removeFromParent();
					auto box=this->getChildByTag(i.first*CHESS_SIZE_H_ANGLE+i.second);
					if(box)
					{
						auto propSkillNode = CSLoader::createNode("animation/xiaochu.csb");
						this->addChild(propSkillNode, 12);
						propSkillNode->setPosition(box->getPosition()+Vec2(0,CHESSMAN_SIZE_START*0.5f));
						auto propSkillAction=CSLoader::createTimeline("animation/xiaochu.csb");
						propSkillNode->runAction(propSkillAction);
						propSkillAction->gotoFrameAndPlay(0,26,false);
						propSkillNode->runAction(Sequence::create(DelayTime::create(0.5f),CallFunc::create([propSkillNode](){
							propSkillNode->removeFromParent();
						}),nullptr));
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
				//addParticle(i.first,i.second,_chessColor[i.first][i.second],10);
				auto par = ParticleSystemQuad::create("lizi/area_boom.plist");
				this->addChild(par, 10);
				par->setPosition(posToposition(i));
				par->setAutoRemoveOnFinish(true);
				par->setScale(0.5f);
				removeBox(i);
			}
			setGameState(STATE_RUN);
			int num=vec.size()*CHESS_SCORE;
			_score+=num;
			scoreMotion(num,make_pair(CHESS_SIZE_W_START/2,CHESS_SIZE_H_START/2));
			if(_score>=_target&&(_score-num)<_target)
			{
				settouchState(TOUCH_FALSE);

				cjMusic::playEffect("video/levelup.mp3");
				spriteShow("guoguan.png");
				runAction(CCSequence::create(CCDelayTime::create(sprintshowtime),CallFunc::create([this](){
					LevelUpEliminate();
				}),nullptr));
			}
		}));
	runAction(Sequence::create(action2));
}

