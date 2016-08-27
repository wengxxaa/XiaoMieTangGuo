
/*!
  @file  		MultipleLabel.cpp
  @brief  		MultipleLabel.cpp
  @details 		（多行文本）
  @date 		 2013/08/16
  @author 		 Tyson
 */
#include "MultipleLabel.h"

MultipleLabel::MultipleLabel()
{
	//CCLOG("MultipleLabel::MultipleLabel	in");

	mColor3 = Color3B(255,255,255);
	setAnchorPoint(Vec2(0, 0));
	mString = "";
	_autoTab=0;
	_firstLineTab = 0;
	_transMeanCharN = 1;

	//CCLOG("MultipleLabel::MultipleLabel	out");
}
MultipleLabel::~MultipleLabel()
{
	//CCLOG("MultipleLabel::~MultipleLabel	in");
	//CCLOG("MultipleLabel::~MultipleLabel	out");
}

MultipleLabel* MultipleLabel::create(const char* label, float width,float gap,float fontSize,int autoTab, int firstline){

	return labelWithString(label, Size(width,gap), Vec2(0,0.5),R_FONT_NORMAL,fontSize,autoTab,firstline);
}

MultipleLabel* MultipleLabel::labelWithString(const char *label, const Size dimensions, Vec2 alignment, const char *fontName, float fontSize,int autoTab,int firstlineTab)
{
	//CCLOG("MultipleLabel::labelWithString	in");

	MultipleLabel* nLabel = new MultipleLabel();
	if(nLabel != NULL)
	{
		nLabel->autorelease();
		nLabel->_autoTab = autoTab;
		nLabel->_firstLineTab = firstlineTab;
		nLabel->InitLabel(label, dimensions, alignment, fontName, fontSize);
	}

	//CCLOG("MultipleLabel::labelWithString	out");
	return nLabel;
}
#if 0
void MultipleLabel::setStringSize(const char* pStr)
{
	//CCLOG("MultipleLabel::setStringSize	in");

	INDEX_STRU nIndex;
	int wCount,sCount, nLen, nKey;
	float wKey;
	nLen = strlen(pStr);
	sCount = 0;
	wCount = 0;
	nIndex.sBegin = 0;
	while(sCount < nLen)
	{
		if(sCount < nLen-1 && pStr[sCount] =='\\' && pStr[sCount+1] == 'n')
		{
			nIndex.sLen = sCount - nIndex.sBegin;
			mIndex.push_back(nIndex);
			sCount += 2;
			wCount = 0;
			nIndex.sBegin = sCount;
			continue;
		}
		if((pStr[sCount]&0x80) == 0)
		{
			wKey = mFontSize/2;
			nKey = 1;
		}
		else
		{
			wKey = mFontSize;
			nKey = 3;
		}
		if(wCount +  wKey > mDimensions.width)
		{
			wCount = 0;
			nIndex.sLen = sCount - nIndex.sBegin;
			mIndex.push_back(nIndex);
			nIndex.sBegin = sCount;
		}
		else
		{	
			wCount += wKey;
			sCount += nKey;
		}
		
			
	}
	nIndex.sLen = nLen - nIndex.sBegin;
	mIndex.push_back(nIndex);
	setContentSize(Size(mDimensions.width, mIndex.size()*(mFontSize+mDimensions.height) - mDimensions.height));
	//setContentSize(Size(mDimensions.width, (mIndex.size())*(mFontSize+mDimensions.height)));
	//setAnchorPoint(Vec2(0.5, 0.5));
	setAnchorPoint(Vec2(0, 0));

	//CCLOG("MultipleLabel::setStringSize	out");
}

#else

