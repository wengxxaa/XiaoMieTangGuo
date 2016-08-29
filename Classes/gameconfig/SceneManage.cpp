#include "SceneManage.h"
#include "../gamescene/MainScene.h"
#include "PointManage.h"
#include "BaseConfig.h"
#include "vigame_ad.h"

const int deltime=10;
const float droptime=0.02f;	
const float leveluptime=1.8f;	//升级动画时间
const float textdelaytime=0.25f;//一排方块消失的时间
const float upboxtimecycle=18.0f;	//升方块时间周期
const float upboxtime=0.5f;	//上升时间
const int guidenumber=2;

SceneManage::SceneManage(){
}

SceneManage::~SceneManage(){
}

void SceneManage::run()
{	
	if(_model==MODEL_SLIP)
	{
		schedule(schedule_selector(SceneManage::boxDropTip),0.1f);
		listener->setEnabled(true);
	}
	else
	{
	//	schedule(schedule_selector(SceneManage::timeUpdate),1.0f);
		listener->setEnabled(true);
	}
	schedule(schedule_selector(SceneManage::boxDropUpdate),DROP_UPDATE_TIME+DROP_STOP_TIME);
	setGameState(STATE_RUN);
}

void SceneManage::pause()
{
	setGameState(STATE_PAUSE);
}

void SceneManage::start()
{
	setGameState(STATE_RUN);
}

bool SceneManage::init()
{
	if(!CCLayer::init())
		return false;

	heightoffest=GameDataInstance()->BANER_HEIGHT;
	LOGGAME(cjTTFLabel::getNameByInt("banner: %d",heightoffest).c_str());

	initChess();

	setTouchState(TOUCH_TRUE);
	setGameState(STATE_STOP);
	setmBoxSave(true);
	_saveBox=-1;
	_recentBox=ShapeIndex(-1);
	_model=GameData::getInstance()->d_Model;
	_scoretime=0.0f;
	_scorewav=0;
	_upboxtime=0.0f;
	_canup=false;
	_guide=false;
	_guidenumber=0;
	_firstSaveIn=false;

	bool tempbool=false;
	if(GameDataInstance()->d_Model==MODEL_POINT)
	{
		if(!GameData::getSaveData()->_challengesstetris._guider)
			_guide=true;
		if(GameData::getSaveData()->_challengesstetris._istip)
			tempbool=true;
	}

	//数据初始化
	if(tempbool)
	{
		_firstSaveIn=true;
		if(GameDataInstance()->d_Model==MODEL_POINT)
			sdata=GameData::getSaveData()->_challengesstetris;

		setmStepNumber(0);
		for(int i=0;i<3;i++)
			_nextBox[i]=(ShapeIndex)sdata._nextbox[i];
		
		runAction(Sequence::create(CCDelayTime::create(leveluptime),CallFunc::create([this](){startSaveBox();}),nullptr));

		setmScore(sdata._score);
		setmLevel(sdata._level);
		setmTarget(sdata._targetscore);

		for(int i=0;i<CHESS_SIZE_W;i++)
			for(int j=0;j<CHESS_SIZE_H;j++)
			{
				_chessFilInfo[i][j]=MIN(1,sdata._chessInfo[i][j]);
			}

		for(int i=0;i<CHESS_SIZE_W;i++)
			for(int j=0;j<CHESS_SIZE_H;j++)
			{
				_chessSaveInfo[i][j]=sdata._chessInfo[i][j];
				if(_chessSaveInfo[i][j])
				{
					auto boxnode=cjSprite::createWithImage(cjTTFLabel::getNameByInt("fangkuai_%d.png",_chessSaveInfo[i][j]));
					_clip->addChild(boxnode,CHESSMAN_LEVEL,i*CHESS_SIZE_H+j);
					boxnode->setPosition(chessPos(i,j));
					boxnode->setScale(CHESSMAN_SCALE);
				}
			}
	}
	else
	{
		initNextBox();

		setmScore(0);
		setmLevel(1);
		setmTarget(500);

		for(int i=0;i<CHESS_SIZE_W;i++)
			for(int j=0;j<CHESS_SIZE_H;j++)
			{
				_chessFilInfo[i][j]=0;
			}

			for(int i=0;i<CHESS_SIZE_W;i++)
				for(int j=0;j<CHESS_SIZE_H;j++)
				{
					_chessSaveInfo[i][j]=0;
				}
	}

	if(_guide)
	{
		if(GameDataInstance()->d_Model==MODEL_SLIP)
		{
			for(int i=0;i<CHESS_SIZE_W;i++)
			{
				if(i<4||i>6)
				{
					char name[30];
					int k=rand()%10+1;
					sprintf(name,"fangkuai_%d.png",k);
					auto boxnode=cjSprite::createWithImage(name);
					_clip->addChild(boxnode,CHESSMAN_LEVEL,i*CHESS_SIZE_H);
					boxnode->setPosition(chessPos(i,0));
					boxnode->setScale(CHESSMAN_SCALE);
					_chessFilInfo[i][0]=1;
					_chessSaveInfo[i][0]=k;
				}
			}
		}
		else if(GameDataInstance()->d_Model==MODEL_POINT)
		{
			for(int i=0;i<CHESS_SIZE_W;i++)
			{
				if(i<2||i>4)
				{
					char name[30];
					int k = rand() % 10 + 1;
					sprintf(name,"fangkuai_%d.png",k);
					auto boxnode=cjSprite::createWithImage(name);
					_clip->addChild(boxnode,CHESSMAN_LEVEL,i*CHESS_SIZE_H);
					boxnode->setPosition(chessPos(i,0));
					boxnode->setScale(CHESSMAN_SCALE);
					_chessFilInfo[i][0]=1;
					_chessSaveInfo[i][0]=k;
				}
			}
		}
	}

	LevelUpMotion();

	listener=EventListenerTouchOneByOne::create();
	listener->onTouchBegan=CC_CALLBACK_2(SceneManage::onTouchBegan,this);
	listener->onTouchMoved=CC_CALLBACK_2(SceneManage::onTouchMoved,this);
	listener->onTouchEnded=CC_CALLBACK_2(SceneManage::onTouchEnded,this);
	listener->setSwallowTouches(true);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
	listener->setEnabled(false);

	schedule(schedule_selector(SceneManage::timeSchedule));

	return true;
}

void SceneManage::initChess()
{
	Vec2 vec[4];	
	vec[0]=WINORIGIN+Vec2(WINSIZE.width/2.0f-CHESS_SIZE_SIZE*CHESS_SIZE_W/2,0+heightoffest);
	vec[1]=WINORIGIN+Vec2(WINSIZE.width/2.0f+CHESS_SIZE_SIZE*CHESS_SIZE_W/2,0+heightoffest);
	vec[2]=WINORIGIN+Vec2(WINSIZE.width/2.0f+CHESS_SIZE_SIZE*CHESS_SIZE_W/2,(CHESS_SIZE_H)*CHESS_SIZE_SIZE+heightoffest);
	vec[3]=WINORIGIN+Vec2(WINSIZE.width/2.0f-CHESS_SIZE_SIZE*CHESS_SIZE_W/2,(CHESS_SIZE_H)*CHESS_SIZE_SIZE+heightoffest);

	auto node=DrawNode::create();
	this->addChild(node,10);
	node->drawSolidPoly(vec,4,Color4F(1.0f,1.0f,1.0f,0.0f));

	_clip=ClippingNode::create(node);
	this->addChild(_clip,10);
	_clip->setInverted(false);

	_PointTipLayer=Layer::create();
	_clip->addChild(_PointTipLayer,CHESSMAN_LEVEL-1,"pointtiplayer");

	for(int i=0;i<CHESS_SIZE_W;i++)
		for(int j=0;j<CHESS_SIZE_H;j++)
		{
			auto sp=cjSprite::createWithImage("fangkuai_hs.png");
			this->addChild(sp);
			sp->setScale(CHESSMAN_SCALE);
			sp->setPosition(WINORIGIN+Vec2(WINSIZE.width/2.0f+CHESS_SIZE_SIZE*(i-(CHESS_SIZE_W-1)/2.0f),(j+0.5f)*CHESS_SIZE_SIZE+heightoffest));
		}
}

void SceneManage::timeSchedule(float ft)
{
	if(getGameState()!=STATE_RUN)
		return;

	_upboxtime+=ft;
	if(_upboxtime>=upboxtimecycle)
	{
		_canup=true;
		_upboxtime=0.0f;
	}


	_scoretime+=ft;
	if(_scoretime>scorevideotime)
	{
		_scorewav=0;
	}

	_touchtime+=ft;
}

void SceneManage::gameFiled()
{
	//for(int i=0;i<CHESS_SIZE_W;i++)
	//	for(int j=0;j<CHESS_SIZE_H;j++)
	//	{
	//		if(_chessFilInfo.at(i*CHESS_SIZE_H+j))
	//		{
	//			_clip->removeChildByTag(i*CHESS_SIZE_H+j);			
	//			_chessFilInfo[i*CHESS_SIZE_H+j]=0;
	//		}
	//	}

	dataSave(false);

	unschedule(schedule_selector(SceneManage::boxDropUpdate));
	unschedule(schedule_selector(SceneManage::boxDropTip));

	for(int i=0;i<CHESS_SIZE_W;i++)
		for(int j=0;j<CHESS_SIZE_H;j++)
		{
			if(_chessFilInfo[i][j])
			{
				runAction(Sequence::create(DelayTime::create(j*boxgraytime),CallFunc::create([i,j,this](){
					auto box=_clip->getChildByTag(i*CHESS_SIZE_H+j);
					box->setVisible(false);

					auto tempbox=cjSprite::createWithImage("fangkuai_gray.png");
					_clip->addChild(tempbox,CHESSMAN_LEVEL,1000+i*CHESS_SIZE_H+j);
					tempbox->setPosition(box->getPosition());
					tempbox->setScale(CHESSMAN_SCALE);
				}),nullptr));
			}
		}

		runAction(Sequence::create(DelayTime::create(boxgraytime*CHESS_SIZE_H),CallFunc::create([this](){
			setGameState(STATE_FAILD);
		}),nullptr));
	//setGameState(STATE_FAILD);
}

