#include"BaseConfig.h"

//StartBox
bool StartBox::init(int color)
{
	if(!Node::init())
		return false;

	_state=STATE_01;
	_color=color;

	this->setCascadeOpacityEnabled(true);

	auto node=cjSprite::createWithImage(startstr1[color-1]);
	this->addChild(node,0,"box01");
	//node->setAnchorPoint(Vec2(0.5f,0.0f));
	node->setPosition(Vec2(0,0));

	return true;
}

void StartBox::setBox2()
{
	if(_state==STATE_02)
		return;

	_state=STATE_02;
	auto box01=getChildByName("box01");
	box01->setVisible(false);

	auto box02=getChildByName("box02");
	if(box02)
		box02->setVisible(true);
	else
	{
		auto node=cjSprite::createWithImage(startstr2[_color-1]);
		this->addChild(node,0,"box02");
		//node->setAnchorPoint(Vec2(0.5f,0.0f));
		node->setPosition(box01->getPosition());
	}
}

void StartBox::setBox1()
{
	if(_state==STATE_01)
		return;

	_state=STATE_01;
	auto box01=getChildByName("box01");
	box01->setVisible(true);
	auto box02=getChildByName("box02");
	box02->setVisible(false);
}
//
