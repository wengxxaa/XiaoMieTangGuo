#ifndef BoxLine_H_
#define BoxLine_H_

#include"cocos2d.h"
#include"ConfigState.h"
#include"../cj/GameData.h"
USING_NS_CC;

struct linepos{
	int x;
	int y;
	int p;
	linepos(int a,int b,int c):x(a),y(b),p(c){};
};

class BoxLine:public Node{
public:
	BoxLine(){};
	~BoxLine(){};

	virtual bool init(ShapeIndex shape,AngleIndex angle);
	static BoxLine* createLine(ShapeIndex shape,AngleIndex angle=ANGLE_0)
	{
		BoxLine *one=new BoxLine();
		if(one&&one->init(shape,angle))
		{
			one->autorelease();
			return one;
		}
		else
		{
			CC_SAFE_DELETE(one);
			return nullptr;
		}
	}

private:
	void addLine(linepos ps);//位置和上下左右

private:
	ShapeIndex _shape;
	AngleIndex _angle;
};

#endif