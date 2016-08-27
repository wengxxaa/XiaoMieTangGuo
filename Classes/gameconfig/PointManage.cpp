#include"PointManage.h"

static int ccc=0;

//bool PointManage::getAToB(PointTip begintip,PointTip endtip,std::map<int,int> chessinfo)
//{
//	dataInit(begintip,endtip,chessinfo);
//	dataBoolProcess();
//	return _canGet;
//}

const PointManage* PointManage::pathAToB(PointTip begintip,PointTip endtip,int chessinfo[CHESS_SIZE_W][CHESS_SIZE_H])
{
	dataInit(begintip,endtip,chessinfo);
	ccc=0;
	dataProces();
	CCLOG("%d",ccc);
	return this;
}

void PointManage::dataInit(PointTip begintip,PointTip endtip,int chessinfo[CHESS_SIZE_W][CHESS_SIZE_H])
{
	_canGet=false;
	_BoxMoveVec.clear();
	_beginTip=begintip;
	_endTip=endtip;
	for(int i=0;i<CHESS_SIZE_W;i++)
		for(int j=0;j<CHESS_SIZE_H;j++)
			_chessFilInfo[i*CHESS_SIZE_H+j]=chessinfo[i][j];

	_theTopTipVec.clear();
	_theTopStep=INT_MAX;

	for(int i=0;i<CHESS_SIZE_W;i++)
	{
		_toph[i]=-1;
		for(int j=(CHESS_SIZE_H-1);j>0;j--)
		{
			if(_chessFilInfo[i*CHESS_SIZE_H+j])
			{
				_toph[i]=j;
				break;
			}
		}
	}
}

void PointManage::dataProces()
{
	_BoxPathVec.clear();
	_BoxPathVec.push_back(BoxMovePath(_endTip,_endTip,0));
	//boxMove(_beginTip,1);
	boxMoveProcessOne(_endTip,1);
	if(topData())
	{
		getPathByVecOne();
		//_BoxMoveVec.push_back(_beginTip);
		_canGet=true;
	}
	//else
	//{
	//	boxMove(_beginTip,1);
	//	getPathByVec();
	//}
}

bool PointManage::topData()
{
	if(_theTopTipVec.size()==0)
		return false;
	else
	{
		for(auto &i:_theTopTipVec)
		{
			if(i.first<_theTopStep)
			{
				_theTopStep=i.first;
				_theTopTip=i.second;
			}
		}
		return true;
	}
}

void PointManage::getPathByVecOne()
{
	vector<PointTip> tempvec;
	tempvec.push_back(_theTopTip);
	PointTip lasttip=_theTopTip;
	int laststep=_theTopStep;
	bool loop=true;
	do 
	{
		loop=true;
		for(auto i=_BoxPathVec.begin();i!=_BoxPathVec.end();i++)
		{
			if(i->NowTip==lasttip&&i->step==laststep)
			{
				lasttip=i->FrontTip;
				--laststep;
				tempvec.push_back(i->FrontTip);
				if(lasttip==_endTip)
				{
					loop=false;
					CCAssert(laststep==0,"laststep!=0");
				}
				break;
			}
			if(i==(_BoxPathVec.end()-1))
			{
				tempvec.clear();
				loop=false;
				break;
			}
		}
	} while (loop);

	if(tempvec.size()>0)
	{
		for(auto i=tempvec.rbegin();i!=tempvec.rend();i++)
			_BoxMoveVec.push_back(*i);
	}

	//for(int i=(_theTopTip.position.second+1);i<=_beginTip.position.second;i++)
	//{
	//	PointTip tip={_theTopTip.shape,_theTopTip.angle,make_pair(_theTopTip.position.first,i)};
	//	_BoxMoveVec.push_back(tip);
	//}
	//if(_theTopTip.angle!=_beginTip.angle)
	//{
	//	PointTip tip={_beginTip.shape,_beginTip.angle,make_pair(_theTopTip.position.first,_beginTip.position.second)};
	//	_BoxMoveVec.push_back(tip);
	//}

	//if(_beginTip.position.first<_theTopTip.position.first)
	//{
	//	for(int i=(_theTopTip.position.first-1);i>=_beginTip.position.first;i--)
	//	{
	//		PointTip tip={_beginTip.shape,_beginTip.angle,make_pair(i,_beginTip.position.second)};
	//		_BoxMoveVec.push_back(tip);
	//	}
	//}
	//else if(_beginTip.position.first>_theTopTip.position.first)
	//{
	//	for(int i=(_theTopTip.position.first+1);i<=_beginTip.position.first;i++)
	//	{
	//		PointTip tip={_beginTip.shape,_beginTip.angle,make_pair(i,_beginTip.position.second)};
	//		_BoxMoveVec.push_back(tip);
	//	}
	//}
}

