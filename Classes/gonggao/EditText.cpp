#include "EditText.h"
#include "GC_Tool.h"


static int _calcCharCount(const char * pszText)
{
	int n = 0;
	char ch = 0;
	while ((ch = *pszText))
	{
		CC_BREAK_IF(! ch);

		if (0x80 != (0xC0 & ch))
		{
			++n;
		}
		++pszText;
	}
	return n;
}

const static float DELTA = 20.0f;

//点击有效区域
//Size m_size;
float m_fontSize;

EditText::~EditText()
{	
}
EditText::EditText()
{
	m_cursorSprite = NULL;	
	_inputText = "";
	m_touchPriority = -125;
	m_inputStyle = INPUTALL;
	m_bTouchEnabled = false;
	m_pListener = NULL;
	m_pfnSelector = NULL;
}

EditText* EditText::create()
{
	return textFieldWithPlaceHolder("",Size::ZERO ,TextHAlignment::CENTER,"黑体",20);
}
 EditText * EditText::textFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize)
{
	return textFieldWithPlaceHolder(placeholder,Size::ZERO ,TextHAlignment::CENTER,fontName,fontSize);
}
 EditText * EditText::textFieldWithPlaceHolder(const char *placeholder, Size dimensions, 
								TextHAlignment alignment, const char *fontName, float fontSize)
{	
	EditText *pRet = new EditText();
	//m_size = dimensions;
	m_fontSize = fontSize;
	
	if(pRet && pRet->initWithString("", fontName, fontSize,dimensions,alignment))
	{
		pRet->autorelease();
		if (placeholder)
		{
			pRet->setPlaceHolder(placeholder);
		}
		pRet->addCursorSprite();
		return pRet;
	}		
		delete pRet; 
		pRet = NULL;		
		return NULL;
}

 bool EditText::initWithString(const char *label, const char *fontName, float fontSize, 
	 const Size& dimensions, TextHAlignment alignment)
 {
	 m_isPsw = false;
	 m_limitNum = 30;
	 m_fontSize = fontSize;
	 m_currentTextPos = 0;
	// m_size = dimensions;
	 m_designedSize = dimensions;
	 m_enabled = true;
	 mWarpEnable = false;
	 if(!Label::createWithSystemFont(label,fontName,fontSize,dimensions,alignment))
	 {
		return false;
	 }	

	 /*m_multiple =  MultipleLabel::labelWithString("",Size(m_designedSize.width,0),Vec2(0,0),fontName,fontSize);
	 m_multiple->setLabelMode(false);
	 this->addChild(m_multiple);
	 */
	
	 return true;
 }

void EditText::onEnter()
{
	TextFieldTTF::onEnter();
	
	this->setDelegate(this);

	if(!m_bTouchEnabled)
	{
		//Director::getInstance()->getTouchDispatcher()->removeDelegate(this);
		//Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,m_touchPriority,false);
		// Register Touch Event
		_eventDispatcher->removeAllEventListeners();
		auto listener = EventListenerTouchOneByOne::create();
		//listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(EditText::ccTouchBegan, this);
		_eventDispatcher->addEventListenerWithFixedPriority(listener, m_touchPriority);
		m_bTouchEnabled = true;
	}
}

void EditText::onExit()
{
	TextFieldTTF::onExit();
	closeIME();
	_eventDispatcher->removeAllEventListeners();	
	//if (m_bTouchEnabled)
	{		
		//Director::getInstance()->getTouchDispatcher()->removeDelegate(this);
		//m_bTouchEnabled = false;
	}

}

void EditText::addCursorSprite(Color3B colo)
{	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

	//const int lwidth = 2;
	//const int lheight = 20;
	//int alp = 255;
	//unsigned int co = 0x0;

	//co = alp<<24|colo.r<<16|colo.g<<8|colo.b;
	////CCLOG("%d",co);
	//int pixels[lheight][lwidth];
	//for(int i =0 ; i < lheight;i++)
	//{
	//for(int j =0 ; j <lwidth;j++)
	//{
	//pixels[i][j] = co;
	//}
	//}
	//Texture2D *texture = new Texture2D();
	//texture->initWithData(pixels, sizeof(pixels) / sizeof(pixels[0][0]),Texture2D::PixelFormat::RGB888, 1, 1, Size(lwidth, lheight));
	//m_cursorSprite = Sprite::createWithTexture(texture);
	//m_cursorSprite->setPosition(Vec2(0,m_fontSize /2));
	//texture->release();	
	//m_cursorSprite->runAction(RepeatForever::create((ActionInterval *) Sequence::createWithTwoActions(FadeOut::create(0.25f), FadeIn::create(0.25f))));
	//m_cursorSprite->setVisible(false);
	//this->addChild(m_cursorSprite);
	//_inputText = "";
#endif
}

