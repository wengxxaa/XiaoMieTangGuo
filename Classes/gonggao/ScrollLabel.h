#ifndef _SCROLL_LABEL__
#define _SCROLL_LABEL__

#include "cocos2d.h"
#include "EditText.h"
#include <string>
#include <vector>

USING_NS_CC;
using namespace std;

//特性：剪切域，切换文字
class ScrollLabel : public CCLayer{
	
	class BroadMessage :public CCObject{
	public:

		BroadMessage(){
			BroadMessage("","","");
		}

		BroadMessage(string color,string content, string type){
			this->color = color;
			this->content = content;
			this->type = type;
		}

		string color;

		string content;

		string type;//0系统， 1用户的
	}; 

	CCArray* m_strings;
	//float m_diaplayDuration;

	CCLabelTTF* m_label;
	CCRect m_rect;

	float m_delay;
	float m_duration;

	string m_stringToShow;
	CCClippingNode* mClipper;

	bool m_isAlive;

	SEL_CallFunc m_callfunc;

	CCObject* m_target;

	void initVisableRect(CCRect rect);

public:

	ScrollLabel()
		:m_label(NULL),mClipper(NULL),m_delay(1.0f),m_duration(0.3f),
		m_fontColor(ccc3(255,255,255)),m_isAlive(false),
		m_target(NULL),m_callfunc(NULL){}
	
	~ScrollLabel(){
		CC_SAFE_RELEASE(m_strings);
	}
	
	static ScrollLabel* createWithRect(CCRect& rect,string fontName,int fontSize){
		
		ScrollLabel* pRet = new ScrollLabel();
		if(pRet &&pRet->init(rect,fontName,fontSize)){//&& pRet->initWitheRect(rect)
			pRet->autorelease();
		}
		else{
			CC_SAFE_DELETE(pRet);
		}
		return pRet; 
	}

	CC_SYNTHESIZE(string,m_fontName,FontName);

	CC_SYNTHESIZE(int,m_fontsize,FontSize);

	CC_SYNTHESIZE(ccColor3B,m_fontColor,FontColor);

	bool init(CCRect rect,string fontName,int fontSize);

	virtual void onEnter();

	virtual void onExit();

	void setDisplayTime(float time);

	bool isScrolling();
	void addStringToQueue(string content ,string color="",string type="");

	void setFinishCallfunc(CCObject* target ,SEL_CallFunc callfunc);

protected:

	CCAction* scrollVertical();
	void destroyLabel();
	void switchLabel(BroadMessage* broadMessage);

	void callfuncChangeLabel();

// 	void addStringToQueue(string content){
// 		CCString* ccStr = CCString::create(content);
// 		m_strings->addObject(ccStr);
// 		if (!m_isAlive)
// 		{
// 			update();
// 		}
// 	}
// 	
	void update();

	ccColor3B parseColor(string str);



// 	void parseContent(string body,BroadMessage* msg){
// 
// 		string color;
// 		string::size_type startPos = 0;
// 		string::size_type colorL = body.find_first_of('[');
// 		string::size_type colorR = body.find_first_of(']');
// 
// 		if(colorL!=string::npos && colorR!=string::npos){
// 			if(colorR>colorL){
// 				color = body.substr(colorL+1,colorR-(colorL+1));
// 				startPos = colorR+1;
// 			}
// 		}
// 
// 		msg->color = color;
// 
// 		string::size_type typeL = body.find("[",startPos);
// 		string::size_type typeR = body.find("]",startPos);
// 		string str_type;
// 		if(typeL!=string::npos && typeR!=string::npos){
// 			if(typeR>colorL){
// 				str_type = body.substr(typeL+1,typeR-(typeL+1));
// 				startPos = typeR+1;
// 			}
// 		}
// 
// 		if(0==str_type.compare("sys"))
// 			msg->type = 0;
// 		else
// 			msg->type = 1;
// 
// 		msg->content = body.substr(startPos,body.length()-startPos);
// 
// 	}

};









#endif //_SCROLL_LABEL__