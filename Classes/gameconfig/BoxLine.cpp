#include"BoxLine.h"

bool BoxLine::init(ShapeIndex shape,AngleIndex angle/* =ANGLE_0 */)
{
	if(!Node::init())
		return false;

	vector<linepos> linevec;
	switch (shape)
	{
	case ShapeIndex::L_SHAPE:
		{
			linevec.push_back(linepos(0,0,0));
			linevec.push_back(linepos(0,0,1));
			linevec.push_back(linepos(-1,0,0));
			linevec.push_back(linepos(-1,0,1));
			linevec.push_back(linepos(-1,0,2));
			linevec.push_back(linepos(1,0,1));
			linevec.push_back(linepos(1,0,3));
			linevec.push_back(linepos(1,1,0));
			linevec.push_back(linepos(1,1,2));
			linevec.push_back(linepos(1,1,3));
		}
		break;
	case ShapeIndex::L_RESHAPE:
		{
			linevec.push_back(linepos(0,0,0));
			linevec.push_back(linepos(0,0,1));
			linevec.push_back(linepos(1,0,0));
			linevec.push_back(linepos(1,0,1));
			linevec.push_back(linepos(1,0,3));
			linevec.push_back(linepos(-1,0,1));
			linevec.push_back(linepos(-1,0,2));
			linevec.push_back(linepos(-1,1,0));
			linevec.push_back(linepos(-1,1,2));
			linevec.push_back(linepos(-1,1,3));
		}
		break;
	case ShapeIndex::LINE_SHAPE:
		{
			linevec.push_back(linepos(0,0,0));
			linevec.push_back(linepos(0,0,1));
			linevec.push_back(linepos(-1,0,0));
			linevec.push_back(linepos(-1,0,1));
			linevec.push_back(linepos(-1,0,2));
			linevec.push_back(linepos(1,0,0));
			linevec.push_back(linepos(1,0,1));
			linevec.push_back(linepos(2,0,0));
			linevec.push_back(linepos(2,0,1));
			linevec.push_back(linepos(2,0,3));
		}
		break;
	case ShapeIndex::SQUARE_SHAPE:
		{
			linevec.push_back(linepos(0,0,1));
			linevec.push_back(linepos(0,0,2));
			linevec.push_back(linepos(1,0,1));
			linevec.push_back(linepos(1,0,3));
			linevec.push_back(linepos(0,1,0));
			linevec.push_back(linepos(0,1,2));
			linevec.push_back(linepos(1,1,0));
			linevec.push_back(linepos(1,1,3));
		}
		break;
	case ShapeIndex::SWAGER_SHAPE:
		{
			linevec.push_back(linepos(0,0,1));
			linevec.push_back(linepos(0,0,2));
			linevec.push_back(linepos(1,0,0));
			linevec.push_back(linepos(1,0,1));
			linevec.push_back(linepos(1,0,3));
			linevec.push_back(linepos(0,1,0));
			linevec.push_back(linepos(0,1,3));
			linevec.push_back(linepos(-1,1,0));
			linevec.push_back(linepos(-1,1,1));
			linevec.push_back(linepos(-1,1,2));
		}
		break;
	case ShapeIndex::SWAGER_RESHAPE:
		{
			linevec.push_back(linepos(0,0,1));
			linevec.push_back(linepos(0,0,3));
			linevec.push_back(linepos(-1,0,0));
			linevec.push_back(linepos(-1,0,1));
			linevec.push_back(linepos(-1,0,2));
			linevec.push_back(linepos(0,1,0));
			linevec.push_back(linepos(0,1,2));
			linevec.push_back(linepos(1,1,0));
			linevec.push_back(linepos(1,1,1));
			linevec.push_back(linepos(1,1,3));
		}
		break;
	case ShapeIndex::T_SHAPE:
		{
			linevec.push_back(linepos(0,0,1));
			linevec.push_back(linepos(-1,0,0));
			linevec.push_back(linepos(-1,0,1));
			linevec.push_back(linepos(-1,0,2));
			linevec.push_back(linepos(1,0,0));
			linevec.push_back(linepos(1,0,1));
			linevec.push_back(linepos(1,0,3));
			linevec.push_back(linepos(0,1,0));
			linevec.push_back(linepos(0,1,2));
			linevec.push_back(linepos(0,1,3));
		}
		break;
	case ShapeIndex::POINT_SHAPE:
		{
			linevec.push_back(linepos(0,0,0));
			linevec.push_back(linepos(0,0,1));
			linevec.push_back(linepos(0,0,2));
			linevec.push_back(linepos(0,0,3));
		}
		break;
	case ShapeIndex::ANGLE_SHAPE:
		{
			linevec.push_back(linepos(0,0,1));
			linevec.push_back(linepos(0,0,2));
			linevec.push_back(linepos(1,0,0));
			linevec.push_back(linepos(1,0,1));
			linevec.push_back(linepos(1,0,3));
			linevec.push_back(linepos(0,1,0));
			linevec.push_back(linepos(0,1,2));
			linevec.push_back(linepos(0,1,3));
		}
		break;
	case ShapeIndex::LINESMALL_SHAPE:
		{
			linevec.push_back(linepos(0,0,0));
			linevec.push_back(linepos(0,0,1));
			linevec.push_back(linepos(-1,0,0));
			linevec.push_back(linepos(-1,0,1));
			linevec.push_back(linepos(-1,0,2));
			linevec.push_back(linepos(1,0,0));
			linevec.push_back(linepos(1,0,1));
			linevec.push_back(linepos(1,0,3));
		}
		break;
	case ShapeIndex::LBIG_SHAPE:
		{
			linevec.push_back(linepos(0,0,0));
			linevec.push_back(linepos(0,0,1));
			linevec.push_back(linepos(1,0,0));
			linevec.push_back(linepos(1,0,1));
			linevec.push_back(linepos(1,0,3));
			linevec.push_back(linepos(-1,0,1));
			linevec.push_back(linepos(-1,0,2));
			linevec.push_back(linepos(-1,1,2));
			linevec.push_back(linepos(-1,1,3));
			linevec.push_back(linepos(-1,2,0));
			linevec.push_back(linepos(-1,2,2));
			linevec.push_back(linepos(-1,2,3));
		}
		break;
	case ShapeIndex::U_SHAPE:
		{
			linevec.push_back(linepos(0,0,0));
			linevec.push_back(linepos(0,0,1));
			linevec.push_back(linepos(1,0,1));
			linevec.push_back(linepos(1,0,3));
			linevec.push_back(linepos(-1,0,1));
			linevec.push_back(linepos(-1,0,2));
			linevec.push_back(linepos(-1,1,0));
			linevec.push_back(linepos(-1,1,2));
			linevec.push_back(linepos(-1,1,3));
			linevec.push_back(linepos(1,1,0));
			linevec.push_back(linepos(1,1,2));
			linevec.push_back(linepos(1,1,3));
		}
		break;
	case ShapeIndex::SQUAREBIG_SHAPE:
		{
			linevec.push_back(linepos(0,0,1));
			linevec.push_back(linepos(1,0,1));
			linevec.push_back(linepos(1,0,3));
			linevec.push_back(linepos(-1,0,1));
			linevec.push_back(linepos(-1,0,2));
			linevec.push_back(linepos(1,1,3));
			linevec.push_back(linepos(-1,1,2));
			linevec.push_back(linepos(1,2,0));
			linevec.push_back(linepos(1,2,3));
			linevec.push_back(linepos(-1,2,0));
			linevec.push_back(linepos(-1,2,2));
			linevec.push_back(linepos(0,2,0));
		}
		break;
	default:
		break;
	}

	for(auto &i:linevec)
	{
		addLine(i);
	}

	this->setRotation(angle*90);
	this->setScale(0.94f);
	return true;
}

void BoxLine::addLine(linepos ps)
{
	int tempsize;
	if(GameDataInstance()->d_Project==GAME_TETRIS)
		tempsize=CHESS_SIZE_SIZE;
	else if(GameDataInstance()->d_Project==GAME_1010)
		tempsize=CHESSMAN_SIZE_1010;
	auto _drawnode=DrawNode::create();
	glLineWidth(1);
	this->addChild(_drawnode);
	_drawnode->drawLine(Vec2(-tempsize/2.0f,0),Vec2(tempsize/2.0f,0),Color4F::WHITE);
	Vec2 addvec2;
	if(ps.p==0)
	{
		addvec2=Vec2(0,tempsize/2.0f);
	}
	else if(ps.p==1)
	{
		addvec2=Vec2(0,-tempsize/2.0f);
	}
	else if(ps.p==2)
	{
		_drawnode->setRotation(90.0f);
		addvec2=Vec2(-tempsize/2.0f,0);
	}
	else if(ps.p==3)
	{
		_drawnode->setRotation(90.0f);
		addvec2=Vec2(tempsize/2.0f,0);
	}
	_drawnode->setPosition(Vec2(ps.x*tempsize,ps.y*tempsize)+addvec2);
}
