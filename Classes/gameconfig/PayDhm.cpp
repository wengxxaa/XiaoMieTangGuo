#include "PayDhm.h"
#include "../cj/cjconfig.h"
#include "../cj/GameData.h"
#include "PayScene.h"
#include "vigame_core.h"
#include "views/AppInfoLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/UIImageView.h"
using namespace cocostudio;

PayDhm* PayDhm::CreateLayer()
{
	auto lay=PayDhm::create();
	return lay;
}

bool PayDhm::init()
{
	if(!Layer::init())
		return false;

	Director::getInstance()->getRunningScene()->addChild(this,100);

	auto cololay=LayerColor::create(Color4B(0,0,0,255*OPA_BEIJING),WINSIZE.width,WINSIZE.height);
	cololay->setPosition(WINORIGIN);
	this->addChild(cololay,1,"colorlay");

	PayScene::getInstance()->diamondRain(cololay);

	auto backcsb=CSLoader::createNode("animation/dhm.csb");
	cololay->addChild(backcsb);
	backcsb->setAnchorPoint(Vec2(0.5f,0.5f));
	backcsb->setPosition(WINSIZE/2.0f);

	//_box=EditBox::create(CCSizeMake(350,80),Scale9Sprite::create("animation/PayScene/bg_s9_2.png"));
	//backcsb->addChild(_box);
	//_box->setFont(TTF_PATH,32);
	//_box->setMaxLength(10);
	//_box->setPosition(Vec2(backcsb->getPosition())+Vec2(0,20));
	//_box->setInputFlag(EditBox::InputFlag::SENSITIVE);
	//_box->setInputMode(EditBox::InputMode::SINGLE_LINE);
	//_box->setReturnType(EditBox::KeyboardReturnType::DONE);
	//_box->setDelegate(this);

	_textfield=dynamic_cast<ui::TextField*>(backcsb->getChildByName("TextField"));

	_tipInfo=LabelTTF::create("",TTF_PATH,20);
	backcsb->addChild(_tipInfo);
	_tipInfo->setColor(Color3B::BLACK);
	_tipInfo->setPosition(_textfield->getPosition()-Vec2(0,55));

	_tipsp=dynamic_cast<Sprite*>(backcsb->getChildByName("tipsp"));

	auto cancel=dynamic_cast<ui::Button*>(backcsb->getChildByName("cha"));
	cancel->addTouchEventListener([=](Ref *,ui::Widget::TouchEventType type){
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			unscheduleUpdate();
			cjMusic::playEffect("video/tap.mp3");
		}
		else if(type==ui::Widget::TouchEventType::ENDED)
		{
			cololay->removeFromParent();
		}
	});

	auto sure=dynamic_cast<ui::Button*>(backcsb->getChildByName("sure"));
	sure->addTouchEventListener([this,cololay](Ref *,ui::Widget::TouchEventType type){
		if(type==ui::Widget::TouchEventType::ENDED)
		{
			auto wait = cololay->getChildByName("wait");
			if (!wait)
			{
				cjMusic::playEffect("video/tap.mp3");
				//CCArmatureDataManager::getInstance()->addArmatureFileInfo("res/wait.ExportJson");
				//auto armature=Armature::create("wait");
				//cololay->addChild(armature,3,"wait");
				//armature->setPosition(WINSIZE/2.0f);
				//armature->getAnimation()->play("wait");

				auto backsp = Scale9Sprite::createWithSpriteFrameName("tsk.png");
				cololay->addChild(backsp, 3, "wait");
				backsp->setPosition(Vec2(WINSIZE / 2.0f) + Vec2(0, 30));
				backsp->setContentSize(CCSizeMake(300, 60));
				auto tipfont = LabelTTF::create(cjTTFLabel::getStringByKey("waitfont"), TTF_PATH, 22);
				backsp->addChild(tipfont);
				tipfont->setPosition(backsp->getContentSize() / 2.0f);

				cjSchedule::getInstance()->runOnNextFrame([=](float ft) {
					datahandle();
				});
			}
		}
	});

	backcsb->setScale(0.0f);
	backcsb->runAction(ScaleTo::create(0.2f,1.0f));

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	cololay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, cololay);

	scheduleUpdate();

	return true;
}

void PayDhm::datahandle()
{
	ss=_textfield->getString();
	if(ss=="10021002"){
		AppInfoLayer::create()->show();
		auto layer = this->getChildByName("colorlay");
		layer->removeChildByName("wait");
		return ;
	}

	auto data= vigame::dhm::use(ss);

	if(atoi(data.state.c_str())==-1)
	{
		_tipInfo->setString(cjTTFLabel::getStringByKey("dhmtip"));
	}
	else
	{
		if(atoi(data.state.c_str())==1)
		{
			int dianum=0;
			string pricestr=data.price;
			if(strcmp(pricestr.c_str(),"600")==0)
				dianum=PAY_DIAMOND[2];
			else if(strcmp(pricestr.c_str(),"1000")==0)
				dianum=PAY_DIAMOND[3];
			else if(strcmp(pricestr.c_str(),"1500")==0)
				dianum=PAY_DIAMOND[4];
			else if(strcmp(pricestr.c_str(),"2900")==0)
				dianum=PAY_DIAMOND[5];
			GameData::getSaveData()->_diamondNumber+=dianum;
			GameData::getInstance()->dataSave();
		}
		_tipInfo->setString(data.message.c_str());
	}

	auto layer=this->getChildByName("colorlay");
	layer->removeChildByName("wait");
}

void PayDhm::update(float ft)
{
	if(_textfield->getString().empty())
		_tipsp->setVisible(true);
	else
		_tipsp->setVisible(false);
}