void  EditText::setCursorColor(Color3B colo)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

	//const int lwidth = 2;
	//const int lheight = 20;
	//int alp = 255;
	//unsigned int co = 0x0;

	//co = alp<<24|colo.r<<16|colo.g<<8|colo.b;
	////CCLOG("%ud",co);
	//int pixels[lheight][lwidth];
	//for(int i =0 ; i < lheight;i++)
	//{
	//	for(int j =0 ; j <lwidth;j++)
	//	{
	//		pixels[i][j] = co;
	//	}
	//}
	//Texture2D *texture = new Texture2D();
	//texture->initWithData(pixels, sizeof(pixels) / sizeof(pixels[0][0]), Texture2D::PixelFormat::RGB888, 1, 1, Size(lwidth, lheight));
	//m_cursorSprite->setTexture(texture);
	//texture->release();
#endif
}
void EditText::setColor(Color3B color3)
{
	TextFieldTTF::setColor(color3);
	_displayedColor = color3;
	m_multiple->setColor(color3);
	//if (m_bOpacityModifyRGB)
	{
		
		_displayedColor.r = color3.r * _displayedOpacity/255.0f;
		_displayedColor.g = color3.g * _displayedOpacity/255.0f;
		_displayedColor.b = color3.b * _displayedOpacity/255.0f;
	}
	updateColor();
	setCursorColor(color3);
}

// CCTextFieldDelegate
bool EditText::onTextFieldAttachWithIME(TextFieldTTF *pSender)
{
	if (_inputText.empty()) {
		return false;
	}
	//CCLog("onTextFieldAttachWithIME  -----------    %f",getContentSize().width );
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	setCursorPosition(NULL);
#endif
	return false;
}
bool EditText::onTextFieldDetachWithIME(TextFieldTTF * pSender)
{
	//CCLOG("onTextFieldDetachWithIME..............");
	return false;
}
bool EditText::onTextFieldInsertText(TextFieldTTF * pSender, const char * text, int nLen)
{
	if ('\n' == *text)
	{		
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
				m_cursorSprite->setVisible(false);
		#endif
			executeCallBack();
			closeIME();
		return false;
	}else if(' ' == *text)
	{
		return true;
	}else 
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
				
			
		int lno = (int)(*text);
		if(INPUTNOONLY == m_inputStyle )
		{
			if(lno < 48 || lno > 57)
			{
				return true;
			}
		}else if(INPUTNOANDCHAR == m_inputStyle)
		{
			if(lno < 48 || lno > 122)
			{
				return true;
			}else if(lno >90 && lno <97)
			{
				return true;
			}
		}	
#else if(CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
		char *chas = (char *)text;
		for(int i =0 ; i < nLen;i++)
		{				
			int lno = text[i] ;
			if(INPUTNOONLY == m_inputStyle )
			{
				if(lno < 48 || lno > 57)
				{
					chas[i]=0;
					nLen--;
				}
			}else if(INPUTNOANDCHAR == m_inputStyle)
			{
				if(lno < 48 || lno > 122)
				{
					chas[i]=0;
					nLen--;
				}else if(lno >90 && lno <97)
				{
					chas[i]=0;
					nLen--;
				}
			}		
		}
		text = chas;		
#endif		
	}			
	std::string tempStr = _inputText.substr();	
	//tempStr.insert(m_currentTextPos,text);
	tempStr.append(text);		

	if (GC_Tool::sharedTool()->getUtf8Length(tempStr.c_str()) > m_limitNum) 
	{		
		tempStr = GC_Tool::sharedTool()->subUtfString(tempStr.c_str(),0,m_limitNum);
	}
	
# if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)		
	
	if(m_limitNum > GC_Tool::sharedTool()->getUtf8Length(tempStr.c_str()))
	{
		if(nLen >1)
		{
			m_currentTextPos +=2;
		}else if(nLen == 1)
		{
			m_currentTextPos ++;	
		}
	}
	else 
	{
		m_currentTextPos = m_limitNum;
	}
	//m_cursorSprite->setPositionX(m_fontSize / 2 * m_currentTextPos);	

	
	setCursorPosition(NULL);
	setString(tempStr.c_str());		