bool SceneManage::checkChess()
{
	bool ttt;
	vector<int> eraselise;
	auto map=getTopHeight();
	int smallh=CHESS_SIZE_H;
	for(int i=0;i<CHESS_SIZE_W;i++)
	{
		if(smallh>map[i])
			smallh=map[i];
	}

	for(int j=0;j<=smallh;j++)
	{
		bool tempb=true;
		for(int i=0;i<CHESS_SIZE_W;i++)
		{
			if(!_chessFilInfo[i][j])
			{
				tempb=false;
				break;
			}
		}
		if(tempb)
			eraselise.push_back(j);
	}
	if(eraselise.size()>0)
	{
		if(eraselise.size()==2)
		{
			spriteShow("zan.png");
			cjMusic::playEffect("video/zan.mp3");
		}
		else if(eraselise.size()>2)
		{
			spriteShow("great.png");
			cjMusic::playEffect("video/great.mp3");
		}

		float tt=rollEliminate(eraselise);

		scoreMotion(100*eraselise.size(),make_pair(CHESS_SIZE_W/2,eraselise[0]));
		setmScore(getmScore()+100*eraselise.size());

		this->runAction(CCSequence::create(CCDelayTime::create(tt),CCCallFunc::create([this](){
			if(!levelUp())
			{
				runAction(CCSequence::create(CCDelayTime::create(CHESS_REMOVE_TIME),CallFunc::create([this](){
					startDropBox();
				}),nullptr));
			}
		}),nullptr));

		return true;;
	}
	else
	{
		if(!levelUp())
			startDropBox();

		return false;;
	}
	return ttt;
}

//行消除
float SceneManage::rollEliminate(vector<int> eraselise)
{
	float ft=0.0f;

	cjMusic::playEffect("video/eliminate.mp3");

	_moveh.clear();
	for(int i=0;i<CHESS_SIZE_H;i++)
	{
		_moveh.push_back(0);
	}

	Vector<FiniteTimeAction*> action;
	for(auto &j:eraselise)
	{
		for(int i=0;i<CHESS_SIZE_W;i++)
		{
			if(_chessFilInfo[i][j])
			{
				action.pushBack(DelayTime::create(0.02f));
				action.pushBack(CallFunc::create([this,i,j](){
					addParticle(i,j,_chessSaveInfo[i][j]-1);
					removeBox(make_pair(i,j));
				}));
				ft+=0.02f;
			}
		}

		for(int k=0;k<CHESS_SIZE_H;k++)
		{
			if(k>j)
				_moveh[k]++;
		}
	}
	action.pushBack(DelayTime::create(0.2f));
	action.pushBack(CallFunc::create([this](){
		for(int i=0;i<CHESS_SIZE_W;i++)
			for(int j=0;j<CHESS_SIZE_H;j++)
			{
				auto box=_clip->getChildByTag(i*CHESS_SIZE_H+j);
				if(_moveh[j]>0&&box)
				{
					auto moveto=CCMoveTo::create(CHESS_REMOVE_TIME*0.8f,box->getPosition()-Vec2(0,_moveh[j]*CHESS_SIZE_SIZE));
					box->runAction(EaseBounceOut::create((ActionInterval*)moveto));

					box->setTag(i*CHESS_SIZE_H+j-_moveh[j]);
					_chessFilInfo[i][j]=0;
					_chessFilInfo[i][j-_moveh[j]]=1;
					_chessSaveInfo[i][j-_moveh[j]]=_chessSaveInfo[i][j];
					_chessSaveInfo[i][j]=0;

				}
			}
	}));

	runAction(Sequence::create(action));

	ft+=CHESS_REMOVE_TIME+0.2f;

	return ft;

	//runAction(CCSequence::create(CCDelayTime::create(textdelaytime+CHESS_REMOVE_TIME*0.2f),CCCallFunc::create([this](){
	//	for(int i=0;i<CHESS_SIZE_W;i++)
	//		for(int j=0;j<CHESS_SIZE_H;j++)
	//		{
	//			auto box=_clip->getChildByTag(i*CHESS_SIZE_H+j);
	//			if(_moveh[j]>0&&box)
	//			{
	//				auto moveto=CCMoveTo::create(CHESS_REMOVE_TIME*0.8f,box->getPosition()-Vec2(0,_moveh[j]*CHESS_SIZE_SIZE));
	//				box->runAction(EaseBounceOut::create((ActionInterval*)moveto));

	//				box->setTag(i*CHESS_SIZE_H+j-_moveh[j]);
	//				_chessFilInfo[i][j]=0;
	//				_chessFilInfo[i][j-_moveh[j]]=1;
	//				_chessSaveInfo[i][j-_moveh[j]]=_chessSaveInfo[i][j];
	//				_chessSaveInfo[i][j]=0;

	//			}
	//		}
	//}),nullptr));
}

void SceneManage::initNextBox()
{
	setmStepNumber(0);
	for(int i=0;i<3;i++)
	{
		if(_guide&&i==0)
		{
			if(GameDataInstance()->d_Model==MODEL_SLIP)
				_nextBox[i]=ShapeIndex::L_SHAPE;
			else if(GameDataInstance()->d_Model==MODEL_POINT)
				_nextBox[i]=ShapeIndex::L_SHAPE;
		}
		else
			_nextBox[i]=ShapeIndex(rand()%7);
	}
}

void SceneManage::createNextBox()
{
	int step=getmStepNumber();
	setmStepNumber(++step);
	for(int i=1;i<3;i++)
		_nextBox[i-1]=_nextBox[i];
	_nextBox[2]=ShapeIndex(rand()%11);
}

void SceneManage::startDropBox()
{
	float time=0.0f;
	if(_canup)
	{
		_canup=false;
		time+=upboxtime;
		int numllnumber=rand()%3+1;
		vector<int> vec;
		for(int i=0;i<numllnumber;i++)
		{
			int k;
			bool temp=false;
			do 
			{
				k=rand()%CHESS_SIZE_W;
				if(find(vec.begin(),vec.end(),k)==vec.end())
				{
					vec.push_back(k);
					temp=true;
				}
			} while (false);
		}

		for(int i=0;i<CHESS_SIZE_W;i++)
		{
			if(_chessFilInfo[i][CHESS_SIZE_H-1])
			{
				gameFiled();
				return;
			}
		}

		for(int i=0;i<CHESS_SIZE_W;i++)
		{
			for(int j=(CHESS_SIZE_H-2);j>=0;j--)
			{
				if(_chessFilInfo[i][j])
				{
					auto box=_clip->getChildByTag(i*CHESS_SIZE_H+j);
					box->runAction(MoveBy::create(upboxtime,Vec2(0,CHESS_SIZE_SIZE)));
					box->setTag(i*CHESS_SIZE_H+j+1);
					_chessFilInfo[i][j+1]=_chessFilInfo[i][j];
					_chessSaveInfo[i][j+1]=_chessSaveInfo[i][j];
					_chessFilInfo[i][j]=0;
					_chessSaveInfo[i][j]=0;
				}
			}
			if(find(vec.begin(),vec.end(),i)==vec.end())
			{
				int k=rand()%10+1;
				char name[30];
				sprintf(name,"fangkuai_%d.png",k);
				auto boxnode=cjSprite::createWithImage(name);
				_clip->addChild(boxnode,CHESSMAN_LEVEL,i*CHESS_SIZE_H);
				boxnode->setPosition(chessPos(i,-1));
				boxnode->setScale(CHESSMAN_SCALE);
				_chessFilInfo[i][0]=1;
				_chessSaveInfo[i][0]=k;
				boxnode->runAction(MoveBy::create(upboxtime,Vec2(0,CHESS_SIZE_SIZE)));
			}
		}
	}

	runAction(Sequence::create(DelayTime::create(time),CallFunc::create([this](){
		_touchcan=false;
		_touchbox=-1;
		if(_firstSaveIn&&sdata._recentbox>=0)
		{
			_recentPos=make_pair(sdata._recentposx,sdata._recentposy);
			_recentBox=ShapeIndex(sdata._recentbox);
		}
		else
		{
			_recentPos=std::make_pair(CHESS_SIZE_W/2.0f,CHESS_SIZE_H-1);
			_recentBox=_nextBox[0];
		}
		auto recentBox=BoxNode::createBox(_recentBox,int(_recentBox));
		_clip->addChild(recentBox,CHESSMAN_LEVEL,"recentbox");
		bool tempbool=true;
		auto boxFill=getBoxFill();
		for(auto &i:boxFill)
		{
			if(i.second<CHESS_SIZE_H &&(i.first<0||i.first>=CHESS_SIZE_W||_chessFilInfo[i.first][i.second]))
			{
				tempbool=false;

				break;
			}
		}
		if(tempbool)
		{
			if(_firstSaveIn&&sdata._recentbox>=0)
			{
				recentBox->setPosition(chessPos(_recentPos.first,_recentPos.second));
				int step=getmStepNumber();
				setmStepNumber(++step); 
			}
			else
			{
				recentBox->setPosition(chessPos(CHESS_SIZE_W/2.0f,CHESS_SIZE_H-1));
				_nextBox[0]=ShapeIndex(-1);
				createNextBox();
			}

			if(GameData::getInstance()->d_Model==MODEL_SLIP)
			{
				auto cloneBox=BoxLine::createLine(_recentBox);
				cloneBox->setPosition(chessPos(-100,-100));
				_clip->addChild(cloneBox,CHESSMAN_LEVEL-1,"clonebox");
			}
			else if(GameData::getInstance()->d_Model==MODEL_POINT)
			{
				unschedule(schedule_selector(SceneManage::timeUpdate));
				schedule(schedule_selector(SceneManage::timeUpdate),1.0f);
				boxDropTip();
			}
			if(!_guide)
				dataSave();
		}
		else
		{
			_clip->removeChildByName("recentbox");
			_recentBox=ShapeIndex(-1);
			gameFiled();
		}

		_firstSaveIn=false;
	}),nullptr));
}

void SceneManage::startSaveBox()
{

}

bool SceneManage::boxDropOneLine()
{
	_recentPos.second-=1;
	auto recentBox=_clip->getChildByName("recentbox");
	//recentBox->runAction(CCMoveBy::create(DROP_UPDATE_TIME,Vec2(0,-CHESS_SIZE_SIZE)));
	recentBox->setPosition(recentBox->getPosition()-Vec2(0,CHESS_SIZE_SIZE));
	return true;
}

