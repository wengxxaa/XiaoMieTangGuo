#include "ScrollLabel.h"

typedef std::vector<std::string> strArray;

// string toolkit
static inline void split(const std::string& src, const std::string& token, strArray& vect)
{
	size_t nend = 0;
	size_t nbegin = 0;
	size_t tokenSize = token.size();
	while(nend != std::string::npos)
	{
		nend = src.find(token, nbegin);
		if(nend == std::string::npos)
			vect.push_back(src.substr(nbegin, src.length()-nbegin));
		else
			vect.push_back(src.substr(nbegin, nend-nbegin));
		nbegin = nend + tokenSize;
	}
}

// first, judge whether the form of the string like this: {x,y}
// if the form is right,the string will be split into the parameter strs;
// or the parameter strs will be empty.
// if the form is right return true,else return false.
static bool splitWithForm(const std::string& content, strArray& strs)
{
	bool bRet = false;

	do
	{
		CC_BREAK_IF(content.empty());

		size_t nPosLeft  = content.find('{');
		size_t nPosRight = content.find('}');

		// don't have '{' and '}'
		CC_BREAK_IF(nPosLeft == std::string::npos || nPosRight == std::string::npos);
		// '}' is before '{'
		CC_BREAK_IF(nPosLeft > nPosRight);

		const std::string pointStr = content.substr(nPosLeft + 1, nPosRight - nPosLeft - 1);
		// nothing between '{' and '}'
		CC_BREAK_IF(pointStr.length() == 0);

		size_t nPos1 = pointStr.find('{');
		size_t nPos2 = pointStr.find('}');
		// contain '{' or '}'
		CC_BREAK_IF(nPos1 != std::string::npos || nPos2 != std::string::npos);

		split(pointStr, ",", strs);
		if (strs.size() < 2 || strs[0].length() == 0 || strs[1].length() == 0)
		{
			strs.clear();
			break;
		}

		bRet = true;
	} while (0);

	return bRet;
}
ccColor3B Color3BFromString(const std::string& str)
{
	ccColor3B ret = ccc3(0,0,0);//cocos2d::ccColor3B::BLACK;

	do
	{
		strArray strs;
		CC_BREAK_IF(!splitWithForm(str, strs));

		GLubyte r = (GLubyte) atoi(strs[0].c_str());
		GLubyte g = (GLubyte) atoi(strs[1].c_str());
		GLubyte b = (GLubyte) atoi(strs[2].c_str());
		ret = ccc3(r, g,b);
	} while (0);

	return ret;
}

ccColor4B Color4BFromString(const std::string& str)
{
	cocos2d::ccColor4B ret = ccc4(0,0,0,0);

	do
	{
		strArray strs;
		CC_BREAK_IF(!splitWithForm(str, strs));

		GLubyte r = (GLubyte) atoi(strs[0].c_str());
		GLubyte g = (GLubyte) atoi(strs[1].c_str());
		GLubyte b = (GLubyte) atoi(strs[2].c_str());
		GLubyte a = (GLubyte) atoi(strs[3].c_str());
		ret = ccc4(r,g,b,a);
	} while (0);

	return ret;
}

void ScrollLabel::initVisableRect( CCRect rect )
{
	m_rect = rect;

	CCClippingNode* clipper = mClipper;
	clipper->setContentSize(  rect.size);
	clipper->setAnchorPoint(  ccp(0.5, 0.5) );
	clipper->setPosition( ccp(rect.origin.x+rect.size.width/2,rect.origin.y+rect.size.height/2));//this->getContentSize().width / 2, this->getContentSize().height / 2) );
	this->addChild(clipper);
	CCDrawNode *stencil = CCDrawNode::create();
	CCPoint rectangle[4];
	rectangle[0] = ccp(0, 0);
	rectangle[1] = ccp(clipper->getContentSize().width, 0);
	rectangle[2] = ccp(clipper->getContentSize().width, clipper->getContentSize().height);
	rectangle[3] = ccp(0, clipper->getContentSize().height);

	
	ccColor4F white = ccc4f(0, 0, 0, 1);
	stencil->drawPolygon(rectangle, 4, white, 1, white);
	clipper->setStencil(stencil);
}

