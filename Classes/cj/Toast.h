/********************************************************************
	created:	2014/09/05
	created:	5:9:2014   9:35
	filename: 	D:\work7\CocoStudio\Source\2.2.3\cocos2d-x\projects\CandyMania\Classes\views\Toast.h
	file path:	D:\work7\CocoStudio\Source\2.2.3\cocos2d-x\projects\CandyMania\Classes\views
	file base:	Toast
	file ext:	h
	author:		Tyson
	
	purpose:	Toast
*********************************************************************/
#ifndef _GC_TOAST_H__
#define _GC_TOAST_H__

#include "cocos2d.h"
#include <string>
using namespace std;
//#include "layers_scenes_transitions_nodes/CCScene.h"
USING_NS_CC;

class Toast : public cocos2d::Node{


	float m_fDuration;
public:

	typedef enum{
		TOAST_LENGTH_SHORT,
		TOAST_LENGTH_LONG,
	}E_TOAST_LENGTH;	

	static Toast* makeText(Scene* scene,const string& text,int duration = TOAST_LENGTH_SHORT);
	static Toast* makeText(const string& text,int duration = TOAST_LENGTH_SHORT);
	void show();

	virtual bool initWithText(const string& text,int duration);

};

#endif//!_GC_TOAST_H__