#include "GameManage.h"
#include "BaseConfig.h"
#include "vigame_ad.h"

const int heightoffest=200;		//棋盘底的高度
const float selectboxsize=0.5f;	//可供选择方块的缩放比例
const float boxeliminatetime=0.2f;
const float chessfilltime=0.1f;

bool GameManage::init()
{
	if(!Layer::init())
		return false;

	selectboxoffest=GameDataInstance()->BANER_HEIGHT;	//可供选择方块的高度

	LOGGAME(cjTTFLabel::getNameByInt("banner: %d",selectboxoffest).c_str());

	initChess();

	_firstSaveIn=false;
	_guide=false;
	bool tempbool=false;
	if(!GameData::getSaveData()->_onezero._guider)
	{
		GameDataInstance()->_guide=true;
		_guide=true;
	}
	if(GameData::getSaveData()->_onezero._istip)
		tempbool=true;
	//初始化
	if(tempbool)
	{
		_firstSaveIn=true;
		sdata=GameData::getSaveData()->_onezero;
		for(int i=0;i<CHESS_SIZE_W_1010;i++)
			for(int j=0;j<CHESS_SIZE_H_1010;j++)
			{
				int key=sdata._chessInfo[i][j];
				_chessInfo.push_back((key>0)?true:false);
				_chessSaveInfo.push_back(key);

				if(key>0)
				{
					char name[30];
					sprintf(name,"fangkuai_%d.png",((key-1)%10)+1);
					auto box=cjSprite::createWithImage(name);
					_clip->addChild(box,CHESSMAN_LEVEL,i*CHESS_SIZE_H_1010+j);
					box->setPosition(positionByPos(make_pair(i,j)));
					box->setScale(CHESSMAN_SCALE_1010);
				};
			}
		for(int i=0;i<3;i++)
			_selectExit[i]=false;
		settouchState(TOUCH_TRUE);
		startNewRound();
		setmLevel(sdata._level);
		setmScore(sdata._score);
		setmTarget(sdata._target);
		_chesstippos=make_pair(-1,-1);
		_scoretime=0.0f;
		_scorewav=0;
	}
	else 
	{
		for(int i=0;i<CHESS_SIZE_W_1010;i++)
			for(int j=0;j<CHESS_SIZE_H_1010;j++)
			{
				_chessInfo.push_back(false);
				_chessSaveInfo.push_back(0);
			}
		for(int i=0;i<3;i++)
			_selectExit[i]=false;
		settouchState(TOUCH_TRUE);
		startNewRound();
		setmLevel(1);
		setmScore(0);
		setmTarget(500);
		_chesstippos=make_pair(-1,-1);
		_scoretime=0.0f;
		_scorewav=0;
	}
	LevelUpMotion();

	if(_guide)
	{
		for(int j=0;j<2;j++)
			for(int i=0;i<CHESS_SIZE_W_1010;i++)
			{
				if((j==0&&i>2)||(j==1&&(i==0||i>1)))
				{
					char name[30];
					int k=rand()%10+1;
					sprintf(name,"fangkuai_%d.png",k);
					auto box=cjSprite::createWithImage(name);
					_clip->addChild(box,CHESSMAN_LEVEL,i*CHESS_SIZE_H_1010+j);
					box->setPosition(positionByPos(make_pair(i,j)));
					box->setScale(CHESSMAN_SCALE_1010);
					_chessInfo[i*CHESS_SIZE_H_1010+j]=true;
					_chessSaveInfo[i*CHESS_SIZE_H_1010+j]=k;
				}
			}
	}

	auto listener=EventListenerTouchOneByOne::create();
	listener->onTouchBegan=CC_CALLBACK_2(GameManage::onTouchBegan,this);
	listener->onTouchMoved=CC_CALLBACK_2(GameManage::onTouchMoved,this);
	listener->onTouchEnded=CC_CALLBACK_2(GameManage::onTouchEnded,this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
	listener->setSwallowTouches(true);

	schedule(schedule_selector(GameManage::timeUpdate));

	return true;
}

void GameManage::start()
{
	setGameState(STATE_RUN);
}
void GameManage::pause()
{
	setGameState(STATE_PAUSE);
}
void GameManage::run()
{
	setGameState(STATE_RUN);
}

void GameManage::initChess()
{
	Vec2 vec[4];	
	vec[0]=WINORIGIN+Vec2(WINSIZE.width/2.0f-CHESSMAN_SIZE_1010*CHESS_SIZE_W_1010/2,0+heightoffest);
	vec[1]=WINORIGIN+Vec2(WINSIZE.width/2.0f+CHESSMAN_SIZE_1010*CHESS_SIZE_W_1010/2,0+heightoffest);
	vec[2]=WINORIGIN+Vec2(WINSIZE.width/2.0f+CHESSMAN_SIZE_1010*CHESS_SIZE_W_1010/2,(CHESS_SIZE_H_1010)*CHESSMAN_SIZE_1010+heightoffest);
	vec[3]=WINORIGIN+Vec2(WINSIZE.width/2.0f-CHESSMAN_SIZE_1010*CHESS_SIZE_W_1010/2,(CHESS_SIZE_H_1010)*CHESSMAN_SIZE_1010+heightoffest);

	auto node=DrawNode::create();
	this->addChild(node,10);
	node->drawSolidPoly(vec,4,Color4F(1.0f,1.0f,1.0f,0.0f));

	_clip=ClippingNode::create(node);
	this->addChild(_clip,10);
	_clip->setInverted(false);

	for(int i=0;i<CHESS_SIZE_W_1010;i++)
		for(int j=0;j<CHESS_SIZE_H_1010;j++)
		{
			auto sp=cjSprite::createWithImage("fangkuai_hs.png");
			this->addChild(sp);
			sp->setScale(CHESSMAN_SCALE_1010);
			sp->setPosition(WINORIGIN+Vec2(WINSIZE.width/2.0f+CHESSMAN_SIZE_1010*(i-(CHESS_SIZE_W_1010-1)/2.0f),(j+0.5f)*CHESSMAN_SIZE_1010+heightoffest));
		}
}

void GameManage::timeUpdate(float ft)
{
	if(getGameState()!=STATE_RUN)
		return ;
	_scoretime+=ft;
	if(_scoretime>scorevideotime)
	{
		_scorewav=0;
	}
}

void GameManage::startNewRound(int key)
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
				ShapeIndex shape;
				if(selectBoxExit(ShapeIndex::SQUAREBIG_SHAPE))
					shape=ShapeIndex(rand()%10);
				else
					shape=ShapeIndex(rand()%13);
				if(key==2&&i==1)
					shape=ShapeIndex::POINT_SHAPE;
				AngleIndex angle;
				if(shape==POINT_SHAPE||shape==ShapeIndex::SQUARE_SHAPE||shape==ShapeIndex::SQUAREBIG_SHAPE)
					angle=AngleIndex::ANGLE_0;
				else if(shape==ShapeIndex::LINE_SHAPE||shape==ShapeIndex::LINESMALL_SHAPE)
					angle=AngleIndex(rand()%2);
				else
					angle=AngleIndex(rand()%4);

				if(_guide&&i==0)
				{
					shape=ShapeIndex::T_SHAPE;
					angle=AngleIndex::ANGLE_0;
				}
				else if(_firstSaveIn)
				{
					shape=(ShapeIndex)sdata._boxshape[i];
					angle=(AngleIndex)sdata._boxangle[i];
				}

				auto selectBox=BoxNode::createBox(shape);
				selectBox->setScale(selectboxsize);
				this->addChild(selectBox,11,1000+i);
				selectBox->setRotate(angle);
				selectBox->setPosition(selectBox->getCenterAnPointX(Vec2(nextboxbegin+nextboxoffest*i,selectboxoffest))+Vec2(WINSIZE.width,0));
				//selectBox->setPosition(Vec2(nextboxbegin+nextboxoffest*i,selectboxoffest)+Vec2(WINSIZE.width,0));
				selectBox->runAction(CCMoveBy::create(startnexttime,Vec2(-WINSIZE.width,0)));
				_selectExit[i]=true;
			}
		}
	}

	if(_firstSaveIn)
		_firstSaveIn=false;

	if(_guide)
	{
		runAction(Sequence::create(DelayTime::create(2.0),CallFunc::create([this](){
			dragGuide();
		}),nullptr));
	}
}

