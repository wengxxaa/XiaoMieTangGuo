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
	//ȡ����ĸ��Ƿ�Χ
	vector<pair<int,int>> getBoxFill();

	static vector<pair<int,int>> getBoxFillByData(ShapeIndex shape,AngleIndex angle);

	//˳ʱ����ת
	void Rotate();

	//��ʱ����ת
	void ReRotate();

	//���ýǶ�
	void setRotate(AngleIndex angle);

	//λ�þ���
	Vec2 getCenterAnPoint(Vec2 vec);

	//x��λ�þ���
	Vec2 getCenterAnPointX(Vec2 vec);

	//ȫ������͸����
	void setOpacity(GLubyte opacity);

	//����С����ֱ���С
	void setSpecialScale(float ft);

	void setGray();//���û�ɫ
	void setNormal();//��������

private:
	void removeFromId(int id);

public:
	CC_SYNTHESIZE(AngleIndex,_angle,Angle);

	ShapeIndex _shape;

	BoxColor _boxcolor;

	int _boxsize;
};

#endif