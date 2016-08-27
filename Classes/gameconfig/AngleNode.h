#ifndef Angle_H_
#define Angle_H_

#include"cocos2d.h"
#include"ConfigState.h"
USING_NS_CC;

class AngleNode:public Node
{
public:
	AngleNode():_id(1){};
	~AngleNode(){};

	static AngleNode* createNode(int id)
	{
		AngleNode *node=new AngleNode();
		if(node&&node->init(id))
		{
			node->autorelease();
			return node;
		}
		else
		{
			CC_SAFE_DELETE(node);
			return nullptr;
		}
	}

	bool init(int id);

	//x轴位置居中
	Vec2 getCenterAnPointX(Vec2 vec);

	void setGray();//设置灰色
	void setNormal();//设置正常

public:
	int _id;
	BoxColor _boxcolor;
	std::vector<std::pair<int,int>> _boxfill;
};

#endif