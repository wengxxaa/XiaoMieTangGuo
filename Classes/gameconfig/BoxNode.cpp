#include"BoxNode.h"

BoxNode::BoxNode():_angle(ANGLE_0)
{

}

BoxNode::~BoxNode(){

}

bool BoxNode::init(ShapeIndex shape,unsigned int index,const char* filename)
{
	if(!Node::init())
		return false;

	_boxcolor=BoxColor::BOX_NORMAL;
	_shape=shape;

	if(GameDataInstance()->d_Project==GAME_TETRIS)
		_boxsize=CHESS_SIZE_SIZE;
	else if(GameDataInstance()->d_Project==GAME_1010)
		_boxsize=CHESSMAN_SIZE_1010;

	Sprite *node[9];
	for(int i=0;i<9;i++)
	{
		char name[30];
		sprintf(name,"fangkuai_%d.png",int(shape%10)+1);
		node[i]=cjSprite::createWithImage(name);
		this->addChild(node[i],0,i+1);
		if(GameDataInstance()->d_Project==GAME_1010)
			node[i]->setScale(CHESSMAN_SCALE_1010);
		else if(GameDataInstance()->d_Project==GAME_TETRIS)
			node[i]->setScale(CHESSMAN_SCALE);
		if(i==0)
			node[i]->setPosition(Vec2::ZERO);
	}

	switch (shape)
	{
	case ShapeIndex::L_SHAPE:
		{
			node[1]->setPosition(Vec2(-_boxsize,0));
			node[2]->setPosition(Vec2(_boxsize,0));
			node[3]->setPosition(Vec2(_boxsize,_boxsize));
			removeFromId(4);
		}
		break;
	case ShapeIndex::L_RESHAPE:
		{
			node[1]->setPosition(Vec2(_boxsize,0));
			node[2]->setPosition(Vec2(-_boxsize,0));
			node[3]->setPosition(Vec2(-_boxsize,_boxsize));
			removeFromId(4);
		}
		break;
	case ShapeIndex::LINE_SHAPE:
		{
			node[1]->setPosition(Vec2(-_boxsize,0));
			node[2]->setPosition(Vec2(_boxsize,0));
			node[3]->setPosition(Vec2(_boxsize*2,0));
			removeFromId(4);
		}
		break;
	case ShapeIndex::SQUARE_SHAPE:
		{
			node[1]->setPosition(Vec2(_boxsize,0));
			node[2]->setPosition(Vec2(0,_boxsize));
			node[3]->setPosition(Vec2(_boxsize,_boxsize));
			removeFromId(4);
		}
		break;
	case ShapeIndex::SWAGER_SHAPE:
		{
			node[1]->setPosition(Vec2(_boxsize,0));
			node[2]->setPosition(Vec2(0,_boxsize));
			node[3]->setPosition(Vec2(-_boxsize,_boxsize));
			removeFromId(4);
		}
		break;
	case ShapeIndex::SWAGER_RESHAPE:
		{
			node[1]->setPosition(Vec2(-_boxsize,0));
			node[2]->setPosition(Vec2(0,_boxsize));
			node[3]->setPosition(Vec2(_boxsize,_boxsize));
			removeFromId(4);
		}
		break;
	case ShapeIndex::T_SHAPE:
		{
			node[1]->setPosition(Vec2(-_boxsize,0));
			node[2]->setPosition(Vec2(_boxsize,0));
			node[3]->setPosition(Vec2(0,_boxsize));
			removeFromId(4);
		}
		break;
	case ShapeIndex::POINT_SHAPE:
		{
			removeFromId(1);
		}
		break;
	case ShapeIndex::ANGLE_SHAPE:
		{
			node[1]->setPosition(Vec2(0,_boxsize));
			node[2]->setPosition(Vec2(_boxsize,0));
			removeFromId(3);
		}
		break;
	case ShapeIndex::LINESMALL_SHAPE:
		{
			node[1]->setPosition(Vec2(_boxsize,0));
			node[2]->setPosition(Vec2(-_boxsize,0));
			removeFromId(3);
		}
		break;
	case ShapeIndex::LBIG_SHAPE:
		{
			node[1]->setPosition(Vec2(_boxsize,0));
			node[2]->setPosition(Vec2(-_boxsize,0));
			node[3]->setPosition(Vec2(-_boxsize,_boxsize));
			node[4]->setPosition(Vec2(-_boxsize,_boxsize*2));
			removeFromId(5);
		}
		break;
	case ShapeIndex::U_SHAPE:
		{
			node[1]->setPosition(Vec2(_boxsize,0));
			node[2]->setPosition(Vec2(-_boxsize,0));
			node[3]->setPosition(Vec2(-_boxsize,_boxsize));
			node[4]->setPosition(Vec2(_boxsize,_boxsize));
			removeFromId(5);
		}
		break;
	case ShapeIndex::SQUAREBIG_SHAPE:
		{
			node[1]->setPosition(Vec2(_boxsize,0));
			node[2]->setPosition(Vec2(-_boxsize,0));
			node[3]->setPosition(Vec2(_boxsize,_boxsize));
			node[4]->setPosition(Vec2(0,_boxsize));
			node[5]->setPosition(Vec2(-_boxsize,_boxsize));
			node[6]->setPosition(Vec2(_boxsize,_boxsize*2));
			node[7]->setPosition(Vec2(0,_boxsize*2));
			node[8]->setPosition(Vec2(-_boxsize,_boxsize*2));
		}
		break;
	default:
		break;
	}

	return true;
}

