#include"AngleNode.h"
#include"../cj/cjconfig.h"

bool AngleNode::init(int id)
{
	if(!Node::init())
		return false;

	_id=id;
	_boxcolor=BoxColor::BOX_NORMAL;

	Sprite *sp[4];
	char name[30];
	sprintf(name,"angle_%d.png",id/2+1);
	for(int i=0;i<4;i++)
	{
		sp[i]=cjSprite::createWithImage(name);
		this->addChild(sp[i],1,i+1);
	}

	_boxfill.push_back(make_pair(0,0));
	switch (id)
	{
	case 1:
		break;
	case 2:
		_boxfill.push_back(make_pair(1,0));
		_boxfill.push_back(make_pair(1,1));
		break;
	case 3:
		_boxfill.push_back(make_pair(0,1));
		_boxfill.push_back(make_pair(1,1));
		break;
	case 4:
		_boxfill.push_back(make_pair(0,1));
		_boxfill.push_back(make_pair(0,2));
		break;
	case 5:
		_boxfill.push_back(make_pair(1,1));
		_boxfill.push_back(make_pair(2,2));
		break;
	case 6:
		_boxfill.push_back(make_pair(-1,0));
		_boxfill.push_back(make_pair(1,0));
		break;
	case 7:
		_boxfill.push_back(make_pair(-1,0));
		_boxfill.push_back(make_pair(1,0));
		_boxfill.push_back(make_pair(2,0));
		break;
	case 8:
		_boxfill.push_back(make_pair(0,1));
		_boxfill.push_back(make_pair(0,2));
		_boxfill.push_back(make_pair(0,3));
		break;
	case 9:
		_boxfill.push_back(make_pair(1,1));
		_boxfill.push_back(make_pair(2,2));
		_boxfill.push_back(make_pair(3,3));
		break;
	case 10:
		_boxfill.push_back(make_pair(-1,0));
		_boxfill.push_back(make_pair(1,0));
		_boxfill.push_back(make_pair(2,1));
		break;
	case 11:
		_boxfill.push_back(make_pair(0,1));
		_boxfill.push_back(make_pair(1,0));
		_boxfill.push_back(make_pair(2,0));
		break;
	case 12:
		_boxfill.push_back(make_pair(1,1));
		_boxfill.push_back(make_pair(2,2));
		_boxfill.push_back(make_pair(2,1));
		break;
	case 13:
		_boxfill.push_back(make_pair(0,1));
		_boxfill.push_back(make_pair(0,2));
		_boxfill.push_back(make_pair(-1,1));
		break;
	case 14:
		_boxfill.push_back(make_pair(1,0));
		_boxfill.push_back(make_pair(1,1));
		_boxfill.push_back(make_pair(2,1));
		break;
	case 15:
		_boxfill.push_back(make_pair(0,1));
		_boxfill.push_back(make_pair(1,0));
		_boxfill.push_back(make_pair(1,1));
		break;
	case 16:
		_boxfill.push_back(make_pair(1,1));
		_boxfill.push_back(make_pair(2,0));
		_boxfill.push_back(make_pair(2,1));
		break;
	case 17:
		_boxfill.push_back(make_pair(1,0));
		_boxfill.push_back(make_pair(0,1));
		_boxfill.push_back(make_pair(2,1));
		break;
	default:
		break;
	}

	int k=0;
	for(auto &i:_boxfill)
	{
		sp[k]->setPosition(Vec2(i.first*CHESSMAN_SIZE_W_ANGLE,i.second*CHESSMAN_SIZE_H_ANGLE)+Vec2((-i.second/2.0f*CHESSMAN_SIZE_W_ANGLE),0));
		k++;
	}
	for(int i=k;i<4;i++)
	{
		sp[k]->removeFromParent();
	}
	return true;
}

//x轴位置居中
Vec2 AngleNode::getCenterAnPointX(Vec2 vec)
{
	Vec2 ve;
	switch(_id)
	{
	case 1:
	case 3:
	case 6:
	case 15:
		ve=vec;
		break;
	case 2:
	case 5:
	case 7:
	case 10:
	case 11:
	case 14:
	case 17:
		ve=vec-Vec2(CHESSMAN_SIZE_W_ANGLE/2.0f,0)*getScale();
		break;
	case 4:
		ve=vec+Vec2(CHESSMAN_SIZE_W_ANGLE/2.0f,0)*getScale();
		break;
	case 8:
	case 13:
		ve=vec+Vec2(CHESSMAN_SIZE_W_ANGLE,0)*getScale();
		break;
	case 9:
	case 12:
	case 16:
		ve=vec-Vec2(CHESSMAN_SIZE_W_ANGLE,0)*getScale();
		break;
	default:
		break;
	}
	return ve+Vec2(0,CHESSMAN_SIZE_H_ANGLE/2.0f);
}

//设置灰色
void AngleNode::setGray()
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
				auto gnode=cjSprite::createWithImage("angle_hs.png");
				this->addChild(gnode,0,100+i+1);
				gnode->setScale(CHESSMAN_SCALE_ANGLE);
				gnode->setPosition(node->getPosition());
			}
		}
	}
}

//设置正常
void AngleNode::setNormal()
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