#else if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	return false;
#endif
	return true;
}

bool EditText::onTextFieldDeleteBackward(TextFieldTTF * pSender, const char * delText, int nLen)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	_inputText.resize(_inputText.size() - nLen);
#else if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)	
	 
	
	if(nLen > 0)
	{		
		_inputText.resize(_inputText.size() - nLen);		
		//_inputText.erase(m_currentTextPos-nLen,nLen);
	}		
	if(nLen > 1)
	{
		m_currentTextPos -= 2; 
	}else 
	{
		m_currentTextPos --;
	}
	
	setCursorPosition(NULL);
#endif
	setString(_inputText);
	return true;
}

bool EditText::ccTouchBegan(Touch *pTouch, Event *pEvent)
{
	if(!m_enabled || !this->isVisible())
	{
		return false;
	}
	if(getParent() && !getParent()->isVisible())
	{

		return false;
	}

	if(isInTextField(pTouch))
	{
		m_beginPos = pTouch->getLocationInView();
		m_beginPos = Director::getInstance()->convertToGL(m_beginPos);	
		return true;
	}	
	closeIME();
	return false;
}
void EditText::ccTouchEnded(Touch *pTouch, Event *pEvent)
{
	Vec2 endPos = convertTouchToNodeSpaceAR(pTouch);
	endPos = Director::getInstance()->convertToGL(endPos);

	if(isInTextField(pTouch))
	{
		openIME();
		setCursorPosition(pTouch);
		//fireEvents();
	}else
	{
		closeIME();
	}

	//CCLog(getString());
}

void EditText::setFocus()
{
	openIME();
	setCursorPosition(NULL);
}


void EditText::setCursorPosition(Touch *pTouch)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if(m_cursorSprite)
	{
		string s = getString();
		Vec2 pToushPos = Vec2::ZERO;
		int cLength = GC_Tool::sharedTool()->getUtf8Length(s.c_str());	
		if(m_currentTextPos == 0)
		{
			m_currentTextPos = cLength;	
		}
		//if(pTouch && !getWarp())
		//{
		//	pToushPos = convertTouchToNodeSpaceAR(pTouch);
		//	pToushPos.x += m_designedSize.width / 2;
		//	if(pToushPos.x < m_cursorSprite->getPositionX())
		//	{
		//		int tem = 0;//(int)((pToushPos.x +m_designedSize.width / 2) / (m_fontSize / 2));

		//		int tel = getHorizontalAlignment();
		//		if(tel == 0)
		//		{
		//			tem = (int)(pToushPos.x / (m_fontSize / 2));
		//		}
		//		else
		//		{
		//			tem = (int)(pToushPos.x * 2 / getHorizontalAlignment() / (m_fontSize / 2));
		//		}

		//		m_currentTextPos = tem;		

		//	}else
		//	{
		//		m_currentTextPos = cLength;
		//	}
		//}	

		float ft = (m_fontSize / 2 * m_currentTextPos + m_designedSize.width) * ((int)getHorizontalAlignment()) / 2.0f ;
		if(ft >= m_designedSize.width && m_currentTextPos <= cLength)
		{
			ft = m_designedSize.width - (cLength - m_currentTextPos) *m_fontSize/ 2;
		}
		else if(ft >m_designedSize.width)
		{
			ft = m_designedSize.width;
		}
		else if(m_currentTextPos >0 && ft<=0)
		{
			ft = m_fontSize / 2 * m_currentTextPos;
		}
		m_cursorSprite->setPositionX(ft);
	}
#endif
}

bool EditText::isInTextField(Touch *pTouch)
{
	Vec2 pToushPos = convertTouchToNodeSpaceAR(pTouch);
	return getRect().containsPoint(pToushPos);
}

Rect EditText::getRect()
{
	Size size	;	
	if(&m_designedSize != NULL)
	{
		size = m_designedSize;
	}else
	{
		size = getContentSize();
	}	
	Rect rect = Rect(0 - size.width * getAnchorPoint().x, 0 - size.height * getAnchorPoint().y, size.width, size.height);	
	if(mWarpEnable)
	{
		rect.setRect(0 - size.width * getAnchorPoint().x, m_fontSize * 2 - size.height - size.height /2 * getAnchorPoint().y,size.width, size.height);
	}
	return  rect;
}


