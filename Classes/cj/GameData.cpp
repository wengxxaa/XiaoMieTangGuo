#include"GameData.h"
#include"cjconfig.h"
#include "vigame_ad.h"

void GameData::dataSave()
{
	//return ;
	//if(_saving)
	//	return;
	//_saving=true;

	static int savenumber=0;

	//存储
	string path=FileUtils::getInstance()->getWritablePath()+SAVE_FILE;
	ofstream outfile(path,ios::binary);
	if(!outfile)
	{
		//CCLOG("open fill error");
		assert(outfile);
	}
	else
	{
		savenumber++;
		CCLOG(cjTTFLabel::getNameByInt("filesave:%d",savenumber).c_str());
		outfile.write((char *)&d_saveData,sizeof(d_saveData));
		outfile.close();
	}
	//

	//Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(1.0f),CallFunc::create([this](){
	//	_saving=false;
	//}),nullptr));
}

void GameData::dataReset()
{
	string path=FileUtils::getInstance()->getWritablePath()+SAVE_FILE;
	ofstream outfile(path,ios::binary);
	if(!outfile)
	{
		CCLOG("dataReset failed");
	}
	//d_savedata初始化
	d_saveData._challengesstetris._istip=false;
	d_saveData._challengesstetris._guider=false;
	d_saveData._onezero._istip=false;
	d_saveData._onezero._guider=false;
	d_saveData._start._istip=false;
	d_saveData._start._guider=false;
	d_saveData._start._highestscore=0;
	d_saveData._angle._istip=false;
	d_saveData._angle._guider=false;
	d_saveData._angle._highestscore=0;

	d_saveData._diamondNumber=50;

	for(int i=0;i<4;i++)
		d_saveData._propnum[i]=0;

	d_saveData._hongbaotime=TIME_HONGBAO[0];

	d_saveData._hongbaonum=0;

	d_saveData._starthongbaonum=0;

	d_saveData._anglehongbaonum=0;

	d_saveData._music=true;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || IOSMODE)
	d_saveData._xieyi = false;
	d_saveData._xinshou = false;
#else
	d_saveData._xieyi = true;
	d_saveData._xinshou = true;
#endif
	
    d_saveData._qd_day=0;
	d_saveData._qd_number=0;

	d_saveData._levelreward_num=0;
	d_saveData._day_num=0;

	d_saveData._gg_guanshu=0;
	
	d_saveData._freeround=true;

	d_saveData._freeroundday=0;

	d_saveData._freeroundnum=0;	

	d_saveData._freediamondnum=0;

	d_saveData._huhightnum=50;

	d_saveData._hunum=0;

	d_saveData._hupropid=0;

	LOGGAME("loadSaveFile");

	//dataSave();
	//
}

void GameData::dataRead()
{
	//读取文件
	string path=FileUtils::getInstance()->getWritablePath()+SAVE_FILE;
	if(FileUtils::getInstance()->isFileExist(path))
	{
		auto data=FileUtils::getInstance()->getDataFromFile(path);
		if(data.getSize()==sizeof(_data))
		{
			auto gamedata=(_data*)data.getBytes();
			//tetris_classes

			//tetris_challenge
			d_saveData._challengesstetris=gamedata->_challengesstetris;

			//onezero
			d_saveData._onezero=gamedata->_onezero;
			//

			//start
			d_saveData._start=gamedata->_start;

			//angle
			d_saveData._angle=gamedata->_angle;

			d_saveData._diamondNumber=gamedata->_diamondNumber;

			for(int i=0;i<4;i++)
				d_saveData._propnum[i]=gamedata->_propnum[i];

			d_saveData._hongbaotime=gamedata->_hongbaotime;

			d_saveData._hongbaonum=gamedata->_hongbaonum;

			d_saveData._starthongbaonum=gamedata->_starthongbaonum;

			d_saveData._anglehongbaonum=gamedata->_anglehongbaonum;

			d_saveData._music=gamedata->_music;

			d_saveData._xieyi=gamedata->_xieyi;

			d_saveData._xinshou=gamedata->_xinshou;

			d_saveData._qd_day=gamedata->_qd_day;

			d_saveData._qd_number=gamedata->_qd_number;

			d_saveData._levelreward_num=gamedata->_levelreward_num;
			d_saveData._day_num=gamedata->_day_num;

			d_saveData._gg_guanshu=gamedata->_gg_guanshu;

			d_saveData._freeround=gamedata->_freeround;

			d_saveData._freeroundday=gamedata->_freeroundday;

			d_saveData._freeroundnum =gamedata->_freeroundday;

			d_saveData._freediamondnum = gamedata->_freeroundday;

			d_saveData._huhightnum=gamedata->_huhightnum;

			d_saveData._hunum=gamedata->_hunum;

			d_saveData._hupropid=gamedata->_hupropid;
		}
		else
		{
			dataReset();
			CCLOG("fill is null");
		}
	}
	else
	{
		dataReset();
		CCLOG("file don't exit");
	}
	//
}

void GameData::loadSaveFile()
{
	_saving=false;
	dataRead();
}