bool GameManage::selectBoxExit(ShapeIndex shape,AngleIndex angle/* =ANGLE_0 */)
{
	for(int i=0;i<3;i++)
	{
		if(_selectExit[i])
		{
			auto box=(BoxNode*)this->getChildByTag(1000+i);
			if(box->_shape==shape&&box->getAngle()==angle)
				return true;
		}
	}
	return false;
}

float GameManage::chessCheck()
{
	std::vector<int> eliminatex;
	std::vector<int> eliminatey;
	for(int i=0;i<CHESS_SIZE_W_1010;i++)
	{
		bool temp=true;
		for(int j=0;j<CHESS_SIZE_H_1010;j++)
		{
			if(!_chessInfo[i*CHESS_SIZE_H_1010+j])
				temp=false;
		}
		if(temp)
			eliminatex.push_back(i);
	}
	for(int j=0;j<CHESS_SIZE_H_1010;j++)
	{
		bool temp=true;
		for(int i=0;i<CHESS_SIZE_W_1010;i++)
		{
			if(!_chessInfo[i*CHESS_SIZE_H_1010+j])
				temp=false;
		}
		if(temp)
			eliminatey.push_back(j);
	}

	if(!eliminatex.empty()||!eliminatey.empty())
	{
		float time=eliminateAnimation(eliminatex,eliminatey);
		return time;
	}
	else
		return 0.0f;
}