void BoxNode::removeFromId(int id)
{
	for(int i=id;i<9;i++)
	{
		this->removeChildByTag(i+1);
	}
}

vector<pair<int,int>> BoxNode::getBoxFill()
{
	vector<std::pair<int,int>> vec;
	vec.push_back(std::make_pair(0,0));
	switch (_shape)
	{
	case ShapeIndex::L_SHAPE:
		{
			switch (_angle)
			{
			case ANGLE_0:
				{
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(1,1));
				}
				break;
			case ANGLE_90:
				{
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(1,-1));
				}
				break;
			case ANGLE_180:
				{
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(-1,-1));
				}
				break;
			case ANGLE_270:
				{
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(-1,1));
				}
				break;
			default:
				break;
			}
		}
		break;
	case ShapeIndex::L_RESHAPE:
		{
			switch (_angle)
			{
			case ANGLE_0:
				{
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(-1,1));
				}
				break;
			case ANGLE_90:
				{
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(1,1));
				}
				break;
			case ANGLE_180:
				{
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(1,-1));
				}
				break;
			case ANGLE_270:
				{
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(-1,-1));
				}
				break;
			default:
				break;
			}
		}
		break;
	case ShapeIndex::LINE_SHAPE:
		{
			switch (_angle)
			{
			case ANGLE_0:
				{
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(2,0));
				}
				break;
			case ANGLE_90:
				{
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(0,-2));
				}
				break;
			case ANGLE_180:
				{
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(-2,0));
				}
				break;
			case ANGLE_270:
				{
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(0,2));
				}
				break;
			default:
				break;
			}
		}
		break;
	case ShapeIndex::SQUARE_SHAPE:
		{
			vec.push_back(std::make_pair(1,0));
			vec.push_back(std::make_pair(0,1));
			vec.push_back(std::make_pair(1,1));
		}
		break;
	case ShapeIndex::SQUAREBIG_SHAPE:
		{
			vec.push_back(std::make_pair(1,0));
			vec.push_back(std::make_pair(-1,0));
			vec.push_back(std::make_pair(1,1));
			vec.push_back(std::make_pair(-1,1));
			vec.push_back(std::make_pair(0,1));
			vec.push_back(std::make_pair(1,2));
			vec.push_back(std::make_pair(-1,2));
			vec.push_back(std::make_pair(0,2));
		}
		break;
	case ShapeIndex::SWAGER_SHAPE:
		{
			switch (_angle)
			{
			case ANGLE_0:
				{
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(-1,1));
				}
				break;
			case ANGLE_90:
				{
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(1,1));
				}
				break;
			case ANGLE_180:
				{
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(1,-1));
				}
				break;
			case ANGLE_270:
				{
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(-1,-1));
				}
				break;
			default:
				break;
			}
		}
		break;
	case ShapeIndex::SWAGER_RESHAPE:
		{
			switch (_angle)
			{
			case ANGLE_0:
				{
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(1,1));
				}
				break;
			case ANGLE_90:
				{
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(1,-1));
				}
				break;
			case ANGLE_180:
				{
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(-1,-1));
				}
				break;
			case ANGLE_270:
				{
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(-1,1));
				}
				break;
			default:
				break;
			}
		}
		break;
	case ShapeIndex::T_SHAPE:
		{
			switch (_angle)
			{
			case ANGLE_0:
				{
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(0,1));
				}
				break;
			case ANGLE_90:
				{
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(1,0));
				}
				break;
			case ANGLE_180:
				{
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(0,-1));
				}
				break;
			case ANGLE_270:
				{
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(-1,0));
				}
				break;
			default:
				break;
			}
		}
		break;
	case ShapeIndex::POINT_SHAPE:
		break;
	case ShapeIndex::ANGLE_SHAPE:
		{
			switch (_angle)
			{
			case ANGLE_0:
				{
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(0,1));
				}
				break;
			case ANGLE_90:
				{
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(0,-1));
				}
				break;
			case ANGLE_180:
				{
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(0,-1));
				}
				break;
			case ANGLE_270:
				{
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(-1,0));
				}
				break;
			default:
				break;
			}
		}
		break;
	case ShapeIndex::LINESMALL_SHAPE:
		{
			switch (_angle)
			{
			case ANGLE_0:
				{
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(1,0));
				}
				break;
			case ANGLE_90:
				{
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(0,-1));
				}
				break;
			case ANGLE_180:
				{
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(1,0));
				}
				break;
			case ANGLE_270:
				{
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(0,-1));
				}
				break;
			default:
				break;
			}
		}
		break;
	case ShapeIndex::LBIG_SHAPE:
		{
			switch (_angle)
			{
			case ANGLE_0:
				{
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(-1,1));
					vec.push_back(std::make_pair(-1,2));
				}
				break;
			case ANGLE_90:
				{
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(1,1));
					vec.push_back(std::make_pair(2,1));				}
				break;
			case ANGLE_180:
				{
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(1,-1));
					vec.push_back(std::make_pair(1,-2));
				}
				break;
			case ANGLE_270:
				{
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(-1,-1));
					vec.push_back(std::make_pair(-2,-1));
				}
				break;
			default:
				break;
			}
		}
		break;
	case U_SHAPE:
		{
			switch (_angle)
			{
			case ANGLE_0:
				{
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(-1,1));
					vec.push_back(std::make_pair(1,1));
				}
				break;
			case ANGLE_90:
				{
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(1,1));
					vec.push_back(std::make_pair(1,-1));				}
				break;
			case ANGLE_180:
				{
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(-1,-1));
					vec.push_back(std::make_pair(1,-1));
				}
				break;
			case ANGLE_270:
				{
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(-1,1));
					vec.push_back(std::make_pair(-1,-1));
				}
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
	return vec;
}