void SceneManage::timeUpdate(float ft)
{
	if(_recentBox<0||getGameState()!=STATE_RUN)
		return;

	//if(getmTime()<=0)
	//{
	//	while(boxMoveJudge(DirectionIndex::DIRECTION_D))
	//	{
	//		auto recentBox=_clip->getChildByName("recentbox");
	//		_recentPos.second-=1;
	//		recentBox->setPosition(recentBox->getPosition()-Vec2(0,CHESS_SIZE_SIZE));
	//	}
	//	_PointTipLayer->removeAllChildren();
	//	_touchbox=-1;
	//	boxDropUpdate(0.0f);
	//}
	//else
	//{
	//	setmTime(getmTime()-1);
	//}
}

void SceneManage::boxDropUpdate(float ft)
{
	if(_recentBox<0||getGameState()!=STATE_RUN)
		return;


	if(boxMoveJudge(DirectionIndex::DIRECTION_D))
	{
		boxDropOneLine();
		if(GameData::getInstance()->d_Model==MODEL_POINT)
			PointTipRemove();

		if(_guide)
		{
			_guidenumber++;
			if(_guidenumber==(guidenumber-1))
			{
				setGameState(STATE_STOP);
				if(GameDataInstance()->d_Model==MODEL_SLIP)
				{
					classesGuide();
				}
				else if(GameDataInstance()->d_Model==MODEL_POINT)
				{
					challengeGuide();
				}
			}
		}
		else if(!_guide)
		{
			dataSave();
		}
	}
	else
	{

		cjMusic::playEffect("video/drop-Dekaron.mp3");
		_PointTipLayer->removeAllChildren();
		if(boxSure())
		{
			checkChess();
		}
		else
		{
			gameFiled();
		}
	}

}

bool SceneManage::boxSure()
{
	bool ret=true;
	auto boxfill=getBoxFill();
	for(auto &i:boxfill)
	{
		if(i.second>(CHESS_SIZE_H-1))
			ret=false;
		
		if(i.second<CHESS_SIZE_H&&!_chessFilInfo[i.first][i.second])
		{
			char name[30];
			sprintf(name,"fangkuai_%d.png",int(_recentBox%10)+1);
			auto boxnode=cjSprite::createWithImage(name);
			_clip->addChild(boxnode,CHESSMAN_LEVEL,i.first*CHESS_SIZE_H+i.second);
			boxnode->setPosition(chessPos(i.first,i.second));
			boxnode->setScale(CHESSMAN_SCALE);
			_chessFilInfo[i.first][i.second]=1;
			_chessSaveInfo[i.first][i.second]=int(_recentBox)%10+1;
		}
	}
	_clip->removeChildByName("recentbox");
	if(GameData::getInstance()->d_Model==MODEL_SLIP)
		_clip->removeChildByName("clonebox");
	_recentBox=ShapeIndex(-1);

	//
	scoreMotion(10,boxfill[0]);
	//
	_score+=10;
	//
	return ret;
}

Vec2 SceneManage::chessPos(int x,int y)
{
	return (WINORIGIN+Vec2(WINSIZE.width/2.0f+CHESS_SIZE_SIZE*(x+0.5f-CHESS_SIZE_W/2.0f),(y+0.5f)*CHESS_SIZE_SIZE+heightoffest));
} 

pair<int,int> SceneManage::positionToPos(Vec2 pt)
{
	Vec2 newpt=pt-WINORIGIN;
	int x=(newpt.x-WINSIZE.width/2.0f)/CHESS_SIZE_SIZE+CHESS_SIZE_W/2.0f;
	int y=(newpt.y-heightoffest)/CHESS_SIZE_SIZE;
	return make_pair(x,y);
}

vector<std::pair<int,int>> SceneManage::getBoxFill(Vec2 vec)
{
	auto boxfill=((BoxNode*)_clip->getChildByName("recentbox"))->getBoxFill();
	for(auto &i:boxfill)
	{
		i.first+=_recentPos.first+vec.x;
		i.second+=_recentPos.second+vec.y;
	}
	return boxfill;
}

vector<std::pair<int,int>> SceneManage::getBoxFill(PointTip tip)
{
	auto boxfill=BoxNode::getBoxFillByData(tip.shape,tip.angle);
	for(auto &i:boxfill)
	{
		i.first+=tip.position.first;
		i.second+=tip.position.second;
	}
	return boxfill;
}

bool SceneManage::boxMoveJudge(DirectionIndex dir,Vec2 vec)
{
	auto boxfill=getBoxFill(vec);

	switch (dir)
	{
	case DirectionIndex::DIRECTION_D:
		{
			for(auto &i:boxfill)
			{
				assert(i.first>=0&&i.first<CHESS_SIZE_W&&i.second>=0);
				if(0==i.second)
					return false;
				else if(i.second<=CHESS_SIZE_H&&_chessFilInfo[i.first][i.second-1]>0)
					return false;
			}
		}
		break;
	case DirectionIndex::DIRECTION_R:
		{
			for(auto &i:boxfill)
			{
				assert(i.first>=0&&i.first<CHESS_SIZE_W&&i.second>=0);
				if((CHESS_SIZE_W-1)==i.first)
					return false;
				else if(i.second<CHESS_SIZE_H&&_chessFilInfo[i.first+1][i.second]>0)
					return false;
			}
		}
		break;
	case DirectionIndex::DIRECTION_L:
		{
			for(auto &i:boxfill)
			{
				assert(i.first>=0&&i.first<CHESS_SIZE_W&&i.second>=0);
				if(0==i.first)
					return false;
				else if(i.second<CHESS_SIZE_H&&_chessFilInfo[i.first-1][i.second]>0)
					return false;
			}
		}
		break;
	default:
		break;
	}

	return true;
}

bool SceneManage::boxChange(int key)
{
	if(_guide&&_guidenumber<guidenumber)
		return false;

	auto recentBox=(BoxNode*)_clip->getChildByName("recentbox");
	if(key==1)
		recentBox->Rotate();
	else
		recentBox->ReRotate();
	auto boxFill=getBoxFill();
	
	bool tempbool=true;
	for(auto &i:boxFill)
	{
		if((i.second<CHESS_SIZE_H)&&(i.second<0||i.first<0||i.first>=CHESS_SIZE_W||_chessFilInfo[i.first][i.second]))
		{
			tempbool=false;
			break;
		}
	}

	if(tempbool)
	{
		return true;
	}
	else
	{
		tempbool=true;
		boxFill=getBoxFill(Vec2(-1,0));
		for(auto &i:boxFill)
		{
			if((i.second<CHESS_SIZE_H)&&(i.second<0||i.first<0||i.first>=CHESS_SIZE_W||_chessFilInfo[i.first][i.second]))
			{
				tempbool=false;
				break;
			}
		}
	}

	//向左移动一个单位
	if(tempbool)
	{
		_recentPos.first-=1;
		recentBox->setPosition(recentBox->getPosition()-Vec2(CHESS_SIZE_SIZE,0));
		return true;
	}
	else
	{
		tempbool=true;
		boxFill=getBoxFill(Vec2(1,0));
		for(auto &i:boxFill)
		{
			if((i.second<CHESS_SIZE_H)&&(i.second<0||i.first<0||i.first>=CHESS_SIZE_W||_chessFilInfo[i.first][i.second]))
			{
				tempbool=false;
				break;
			}
		}
	}

	//向右移动一个单位
	if(tempbool)
	{
		_recentPos.first+=1;
		recentBox->setPosition(recentBox->getPosition()+Vec2(CHESS_SIZE_SIZE,0));
		return true;
	}
	else
	{
		tempbool=true;
		if(_recentBox==T_SHAPE||_recentBox==SWAGER_RESHAPE)
		{
			boxFill=getBoxFill(Vec2(-1,-1));
			for(auto &i:boxFill)
			{
				if((i.second<CHESS_SIZE_H)&&(i.second<0||i.first<0||i.first>=CHESS_SIZE_W||_chessFilInfo[i.first][i.second]))
				{
					tempbool=false;
					break;
				}
			}
		}
		else
			tempbool=false;
	}

	//向左下移动
	if(tempbool)
	{
		_recentPos.first-=1;
		_recentPos.second-=1;
		recentBox->setPosition(recentBox->getPosition()+Vec2(-CHESS_SIZE_SIZE,-CHESS_SIZE_SIZE));
		return true;
	}
	else
	{
		//tempbool=true;
		//if(_recentBox==ShapeIndex::T_SHAPE)
		//{
		//	boxFill=getBoxFill(Vec2(1,-1));
		//	for(auto &i:boxFill)
		//	{
		//		if((i.second<CHESS_SIZE_H)&&(i.first<0||i.first>=CHESS_SIZE_W||_chessFilInfo[i.first*CHESS_SIZE_H+i.second]))
		//		{
		//			tempbool=false;
		//			break;
		//		}
		//	}
		//}
		//else
			tempbool=false;
	}

	//向右下移动
	if(tempbool)
	{
		_recentPos.first+=1;
		_recentPos.second-=1;
		recentBox->setPosition(recentBox->getPosition()+Vec2(CHESS_SIZE_SIZE,-CHESS_SIZE_SIZE));
		return true;
	}
	else
	{
		tempbool=true;
		if(_recentBox==ShapeIndex::LINE_SHAPE||_recentBox==ShapeIndex::LINESMALL_SHAPE||_recentBox==ShapeIndex::SWAGER_SHAPE||_recentBox==ShapeIndex::T_SHAPE)
		{
			boxFill=getBoxFill(Vec2(0,-1));
			for(auto &i:boxFill)
			{
				if((i.second<CHESS_SIZE_H)&&(i.second<0||i.first<0||i.first>=CHESS_SIZE_W||_chessFilInfo[i.first][i.second]))
				{
					tempbool=false;
					break;
				}
			}
		}
		else
			tempbool=false;
	}

	//向下移动
	if(tempbool)
	{
		_recentPos.second-=1;
		recentBox->setPosition(recentBox->getPosition()+Vec2(0,-CHESS_SIZE_SIZE));
		return true;
	}
	//旋转失败
	else
	{
		if(key==1)
			recentBox->ReRotate();
		else
			recentBox->Rotate();
		return false;
	}

	return false;
}

