#ifndef __MULTIPLE_LABEL_H__
#define __MULTIPLE_LABEL_H__
/**
* 分段的文字显示
* 
*/

#define R_FONT_NORMAL "黑体"

#include "cocos2d.h"

using namespace cocos2d;

struct INDEX_STRU
{
	int sBegin;
	int sLen;
	int offsetX;
};

class MultipleLabel : public Node
{
public:
	MultipleLabel();
	~MultipleLabel();
	static MultipleLabel* create(const char* label, float width,float gap,float fontSize,int autoTab, int firstline);
	static MultipleLabel* labelWithString(const char *label, const Size dimensions, Vec2 alignment, const char *fontName, float fontSize,int autoTab=0,int firstlineTab=0);
	//==================
	void setString(const char* pStr);
	const char* getString(void);
	void setStringSize(const char* pStr);
	void InitLabel(const char *label, const Size& dimensions, Vec2 alignment, const char *fontName, float fontSize);
	void setColor(const Color3B& color3);
	void setOpacity(GLubyte byte);
	CC_SYNTHESIZE(int, _autoTab,AutoTab);
	CC_SYNTHESIZE(int, _firstLineTab,FirstLineTab);
	CC_SYNTHESIZE(int, _transMeanCharN,TransMeanCharN);
	//设置显示类型  true 最下面的label位置不变  false 第一个label的位置不变 
	void setLabelMode(bool labelModel = true);
	bool getLabelMode();
private:	
	const char*					mFontName;
	float						mFontSize;
	Size						mDimensions;
	Vec2						mAlignment;
	Color3B					mColor3;
	std::vector<INDEX_STRU>		mIndex;
	std::vector<Label*>	mLabel;
	std::string					mString;
	bool						mLabelMode;
};

#endif//__MULTIPLE_LABEL_H__