void GameManage::removeBox(pair<int,int> pos)
{
	_clip->removeChildByTag(pos.first*CHESS_SIZE_H_1010+pos.second);
	_chessInfo[pos.first*CHESS_SIZE_H_1010+pos.second]=0;
	_chessSaveInfo[pos.first*CHESS_SIZE_H_1010+pos.second]=0;
}

float GameManage::eliminateAnimation(const std::vector<int> &vx,const std::vector<int> &vy)
{
	float ftime = 0.02f;
	int orign=_score;
	Vector<FiniteTimeAction*> action;
	if(!vx.empty())
	{
		for(auto &i:vx)
		{
			for(int j=0;j<CHESS_SIZE_H_1010;j++)
			{
				if(_chessInfo[i*CHESS_SIZE_H_1010+j])
				{
					if(i!=0)
						action.pushBack(DelayTime::create(0.02f));
					action.pushBack(CallFunc::create([this,i,j](){
						addParticle(i,j,_chessSaveInfo[i*CHESS_SIZE_H_1010+j],1);
						removeBox(make_pair(i,j));
					}));
					_score+=CHESS_SCORE;
					ftime += 0.02f;
				}
			}
		}
	}
	if(!vy.empty())
	{
		for(auto &j:vy)
		{
			for(int i=0;i<CHESS_SIZE_W_1010;i++)
			{
				if(std::find(vx.begin(),vx.end(),i)==vx.end())
				{
					if(i!=0)
						action.pushBack(DelayTime::create(0.02f));
					action.pushBack(CallFunc::create([this,i,j](){
						addParticle(i,j,_chessSaveInfo[i*CHESS_SIZE_H_1010+j],1);
						removeBox(make_pair(i,j));
					}));
					_score+=CHESS_SCORE;
					ftime += 0.02f;
				}
			}
		}
	}

	if(action.size()>0)
		runAction(Sequence::create(action));

	if(!vx.empty()||!vy.empty())
	{
		cjMusic::playEffect("video/eliminate.mp3");
		if(_score<_target||(_score-orign)>=_target)
		{
			if((vx.size()+vy.size())==2)
			{
				spriteShow("zan.png");
				cjMusic::playEffect("video/zan.mp3");
			}
			else if((vx.size()+vy.size())>2)
			{
				spriteShow("great.png");
				cjMusic::playEffect("video/great.mp3");
			}
		}
	}
	return ftime;
}