void MultipleLabel::setStringSize(const char* pStr)
{
/*	char nStrTmp[500];*/
	INDEX_STRU nIndex;
	int wCount,sCount, nLen, nKey;
	float wKey;
	nLen = strlen(pStr);
	sCount = 0;
	wCount = 0;
	nIndex.sBegin = 0;
	
	//中国
	const char ARMY_CHINA[] = {(char)0xe4,(char)0xb8,(char)0xad,(char)0xe5,(char)0x9b,(char)0xbd,(char)0x00};
	Label* nLabel = Label::createWithSystemFont(ARMY_CHINA, mFontName, mFontSize);

	int TabW = nLabel->getContentSize().width;
	int OneWordW = TabW/2;

	int baseStrWidth = TabW;


	int isFirstLine=1;
	int firstTabW=0;
	if(_firstLineTab)
	{		
		firstTabW = TabW;		
	}

	int curTabW = 0;
	if(_autoTab && (mAlignment.x==0))
	{
		curTabW = TabW;
	}
	else
	{
		curTabW = 0;
		TabW = 0;
	}

	while(sCount < nLen)
	{
		int isReturn=0;
		if(pStr[sCount] =='\r')
		{
			isReturn=1;
			if(sCount+1<nLen)
			{
				if(pStr[sCount+1] == '\n')
				{
					isReturn=2;
				}
			}			
		}
		else if(pStr[sCount] =='\n')
		{
			isReturn=1;			
		}
		else if(_transMeanCharN)//转移符
		{
			if(sCount+1<nLen)
			{
				if(pStr[sCount] =='\\'&& pStr[sCount+1] == 'n')
				{
					isReturn=2;
				}
			}
		}

// 		if(
// 			(sCount <= nLen-1 && ((pStr[sCount] =='\r'&& pStr[sCount+1] == '\n') || pStr[sCount] =='\n' ||  pStr[sCount] =='\r'))
// 			||(_transMeanCharN && sCount <= nLen-1 && (pStr[sCount] =='\\'&& pStr[sCount+1] == 'n')) 
// 			)
		if(isReturn)
		{
			nIndex.sLen = sCount - nIndex.sBegin;			
			if(isFirstLine)
			{
				nIndex.offsetX = firstTabW;
				isFirstLine=0;
			}
			else
			{
				nIndex.offsetX = curTabW;
			}
			mIndex.push_back(nIndex);			
// 			if((pStr[sCount] =='\r')&& (pStr[sCount+1] == '\n'))
// 			{
// 				sCount += 2;
// 			}
// 			else if(_transMeanCharN && (pStr[sCount] =='\\')&& (pStr[sCount+1] == 'n'))
// 			{
// 				sCount += 2;
// 			}
// 			else
// 			{
// 				sCount += 1;
// 			}
			sCount += isReturn;
			wCount = 0;
			nIndex.sBegin = sCount;	
			curTabW = TabW;			
			continue;
		}

		if((pStr[sCount]&0x80) == 0)
		{
			char mesureStr[64];
			char tmpStr[2];
			tmpStr[0] = pStr[sCount];
			tmpStr[1] = 0x00;
			//nLabel->setString(tmpStr);			
			//wKey = nLabel->getContentSize().width;
			//修改闪退问题测试 //测量一个不可见字符可能会闪退
			sprintf(mesureStr,"%s%s",ARMY_CHINA,tmpStr);
			nLabel->setString(mesureStr);			
			wKey = nLabel->getContentSize().width;
			wKey -= baseStrWidth;
			if(wKey<0)
				wKey=0;

			//wKey = OneWordW/2;
			nKey = 1;
		}
		else
		{
			wKey = OneWordW;
			nKey = 3;

			//《》？“”：。，、符号特殊处理
			unsigned char key_S[9][4]={
				{0XE3,0X80,0X8A,0x00},
				{0XE3,0X80,0X8B,0x00},
				{0XEF,0XBC,0X9F,0x00},
				{0XE2,0X80,0X9C,0x00},
				{0XE2,0X80,0X9D,0x00},
				{0XEF,0XBC,0X9A,0x00},
				{0XE3,0X80,0X82,0x00},
				{0XEF,0XBC,0X8C,0x00},
				{0XE3,0X80,0X81,0x00},
			};
			for(int i=0;i<9;i++)
			{
				unsigned char c1 = (unsigned char)pStr[sCount];
				unsigned char c2 = 0x00;
				unsigned char c3 = 0x00;

				if(sCount+1<nLen)
				{
					c2 = (unsigned char)pStr[sCount+1];
				}
				if(sCount+2<nLen)
				{
					c3 = (unsigned char)pStr[sCount+2];
				}
				
				if(
					(c1==key_S[i][0]) 
					&& (c2==key_S[i][1])
					&& (c3==key_S[i][2])
					)
				{
					nLabel->setString((char*)key_S[i]);			
					wKey = nLabel->getContentSize().width;
					break;
				}
			}			
			
		}
		
		//memset(nStrTmp,0x00,sizeof(nStrTmp));
		//memcpy(nStrTmp, &pStr[nIndex.sBegin], sCount - nIndex.sBegin +  nKey);
		//Label* nLabel = Label::labelWithString(nStrTmp, mFontName, mFontSize);
		//nLabel->setString(nStrTmp);
		//int wCal = nLabel->getContentSize().width;
		
		//首行缩进否
		if(isFirstLine)
		{
			if(wCount + wKey + firstTabW > mDimensions.width)
			{
				wCount = 0;
				nIndex.sLen = sCount - nIndex.sBegin;								
				nIndex.offsetX = firstTabW;
				mIndex.push_back(nIndex);
				nIndex.sBegin = sCount;
				isFirstLine=0;
				curTabW=0;
			}
			else
			{
				wCount += wKey;
				sCount += nKey;
			}

		}
		else
		{
			if(wCount + wKey + curTabW > mDimensions.width)
			{
				wCount = 0;
				nIndex.sLen = sCount - nIndex.sBegin;				
				nIndex.offsetX = curTabW;
				mIndex.push_back(nIndex);
				nIndex.sBegin = sCount;
				curTabW= 0;
			}
			else
			{	
				wCount += wKey;
				sCount += nKey;
			}
		}
		


	}
	nIndex.sLen = nLen - nIndex.sBegin;
	//首行缩进否
	if(isFirstLine)
	{
		nIndex.offsetX = firstTabW;
	}
	else
	{
		nIndex.offsetX = curTabW;
	}
	mIndex.push_back(nIndex);

	//setContentSize(Size(mDimensions.width, (mIndex.size())*(mFontSize+mDimensions.height)-mDimensions.height));
	setContentSize(Size(mDimensions.width, (mIndex.size())*(mFontSize+mDimensions.height)));
	setAnchorPoint(Vec2(0, 0));

	//CCLOG("MultipleLabel::setStringSize	out");
}
#endif

