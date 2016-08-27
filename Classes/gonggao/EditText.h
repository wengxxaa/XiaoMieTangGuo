#pragma once
#ifndef __EDITTEXT_H__
#define __EDITTEXT_H__


#include <string>
#include "MultipleLabel.h"

USING_NS_CC;
using namespace std;

typedef enum
{
	INPUTALL,
	INPUTNOONLY,//数字
	INPUTNOANDCHAR//字母，数字
}INPUTSTYLE;


class EditText: public TextFieldTTF,public TextFieldDelegate
{
public:
	
	EditText(void);
	~EditText(void);
	static EditText* create();
	
	bool initWithString(const char *label, const char *fontName, float fontSize, 
							const Size& dimensions, TextHAlignment alignment);

	// CCLayer
	void onEnter();
	void onExit();	
	void setColor(Color3B color3);
	// CCTextFieldDelegate
	virtual bool onTextFieldAttachWithIME(TextFieldTTF *pSender);
	virtual bool onTextFieldDetachWithIME(TextFieldTTF * pSender);
	virtual bool onTextFieldInsertText(TextFieldTTF * pSender, const char * text, int nLen);
	virtual bool onTextFieldDeleteBackward(TextFieldTTF * pSender, const char * delText, int nLen);

	// CCLayer Touch
	bool ccTouchBegan(Touch *pTouch, Event *pEvent);
	void ccTouchEnded(Touch *pTouch, Event *pEvent);

	static EditText * textFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize
								);
	static EditText * textFieldWithPlaceHolder(const char *placeholder, Size dimensions, TextHAlignment alignment, 
								const char *fontName, float fontSize);

	//设置字符长度限制，一个汉字三个字符
	void setLimitNum(int limitNum);
	int getLimitNum();
	//设置是否星号显示
	bool getIsPsw();
	void setIsPsw(bool bFlag);
	//重载原函数，用来显示星号
	void setString(string str);
	virtual void setString(const char *displayTx);
	//设置是否可以点击
	void setEnabled(bool ena);
	bool getEnabled();

	//点击弹出输入法的尺寸范围
	void setDesignedSize(Size size);
	Size getDesignedSize();
	//设置焦点
	void setFocus();
	//触屏优先级(适配弹框中的EditText)  默认为 -125
	void setTouchPriority(int priority);

	int getTouchPriority();
	
	//是否是多行显示
	void setWarp(bool warp);
	bool getWarp();

	virtual void setPlaceHolder(const char * text);
	//virtual const char * getPlaceHolder(void);
	void setInputStyle(INPUTSTYLE style);

	int getInputStyle();

	void setEnterInputCallback(Ref *mTag,SEL_CallFunc sel);

	//设置光标颜色
	void setCursorColor(Color3B colo);

protected:
	//添加光标精灵
	void addCursorSprite(Color3B colo = Color3B::WHITE);


	 
private:
	Ref*       m_pListener; 
	SEL_CallFunc    m_pfnSelector;	

	Sprite *m_cursorSprite;	
	int m_limitNum;
	bool m_isPsw;
	// 点击开始位置
	Vec2 m_beginPos;
	// 光标坐标
	Vec2 m_cursorPos;

	//string m_pInputText;
	//关闭输入法
	void closeIME();
	//开启输入法
	void openIME();
	//设置光标的位置
	void setCursorPosition(Touch *pTouch);
	// 判断是否点击在TextField处
	bool isInTextField(Touch *pTouch);
	// 得到TextField矩形
	Rect getRect();

	int m_currentTextPos;

	//是否可以点击
	bool m_enabled;
	//响应点击事件的区域
	Size m_designedSize;
	int m_touchPriority;
	bool m_bTouchEnabled;
	
	void executeCallBack();
	//是否是多行
	bool mWarpEnable;
	MultipleLabel *m_multiple;
	INPUTSTYLE m_inputStyle;
	//std::string m_tempStr;//暂时保存的字符串
};

#endif