bool GameManage::onTouchBegan(Touch *touch, Event *unused_event)
{
	if(getGameState()!=STATE_RUN||gettouchState()==TOUCH_FALSE||_touchselect>0)
		return false;

	_touchselect=0;
 	auto startpos=touch->getLocation(); 

	//test
	//auto pos = posByPosition(startpos);
	//if (_chessInfo[pos.first*CHESS_SIZE_H_1010 + pos.second])
	//	addParticle(pos.first, pos.second, _chessSaveInfo[pos.first*CHESS_SIZE_H_1010 + pos.second], 1);
	//return false;
	//

	if(startpos.y>0&&startpos.y<heightoffest&&startpos.x)
	{
		int k=((startpos.x-nextboxbegin)/nextboxoffest+0.5f);
		if(k>=0&&k<3&&_selectExit[k])
		{
			if(_guide)
			{
				if(k==0)
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
			auto box=(BoxNode *)this->getChildByTag(1000+k);
			if(box->_boxcolor==BoxColor::BOX_NORMAL)
			{
				box->stopAllActions();
				box->setPosition(box->getCenterAnPoint(startpos)+Vec2(0,boxtofiger));
				box->setScale(1.0f);
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

void GameManage::onTouchMoved(Touch *touch, Event *unused_event)
{
	if(_touchselect<=0||gettouchState()==TOUCH_FALSE)
		return;

	auto recentpos=touch->getLocation();

	auto selectbox=(BoxNode*)this->getChildByTag(1000+_touchselect-1);
	selectbox->setPosition(selectbox->getCenterAnPoint(recentpos)+Vec2(0,boxtofiger));

	showTipBox(selectbox);
}

void GameManage::onTouchEnded(Touch *touch, Event *unused_event)
{
	if(_touchselect<=0||gettouchState()==TOUCH_FALSE)
		return;
	removeTipBox();
	settouchState(TOUCH_FALSE);
	auto box=(BoxNode *)this->getChildByTag(1000+_touchselect-1);
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
			GameData::getSaveData()->_onezero._guider=true;
			GameData::getInstance()->dataSave();
		}

		//放置成功
		cjMusic::playEffect("video/drop-drag.mp3");
		box->runAction(Sequence::create(CCMoveTo::create(0.01f,positionByPos(fillvec[0])),CallFunc::create([this,box,fillvec](){
			box->removeFromParent();
			_selectExit[_touchselect-1]=false;
			startNewRound();
			_touchselect=0;
			chessFill(box->_shape,fillvec);
			runAction(Sequence::create(DelayTime::create(chessfilltime),CallFunc::create([this](){
				float time=chessfilltime+chessCheck();
				runAction(Sequence::create(DelayTime::create(time),CallFunc::create([this](){
					BoxFillJudge();
					faildJudge();
				}),nullptr));
			}),nullptr));
		}),nullptr));
	}
}

void GameManage::Rotate(int key)
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
}

void GameManage::Resurrection()
{
	for(int i=0;i<CHESS_SIZE_W_1010;i++)
		for(int j=0;j<CHESS_SIZE_H_1010;j++)
		{
			if(_chessInfo[i*CHESS_SIZE_H_1010+j])
			{
				auto box=_clip->getChildByTag(i*CHESS_SIZE_H_1010+j);
				box->setVisible(true);

				_clip->removeChildByTag(1000+i*CHESS_SIZE_H_1010+j);
			}
		}

	vector<pair<int,int>> selectvec;
	for(int i=0;i<CHESS_SIZE_W_1010;i++)
		for(int j=0;j<CHESS_SIZE_H_1010;j++)
		{
			if(_chessInfo[i*CHESS_SIZE_H_1010+j])
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

		levelupmusic=0;
		int key=0;
		for(auto &i:eliminatebox)
		{
			auto pos=i;
			_chessInfo[pos.first*CHESS_SIZE_H_1010+pos.second]=0;
			runAction(Sequence::create(DelayTime::create(0.25f*MIN(3,key)),CallFunc::create([this,pos,key](){
				addParticle(pos.first,pos.second,_chessSaveInfo[pos.first*CHESS_SIZE_H_1010+pos.second],2);
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

//方块可填充判断
void GameManage::BoxFillJudge()
{
	for(int i=0;i<3;i++)
	{
		if(_selectExit[i])
		{
			auto box=(BoxNode*)getChildByTag(1000+i);
			if(BoxJudge(box))
				box->setNormal();
			else
				box->setGray();
		}
	}
}

//单独判断
bool GameManage::BoxJudge(BoxNode *node)
{
	auto fillvec=node->getBoxFill();
	for(int i=0;i<CHESS_SIZE_W_1010;i++)
		for(int j=0;j<CHESS_SIZE_H_1010;j++)
		{
			bool temp=true;
			for(auto &k:fillvec)
			{
				int kx=k.first+i;
				int ky=k.second+j;
				if(kx>=0&&kx<CHESS_SIZE_W_1010&&ky>=0&&ky<CHESS_SIZE_H_1010)
				{
					if(_chessInfo[kx*CHESS_SIZE_H_1010+ky])
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

//得到选中方块覆盖的位置
vector<pair<int,int>> GameManage::getFillBlack(BoxNode *node,Vec2 pt)
{
	vector<pair<int,int>> vec;
	auto centerpos=posByPosition(pt);
	float centerx=centerpos.first;
	float centery=centerpos.second;
	if(centerx<0||centery<0||centerx>float(CHESS_SIZE_W_1010)||centery>float(CHESS_SIZE_H_1010))
	{
		//放置失败
	}
	else
	{
		int px=int(centerx);
		int py=int(centery);
		auto fillvec=node->getBoxFill();
		for(auto &i:fillvec)
		{
			i.first+=px;
			i.second+=py;
			if(i.first<0||i.first>=CHESS_SIZE_W_1010||i.second<0||i.second>=CHESS_SIZE_H_1010||_chessInfo[i.first*CHESS_SIZE_H_1010+i.second])
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

Vec2 GameManage::positionByPos(pair<int,int> pos)
{
	return WINORIGIN+Vec2(WINSIZE.width/2.0f+(pos.first-(CHESS_SIZE_W_1010-1)/2.0f)*CHESSMAN_SIZE_1010,heightoffest+(0.5f+pos.second)*CHESSMAN_SIZE_1010);
}

pair<int,int> GameManage::posByPosition(Vec2 pt)
{
	float centerx=((pt.x-WINORIGIN.x)-WINSIZE.width/2.0f)/CHESSMAN_SIZE_1010+CHESS_SIZE_W_1010/2.0f;
	float centery=(pt.y-heightoffest)/CHESSMAN_SIZE_1010;
	if(centerx<0)
		centerx-=1;
	if(centery<0)
		centery-=1;
	return make_pair(int(centerx),int(centery));
}

void GameManage::chessFill(ShapeIndex shape,vector<pair<int,int>> vec)
{
	int score=_score;
	for(auto &i:vec)
	{
		char name[30];
		sprintf(name,"fangkuai_%d.png",int(shape%10)+1);
		auto box=cjSprite::createWithImage(name);
		_clip->addChild(box,CHESSMAN_LEVEL,i.first*CHESS_SIZE_H_1010+i.second);
		box->setPosition(positionByPos(i));
		box->setScale(CHESSMAN_SCALE_1010);
		_chessInfo[i.first*CHESS_SIZE_H_1010+i.second]=true;
		_chessSaveInfo[i.first*CHESS_SIZE_H_1010+i.second]=int(shape%10)+1;
		box->runAction(Sequence::create(ScaleTo::create(0.03f,0.97f*CHESSMAN_SCALE_1010),ScaleTo::create(0.03f,1.03f*CHESSMAN_SCALE_1010),ScaleTo::create(0.03f,1.0f*CHESSMAN_SCALE_1010),nullptr));
		_score+=CHESS_SCORE;
	}
	this->runAction(Sequence::create(DelayTime::create(0.2f),CallFunc::create([score,this,vec](){
		scoreMotion(_score-score,vec[0]);
	}),nullptr));
}

bool GameManage::faildJudge()
{
	if(chessFailedJudge())
	{
		runAction(Sequence::create(DelayTime::create(0.2f),CallFunc::create([this](){
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

void GameManage::gameFaild()
{
	for(int i=0;i<CHESS_SIZE_W_1010;i++)
		for(int j=0;j<CHESS_SIZE_H_1010;j++)
		{
			if(_chessInfo[i*CHESS_SIZE_H_1010+j])
			{
				runAction(Sequence::create(DelayTime::create(j*boxgraytime),CallFunc::create([i,j,this](){
					auto box=_clip->getChildByTag(i*CHESS_SIZE_H_1010+j);
					box->setVisible(false);

					auto tempbox=cjSprite::createWithImage("fangkuai_gray.png");
					_clip->addChild(tempbox,CHESSMAN_LEVEL,1000+i*CHESS_SIZE_H_1010+j);
					tempbox->setPosition(box->getPosition());
					tempbox->setScale(CHESSMAN_SCALE_1010);
				}),nullptr));
			}
		}

		runAction(Sequence::create(DelayTime::create(CHESS_SIZE_H_1010*boxgraytime),CallFunc::create([this](){
			setGameState(STATE_FAILD);
		}),nullptr));
		//setGameState(STATE_FAILD);
}

bool GameManage::chessFailedJudge()
{
	for(int i=0;i<3;i++)
	{
		if(_selectExit[i])
		{
			auto box=(BoxNode*)this->getChildByTag(1000+i);
			if(box->_boxcolor==BoxColor::BOX_NORMAL)
				return false;
		}
	}
	return true;
}

void GameManage::spriteShow(const string& str)
{
	auto spr=cjSprite::createWithImage(str);
	_clip->addChild(spr,CHESSMAN_LEVEL+1);
	spr->setPosition(Vec2(WINSIZE/2.0f)+Vec2(0,heightoffest*0.5f));
	spr->setScale(1.4f);
	spr->runAction(CCSequence::create(FadeOut::create(0.08f),FadeIn::create(0.08f),FadeOut::create(0.08f),FadeIn::create(0.08f),FadeOut::create(0.08f),FadeIn::create(0.08f),
		ScaleTo::create(0.2f,1.0f),
		DelayTime::create(0.52f),
		FadeOut::create(0.3f),
		CallFunc::create([spr](){
			spr->removeFromParent();
	}),nullptr));
}

void GameManage::addParticle(int x,int y,int shapeid,int type)
{
	Color3B color= particleColor[shapeid - 1];
	string parstr;
	if (type == 1)
	{
		parstr = "lizi/liujiao/dan_boom.plist";
	}
	else if (type == 2)
	{
		parstr = "lizi/liujiao/single_boom.plist";
	}
	auto par=CCParticleSystemQuad::create(parstr.c_str());
	this->addChild(par,11);
	par->setPosition(positionByPos(make_pair(x,y)));
	par->setStartColor(Color4F(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1.0f));
	par->setStartColorVar(Color4F(0.0f, 0.0f, 0.0f, 0.0f));
	par->setEndColor(Color4F(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1.0f));
	par->setEndColorVar(Color4F(0.0f, 0.0f, 0.0f, 0.0f));
	par->setAutoRemoveOnFinish(true);

	//
	auto ddd=cjSprite::createWithImage("backsp.png");
	this->addChild(ddd,12);
	ddd->setScale(CHESSMAN_SCALE_1010);
	ddd->setPosition(positionByPos(make_pair(x,y)));
	ddd->setOpacity(0);
	ddd->runAction(Sequence::create(FadeIn::create(0.15f),FadeOut::create(0.15f),CallFunc::create([ddd](){
		ddd->removeFromParent();
	}),nullptr));
	//
}

void GameManage::showTipBox(BoxNode *box)
{
	auto fillvec=getFillBlack(box,box->getPosition());
	if(!fillvec.empty())
	{
		if(fillvec[0]!=_chesstippos)
		{
			_clip->removeChildByTag(2000);
			_chesstippos=fillvec[0];
			auto drawnode=BoxLine::createLine(box->_shape,box->getAngle());
			_clip->addChild(drawnode,CHESSMAN_LEVEL);
			drawnode->setTag(2000);
			drawnode->setPosition(positionByPos(fillvec[0]));
		}
	}
	else
	{
		_clip->removeChildByTag(2000);
		_chesstippos=posByPosition(box->getPosition());
	}
}

void GameManage::removeTipBox()
{
	_clip->removeChildByTag(2000);
	_chesstippos=make_pair(-1,-1);
}
//分数动画
void GameManage::scoreMotion(int score,pair<int,int> pt)
{
	auto scorefnt=cjAtlasLabel::createWithInt(score);
	this->addChild(scorefnt,15);
	scorefnt->setScale(2.0f);
	scorefnt->setPosition(positionByPos(pt)+Vec2(-scorefnt->getContentSize().width,-20));
	scorefnt->runAction(Sequence::create(ScaleTo::create(0.1f,2.5f),ScaleTo::create(0.1f,2.0f),MoveTo::create(0.3f,WINORIGIN+Vec2(370,780)),CallFunc::create([scorefnt,score,this](){
		scorefnt->removeFromParent();
		_scoretime=0.0f;
		++_scorewav;
		char scorename[30];
		sprintf(scorename,"video/score%02d.mp3",MIN(_scorewav,12));
		//cjMusic::playEffect(scorename);
	}),nullptr));
	scorefnt->runAction(Sequence::create(DelayTime::create(0.2f),ScaleTo::create(0.3f,1.0f),nullptr));
}

bool GameManage::levelUp()
{
	if(getmScore()>=getmTarget()&&getmScore()>0)
	{
		setmLevel(getmLevel()+1);
		setmTarget(getmTarget()+1000);

		if(getmLevel()==4)
			vigame::ad::ADManager::openAd("level_win");

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

void GameManage::LevelUpMotion()
{
	BoxFillJudge();
	settouchState(TOUCH_TRUE);

	dataSave();

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
		//setGameState(STATE_RUN);
	}), nullptr));
}

void GameManage::LevelUpEliminate()
{
	auto eliminatebox=getLevelUpBox();

	levelupmusic=0;
	int key=0;
	for(auto &i:eliminatebox)
	{
		auto pos=i;
		_chessInfo[pos.first*CHESS_SIZE_H_1010+pos.second]=0;
		runAction(Sequence::create(DelayTime::create(0.25f*MIN(3,key)),CallFunc::create([this,pos,key](){
			addParticle(pos.first,pos.second,_chessSaveInfo[pos.first*CHESS_SIZE_H_1010+pos.second],2);
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
	}),nullptr));
}

vector<pair<int,int>> GameManage::getLevelUpBox()
{
	vector<pair<int,int>> selectvec;
	for(int i=0;i<CHESS_SIZE_W_1010;i++)
		for(int j=0;j<CHESS_SIZE_H_1010;j++)
		{
			if(_chessInfo[i*CHESS_SIZE_H_1010+j])
				selectvec.push_back(make_pair(i,j));
		}

	vector<pair<int,int>> vec;
	int number=MIN(6,4+getmLevel()/3);
	number=MIN(number,selectvec.size());
	for(int i=0;i<number;i++)
	{
		int num=rand()%selectvec.size();
		vec.push_back(selectvec[num]);
		selectvec.erase(selectvec.begin()+num);
	}
	return vec;
}

//拖动模式指引
void GameManage::dragGuide()
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

	auto sten=Node::create();
	bclayer->addChild(sten,1,"sten");

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
	auto node1=BoxNode::createBox(ShapeIndex::T_SHAPE);
	node1->setPosition(node1->getCenterAnPointX(Vec2(nextboxbegin,selectboxoffest))-Vec2(0,14));
	node1->setScale(selectboxsize);
	sten->addChild(node1,1,"node1");

	//hand
	auto handnode=Node::create();
	bclayer->addChild(handnode,1);
	auto hand2=cjSprite::createWithImage("ybb.png");
	hand2->setAnchorPoint(Vec2(0.15f,0.9f));
	auto seq3=Sequence::createWithTwoActions(DelayTime::create(0.5f),MoveTo::create(1.0f,positionByPos(make_pair(1,0))));
	auto seq4=Sequence::createWithTwoActions(DelayTime::create(0.5f),MoveTo::create(0.0f,node1->getPosition()));
	//hand2->runAction(RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(seq3,seq4)));
	handnode->addChild(hand2);
	handnode->setPosition(node1->getPosition());
	handnode->runAction(RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(seq3,seq4)));
	handnode->setScale(0.6f);
	//

	auto node2=BoxNode::createBox(ShapeIndex::T_SHAPE);
	node2->setPosition(positionByPos(make_pair(1,0)));
	node2->setOpacity(0);
	auto node3=BoxLine::createLine(ShapeIndex::T_SHAPE);
	node3->setPosition(positionByPos(make_pair(1,0)));
	sten->addChild(node3);
	sten->addChild(node2);
	//

	clipnode->setStencil(sten);


	clipnode->addChild(drawback);
	clipnode->setInverted(true);
}

//记录保存
void GameManage::dataSave(bool tt)
{
	if(tt)
	{
		GameData::getSaveData()->_onezero._istip=true;
		for(int i=0;i<CHESS_SIZE_W_1010;i++)
			for(int j=0;j<CHESS_SIZE_H_1010;j++)
				GameData::getSaveData()->_onezero._chessInfo[i][j]=_chessSaveInfo[i*CHESS_SIZE_H_1010+j];
		GameData::getSaveData()->_onezero._level=getmLevel();
		GameData::getSaveData()->_onezero._score=getmScore();
		GameData::getSaveData()->_onezero._target=getmTarget();
		for(int i=0;i<3;i++)
		{
			auto box=(BoxNode*)getChildByTag(1000+i);
			GameData::getSaveData()->_onezero._boxshape[i]=box->_shape;
			GameData::getSaveData()->_onezero._boxangle[i]=box->getAngle();
		}
	}
	else
	{
		GameData::getSaveData()->_onezero._istip=false;
	}
	GameData::getInstance()->dataSave();
}