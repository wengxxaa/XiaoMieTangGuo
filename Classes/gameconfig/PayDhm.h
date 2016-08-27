#ifndef PayDhm_H_
#define PayDhm_H_

#include "ui/UITextField.h"
#include"cocos2d.h"
USING_NS_CC;
#include"cocos-ext.h"
USING_NS_CC_EXT;

class PayDhm:public Layer
{
public:
	static PayDhm* CreateLayer();

	CREATE_FUNC(PayDhm);

	void update(float ft);

private:
	bool init();

	void datahandle();

	ui::EditBox *_box;

	ui::TextField *_textfield;

	LabelTTF *_tipInfo;

	std::string ss;

	Sprite *_tipsp;
};
#endif