void SceneManage::boxSave()
{
	if(!getmBoxSave())
		return;
	if(_recentBox>=0)
	{
		setmBoxSave(false);
		_clip->removeChildByName("recentbox");
		_clip->removeChildByName("clonebox");
		if(_saveBox>=0)
		{
			int temp=_saveBox;
			_saveBox=_recentBox;
			_recentBox=(ShapeIndex)temp;

			_recentPos=std::make_pair(CHESS_SIZE_W/2,CHESS_SIZE_H-1);
			auto recentBox=BoxNode::createBox(_recentBox,int(_recentBox));
			_clip->addChild(recentBox,CHESSMAN_LEVEL,"recentbox");
			recentBox->setPosition(chessPos(CHESS_SIZE_W/2,CHESS_SIZE_H-1));

			if(GameData::getInstance()->d_Model==MODEL_SLIP)
			{
				auto cloneBox=BoxLine::createLine(_recentBox);
				cloneBox->setPosition(chessPos(-100,-100));
				cloneBox->setOpacity(100);
				_clip->addChild(cloneBox,CHESSMAN_LEVEL-1,"clonebox");
			}
			else if(GameData::getInstance()->d_Model==MODEL_POINT)
			{
				boxDropTip();
			}
		}
		else
		{
			_saveBox=_recentBox;
			_clip->removeChildByName("recentbox");
			cjSchedule::getInstance()->runOnNextFrame([this](float ft){
				startDropBox();
			});
		}
	}
}

//鼠标处理
bool SceneManage::onTouchBegan(Touch *touch, Event *unused_event)
{
	if(_guide)
	{
		if(_guidenumber==4)
		{
			bool temp=false;
			if(GameDataInstance()->d_Model==MODEL_SLIP)
			{
				temp=true;
			}
			else if(GameDataInstance()->d_Model==MODEL_POINT)
			{
				vector<pair<int,int>> pos;
				pos.push_back(make_pair(2,0));
				pos.push_back(make_pair(3,0));
				pos.push_back(make_pair(4,0));
				pos.push_back(make_pair(4,1));
				auto position=touch->getLocation();
				auto po=positionToPos(position);
				if(std::find(pos.begin(),pos.end(),po)!=pos.end())
				{
					temp=true;
				}
			}
			if(temp)
			{
				setGameState(STATE_RUN);
				Director::getInstance()->getRunningScene()->removeChildByName("bclayer");
				_guide=false;
				_guidenumber=0;
				GameDataInstance()->_guide=false;
				if(GameDataInstance()->d_Model==MODEL_POINT)
				{
					GameData::getSaveData()->_challengesstetris._guider=true;
				}
				GameData::getInstance()->dataSave();
			}
		}
		else
			return false;
	}

	if(getGameState()!=GameState::STATE_RUN||getTouchState()!=TOUCH_TRUE||_recentBox<0)
		return false;

	setLostTouchPos(touch->getLocation());
	_touchbool=false;
	_touchtime=0.0f;
	_touchcan=true;
	_touchbox=-1;

	if(GameData::getInstance()->d_Model==MODEL_POINT)
	{
		auto pos=positionToPos(touch->getLocation());
		auto tipvec=_pointTipPair.at(_recentPointNumber);
		for(auto &i:tipvec)
		{
			auto boxfill=getBoxFill(_pointTipVec[i]);
			bool temp=false;
			for(auto &j:boxfill)
			{
				if(pos.first==j.first&&pos.second==j.second)
				{
					showTipWithBox(_pointTipVec[i]);
					_touchbox=i;
					return true;
				}
			}
		}
	}
	else if(GameData::getInstance()->d_Model==MODEL_SLIP)
		return true;

	_touchcan=false;

	return false;
}

void SceneManage::onTouchMoved(Touch *touch, Event *unused_event)
{
	if(_recentBox<0||!_touchcan)
		return;

	Vec2 startpos=touch->getStartLocation();
	Vec2 previouspos=getLostTouchPos();
	Vec2 recentpos=touch->getLocation();

	if(sqrt(abs(recentpos.x-startpos.x)*abs(recentpos.x-startpos.x)+abs(recentpos.y-startpos.y)*abs(recentpos.y-startpos.y))>5)
		_touchbool=true;

	if(GameData::getInstance()->d_Model==MODEL_SLIP)
	{
		//向左
		if((recentpos.x>previouspos.x)&&(abs(recentpos.x-previouspos.x)>=abs(recentpos.y-previouspos.y))&&(recentpos.x-previouspos.x)>=CHESS_SIZE_SIZE)
		{
			setLostTouchPos(recentpos);
			if(boxMoveJudge(DirectionIndex::DIRECTION_R))
			{
				auto recentBox=_clip->getChildByName("recentbox");
				_recentPos.first+=1;
				recentBox->setPosition(recentBox->getPosition()+Vec2(CHESS_SIZE_SIZE,0));
			}
		}
		//向右
		else if((recentpos.x<previouspos.x)&&(abs(recentpos.x-previouspos.x)>=abs(recentpos.y-previouspos.y))&&(previouspos.x-recentpos.x)>=CHESS_SIZE_SIZE)
		{
			setLostTouchPos(recentpos);
			if(boxMoveJudge(DirectionIndex::DIRECTION_L))
			{
				auto recentBox=_clip->getChildByName("recentbox");
				_recentPos.first-=1;
				recentBox->setPosition(recentBox->getPosition()-Vec2(CHESS_SIZE_SIZE,0));
			}
		}
		//向下
		else if(_touchtime>=0.3f&&(recentpos.y<previouspos.y)&&(abs(recentpos.x-previouspos.x)<abs(recentpos.y-previouspos.y))&&(previouspos.y-recentpos.y)>=CHESS_SIZE_SIZE*0.5f)
		{
			setLostTouchPos(recentpos);
			if(boxMoveJudge(DirectionIndex::DIRECTION_D))
			{
				auto recentBox=_clip->getChildByName("recentbox");
				_recentPos.second-=1;
				recentBox->setPosition(recentBox->getPosition()-Vec2(0,CHESS_SIZE_SIZE));
			}
		}
	}
	else if(GameData::getInstance()->d_Model==MODEL_POINT)
	{
		auto pos=positionToPos(recentpos);
		auto lostpost=positionToPos(getLostTouchPos());
		if(pos!=lostpost)
		{
			setLostTouchPos(recentpos);
			if(pos.first<0||pos.first>=CHESS_SIZE_W)
			{
				showTipWithId(_recentPointNumber);
				_touchbox=-1;
			}
			else
			{
				bool temp=false;
				int key=0;
				for(auto &i:_pointTipPriority)
				{
					auto boxfill=getBoxFill(_pointTipVec[i]);
					for(auto &j:boxfill)
					{
						if(j==pos)
						{
							temp=true;
							if(_touchbox!=i)
							{
								_touchbox=i;
								showTipWithBox(_pointTipVec[i]);
							}
						}
					}
					if(temp)
						break;
				}
				//if(!temp)
				//{
				//	showTipWithId(_recentPointNumber);
				//	_touchbox=-1;
				//}
			}
		}
	}
}

void SceneManage::onTouchEnded(Touch *touch, Event *unused_event)
{
	if(_recentBox<0||!_touchcan)
		return;

	if(GameData::getInstance()->d_Model==MODEL_SLIP)
	{
		if(!_touchbool)
		{
			_touchcan=false;
			if(_recentBox!=ShapeIndex::SQUARE_SHAPE&&_recentBox!=ShapeIndex::POINT_SHAPE)
			{
				//cjMusic::playEffect("video/rotate.mp3");
				if(!boxChange())
				{

				}
			//		boxChange(3);
			}
		}
		else
		{
			auto previouspos=touch->getStartLocation();
			auto recentpos=touch->getLocation();
			if(_touchtime<0.3f&&(recentpos.y<previouspos.y)&&(abs(recentpos.x-previouspos.x)<abs(recentpos.y-previouspos.y))&&(previouspos.y-recentpos.y)>=CHESS_SIZE_SIZE)
			{
				int hoffest=0;
				while(boxMoveJudge(DirectionIndex::DIRECTION_D))
				{
					hoffest++;
					_recentPos.second--;
				}
				if(hoffest>0)
				{
					_clip->removeChildByName("clonebox");
					setGameState(STATE_STOP);
					auto recentBox=_clip->getChildByName("recentbox");
					MoveBy *move=MoveBy::create(hoffest*0.01f,Vec2(0,-CHESS_SIZE_SIZE*hoffest));
					auto easein=EaseIn::create((ActionInterval*)move,EASE_STRENGTH);
					recentBox->runAction(CCSequence::create(easein,CCCallFunc::create([](){
						//cjMusic::playEffect("video/drop-tetris.mp3");
					}),DelayTime::create(CHESS_CHECK_TIME),CallFunc::create([hoffest,this](){
						if(getGameState()==STATE_STOP)
							setGameState(STATE_RUN);
						boxDropUpdate(0.0f);
					}),nullptr));
				}
			}
			else
				_touchcan=false;
		}
	}
	else if(GameData::getInstance()->d_Model==MODEL_POINT)
	{
		if(_touchbox>=0)
		{
			_PointTipLayer->removeAllChildren();
			moveBoxToId(_touchbox);
			
		}
		else
		{
			//if(!_touchbool)
			//{
			//	_recentPointNumber++;
			//	if(unsigned(_recentPointNumber)>_pointTipPair.size())
			//		_recentPointNumber=1;
			//	showTipWithId(_recentPointNumber);

			//	cjMusic::palyEffect("video/rotate.mp3");
			//}
		}
	}
}

//MODEL_SLIP
void SceneManage::boxDropTip(float ft)
{
	if(getGameState()!=STATE_RUN||_recentBox<0)
		return;

	auto recentBox=(BoxNode*)_clip->getChildByName("recentbox");
	auto cloneBox=(BoxNode*)_clip->getChildByName("clonebox");
	if(cloneBox)
	{
		cloneBox->setRotation(recentBox->getAngle()*90);

		Vec2 vec(0,0);
		if(boxMoveJudge(DIRECTION_D))
		{
			do 
			{
				vec-=Vec2(0,1);
			} while (boxMoveJudge(DIRECTION_D,vec));
		}
		if(vec.y<0)
			cloneBox->setPosition(chessPos(_recentPos.first,_recentPos.second+vec.y));
		else
			cloneBox->setVisible(false);
	}

}