bool PointManage::boxMoveProcessOne(PointTip recenttip,int step)
{
	++ccc;
	if(ccc>30)
		return true;
	int newstep=step+1;
	//向左
	auto res=BoxMove(recenttip,make_pair(-1,0));
	if(res.result&&!BoxPathVecJudge(recenttip,res.newTip,step))
	{
		_BoxPathVec.push_back(BoxMovePath(recenttip,res.newTip,step));
		if(isTheTop(res.newTip))
		{
			_theTopTipVec.push_back(make_pair(step,res.newTip));
			return true;
		}
		else
		{
			if(boxMoveProcessOne(res.newTip,newstep))
				return true;
		}
	}

	//向右
	res=BoxMove(recenttip,make_pair(1,0));
	if(res.result&&!BoxPathVecJudge(recenttip,res.newTip,step))
	{
		_BoxPathVec.push_back(BoxMovePath(recenttip,res.newTip,step));
		if(isTheTop(res.newTip))
		{
			_theTopTipVec.push_back(make_pair(step,res.newTip));
			return true;
		}
		else
		{
			if(boxMoveProcessOne(res.newTip,newstep))
				return true;
		}
	}

	//向上
	res=BoxMove(recenttip,make_pair(0,1));
	if(res.result&&!BoxPathVecJudge(recenttip,res.newTip,step))
	{
		_BoxPathVec.push_back(BoxMovePath(recenttip,res.newTip,step));
		if(boxMoveProcessOne(res.newTip,newstep))
			return true;
	}

	if(_beginTip.shape!=ShapeIndex::SQUARE_SHAPE&&_beginTip.shape!=ShapeIndex::POINT_SHAPE)
	{
		////顺时针旋转
		//res=ReBoxRotate(recenttip);
		//if(res.result&&!BoxPathVecJudge(res.newTip)&&res.newTip.shape!=ShapeIndex::SQUARE_SHAPE)
		//{
		//	_BoxPathVec.push_back(BoxMovePath(recenttip,res.newTip,0));
		//	if(isTheTop(res.newTip))
		//	{
		//		_theTopTip=res.newTip;
		//		return true;
		//	}
		//	else if(boxMoveProcessOne(res.newTip))
		//		return true;
		//}

		//逆时针旋转
		res=ReBoxRotate(recenttip,3);
		if(res.result&&!BoxPathVecJudge(recenttip,res.newTip,step)&&res.newTip.shape!=ShapeIndex::SQUARE_SHAPE)
		{
			_BoxPathVec.push_back(BoxMovePath(recenttip,res.newTip,step));
			if(isTheTop(res.newTip))
			{
				_theTopTipVec.push_back(make_pair(step,res.newTip));
				return true;
			}
			else
			{
				if(boxMoveProcessOne(res.newTip,newstep))
					return true;
			}
		}
	}

	return false;
}

bool PointManage::isTheTop(PointTip pt)
{
	auto boxfill=BoxNode::getBoxFillByData(pt.shape,pt.angle);   
	for(auto &i:boxfill)
	{
		i.first+=pt.position.first;
		i.second+=pt.position.second;
	}

	for(auto &i:boxfill)
	{
		if(i.second<_toph.at(i.first))
			return false;
	}
	return true;
}