bool ScrollLabel::init(CCRect rect,string fontName,int fontSize)
{
	bool ret = false;
	if (CCLayer::init())
	{
		m_strings = CCArray::create();
		CC_SAFE_RETAIN(m_strings);	

		mClipper = CCClippingNode::create();

		initVisableRect(rect);
		setFontName(fontName);
		setFontSize(fontSize);
		ret = true;
	}
	return ret;
}

void ScrollLabel::onEnter()
{

	CCNode::onEnter();
}

void ScrollLabel::onExit()
{
	CCNode::onExit();
}

void ScrollLabel::setDisplayTime( float time )
{
	this->m_delay = time;
}

bool ScrollLabel::isScrolling()
{
	if(m_label)
		return true;
	else
		return false;
}

void ScrollLabel::addStringToQueue( string content ,string color/*=""*/,string type/*=""*/ )
{
	BroadMessage* pMsg = new BroadMessage(color,content,type);
	pMsg->autorelease();
	m_strings->addObject(pMsg);
	if(!m_isAlive){
		update();
	}
}

void ScrollLabel::setFinishCallfunc( CCObject* target ,SEL_CallFunc callfunc )
{
	this->m_callfunc = callfunc;
	this->m_target = target;
}

CCAction* ScrollLabel::scrollVertical()
{
	float label_w = m_label->getContentSize().width;
	float rect_w = m_rect.size.width;
	float dis = 0;
	float delay = 0;
	if (label_w>rect_w)
	{
		const int GAP = 15;
		dis = label_w-rect_w+GAP;
		delay = MAX(2.0f,0.01f*dis);

	}
	return 	CCSequence::create(
		CCMoveBy::create(delay,ccp(0-dis,0)),
		CCDelayTime::create(1.5f),
		NULL);
}

void ScrollLabel::destroyLabel()
{

	if(m_label){
		m_label->removeFromParent();
		m_label = NULL;
	}

	if(0==m_strings->count()){//没有新数据

		m_isAlive = false;
		if(this->m_target){
			(m_target->*m_callfunc)();
		}
	}
}

void ScrollLabel::switchLabel( BroadMessage* broadMessage )
{
	if (!broadMessage->content.empty())
	{
		if(!broadMessage->color.empty())
			setFontColor(parseColor(broadMessage->color));
		m_isAlive = true;
		float duration = m_duration;
		m_label = CCLabelTTF::create(broadMessage->content.c_str(),m_fontName.c_str(),m_fontsize);
		m_label->setColor(m_fontColor);
		m_label->setAnchorPoint(ccp(0,0.5));
		m_label->setPosition( ccp(0, mClipper->getContentSize().height / 2 - m_rect.size.height));
		mClipper->addChild(m_label);

		CCAction* action = CCSequence::create(
			CCSpawn::create(
			CCFadeIn::create(duration),
			CCMoveBy::create(duration,ccp(0,m_rect.size.height)),
			NULL),
			CCDelayTime::create(3.0f),
			scrollVertical(),
			CCSpawn::create(
			CCFadeOut::create(duration),
			CCMoveBy::create(duration,ccp(0,m_rect.size.height)),
			NULL),
			CCDelayTime::create(m_delay),
			CCCallFunc::create(this,callfunc_selector(ScrollLabel::destroyLabel)),
			CCCallFunc::create(this,callfunc_selector(ScrollLabel::update)),
			NULL
			);
		m_label->runAction(action);

	}
}

void ScrollLabel::callfuncChangeLabel()
{
	float y = m_label->getPositionY()-100;
	m_label->setPositionY(y);
	m_label->setString(m_stringToShow.c_str());
}

void ScrollLabel::update()
{
	if (m_strings->count()>0)
	{
		CCObject* obj = NULL;
		CCARRAY_FOREACH(m_strings,obj){
			BroadMessage* bMsg = (BroadMessage*)obj;
			switchLabel(bMsg);
			m_strings->removeObject(obj);
			break;
		}
	}
}

cocos2d::ccColor3B ScrollLabel::parseColor( string str )
{
	return Color3BFromString(str);
}