//MODEL_POINT
void SceneManage::boxDropTip()
{
	_pointPath.clear();
	_pointTipPair.clear();
	_pointTipVec.clear();
	_pointTipPriority.clear();
	_noCanPutIn.clear();
	ShapeIndex shape=_recentBox;
	AngleIndex angle=ANGLE_0;
	Point pt;
	int kk=4;
	if(shape==ShapeIndex::LINE_SHAPE||shape==ShapeIndex::SWAGER_SHAPE||shape==ShapeIndex::SWAGER_RESHAPE||shape==ShapeIndex::LINESMALL_SHAPE)
		kk=2;
	else if(shape==ShapeIndex::SQUARE_SHAPE||shape==ShapeIndex::POINT_SHAPE)
		kk=1;

	int unFillNumber=getBlankOfUnFill();

	//遍历所有方法
	for(int i=0;i<kk;i++)
	{
		angle=(AngleIndex)i;
		for(int i=0;i<CHESS_SIZE_W;i++)
		{
			for(int j=0;j<CHESS_SIZE_H;j++)
			{
				PointTip pt(shape,angle,make_pair(i,j));
				int key=PointTipJudge(pt);
				if(key==2)
					break;
				else if(key==1)
				{
					_pointTipVec.push_back(pt);
					break;
				}
				else if(key==3)
				{
					if(canPutIn(pt))
						_pointTipVec.push_back(pt);
				}
			}
		}
	}

	if(_pointTipVec.size()==0)
	{
		gameFiled();
		return;
	}

	//划分优先级
	vector<pair<int ,int >> tipVecWithPriority;
	int tempk=0;
	for(auto &i:_pointTipVec)
	{
		int value=0;
		auto boxfill=getBoxFill(i);
		//int newunFillNumber=getBlankOfUnFill(boxfill);
		//assert(newunFillNumber>=unFillNumber);
		//value+=(newunFillNumber-unFillNumber)*2;
		for(auto &j:boxfill)
		{
			//x-1,y
			if(j.first==0)
				value-=1;
			else if(_chessFilInfo[(j.first-1)][j.second])
				value-=2;
			//x+1,y
			if(j.first==(CHESS_SIZE_W-1))
				value-=1;
			else if(_chessFilInfo[(j.first+1)][j.second])
				value-=2;
			//x,y-1
			if(j.second==0)
				value-=1;
			else if(_chessFilInfo[j.first][j.second-1])
				value-=2;
			//x,y+1
			if(j.second<(CHESS_SIZE_H-1)&&_chessFilInfo[j.first][j.second+1])
				value-=3;

		}
		value-=getRemoveRow(i)*100;
		tipVecWithPriority.push_back(make_pair(tempk,value));
		tempk++;
	}

	//优先级排序
	int pronumber=tipVecWithPriority.size();
	for(int i=0;i<(pronumber-1);i++)
		for(int j=0;j<(pronumber-1-i);j++)
		{
			if(tipVecWithPriority[j].second>tipVecWithPriority[j+1].second)
			{
				std::swap(tipVecWithPriority[j],tipVecWithPriority[j+1]);
			}
			else if((tipVecWithPriority[j].second==tipVecWithPriority[j+1].second)&&(getButtonPosition(_pointTipVec[tipVecWithPriority[j].first])>getButtonPosition(_pointTipVec[tipVecWithPriority[j+1].first])))
			{
				std::swap(tipVecWithPriority[j],tipVecWithPriority[j+1]);
			}
		}

	for(auto &i:tipVecWithPriority)
		_pointTipPriority.push_back(i.first);

	//分组
	int pairnumber=1;
	vector<int> pairvec;
	do
	{
		_pointTipPairTemp.clear();
		pairvec.clear();
		for(auto i=tipVecWithPriority.begin();i!=tipVecWithPriority.end();)
		{
			if(pointTipPairTempJudge(i->first))
			{
				pairvec.push_back(i->first);
				i=tipVecWithPriority.erase(i);
				if(pairvec.size()>=4)
					break;
			}
			else
				i++;
		}

		//显示个数小于3，填充
		if(pairvec.size()<3)
		{
			for(unsigned int i=0;i<_pointTipVec.size();i++)
			{
				if(pointTipPairTempJudge(i))
				{
					pairvec.push_back(i);
					if(pairvec.size()>=4)
						break;
				}
			}
		}
		//

		_pointTipPair[pairnumber]=pairvec;
		pairnumber++;
	} while (tipVecWithPriority.size()>0);

	//显示
	_recentPointNumber=1;

#ifdef AUTO_MODEL
	moveBoxToId(_pointTipPair[1][0]);
#else
	showTipWithId(_recentPointNumber);
#endif
}

int SceneManage::getButtonPosition(PointTip tip)
{
	auto boxfill=BoxNode::getBoxFillByData(tip.shape,tip.angle);
	int buttonps=CHESS_SIZE_H-1;
	for(auto &i:boxfill)
	{
		if(i.second<buttonps)
			buttonps=i.second;
	}
	return buttonps;
}

bool SceneManage::canPutIn(PointTip tip)
{
	if(!getVecFromNoPutIn(tip))
	{
		PointTip recenttip;
		recenttip.shape=_recentBox;
		recenttip.angle=AngleIndex(0);
		recenttip.position=_recentPos;
		auto manage=PointManage::getInstance()->pathAToB(recenttip,tip,_chessFilInfo);
		if(manage->getJudge())
		{
			_pointPath.insert(make_pair(_pointTipVec.size(),manage->getData()));
			return true;
		}
		else
		{
			_noCanPutIn.push_back(tip);
			return false;
		}
	}
	else
		return false;
}

bool SceneManage::getVecFromPutIn(PointTip tip)
{
	vector<PointTip> tempvec;
	bool canput=false;
	for(auto &i:_pointPath)
	{
		for(auto &j:i.second)
		{
			if(j==tip)
			{
				tempvec.clear();
				tempvec.push_back(j);
				canput=true; 
			}
			else if(canput)
			{
				tempvec.push_back(j);
			}
		}
		if(canput)
		{
			_pointPath.insert(make_pair(_pointTipVec.size(),tempvec));
			return true;
		}
		else
		{
			auto temptip=i.second[0];
			if(temptip.shape==tip.shape&&temptip.angle==tip.angle)
			{
				if(getByLine(temptip,tip))
				{
					vector<PointTip> tempvec;
					tempvec.push_back(tip);
					for(auto &k:i.second)
						tempvec.push_back(k);
					_pointPath.insert(make_pair(_pointTipVec.size(),tempvec));
					return true;
				}
			}
		}
	}
	return false;
}

bool SceneManage::getVecFromNoPutIn(PointTip tip)
{
	bool canput=false;
	for(auto &i:_noCanPutIn)
	{
		if(getByLine(i,tip))
		{
			return true;
		}
	}
	return false;
}

bool SceneManage::getByLine(PointTip fronttip,PointTip targettip)
{
	bool temp=true;
	if(fronttip.position.second==targettip.position.second)
	{
		temp=true;
		int len=abs(fronttip.position.first-targettip.position.first);
		int kk=1;
		if(fronttip.position.first>targettip.position.first)
			kk=-1;
		auto fillvec=getBoxFill(fronttip);
		for(auto &i:fillvec)
		{
			for(int n=1;n<=len;n++)
			{
				if(_chessFilInfo[(i.first+kk*n)][i.second])
				{
					temp=false;
					break;
				} 
			}
			if(!temp)
				break;
		}
		if(temp)
			return true;
		else
			return false;
	}
	else if(fronttip.position.first==targettip.position.first)
	{
		temp=true;
		int len=abs(fronttip.position.second-targettip.position.second);
		int kk=1;
		if(fronttip.position.second>targettip.position.second)
			kk=-1;
		auto fillvec=getBoxFill(fronttip);
		for(auto &i:fillvec)
		{
			for(int n=1;n<=len;n++)
			{
				if(_chessFilInfo[i.first][i.second+n*kk])
				{
					temp=false;
					break;
				} 
			}
			if(!temp)
				break;
		}
		if(temp)
			return true;
		else
			return false;
	}
	else
	{
		return false;
	}
}

bool SceneManage::getByAngle(PointTip fronttip,PointTip targettip)
{
	auto frontfillvec=getBoxFill(fronttip);
	int buttony=CHESS_SIZE_H;
	int buttonx;
	for(auto &i:frontfillvec)
	{
		if(i.second<buttony)
		{
			buttony=i.second;
			buttonx=i.first;
		}
	}

	auto targetfillvec=getBoxFill(targettip);
	int targety=CHESS_SIZE_H;
	int targetx;
	for(auto &i:targetfillvec)
	{
		if(i.second<targety)
		{
			targety=i.second;
			targetx=i.first;
		}
	}

	if(targety>buttony)
		return false;

	int off=targetx-buttonx;
	int kk=1;
	if(off<0)
		kk=-1;
	for(int i=1;i<=abs(off);i++)
	{
		for(auto &k:frontfillvec)
		{
			if((k.second<CHESS_SIZE_H)&&(_chessFilInfo[(k.first+i*kk)][k.second]))
				return false;
		}
	}

	return true;



	return true;
}

bool SceneManage::pointTipPairTempJudge(int pairid)
{
	auto boxfill=getBoxFill(_pointTipVec[pairid]);
	for(auto &i:_pointTipPairTemp)
		for(auto &j:boxfill)
		{
			if(i.first==j.first)
				return false;
		}
	for(auto &i:boxfill)
	{
		_pointTipPairTemp.push_back(make_pair(i.first,i.second));
	}
	return true;
}

std::map<int,int> SceneManage::getTopHeight()
{
	//每一列的最高层
	std::map<int,int> topheight;
	for(int i=0;i<CHESS_SIZE_W;i++)
	{
		topheight[i]=-1;
		for(int j=(CHESS_SIZE_H-1);j>=0;j--)
		{
			if(_chessFilInfo[i][j])
			{
				topheight[i]=j;
				_toph[i]=j;
				break;
			}
		}
	}
	return topheight;
	//
}