//BoxMoveResult PointManage::BoxRotate(PointTip recenttip,int rotate)
//{
//	AngleIndex newangle=AngleIndex((recenttip.angle+rotate)%4);
//	auto boxFill=BoxNode::getBoxFillByData(recenttip.shape,newangle);
//	for(auto &i:boxFill)
//	{
//		i.first+=recenttip.position.first;
//		i.second+=recenttip.position.second;
//	}
//
//	BoxMoveResult result(false,recenttip);
//	result.newTip.angle=newangle;
//
//	bool tempbool=true;
//	for(auto &i:boxFill)
//	{
//		if((i.second<CHESS_SIZE_H)&&(i.second<0||i.first<0||i.first>=CHESS_SIZE_W||_chessFilInfo[i.first*CHESS_SIZE_H+i.second]))
//		{
//			tempbool=false;
//			break;
//		}
//	}
//
//	if(tempbool)
//	{
//		result.result=true;
//		return result;
//	}
//	else
//	{
//		tempbool=true;
//		auto newboxFill=BoxFillOffest(boxFill,make_pair(-1,0));
//		for(auto &i:newboxFill)
//		{
//			if((i.second<CHESS_SIZE_H)&&(i.second<0||i.first<0||i.first>=CHESS_SIZE_W||_chessFilInfo[i.first*CHESS_SIZE_H+i.second]))
//			{
//				tempbool=false;
//				break;
//			}
//		}
//	}
//
//	//向左移动一个单位
//	if(tempbool)
//	{
//		result.result=true;
//		result.newTip.position.first-=1;
//		return result;
//	}
//	else
//	{
//		tempbool=true;
//		auto newboxFill=BoxFillOffest(boxFill,make_pair(1,0));
//		for(auto &i:newboxFill)
//		{
//			if((i.second<CHESS_SIZE_H)&&(i.second<0||i.first<0||i.first>=CHESS_SIZE_W||_chessFilInfo[i.first*CHESS_SIZE_H+i.second]))
//			{
//				tempbool=false;
//				break;
//			}
//		}
//	}
//
//	//向右移动一个单位
//	if(tempbool)
//	{
//		result.result=true;
//		result.newTip.position.first+=1;
//		return result;
//	}
//	else
//	{
//		tempbool=true;
//		boxFill=BoxFillOffest(boxFill,make_pair(-1,-1));
//		for(auto &i:boxFill)
//		{
//			if((i.second<CHESS_SIZE_H)&&(i.second<0||i.first<0||i.first>=CHESS_SIZE_W||_chessFilInfo[i.first*CHESS_SIZE_H+i.second]))
//			{
//				tempbool=false;
//				break;
//			}
//		}
//	}
//
//	//向左下移动
//	if(tempbool)
//	{
//		result.result=true;
//		result.newTip.position.first-=1;
//		result.newTip.position.second-=1;
//		return result;
//	}
//	else
//	{
//		tempbool=true;
//		boxFill=BoxFillOffest(boxFill,make_pair(1,-1));
//		for(auto &i:boxFill)
//		{
//			if((i.second<CHESS_SIZE_H)&&(i.second<0||i.first<0||i.first>=CHESS_SIZE_W||_chessFilInfo[i.first*CHESS_SIZE_H+i.second]))
//			{
//				tempbool=false;
//				break;
//			}
//		}
//	}
//
//	//向右下移动
//	if(tempbool)
//	{
//		result.result=true;
//		result.newTip.position.first+=1;
//		result.newTip.position.second-=1;
//		return result;
//	}
//	//旋转失败
//	else
//	{
//		return result;
//	}
//}