vector<pair<int,int>> BoxNode::getBoxFillByData(ShapeIndex shape,AngleIndex angle)
{
	vector<std::pair<int,int>> vec;
	vec.push_back(std::make_pair(0,0));
	switch (shape)
	{
	case ShapeIndex::L_SHAPE:
		{
			switch (angle)
			{
			case ANGLE_0:
				{
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(1,1));
				}
				break;
			case ANGLE_90:
				{
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(1,-1));
				}
				break;
			case ANGLE_180:
				{
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(-1,-1));
				}
				break;
			case ANGLE_270:
				{
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(-1,1));
				}
				break;
			default:
				break;
			}
		}
		break;
	case ShapeIndex::L_RESHAPE:
		{
			switch (angle)
			{
			case ANGLE_0:
				{
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(-1,1));
				}
				break;
			case ANGLE_90:
				{
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(1,1));
				}
				break;
			case ANGLE_180:
				{
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(1,-1));
				}
				break;
			case ANGLE_270:
				{
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(-1,-1));
				}
				break;
			default:
				break;
			}
		}
		break;
	case ShapeIndex::LINE_SHAPE:
		{
			switch (angle)
			{
			case ANGLE_0:
				{
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(2,0));
				}
				break;
			case ANGLE_90:
				{
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(0,-2));
				}
				break;
			case ANGLE_180:
				{
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(-2,0));
				}
				break;
			case ANGLE_270:
				{
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(0,2));
				}
				break;
			default:
				break;
			}
		}
		break;
	case ShapeIndex::SQUARE_SHAPE:
		{
			vec.push_back(std::make_pair(1,0));
			vec.push_back(std::make_pair(0,1));
			vec.push_back(std::make_pair(1,1));
		}
		break;
	case ShapeIndex::SWAGER_SHAPE:
		{
			switch (angle)
			{
			case ANGLE_0:
				{
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(-1,1));
				}
				break;
			case ANGLE_90:
				{
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(1,1));
				}
				break;
			case ANGLE_180:
				{
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(1,-1));
				}
				break;
			case ANGLE_270:
				{
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(-1,-1));
				}
				break;
			default:
				break;
			}
		}
		break;
	case ShapeIndex::SWAGER_RESHAPE:
		{
			switch (angle)
			{
			case ANGLE_0:
				{
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(1,1));
				}
				break;
			case ANGLE_90:
				{
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(1,-1));
				}
				break;
			case ANGLE_180:
				{
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(-1,-1));
				}
				break;
			case ANGLE_270:
				{
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(-1,1));
				}
				break;
			default:
				break;
			}
		}
		break;
	case ShapeIndex::T_SHAPE:
		{
			switch (angle)
			{
			case ANGLE_0:
				{
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(0,1));
				}
				break;
			case ANGLE_90:
				{
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(1,0));
				}
				break;
			case ANGLE_180:
				{
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(0,-1));
				}
				break;
			case ANGLE_270:
				{
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(-1,0));
				}
				break;
			default:
				break;
			}
		}
		break;
	case ShapeIndex::POINT_SHAPE:
		break;
	case ShapeIndex::ANGLE_SHAPE:
		{
			switch (angle)
			{
			case ANGLE_0:
				{
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(0,1));
				}
				break;
			case ANGLE_90:
				{
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(0,-1));
				}
				break;
			case ANGLE_180:
				{
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(0,-1));
				}
				break;
			case ANGLE_270:
				{
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(-1,0));
				}
				break;
			default:
				break;
			}
		}
		break;
	case ShapeIndex::LINESMALL_SHAPE:
		{
			switch (angle)
			{
			case ANGLE_0:
				{
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(1,0));
				}
				break;
			case ANGLE_90:
				{
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(0,-1));
				}
				break;
			case ANGLE_180:
				{
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(1,0));
				}
				break;
			case ANGLE_270:
				{
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(0,-1));
				}
				break;
			default:
				break;
			}
		}
		break;
	case ShapeIndex::LBIG_SHAPE:
		{
			switch (angle)
			{
			case ANGLE_0:
				{
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(-1,1));
					vec.push_back(std::make_pair(-1,2));
				}
				break;
			case ANGLE_90:
				{
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(1,1));
					vec.push_back(std::make_pair(2,1));				}
				break;
			case ANGLE_180:
				{
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(1,-1));
					vec.push_back(std::make_pair(1,-2));
				}
				break;
			case ANGLE_270:
				{
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(-1,-1));
					vec.push_back(std::make_pair(-2,-1));
				}
				break;
			default:
				break;
			}
		}
		break;
	case U_SHAPE:
		{
			switch (angle)
			{
			case ANGLE_0:
				{
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(-1,1));
					vec.push_back(std::make_pair(1,1));
				}
				break;
			case ANGLE_90:
				{
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(1,1));
					vec.push_back(std::make_pair(1,-1));				}
				break;
			case ANGLE_180:
				{
					vec.push_back(std::make_pair(-1,0));
					vec.push_back(std::make_pair(1,0));
					vec.push_back(std::make_pair(-1,-1));
					vec.push_back(std::make_pair(1,-1));
				}
				break;
			case ANGLE_270:
				{
					vec.push_back(std::make_pair(0,1));
					vec.push_back(std::make_pair(0,-1));
					vec.push_back(std::make_pair(-1,1));
					vec.push_back(std::make_pair(-1,-1));
				}
				break;
			default:
				break;
			}
		}
		break;
	case ShapeIndex::SQUAREBIG_SHAPE:
		{
			vec.push_back(std::make_pair(1,0));
			vec.push_back(std::make_pair(-1,0));
			vec.push_back(std::make_pair(1,1));
			vec.push_back(std::make_pair(-1,1));
			vec.push_back(std::make_pair(0,1));
			vec.push_back(std::make_pair(1,2));
			vec.push_back(std::make_pair(-1,2));
			vec.push_back(std::make_pair(0,2));
		}
		break;
	default:
		break;
	}
	return vec;
}