//1:可以放置且是最高层 2.左右越界 3.可以放置不是最高层  4.不能放置  ??返回值为3还要判断是否可以到达
int SceneManage::PointTipJudge(PointTip pt)
{
	auto boxfill=getBoxFill(pt);   

	for(auto &i:boxfill)
	{
		if(i.first<0||i.first>=CHESS_SIZE_W||i.second>=CHESS_SIZE_H)
			return 2;
	}

	bool tempbool=false;
	for(auto &i:boxfill)
	{
		if(i.second<0||_chessFilInfo[i.first][i.second])
			return 4;
		else if(i.second==0||_chessFilInfo[i.first][i.second-1])
			tempbool=true;
	}

	if(!tempbool)
		return 4;

	bool temp=false;
	for(auto &i:boxfill)
	{
		if(_BlankOfUnFillVec.find(i)==_BlankOfUnFillVec.end())
		{
			temp=true;
			break;
		}
	}
	if(!temp)
		return 4;


	for(auto &i:boxfill)
	{
		if(i.second<=getTopHeight().at(i.first))
			return 3;
	}

	return 1;
	
}

int SceneManage::getBlankOfUnFill()
{
	int number=0;
	_BlankOfUnFillVec.clear();
	_toph=getTopHeight();
	for(int i=0;i<CHESS_SIZE_W;i++)
		for(int j=0;j<_toph.at(i);j++)
		{
			if(!_chessFilInfo[i*CHESS_SIZE_H+j])
			{
				_tempUnFillVec.clear();
				if(isBlankOfUnFill(make_pair(i,j)))
				{
					_BlankOfUnFillVec.insert(make_pair(i,j));
					number++;
				}
			}
		}
	return number;
}

int SceneManage::getBlankOfUnFill(vector<pair<int,int>> map)
{
	int number=0;
	for(auto &i:map)
	{
		if(i.first>=0&&i.first<CHESS_SIZE_W&&i.second>=0&&i.second<CHESS_SIZE_H)
			_chessFilInfo[i.first][i.second]=1;
	}
	_toph=getTopHeight();
	for(int i=0;i<CHESS_SIZE_W;i++)
	{
		for(int j=0;j<_toph.at(i);j++)
		{
			if(!_chessFilInfo[i][j])
			{
				_tempUnFillVec.clear();
				_tempUnFillVec.push_back(make_pair(i,j));
				if(isBlankOfUnFill(make_pair(i,j)))
					number++;
			}
		}
	}
	for(auto &i:map)
	{
		if(i.first>=0&&i.first<CHESS_SIZE_W&&i.second>=0&&i.second<CHESS_SIZE_H)
			_chessFilInfo[i.first][i.second]=0;
	}
	return number;
}

bool SceneManage::isBlankOfUnFill(pair<int,int> pt)
{
	if(((pt.first+1)<CHESS_SIZE_W)&&!_chessFilInfo[(pt.first+1)][pt.second]&&!tempUnFillJudeg(make_pair(pt.first+1,pt.second)))
	{
		_tempUnFillVec.push_back(make_pair(pt.first+1,pt.second));
		if(pt.second>_toph.at(pt.first+1))
			return false;
		else if(!isBlankOfUnFill(make_pair(pt.first+1,pt.second)))
			return false;
	}
	
	if((pt.first-1>=0)&&!_chessFilInfo[(pt.first-1)][pt.second]&&!tempUnFillJudeg(make_pair(pt.first-1,pt.second)))
	{
		_tempUnFillVec.push_back(make_pair(pt.first-1,pt.second));
		if(pt.second>_toph.at(pt.first-1))
			return false;
		else if(!isBlankOfUnFill(make_pair(pt.first-1,pt.second)))
			return false;
	}
	
	if((pt.second-1>=0)&&!_chessFilInfo[pt.first][pt.second-1]&&!tempUnFillJudeg(make_pair(pt.first,pt.second-1)))
	{
		_tempUnFillVec.push_back(make_pair(pt.first,pt.second-1));
		if((pt.second-1)>_toph.at(pt.first))
			return false;
		else if(!isBlankOfUnFill(make_pair(pt.first,pt.second-1)))
			return false;
	}
	
	if((pt.second+1<CHESS_SIZE_H)&&!_chessFilInfo[pt.first][pt.second+1]&&!tempUnFillJudeg(make_pair(pt.first,pt.second+1)))
	{
		_tempUnFillVec.push_back(make_pair(pt.first,pt.second+1));
		if((pt.second+1)>_toph.at(pt.first))
			return false;
		else if(!isBlankOfUnFill(make_pair(pt.first,pt.second+1)))
			return false;
	}

	return true;
}

bool SceneManage::tempUnFillJudeg(pair<int,int> vec)
{
	for(auto &i:_tempUnFillVec)
	{
		if(i==vec)
			return true;
	}
	return false;
}

void SceneManage::showTipWithId(int pairid)
{
	_PointTipLayer->removeAllChildren();
	auto idvec=_pointTipPair.at(pairid);
	for(auto &i:idvec)
	{
		auto pointTip=_pointTipVec[i];
		auto box=BoxLine::createLine(pointTip.shape,pointTip.angle);
		_PointTipLayer->addChild(box);
		box->setPosition(chessPos(pointTip.position.first,pointTip.position.second));
	}
}

void SceneManage::showTipWithBox(PointTip tip)
{
	_PointTipLayer->removeAllChildren();
	auto box=BoxLine::createLine(tip.shape,tip.angle);
	_PointTipLayer->addChild(box);
	box->setPosition(chessPos(tip.position.first,tip.position.second));
}

void SceneManage::moveBoxToId(int boxid)
{
	auto recentbox=(BoxNode*)_clip->getChildByName("recentbox");
	bool ontop=true;
	auto pointtip=_pointTipVec[boxid];
	auto fillbox=getBoxFill(pointtip);
	auto topheight=getTopHeight();
	for(auto &i:fillbox)
	{
		if(i.second<topheight.at(i.first))
			ontop=false;
	}
	if(ontop)
	{
		setGameState(STATE_STOP);
		pair<int,int> ofpos;
		ofpos.first=pointtip.position.first-_recentPos.first;
		ofpos.second=pointtip.position.second-_recentPos.second;
		auto move1=CCMoveBy::create(droptime,Vec2(CHESS_SIZE_SIZE*ofpos.first,0));
		auto call=CCCallFunc::create([recentbox,pointtip](){
			recentbox->setRotate(pointtip.angle);
		});
		auto move2=CCMoveBy::create(droptime,Vec2(0,ofpos.second*CHESS_SIZE_SIZE));
		auto ease2=EaseIn::create((ActionInterval*)move2,EASE_STRENGTH);
		recentbox->runAction(CCSequence::create(move1,call,ease2,CallFunc::create([](){
				cjMusic::playEffect("video/drop-Dekaron.mp3");
		}),CCDelayTime::create(CHESS_CHECK_TIME),CCCallFunc::create([recentbox,pointtip,this](){
			if(getGameState()==STATE_STOP)
				setGameState(STATE_RUN);
			_recentPos=pointtip.position;
			if(boxSure())
			{
				checkChess();
			}
			else
			{
				gameFiled();
			}
		}),nullptr));
	}
	else
	{
		_pointMoveP.clear();
		_pointMoveP=_pointPath[boxid];
		assert(_pointMoveP.size()>0);
		_pointMoveNumber=0;

		auto toptip=_pointMoveP[_pointMoveP.size()-1];

		setGameState(STATE_STOP);
		pair<int,int> ofpos;
		ofpos.first=toptip.position.first-_recentPos.first;
		ofpos.second=toptip.position.second-_recentPos.second;
		auto move1=CCMoveBy::create(droptime,Vec2(CHESS_SIZE_SIZE*ofpos.first,0));
		auto call=CCCallFunc::create([recentbox,toptip](){
			recentbox->setRotate(toptip.angle);
		});
		auto move2=CCMoveBy::create(droptime,Vec2(0,ofpos.second*CHESS_SIZE_SIZE));
		recentbox->runAction(CCSequence::create(move1,call,move2,CCCallFunc::create([recentbox,pointtip,this](){
			schedule(schedule_selector(SceneManage::PointMove));
		}),nullptr));
	}
}

void SceneManage::PointMove(float ft)
{
	auto ppp=_pointMoveP[_pointMoveP.size()-1-_pointMoveNumber];
	auto box=(BoxNode*)_clip->getChildByName("recentbox");
	box->setRotate(ppp.angle);

	_pointMoveNumber++;
	if(unsigned(_pointMoveNumber)>=_pointMoveP.size())
	{
		cjMusic::playEffect("video/drop-Dekaron.mp3");

		auto ease=EaseIn::create((ActionInterval*)CCMoveTo::create(1/60.0f,chessPos(ppp.position.first,ppp.position.second)),EASE_STRENGTH);
		box->runAction(ease);

		_recentPos=ppp.position;
		unschedule(schedule_selector(SceneManage::PointMove));
		this->runAction(CCSequence::create(CCDelayTime::create(CHESS_CHECK_TIME),CCCallFunc::create([this](){
			if(getGameState()==STATE_STOP)
				setGameState(STATE_RUN);
			if(boxSure())
			{
				checkChess();
			}
			else
			{
				gameFiled();
			}
		}
		),nullptr));
	}
	else
	{
		box->setPosition(chessPos(ppp.position.first,ppp.position.second));
	}
}

void SceneManage::textShow(const string& str,int size,Vec2 pos)
{
	auto text=cjTTFLabel::createWithString(str,size);
	_clip->addChild(text,CHESSMAN_LEVEL+1);
	text->setPosition(pos+Vec2(0,heightoffest*0.5f));
	text->runAction(CCSequence::create(CCScaleTo::create(0.5f,1.1f),CCScaleTo::create(0.5f,1.0f),CCCallFunc::create([text](){
		text->removeFromParent();
	}),nullptr));
}