BoxMoveResult PointManage::ReBoxRotate(PointTip recenttip,int key)
{
	AngleIndex newangle=AngleIndex((recenttip.angle+key)%4);
	auto boxFill=BoxNode::getBoxFillByData(recenttip.shape,newangle);
	for(auto &i:boxFill)
	{
		i.first+=recenttip.position.first;
		i.second+=recenttip.position.second;
	}

	BoxMoveResult result(false,recenttip);
	result.newTip.angle=newangle;

	bool tempbool=true;
	for(auto &i:boxFill)
	{
		if((i.second<CHESS_SIZE_H)&&(i.second<0||i.first<0||i.first>=CHESS_SIZE_W||_chessFilInfo[i.first*CHESS_SIZE_H+i.second]))
		{
			tempbool=false;
			break;
		}
	}

	if(tempbool)
	{
		result.result=true;
		return result;
	}
	else
	{
		tempbool=true;
		if(_beginTip.shape==ShapeIndex::U_SHAPE)
			tempbool=false;
		else
		{
			auto newboxFill=BoxFillOffest(boxFill,make_pair(-1,0));
			for(auto &i:newboxFill)
			{
				if((i.second<CHESS_SIZE_H)&&(i.second<0||i.first<0||i.first>=CHESS_SIZE_W||_chessFilInfo[i.first*CHESS_SIZE_H+i.second]))
				{
					tempbool=false;
					break;
				}
			}
		}
	}

	//向左移动一个单位
	if(tempbool)
	{
		result.result=true;
		result.newTip.position.first-=1;
		return result;
	}
	else
	{
		tempbool=true;
		if(_beginTip.shape==ShapeIndex::U_SHAPE)
			tempbool=false;
		else
		{
			auto newboxFill=BoxFillOffest(boxFill,make_pair(1,0));
			for(auto &i:newboxFill)
			{
				if((i.second<CHESS_SIZE_H)&&(i.second<0||i.first<0||i.first>=CHESS_SIZE_W||_chessFilInfo[i.first*CHESS_SIZE_H+i.second]))
				{
					tempbool=false;
					break;
				}
			}
		}
	}

	//向右移动一个单位
	if(tempbool)
	{
		result.result=true;
		result.newTip.position.first+=1;
		return result;
	}
	else
	{
		//tempbool=true;
		//if(_beginTip.shape==ShapeIndex::T_SHAPE)
		//{
		//	auto newboxFill=BoxFillOffest(boxFill,make_pair(-1,1));
		//	for(auto &i:newboxFill)
		//	{
		//		if((i.second<CHESS_SIZE_H)&&(i.second<0||i.first<0||i.first>=CHESS_SIZE_W||_chessFilInfo[i.first*CHESS_SIZE_H+i.second]))
		//		{
		//			tempbool=false;
		//			break;
		//		}
		//	}
		//}
		//else
			tempbool=false;
	}

	//向左上移动
	if(tempbool)
	{
		result.result=true;
		result.newTip.position.first-=1;
		result.newTip.position.second+=1;
		return result;
	}
	else
	{
		tempbool=true;
		if(_beginTip.shape==ShapeIndex::T_SHAPE||_beginTip.shape==ShapeIndex::SWAGER_RESHAPE)
		{
			auto newboxFill=BoxFillOffest(boxFill,make_pair(1,1));
			for(auto &i:newboxFill)
			{
				if((i.second<CHESS_SIZE_H)&&(i.second<0||i.first<0||i.first>=CHESS_SIZE_W||_chessFilInfo[i.first*CHESS_SIZE_H+i.second]))
				{
					tempbool=false;
					break;
				}
			}
		}
		else
			tempbool=false;
	}

	//向右上移动
	if(tempbool)
	{
		result.result=true;
		result.newTip.position.first+=1;
		result.newTip.position.second+=1;
		return result;
	}
	else
	{
		tempbool=true;
		if(_beginTip.shape==ShapeIndex::SWAGER_SHAPE||_beginTip.shape==ShapeIndex::LINE_SHAPE||_beginTip.shape==ShapeIndex::LINESMALL_SHAPE||_beginTip.shape==ShapeIndex::T_SHAPE)
		{
			auto newboxFill=BoxFillOffest(boxFill,make_pair(0,1));
			for(auto &i:newboxFill)
			{
				if((i.second<CHESS_SIZE_H)&&(i.second<0||i.first<0||i.first>=CHESS_SIZE_W||_chessFilInfo[i.first*CHESS_SIZE_H+i.second]))
				{
					tempbool=false;
					break;
				}
			}
		}
		else
			tempbool=false;
	}

	//向上
	if(tempbool)
	{
		result.result=true;
		result.newTip.position.second+=1;
		return result;
	}
	//旋转失败
	else
	{
		return result;
	}
}

BoxMoveResult PointManage::BoxMove(PointTip recenttip,pair<int,int> move)
{
	BoxMoveResult result(false,recenttip);
	auto boxfill=BoxNode::getBoxFillByData(recenttip.shape,recenttip.angle);
	for(auto &i:boxfill)
	{
		i.first+=recenttip.position.first;
		i.second+=recenttip.position.second;
	}

	bool tempbool=true;
	auto newboxfill=BoxFillOffest(boxfill,move);
	for(auto &i:newboxfill)
	{
		if((i.second<CHESS_SIZE_H)&&(i.second<0||i.first<0||i.first>=CHESS_SIZE_W||_chessFilInfo[i.first*CHESS_SIZE_H+i.second]))
		{
			tempbool=false;
			break;
		}
	}
	if(tempbool)
	{
		result.result=true;
		result.newTip.position.first+=move.first;
		result.newTip.position.second+=move.second;
	}

	return result;
}

bool PointManage::BoxPathVecJudge(PointTip fronttip,PointTip nowtip,int step)
{
	for(auto i=_BoxPathVec.begin();i!=_BoxPathVec.end();i++)
	{
		if(i->NowTip==nowtip&&step>i->step)
		{
			return true;
		}
	}
	return false;
}

vector<pair<int,int>> PointManage::BoxFillOffest(vector<std::pair<int,int>> vec,pair<int,int> offest)
{
	for(auto &i:vec)
	{
		i.first+=offest.first;
		i.second+=offest.second;
	}
	return vec;
}