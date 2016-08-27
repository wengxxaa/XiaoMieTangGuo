#ifndef BOXNODE_H_
#define BOXNODE_H_

#include"cocos2d.h"
#include"ConfigState.h"
#include"../cj/GameData.h"
#include"../cj/cjconfig.h"
USING_NS_CC;

class BoxNode:public Node
{
public:
	BoxNode();
	~BoxNode();

	virtual bool init(ShapeIndex shape,unsigned int colorindex,const char* filename);

	static BoxNode* createBox(ShapeIndex shape,unsigned int colorindex=1,const char* filename="cube.png"){
		BoxNode *node=new BoxNode();
		if(node&&node->init(shape,colorindex,filename)){
			node->autorelease();
			return node;
		}
		else{
			CC_SAFE_DELETE(node);
			return nullptr;
		}
	}

public:
	//取方块的覆盖范围
	vector<pair<int,int>> getBoxFill();

	static vector<pair<int,int>> getBoxFillByData(ShapeIndex shape,AngleIndex angle);

	//顺时针旋转
	void Rotate();

	//逆时针旋转
	void ReRotate();

	//设置角度
	void setRotate(AngleIndex angle);

	//位置居中
	Vec2 getCenterAnPoint(Vec2 vec);

	//x轴位置居中
	Vec2 getCenterAnPointX(Vec2 vec);

	//全体设置透明度
	void setOpacity(GLubyte opacity);

	//设置小方块分别缩小
	void setSpecialScale(float ft);

	void setGray();//设置灰色
	void setNormal();//设置正常

private:
	void removeFromId(int id);

public:
	CC_SYNTHESIZE(AngleIndex,_angle,Angle);

	ShapeIndex _shape;

	BoxColor _boxcolor;

	int _boxsize;
};

#endif