void SceneManage::PointTipRemove()
{
	set<int> removetip;
	PointTip nowtip=PointTip(_recentBox,ANGLE_0,_recentPos);
	for(auto &i:_pointTipPair)
		for(auto j=i.second.begin();j!=i.second.end();)
		{
			PointTip tip;
			if(_pointPath.find(*j)!=_pointPath.end())
				tip=_pointPath.find(*j)->second[_pointPath.find(*j)->second.size()-1];
			else
				tip=_pointTipVec[*j];
			nowtip.angle=tip.angle;
			if(getByAngle(nowtip,tip))
				j++;
			else
			{
				for(auto k=_pointTipPriority.begin();k!=_pointTipPriority.end();k++)
				{
					if(*k==*j)
					{
						_pointTipPriority.erase(k);
						break;
					}
				}
				removetip.insert(*j);
				j=i.second.erase(j);
			}
		}
	if(removetip.size()>0)
	{
		if(_touchbox>0)
		{
			if(removetip.find(_touchbox)!=removetip.end())
			{
				_touchbox=-1;
				showTipWithId(_recentPointNumber);
			}
		}
		else
			showTipWithId(_recentPointNumber);
	}
}

void SceneManage::addParticle(int x,int y,int shapeid,int type)
{
	Color3B color=particleColor[shapeid];
	assert(shapeid < 10);
	string parstr;
	if (type == 1)
	{
		parstr = "lizi/liujiao/dan_boom.plist";
	}
	else if (type==2)
	{
		parstr = "lizi/liujiao/single_boom.plist";
	}
	auto par = CCParticleSystemQuad::create(parstr.c_str());
	this->addChild(par, 11);
	par->setPosition(chessPos(x,y));
	par->setStartColor(Color4F(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1.0f));
	par->setStartColorVar(Color4F(0.0f, 0.0f, 0.0f, 0.0f));
	par->setEndColor(Color4F(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1.0f));
	par->setEndColorVar(Color4F(0.0f, 0.0f, 0.0f, 0.0f));
	par->setAutoRemoveOnFinish(true);

	//
	auto ddd=cjSprite::createWithImage("backsp.png");
	this->addChild(ddd,12);
	ddd->setScale(CHESSMAN_SCALE);
	ddd->setPosition(chessPos(x,y));
	ddd->setOpacity(0);
	ddd->runAction(Sequence::create(FadeIn::create(0.15f),FadeOut::create(0.15f),CallFunc::create([ddd](){
		ddd->removeFromParent();
	}),nullptr));
	//
}