void EditText::closeIME()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	m_cursorSprite->setVisible(false);
#endif	
	//if()
	this->detachWithIME();
}
void EditText::openIME()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	m_cursorSprite->setVisible(true);
#endif
	this->attachWithIME();
}


int EditText::getLimitNum()
{
	return m_limitNum;
}
//设置字符长度
void EditText::setLimitNum(int limitNum)
{
	m_limitNum = limitNum;
}
bool EditText::getIsPsw()
{
	return m_isPsw;
}
//设置星号显示否
void EditText::setIsPsw( bool bFlag)
{
	m_isPsw = bFlag;
}
void EditText::setString(string str)
{
	setString(str.c_str());
}
void EditText::setString(const char* displayTx)
{	
	string s = displayTx;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (GC_Tool::sharedTool()->getUtf8Length(s.c_str()) > m_limitNum) 
	{		
		s = GC_Tool::sharedTool()->subUtfString(s.c_str(),0,m_limitNum);
	}
#endif
	_inputText = "";
	
	_inputText = s;
	
	if (! _inputText.length())
	{
		if (m_isPsw) {
			std::string tempStr;
			for (unsigned int i = 0; i < _placeHolder.size(); i++) {
				tempStr.append("*");
			}		
			Label::setString(tempStr.c_str());
		}else
		{
			Label::setString(_placeHolder.c_str());
		}		
		if(mWarpEnable)
		{
			m_multiple->setString("");
		}
		m_currentTextPos = 0;
		setCursorPosition(NULL);
	}
	else
	{
		if (m_isPsw) {
			std::string tempStr;
			for (unsigned int i = 0; i < _inputText.size(); i++) {
				tempStr.append("*");
			}		
			Label::setString(tempStr.c_str());
		}else
		{
			if(mWarpEnable)
			{					
				m_multiple->setString(_inputText.c_str());				
				Label::setString("");
			}else
			{
				Label::setString(_inputText.c_str());
			}
		}		
	}
	  
}

void EditText::setDesignedSize(cocos2d::Size size)
{
	m_designedSize = size;	
	Label::setDimensions(m_designedSize.width,m_designedSize.height);
}

Size EditText::getDesignedSize()
{
	return m_designedSize;
}


void EditText::setEnabled(bool ena)
{
	m_enabled = ena;
}

bool EditText::getEnabled()
{
	return m_enabled;
}


//void EditText::fireEvents()
//{
//	/*if(m_taget && m_selector)
//	{
//	m_taget->runAction(m_selector);
//	}*/
//}

int EditText::getTouchPriority()
{
	return m_touchPriority;
}

void EditText::setTouchPriority( int priority )
{
	if(priority!=m_touchPriority){
		if(m_bTouchEnabled)
		{
			_eventDispatcher->removeAllEventListeners();
		}
		//Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this,priority,false);//-110
				
		// Register Touch Event
		auto listener = EventListenerTouchOneByOne::create();	
		listener->onTouchBegan = CC_CALLBACK_2(EditText::ccTouchBegan, this);		
		_eventDispatcher->addEventListenerWithFixedPriority(listener,priority);


		m_touchPriority = priority;
		m_bTouchEnabled = true;
	}
	
}

void EditText::setWarp(bool warp)
{
	mWarpEnable = warp;
}
bool EditText::getWarp()
{
	return mWarpEnable;
}

void EditText::setPlaceHolder(const char * text)
{
	if(_placeHolder.length() < 0) return;
	_placeHolder = (text) ? (text) : "";
	if (! _inputText.length())
	{
		if(m_isPsw)
		{
			std::string tempStr;
			for (unsigned int i = 0; i < _placeHolder.size(); i++) {
				tempStr.append("*");
			}		
			Label::setString(tempStr.c_str());
		}else
		{
			Label::setString(_placeHolder.c_str());
		}
		
	}
}

void EditText::setInputStyle(INPUTSTYLE style)
{
	m_inputStyle = style;
}

int EditText::getInputStyle()
{

	return m_inputStyle;
}

void EditText::setEnterInputCallback(Ref *mTag,SEL_CallFunc sel) 
{
	m_pListener = mTag;
	m_pfnSelector = sel;	
}

void EditText::executeCallBack() 
{		
	if (m_pListener && m_pfnSelector)
	{			 
			(m_pListener->*m_pfnSelector)();
	}	
}