void BoxNode::Rotate()
{
	_angle=AngleIndex((_angle+1)%4);
	this->setRotation(_angle*90.0f);
	for(int i=0;i<9;i++)
	{
		auto node=getChildByTag(i+1);
		if(node)
			node->setRotation(360-(_angle*90));
	}
}

void BoxNode::ReRotate()
{
	_angle=AngleIndex((_angle+3)%4);
	this->setRotation(_angle*90.0f);
	for(int i=0;i<9;i++)
	{
		auto node=getChildByTag(i+1);
		if(node)
			node->setRotation(360-(_angle*90));
	}
}

void BoxNode::setRotate(AngleIndex angle)
{
	_angle=angle;
	this->setRotation(_angle*90.0f);
	for(int i=0;i<9;i++)
	{
		auto node=getChildByTag(i+1);
		if(node)
			node->setRotation(360-(_angle*90));
	}
}

Vec2 BoxNode::getCenterAnPoint(Vec2 vec)
{
	Vec2 vv;
	switch (_shape)
	{
	case L_SHAPE:
	case L_RESHAPE:
	case T_SHAPE:
	case U_SHAPE:
		{
			switch(_angle)
			{
			case ANGLE_0:
				vv=vec+Vec2(0,-_boxsize/2.0f)*getScale();
				break;
			case ANGLE_90:
				vv=vec+Vec2(-_boxsize/2.0f,0)*getScale();
				break;
			case ANGLE_180:
				vv=vec+Vec2(0,_boxsize/2.0f)*getScale();
				break;
			case ANGLE_270:
				vv=vec+Vec2(_boxsize/2.0f,0)*getScale();
				break;
			default:
				break;
			}
		}
		break;
	case LINE_SHAPE:
		{
			switch(_angle)
			{
			case ANGLE_0:
				vv=vec+Vec2(-_boxsize/2.0f,0)*getScale();
				break;
			case ANGLE_90:
				vv=vec+Vec2(0,_boxsize/2.0f)*getScale();
				break;
			}
		}
		break;
	case SQUARE_SHAPE:
	case ANGLE_SHAPE:
		{
			switch (_angle)
			{
			case ANGLE_0:
				vv=vec+Vec2(-_boxsize/2.0f,-_boxsize/2.0f)*getScale();
				break;
			case ANGLE_90:
				vv=vec+Vec2(-_boxsize/2.0f,_boxsize/2.0f)*getScale();
				break;
			case ANGLE_180:
				vv=vec+Vec2(_boxsize/2.0f,_boxsize/2.0f)*getScale();
				break;
			case ANGLE_270:
				vv=vec+Vec2(_boxsize/2.0f,-_boxsize/2.0f)*getScale();
				break;
			default:
				break;
			}
		}
		break;
	case SWAGER_SHAPE:
	case SWAGER_RESHAPE:
		{
			switch(_angle)
			{
			case ANGLE_0:
				vv=vec+Vec2(0,-_boxsize/2.0f)*getScale();
				break;
			case ANGLE_90:
				vv=vec+Vec2(-_boxsize/2.0f,0)*getScale();
				break;
			case ANGLE_180:
				vv=vec+Vec2(0,_boxsize/2.0f)*getScale();
				break;
			case ANGLE_270:
				vv=vec+Vec2(_boxsize/2.0f,0)*getScale();
				break;
			}
		}
		break;
	case POINT_SHAPE:
	case LINESMALL_SHAPE:
		vv=vec;
		break;
	case SQUAREBIG_SHAPE:
	case LBIG_SHAPE:
		{
			switch (_angle)
			{
			case ANGLE_0:
				vv=vec+Vec2(0,-_boxsize)*getScale();
				break;
			case ANGLE_90:
				vv=vec+Vec2(-_boxsize,0)*getScale();
				break;
			case ANGLE_180:
				vv=vec+Vec2(0,_boxsize)*getScale();
				break;
			case ANGLE_270:
				vv=vec+Vec2(_boxsize,0)*getScale();
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
	return vv;
}

Vec2 BoxNode::getCenterAnPointX(Vec2 vec)
{
	Vec2 vv;
	switch (_shape)
	{
	case L_SHAPE:
	case L_RESHAPE:
	case T_SHAPE:
	case U_SHAPE:
		{
			switch(_angle)
			{
			case ANGLE_0:
				vv=vec+Vec2(0,_boxsize/2.0f)*getScale();
				break;
			case ANGLE_90:
				vv=vec+Vec2(-_boxsize/2.0f,_boxsize*1.5f)*getScale();
				break;
			case ANGLE_180:
				vv=vec+Vec2(0,_boxsize*1.5f)*getScale();
				break;
			case ANGLE_270:
				vv=vec+Vec2(_boxsize/2.0f,_boxsize*1.5f)*getScale();
				break;
			default:
				break;
			}
		}
		break;
	case LINE_SHAPE:
		{
			switch(_angle)
			{
			case ANGLE_0:
				vv=vec+Vec2(-_boxsize/2.0f,_boxsize*0.5f)*getScale();
				break;
			case ANGLE_90:
				vv=vec+Vec2(0,_boxsize*2.5f)*getScale();
				break;
			}
		}
		break;
	case SQUARE_SHAPE:
	case ANGLE_SHAPE:
		{
			switch (_angle)
			{
			case ANGLE_0:
				vv=vec+Vec2(-_boxsize/2.0f,_boxsize*0.5f)*getScale();
				break;
			case ANGLE_90:
				vv=vec+Vec2(-_boxsize/2.0f,_boxsize*1.5f)*getScale();
				break;
			case ANGLE_180:
				vv=vec+Vec2(_boxsize/2.0f,_boxsize*1.5f)*getScale();
				break;
			case ANGLE_270:
				vv=vec+Vec2(_boxsize/2.0f,_boxsize*0.5f)*getScale();
				break;
			default:
				break;
			}
		}
		break;
	case SWAGER_SHAPE:
	case SWAGER_RESHAPE:
		{
			switch(_angle)
			{
			case ANGLE_0:
				vv=vec+Vec2(0,_boxsize*0.5f)*getScale();
				break;
			case ANGLE_90:
				vv=vec+Vec2(-_boxsize/2.0f,_boxsize*1.5f)*getScale();
				break;
			case ANGLE_180:
				vv=vec+Vec2(0,_boxsize*1.5f)*getScale();
				break;
			case ANGLE_270:
				vv=vec+Vec2(_boxsize/2.0f,_boxsize*1.5f)*getScale();
				break;
			}
		}
		break;
	case POINT_SHAPE:
		vv=vec+Vec2(0,_boxsize*0.5f)*getScale();
		break;
	case LINESMALL_SHAPE:
		{
			switch(_angle)
			{
			case ANGLE_0:
				vv=vec+Vec2(0,_boxsize*0.5f)*getScale();
				break;
			case ANGLE_90:
				vv=vec+Vec2(0,_boxsize*1.5f)*getScale();
				break;
			}
		}
		break;
	case SQUAREBIG_SHAPE:
	case LBIG_SHAPE:
		{
			switch (_angle)
			{
			case ANGLE_0:
				vv=vec+Vec2(0,_boxsize*0.5f)*getScale();
				break;
			case ANGLE_90:
				vv=vec+Vec2(-_boxsize,_boxsize*1.5f)*getScale();
				break;
			case ANGLE_180:
				vv=vec+Vec2(0,_boxsize*2.5f)*getScale();
				break;
			case ANGLE_270:
				vv=vec+Vec2(_boxsize,_boxsize*1.5f)*getScale();
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
	return vv;
}

void BoxNode::setOpacity(GLubyte opacity)
{
	for(int i=0;i<9;i++)
	{
		auto node=getChildByTag(i+1);
		if(node)
			getChildByTag(i+1)->setOpacity(opacity);
	}
}

void BoxNode::setSpecialScale(float ft)
{
	float sc=1.0f;
	if(GameDataInstance()->d_Project==GAME_1010)
		sc=CHESSMAN_SCALE_1010;
	else if(GameDataInstance()->d_Project==GAME_TETRIS)
		sc=CHESSMAN_SCALE;
	for(int i=0;i<9;i++)
	{
		auto node=getChildByTag(i+1);
		if(node)
			node->setScale(ft*sc);
	}
}

void BoxNode::setNormal()
{
	if(_boxcolor==BoxColor::BOX_NORMAL)
		return;

	_boxcolor=BoxColor::BOX_NORMAL;
	for(int i=0;i<9;i++)
	{
		for(int i=0;i<9;i++)
		{
			auto node=getChildByTag(i+1);
			if(node)
			{
				node->setVisible(true);
				auto graynode=getChildByTag(100+i+1);
				if(graynode)
					graynode->setVisible(false);

			}
		}
	}
}

void BoxNode::setGray()
{
	if(_boxcolor==BoxColor::BOX_GRAY)
		return;

	_boxcolor=BoxColor::BOX_GRAY;
	for(int i=0;i<9;i++)
	{
		auto node=getChildByTag(i+1);
		if(node)
		{
			node->setVisible(false);

			auto graynode=getChildByTag(100+i+1);
			if(graynode)
				graynode->setVisible(true);
			else
			{
				auto gnode=cjSprite::createWithImage("fangkuai_gray.png");
				this->addChild(gnode,0,100+i+1);
				if(GameDataInstance()->d_Project==GAME_1010)
					gnode->setScale(CHESSMAN_SCALE_1010);
				else if(GameDataInstance()->d_Project==GAME_TETRIS)
					gnode->setScale(CHESSMAN_SCALE);
				gnode->setPosition(node->getPosition());
			}
		}
	}
}