#include "Toast.h"
#include "cjconfig.h"
USING_NS_CC;
#include "cocos-ext.h"
//#include "GUI/CCControlExtension/CCScale9Sprite.h"
USING_NS_CC_EXT;

#define TOAST_FONT	"arial"


Toast* Toast::makeText( Scene* scene,const string& text,int duration )
{
	Toast* pToast = new Toast();
	if(pToast&&pToast->initWithText(text,duration)){
		pToast->autorelease();
		scene->addChild(pToast,1000);
		Size winSize = CCDirector::getInstance()->getWinSize();
		pToast->setPosition(Vec2(winSize.width/2,0));
		return pToast;
	}
	CC_SAFE_DELETE(pToast);
	return NULL;
}

Toast* Toast::makeText( const string& text,int duration /*= TOAST_LENGTH_SHORT*/ )
{
	return makeText(Director::getInstance()->getRunningScene(),text,duration);
}

void Toast::show()
{
	setVisible(true);
	Size winsize = Director::getInstance()->getWinSize();
	runAction(
		Sequence::create(
		//CCFadeIn::create(0.5f),
		EaseIn::create(MoveTo::create(0.3f,Vec2(winsize.width/2,70)),2),
		DelayTime::create(m_fDuration),
		FadeOut::create(0.5f),
		CallFunc::create(this,callfunc_selector(Toast::removeFromParent)),
		NULL)
		);
}

bool Toast::initWithText(const string& text,int duration)
{
	if(CCNode::init()){
		if(duration==TOAST_LENGTH_LONG)
			m_fDuration = 2.5f;
		else 
			m_fDuration = 1.5f;
		Size visableSize = Director::getInstance()->getVisibleSize();
		LabelTTF* pLabel = LabelTTF::create(cjTTFLabel::getStringByKey(text.c_str()),TOAST_FONT,28);
		//pToast->setPosition(ccp(visableSize.width/2,visableSize.height/5));
		Scale9Sprite* sBg = Scale9Sprite::createWithSpriteFrameName("tsk.png");
		addChild(sBg);
		sBg->setContentSize(Size(pLabel->getContentSize().width+20,pLabel->getContentSize().height+20));
		sBg->addChild(pLabel);
		pLabel->setPosition(Vec2(sBg->getContentSize().width/2,sBg->getContentSize().height/2));
		setVisible(false);
		return true;
	}
	return false;
}

