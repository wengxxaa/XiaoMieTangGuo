/********************************************************************
	created:	2014/07/24
	created:	24:7:2014   16:40
	filename: 	D:\work7\CocoStudio\Source\2.2.3\cocos2d-x\common\views\AppInfoLayer.h
	file path:	D:\work7\CocoStudio\Source\2.2.3\cocos2d-x\common\views
	file base:	AppInfoLayer
	file ext:	h
	author:		tyson
	
	purpose:	当兑换码是10021002时，显示此弹框。
*********************************************************************/
#ifndef _APP_INFO_LAYER_H__
#define _APP_INFO_LAYER_H__

#include "cocos2d.h"
USING_NS_CC;
using namespace std;

class AppInfoLayer : public Layer{

	void _menuCloseCallback(Ref* obj);

public:
	
	CREATE_FUNC(AppInfoLayer);
	
	virtual bool init();

	void show();








};
















#endif//!_APP_INFO_LAYER_H__