void SceneManage::spriteShow(const string& str)
{
	float sck=1.0f;
	if(str=="guoguan.png")
		sck=0.7f;

	auto spr=cjSprite::createWithImage(str);
	_clip->addChild(spr,CHESSMAN_LEVEL+1);
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

bool SceneManage::levelUp()
{
	if(getmScore()>=getmTarget())
	{
		setmLevel(getmLevel()+1);
		setmTarget(getmTarget()+500*getmLevel());
		
		dataSave();

		if(getmLevel()==4)
			vigame::ad::ADManager::openAd("level_win");

		unschedule(schedule_selector(SceneManage::boxDropUpdate));
		schedule(schedule_selector(SceneManage::boxDropUpdate),MAX(0.2f,(DROP_UPDATE_TIME+DROP_STOP_TIME)*(1.0f-getmLevel()/20.0f-0.1f*getmLevel()/5)));

		setGameState(STATE_STOP);

		cjMusic::playEffect("video/levelup.mp3");

		spriteShow("guoguan.png");
		runAction(CCSequence::create(CCDelayTime::create(sprintshowtime),CallFunc::create([this](){
			LevelUpEliminate();
		}),nullptr));
		return true;
	}
	return false;
}

void SceneManage::LevelUpMotion()
{
	if(getmLevel()==1)
		cjMusic::playEffect("video/ReadyGo.mp3");

	auto fnt1 = cjSprite::createWithImage("level_guanshu.png");
	_clip->addChild(fnt1, CHESSMAN_LEVEL + 1);
	fnt1->setCascadeOpacityEnabled(true);
	fnt1->setPosition(Vec2(WINSIZE / 2.0f) + Vec2(0, heightoffest*0.5f - 140));
	fnt1->runAction(Sequence::create(CCMoveBy::create(0.3f, Vec2(0, 200)), DelayTime::create(1.1f), FadeOut::create(0.3f), nullptr));

	auto fnt1num = cjAtlasLabel::createWithInt(getmLevel(), ATLAS_PATH2);
	fnt1->addChild(fnt1num);
	fnt1num->setScale(levelupscale);
	fnt1num->setCascadeOpacityEnabled(true);
	fnt1num->setPosition(fnt1->getContentSize() / 2.0f);

	auto fnt2 = cjSprite::createWithImage("level_mubiao.png");
	_clip->addChild(fnt2, CHESSMAN_LEVEL + 1);
	fnt2->setAnchorPoint(Vec2(0.5f, 1.0f));
	fnt2->setCascadeOpacityEnabled(true);
	fnt2->setPosition(fnt1->getPosition() - Vec2(0, 50));

	auto fnt2num = cjAtlasLabel::createWithInt(getmTarget(), ATLAS_PATH2);
	fnt2->addChild(fnt2num);
	fnt2num->setAnchorPoint(Vec2(0.0f, 0.5f));
	fnt2num->setScale(levelupscale + 0.1f);
	fnt2num->setCascadeOpacityEnabled(true);
	fnt2num->setPosition(fnt2->getContentSize().width + 10, fnt2->getContentSize().height / 2.0f);

	fnt2->setPosition(fnt2->getPosition() - Vec2(fnt2num->getContentSize().width / 2.0f + 10, 0));
	fnt2->runAction(Sequence::create(CCMoveBy::create(0.3f, Vec2(0, 200)), CCDelayTime::create(0.2f), ScaleTo::create(0.2f, 1.2f), ScaleTo::create(0.2f, 1.0f), CCDelayTime::create(0.5f), CCFadeOut::create(0.3f),
		CallFunc::create([fnt1, fnt2, this]() {
		fnt1->removeFromParent();
		fnt2->removeFromParent();

		if (getGameState() == STATE_STOP)
			setGameState(STATE_RUN);
		startDropBox();

	}), nullptr));
}

void SceneManage::LevelUpEliminate()
{
	auto eliminatebox=getLevelUpBox();

	levelupmusic=0;
	int key=0;
	if(eliminatebox.size()>0)
	{
		int eliminatenumber=4+rand()%3;
		do 
		{
			if(eliminatebox.empty())
			{
				eliminatebox.clear();
				eliminatebox=getLevelUpBox();
			}

			if(eliminatebox.size()>0)
			{
				int num=rand()%eliminatebox.size();
				auto pos=eliminatebox[num];
				eliminatebox.erase(eliminatebox.begin()+num);
				_chessFilInfo[pos.first][pos.second]=0;
				int kk;
				if(key<3)
					kk=1;
				else if(key==3)
					kk=2;
				runAction(Sequence::create(DelayTime::create(0.25f*MIN(3,key)),CallFunc::create([this,pos,kk](){
					addParticle(pos.first,pos.second,_chessSaveInfo[pos.first][pos.second]-1,2);
					removeBox(pos);
					char musicname[30];
					if(kk==1)
					{
						sprintf(musicname,"video/levelupbegin%d.mp3",levelupmusic%4);
						++levelupmusic;
					}
					else if(kk==2)
						sprintf(musicname,"video/levelupend.mp3");
					cjMusic::playEffect(musicname);
				}),nullptr));
			}
			else
				eliminatenumber=0;
			key++;
		} while (key<eliminatenumber);
	}
	runAction(Sequence::create(DelayTime::create(MIN(key,3)*0.25f+1.0f),CallFunc::create([this](){
		LevelUpMotion();
	}),nullptr));
}

void SceneManage::removeBox(pair<int,int> pos)
{
	_chessFilInfo[pos.first][pos.second]=0;
	_chessSaveInfo[pos.first][pos.second]=0;
	_clip->removeChildByTag(pos.first*CHESS_SIZE_H+pos.second);
}

void SceneManage::scoreMotion(int score,pair<int,int> pt)
{
	auto scorefnt=cjAtlasLabel::createWithInt(score);
	this->addChild(scorefnt,15);
	scorefnt->setScale(2.0f);
	scorefnt->setPosition(chessPos(pt.first,pt.second)+Vec2(-scorefnt->getContentSize().width,-20));
	scorefnt->runAction(Sequence::create(ScaleTo::create(0.1f,2.5f),ScaleTo::create(0.1f,2.0f),MoveTo::create(0.3f,WINORIGIN+Vec2(370,780)),CallFunc::create([scorefnt,score,this](){
		scorefnt->removeFromParent();
		_scoretime=0.0f;
		++_scorewav;
		char scorename[30];
		sprintf(scorename,"video/score%02d.mp3",MIN(_scorewav,12));
		//cjMusic::playEffect(scorename);
	}),nullptr));
	scorefnt->runAction(Sequence::create(DelayTime::create(0.2f),ScaleTo::create(0.3f,1.2f),nullptr));
} 

int SceneManage::getRemoveRow(PointTip pt)
{
	auto map=getBoxFill(pt);
	int k=0;
	for(int j=0;j<CHESS_SIZE_H;j++)
	{
		bool temp=true;
		for(int i=0;i<CHESS_SIZE_W;i++)
		{
			if(_chessFilInfo[i][j]==0&&(find(map.begin(),map.end(),make_pair(i,j))==map.end()))
			{
				temp=false;
				break;
			}
		}
		if(temp)
			++k;
	}
	return k;
}

vector<pair<int,int>> SceneManage::getLevelUpBox()
{
	vector<pair<int,int>> vec;
	for(int i=0;i<CHESS_SIZE_W;i++)
	{
		for(int j=(CHESS_SIZE_H-1);j>=0;j--)
		{
			if(_chessFilInfo[i][j])
			{
				vec.push_back(make_pair(i,j));
				break;
			}
		}
	}
	return vec;
}

void SceneManage::Rotate()
{

	if(getGameState()==STATE_RUN&&!_touchcan&&_recentBox>=0)
	{
		if(GameDataInstance()->d_Model==MODEL_POINT)
		{
			if(_guide&&_guidenumber<guidenumber)
				return;
			_recentPointNumber++;
			if(unsigned(_recentPointNumber)>_pointTipPair.size())
				_recentPointNumber=1;
			showTipWithId(_recentPointNumber);
		}
		else if(GameDataInstance()->d_Model==MODEL_SLIP)
		{
			if(_recentBox!=ShapeIndex::SQUARE_SHAPE&&_recentBox!=ShapeIndex::POINT_SHAPE)
				boxChange();
		}
	}
}

void SceneManage::Resurrection()
{
	for(int i=0;i<CHESS_SIZE_W;i++)
		for(int j=0;j<CHESS_SIZE_H;j++)
		{
			if(_chessFilInfo[i][j])
			{
				auto box=_clip->getChildByTag(i*CHESS_SIZE_H+j);
				box->setVisible(true);

				_clip->removeChildByTag(1000+i*CHESS_SIZE_H+j);
			}
		}

	vector<int> vec;
	for(int i=1;i<=4;i++)
		vec.push_back(i*2);
	float tt=rollEliminate(vec);
	setGameState(STATE_RUN);
	runAction(CCSequence::create(CCDelayTime::create(tt),CallFunc::create([this](){
		if(_model==MODEL_SLIP)
		{
			schedule(schedule_selector(SceneManage::boxDropTip),0.1f);
		}
		schedule(schedule_selector(SceneManage::boxDropUpdate),MAX(0.2f,(DROP_UPDATE_TIME+DROP_STOP_TIME)*(1.0f-getmLevel()/20.0f-0.1f*getmLevel()/5)));
		startDropBox();
	}),nullptr));
}

//经典模式指引
void SceneManage::classesGuide()
{
	runAction(Sequence::create(DelayTime::create(0.2f),CallFunc::create([this](){
		_guidenumber=4;
	}),nullptr));

	GameDataInstance()->_guide=true;

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

	auto sten=Node::create();
	bclayer->addChild(sten);

	auto draw=DrawNode::create();
	draw->drawSolidPoly(v1,4,Color4F(1.0f,1.0f,1.0f,0.0f));
	draw->drawPoly(v1,4,true,Color4F(1.0f,1.0f,1.0f,1.0f));
	draw->drawSolidPoly(v2,4,Color4F(1.0f,1.0f,1.0f,0.0f));
	draw->drawPoly(v2,4,true,Color4F(1.0f,1.0f,1.0f,1.0f));
	draw->drawSolidPoly(v3,4,Color4F(1.0f,1.0f,1.0f,0.0f));
	draw->drawPoly(v3,4,true,Color4F(1.0f,1.0f,1.0f,1.0f));

	draw->drawLine(Vec2(62,760),Vec2(62,700),Color4F(1,1,1,1));
	draw->drawLine(Vec2(187,760),Vec2(187,660),Color4F(1,1,1,1));
	draw->drawLine(Vec2(315,760),Vec2(315,700),Color4F(1,1,1,1));
	sten->addChild(draw);

	//font
	auto fnt1=LabelTTF::create(cjTTFLabel::getStringByKey("tiplevel"),TTF_PATH,32);
	fnt1->setAnchorPoint(Vec2(0.5f,1.0f));
	fnt1->setPosition(Vec2(62,700));
	bclayer->addChild(fnt1,2);

	auto fnt2=LabelTTF::create(cjTTFLabel::getStringByKey("tiptarget"),TTF_PATH,32);
	fnt2->setAnchorPoint(Vec2(0.5f,1.0f));
	fnt2->setPosition(Vec2(187,660));
	bclayer->addChild(fnt2,2);

	auto fnt3=LabelTTF::create(cjTTFLabel::getStringByKey("tipscore"),TTF_PATH,32);
	fnt3->setAnchorPoint(Vec2(0.5f,1.0f));
	fnt3->setPosition(Vec2(315,700));
	bclayer->addChild(fnt3,2);

	auto fnt4=LabelTTF::create(cjTTFLabel::getStringByKey("tipclasses1"),TTF_PATH,35);
	fnt4->setAnchorPoint(Vec2(0.5f,1.0f));
	fnt4->setPosition(Vec2(WINSIZE.width/2.0f,350));
	bclayer->addChild(fnt4,2);

	auto fnt5=LabelTTF::create(cjTTFLabel::getStringByKey("tipclasses2"),TTF_PATH,35);
	fnt5->setAnchorPoint(Vec2(0.5f,1.0f));
	fnt5->setPosition(Vec2(WINSIZE.width/2.0f,290));
	bclayer->addChild(fnt5,2);
	//

	//hand
	auto handnode=handNode::createHand(0.6f);
	bclayer->addChild(handnode);
	handnode->setPosition(chessPos(6,12));
	//

	auto node1=BoxNode::createBox(ShapeIndex::L_SHAPE);
	node1->setPosition(chessPos(5,12));
	sten->addChild(node1);
	//auto node2=BoxLine::createLine(ShapeIndex::L_SHAPE);
	//node2->setPosition(chessPos(5,0));
	//sten->addChild(node2);
	//node2->setScale(1.01f);
	auto node3=BoxNode::createBox(ShapeIndex::L_SHAPE);
	node3->setPosition(chessPos(5,0));
	node3->setOpacity(0);
	sten->addChild(node3);

	clipnode->setStencil(sten);

	clipnode->addChild(drawback);
	clipnode->setInverted(true);
}

//挑战模式指引
void SceneManage::challengeGuide()
{
	//return ;
	runAction(Sequence::create(DelayTime::create(0.2f),CallFunc::create([this](){
		_guidenumber=4;
	}),nullptr));

	GameDataInstance()->_guide=true;

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

	auto sten=Node::create();
	bclayer->addChild(sten);

	auto draw=DrawNode::create();
	draw->drawSolidPoly(v1,4,Color4F(1.0f,1.0f,1.0f,0.0f));
	draw->drawPoly(v1,4,true,Color4F(1.0f,1.0f,1.0f,1.0f));
	draw->drawSolidPoly(v2,4,Color4F(1.0f,1.0f,1.0f,0.0f));
	draw->drawPoly(v2,4,true,Color4F(1.0f,1.0f,1.0f,1.0f));
	draw->drawSolidPoly(v3,4,Color4F(1.0f,1.0f,1.0f,0.0f));
	draw->drawPoly(v3,4,true,Color4F(1.0f,1.0f,1.0f,1.0f));

	draw->drawLine(Vec2(62,760),Vec2(62,700),Color4F(1,1,1,1));
	draw->drawLine(Vec2(187,760),Vec2(187,660),Color4F(1,1,1,1));
	draw->drawLine(Vec2(315,760),Vec2(315,700),Color4F(1,1,1,1));
	sten->addChild(draw);

	//font
	auto fnt1=LabelTTF::create(cjTTFLabel::getStringByKey("tiplevel"),TTF_PATH,32);
	fnt1->setAnchorPoint(Vec2(0.5f,1.0f));
	fnt1->setPosition(Vec2(62,700));
	bclayer->addChild(fnt1,2);

	auto fnt2=LabelTTF::create(cjTTFLabel::getStringByKey("tiptarget"),TTF_PATH,32);
	fnt2->setAnchorPoint(Vec2(0.5f,1.0f));
	fnt2->setPosition(Vec2(187,660));
	bclayer->addChild(fnt2,2);

	auto fnt3=LabelTTF::create(cjTTFLabel::getStringByKey("tipscore"),TTF_PATH,32);
	fnt3->setAnchorPoint(Vec2(0.5f,1.0f));
	fnt3->setPosition(Vec2(315,700));
	bclayer->addChild(fnt3,2);

	auto fnt4=LabelTTF::create(cjTTFLabel::getStringByKey("tipchallenge1"),TTF_PATH,35);
	fnt4->setAnchorPoint(Vec2(0.5f,1.0f));
	fnt4->setPosition(Vec2(WINSIZE.width/2.0f,380));
	bclayer->addChild(fnt4,2);

	auto fnt5=LabelTTF::create(cjTTFLabel::getStringByKey("tipchallenge2"),TTF_PATH,35);
	fnt5->setAnchorPoint(Vec2(0.5f,1.0f));
	fnt5->setPosition(Vec2(WINSIZE.width/2.0f,320));
	bclayer->addChild(fnt5,2);
	//

	//hand
	auto handnode=handNode::createHand(0.6f);
	bclayer->addChild(handnode);
	handnode->setPosition(chessPos(4,1));
	//

	//方块
	auto node1=BoxNode::createBox(ShapeIndex::L_SHAPE);
	node1->setPosition(chessPos(5,12));
	sten->addChild(node1);
	//auto line1=BoxLine::createLine(ShapeIndex::L_SHAPE);
	//sten->addChild(line1);
	//line1->setPosition(chessPos(5,12));
	for(int i=0;i<3;i++)
	{
		auto node3=BoxNode::createBox(ShapeIndex::L_SHAPE);
		node3->setOpacity(0);
		sten->addChild(node3);
		auto line3=BoxLine::createLine(ShapeIndex::L_SHAPE);
		sten->addChild(line3);
		int k=0;
		if(i>0)
			k=1;
		node3->setPosition(chessPos(3+3*i,k));
		line3->setPosition(chessPos(3+3*i,k));
	}
	auto node3=BoxNode::createBox(ShapeIndex::L_SHAPE);
	node3->setRotate(ANGLE_90);
	node3->setPosition(chessPos(0,2));
	node3->setOpacity(0);
	sten->addChild(node3);

	auto line3=BoxLine::createLine(ShapeIndex::L_SHAPE,ANGLE_90);
	sten->addChild(line3);
	line3->setPosition(chessPos(0,2));
	//

	clipnode->setStencil(sten);



	clipnode->addChild(drawback);
	clipnode->setInverted(true);
}

//记录保存
void SceneManage::dataSave(bool tt)
{
	if(tt)
	{
		sdata._istip=true;
		for(int i=0;i<CHESS_SIZE_W;i++)
			for(int j=0;j<CHESS_SIZE_H;j++)
			{
				sdata._chessInfo[i][j]=_chessSaveInfo[i][j];
			}
			sdata._level=_level;
			sdata._score=_score;
			sdata._targetscore=_target;
			sdata._recentbox=_recentBox;
			sdata._recentposx=_recentPos.first;
			sdata._recentposy=_recentPos.second;
			for(int i=0;i<3;i++)
			{
				sdata._nextbox[i]=_nextBox[i];
			}
			sdata._guider=true;
	}
	else
	{
		sdata._istip=false;
		sdata._guider=true;
	}

	if(GameDataInstance()->d_Model==MODEL_POINT)
		GameData::getSaveData()->_challengesstetris=sdata;
	GameData::getInstance()->dataSave();

}
