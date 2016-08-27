#include "AppInfoLayer.h"

#include "vigame_core.h"


void AppInfoLayer::show()
{
	Scene* pScene = Director::getInstance()->getRunningScene();
	if(pScene->getChildByTag(10021002)==NULL)
		pScene->addChild(this,10000,10021002);
}

bool AppInfoLayer::init()
{
	if(Layer::init()){
		string packageName = vigame::SysConfig::getInstance()->getPackage();
		string prjId = vigame::SysConfig::getInstance()->getPrjid();
		string mmChannelID = vigame::SysConfig::getInstance()->getMmid();
		string imsi = vigame::SysConfig::getInstance()->getImsi();
		string mac = vigame::SysConfig::getInstance()->getMacaddress();
		string lsn = vigame::SysConfig::getInstance()->getLsn();
		string appId = vigame::SysConfig::getInstance()->getAppid();
		string appVer = vigame::SysConfig::getInstance()->getAppVersion();

		string s_message = "package: "+packageName
			+"\nprjid: "+prjId
			+"\nmmChannelID: "+mmChannelID
			+"\nimsi: "+imsi
			+"\nmacaddress: "+mac
			+"\nlsn: "+lsn
			+"\nappid: "+appId
			+"\nappver: "+appVer;

		Size winsize = Director::getInstance()->getWinSize();
		LayerColor* colorLayer = LayerColor::create(Color4B(0,0,0,160));
		addChild(colorLayer);
		colorLayer->setContentSize(Size(400,400));
		colorLayer->setPosition(Vec2((winsize.width-colorLayer->getContentSize().width)/2,(winsize.height-colorLayer->getContentSize().height)/2));
		Label* label = Label::createWithSystemFont(s_message.c_str(),"Arial",20,colorLayer->getContentSize(),TextHAlignment::CENTER);
		label->setVerticalAlignment(TextVAlignment::CENTER);
		colorLayer->addChild(label);
		label->setPosition(Vec2(colorLayer->getContentSize().width/2,colorLayer->getContentSize().height/2));
		
		MenuItemFont* itemFont = MenuItemFont::create("Close",CC_CALLBACK_1(AppInfoLayer::_menuCloseCallback,this));
		Menu* pMenu = Menu::create(itemFont,NULL);
		colorLayer->addChild(pMenu);
		pMenu->setPosition(Vec2(colorLayer->getContentSize().width/2,50));
		return true;
	}
	return false;
}

void AppInfoLayer::_menuCloseCallback( Ref* obj )
{
	removeFromParent();
}