void MultipleLabel::setString(const char* pStr)
{
//	CCLOG("MultipleLabel::setString	in");
	
	mString = pStr;
	removeAllChildrenWithCleanup(true);
	mIndex.clear();
	mLabel.clear();
	setStringSize(pStr);
	int new_h=0;
	//====================
	Label* nLabel;
	char nStrTmp[600];
	int i;
	float nY=0, nX=0;
	nX = mDimensions.width*mAlignment.x;
	if(mLabelMode)
	{
		nY = (mIndex.size()-1)*(mFontSize+mDimensions.height);
	}
	for(i = 0; i < (int)(mIndex.size()); i++)
	{
		memcpy(nStrTmp, &pStr[mIndex[i].sBegin], mIndex[i].sLen);
		nStrTmp[mIndex[i].sLen] = 0;
		nLabel = Label::createWithSystemFont(nStrTmp, mFontName, mFontSize);
		nLabel->setAnchorPoint(mAlignment);
		if(mLabelMode)
		{
			nLabel->setPosition(Vec2(nX+mIndex[i].offsetX, nY));	
		}else
		{
			nLabel->setPosition(Vec2(nX+mIndex[i].offsetX, -nY));	
		}
		nLabel->setColor(mColor3);
		mLabel.push_back(nLabel);
		addChild(nLabel);
		//====================
		if(mLabelMode)
		{
			nY -= (mFontSize+mDimensions.height);
		}else
		{
			nY += (mFontSize+mDimensions.height);
		}
	}
	//修正 contentSize();出错的情况
	//CCLabel遇到"\r\n"也会自动换行
	//

//	CCLOG("MultipleLabel::setString	out");
}

const char* MultipleLabel::getString()
{
	return mString.c_str();
}
void MultipleLabel::InitLabel(const char *label, const Size& dimensions, Vec2 alignment, const char *fontName, float fontSize)
{
//	CCLOG("MultipleLabel::InitLabel	in");

	mFontName = fontName;
	mFontSize = fontSize;
	mDimensions = dimensions;
	mAlignment = alignment;
	mLabelMode = true;
	setString(label);

	//CCLOG("MultipleLabel::InitLabel	out");
}
void MultipleLabel::setColor(const Color3B& color3)
{
//	CCLOG("MultipleLabel::setColor	in");

	mColor3 = color3;
	for(int i = 0; i < (int)(mLabel.size()); i++)
	{
		mLabel[i]->setColor(color3);
	}

//	CCLOG("MultipleLabel::setColor	out");
}

void MultipleLabel::setOpacity(GLubyte byte)
{
	//CCLOG("CCLabelSTF::setOpacity	in");
	for(int i = 0; i < (int)(mLabel.size()); i++)
	{
		mLabel[i]->setOpacity(byte);
	}
	//CCLOG("CCLabelSTF::setOpacity	out");
}
void MultipleLabel::setLabelMode(bool labelModel)
{
	mLabelMode = labelModel;
}
bool MultipleLabel::getLabelMode()
{
	return mLabelMode;
}