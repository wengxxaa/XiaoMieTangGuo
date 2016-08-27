#ifndef CONFIGSTATE_H_
#define CONFIGSTATE_H_

#include<iostream>
#include "cocos2d.h"

USING_NS_CC;

using namespace std;

//∞Æ”Œœ∑
//#define EGAME

//IOS∞Ê±æ
#define IOSMODE 1

#define TTF_PATH  "fonts/arial.ttf"
#define FNT_PATH "animation/fonts/shuzi_11.fnt"
#define ATLAS_PATH "ziti/sz_1.png"
#define ATLAS_PATH2 "ziti/sz_2.png"
#define ATLAS_PATH3 "ziti/sz_3.png"
#define ATLAS_PATH4 "ziti/sz_4.png"
#define ATLAS_PATH5 "ziti/sz_5.png"
#define HANZI_XML "xml/hanzi_config.xml"
#define SAVE_FILE "savadata.dat"

//#define  AUTO_MODEL

static const float DROP_UPDATE_TIME=0.1f;	//∑ΩøÈ√ø––œ¬¬‰–Ë“™µƒ ±º‰
static const float DROP_STOP_TIME=1.0f;	//∑ΩøÈÕ£∂Ÿµƒ ±º‰
static const float DROP_MOVE_TIME=0.1f;	//∆Â◊”ª¨∂Ø–Ë“™µƒ ±º‰
static const float CHESS_REMOVE_TIME=0.1f;	//∆Â◊”œ˚≥˝µƒ ±º‰
static const float CHESS_CHECK_TIME=0.2f;	//∆Â≈Ãcheakµƒ ±º‰
static const int CHESS_SIZE_W=11;	//∑ΩøÈ∆Â≈ÃµƒøÌ
static const int CHESS_SIZE_H=14;	//∑ΩøÈ∆Â≈Ãµƒ∏ﬂ
static const float CHESSMAN_SCALE=1.0f;	//∑ΩøÈµƒÀı∑≈
static const int CHESS_SIZE_SIZE=42;	//∆Â◊”µƒ∏ﬂ
static const int CHESSMAN_LEVEL=2;	//∆Â◊”µƒ≤„
static const int EASE_STRENGTH=5;	//œ¬¬‰«ø∂»
static const int  AtlasYOff=-8;
static const int LEVELREWARD=5;	//Ω±¿¯◊Ó∏ﬂ¥Œ ˝
static const float OPA_BEIJING=0.8f;	//±≥æ∞µƒÕ∏√˜∂»
static const float DIAMOND_HIGH=0.6f;	//◊Í Øµƒ∏ﬂ∂»
static const float MENU_SCALE=1.15f;	//∞¥≈•∑≈¥Û

//pay
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS|| IOSMODE)
static const int PAY_2_DIAMOND = 10;	//∏¥ªÓ◊Í Ø ˝
static const int PAY_DIAMOND[9]={20,400,55,120,220,400,400,10,400};
#else
static const int PAY_2_DIAMOND = 5;	//∏¥ªÓ◊Í Ø ˝
static const int PAY_DIAMOND[9]={20,400,55,100,180,400,400,10,400};
#endif
//

static const int TIME_HONGBAO[7]={180,300,480,900,1200,1800,1800};	//∫Ï∞¸ ±º‰
//static const int TIME_HONGBAO[7]={10,10,10,10,10,10,10};	//∫Ï∞¸ ±º‰
	
static const int DAILY_REWARD[7]={10,53,52,51,20,54,30};	//√ø»’«©µΩΩ±¿¯

//if(GameData::getInstance()->_chessmodel==MODEL_10_10)
//{
//	GameDataInstance()->CHESS_SIZE_W=10;
//	GameDataInstance()->CHESS_SIZE_H=10;
//	GameDataInstance()->CHESS_SIZE_SIZE=47;
//	GameDataInstance()->CHESS_SCALE=1.125f;
//}
//else if(GameData::getInstance()->_chessmodel==MODEL_9_10)
//{
//	GameDataInstance()->CHESS_SIZE_W=9;
//	GameDataInstance()->CHESS_SIZE_H=10;
//	GameDataInstance()->CHESS_SIZE_SIZE=52;
//	GameDataInstance()->CHESS_SCALE=1.25f;
//}
//else if(GameData::getInstance()->_chessmodel==MODEL_9_9)
//{
//	GameDataInstance()->CHESS_SIZE_W=9;
//	GameDataInstance()->CHESS_SIZE_H=9;
//	GameDataInstance()->CHESS_SIZE_SIZE=52;
//	GameDataInstance()->CHESS_SCALE=1.25f;
//}

static const int CHESS_SIZE_W_1010=9;	//1010µƒ∆Â≈ÃøÌ
static const int CHESS_SIZE_H_1010=10;	//1010µƒ∆Â≈Ã∏ﬂ
static const int CHESSMAN_SIZE_1010=52;	//1010µƒ∆Â◊”∏ﬂ
static const float CHESSMAN_SCALE_1010=1.25f;	//1010µƒÀı∑≈
static const int CHESS_SCORE=10;	//√ø∏ˆ∆Â◊”µƒ∑÷ ˝

//œ˚√–«–«
static const int CHESS_SIZE_W_START=10;	//œ˚√–«–«µƒøÌ
static const int CHESS_SIZE_H_START=10;	//œ˚√–«–«µƒ∏ﬂ
static const int CHESSMAN_SIZE_START=48;	//œ˚√–«–«µƒ∆Â◊”∏ﬂ∂»
static const float CHESSMAN_SCALE_START=1.25f;	//œ˚√–«–«µƒÀı∑≈
static const int START_INDEX_NUMBER=5;	//–«–«∆Â◊”¿‡–Õ∏ˆ ˝
static const Vec2 START_SCORE_PT=Vec2(240,711);	//–«–«∑÷ ˝µƒŒª÷√

//¡˘Ω«∑ΩøÈ
static const int CHESS_SIZE_W_ANGLE=9;	//¡˘Ω«µƒøÌ
static const int CHESS_SIZE_H_ANGLE=9;	//¡˘Ω«µƒ∏ﬂ
static const int CHESSMAN_SIZE_W_ANGLE=53;	//∆Â◊”µƒøÌæ‡
static const int CHESSMAN_SIZE_H_ANGLE=47;	//∆Â◊”µƒ∏ﬂæ‡
static const float CHESSMAN_SCALE_ANGLE=1.0f;	
static const Vec2 ANGLE_SCORE_PT=Vec2(240,711);	//¡˘Ω«∑÷ ˝µƒŒª÷√

//¡¨œﬂƒ£ Ω
static const int CHESS_SIZE_W_LINE=9;	//¡¨œﬂµƒøÌ
static const int CHESS_SIZE_H_LINE=10;	//¡¨œﬂµƒ∏ﬂ
static const int CHESSMAN_SIZE_LINE=52;	//¡¨œﬂµƒ∆Â◊”∏ﬂ∂»
static const float CHESSMAN_SCALE_LINE=1.25f;	//¡¨œﬂµƒÀı∑≈

//∑ΩøÈ–Œ◊¥
typedef enum _SHAPE_INDEX
{
	L_SHAPE,
	L_RESHAPE,
	SQUARE_SHAPE,
	ANGLE_SHAPE,
	T_SHAPE,
	LINE_SHAPE,
	SWAGER_SHAPE,
	SWAGER_RESHAPE,
	POINT_SHAPE,
	LINESMALL_SHAPE,
	U_SHAPE,
	SQUAREBIG_SHAPE,
	LBIG_SHAPE,
}ShapeIndex;

//∑ΩøÈΩ«∂»
typedef enum _ANGLE_INDEX
{
	ANGLE_0,
	ANGLE_90,
	ANGLE_180,
	ANGLE_270,
}AngleIndex;

//∑ΩøÈ“∆∂Ø∑ΩœÚ
typedef enum _DIRECTION_INDEX
{
	DIRECTION_D,
	DIRECTION_R,
	DIRECTION_L,
}DirectionIndex;

//∑ΩøÈ∆Â≈Ã◊¥Ã¨
typedef enum _GAME_STATE
{
	STATE_RUN,
	STATE_STOP,
	STATE_PAUSE,
	STATE_FAILD,
}GameState;

//∑ΩøÈ“∆∂Ø∫Õ∑ΩøÈœ¬ª¨◊¥Ã¨
typedef enum _TOUCH_STATE
{
	TOUCH_TRUE,	//ø…“‘
	TOUCH_FALSE,	//≤ªø…“‘
}TouchState;

//µ„¥•ƒ£ ΩÃ· æµƒΩ·ππ
typedef struct _POINT_TIP
{
	ShapeIndex shape;
	AngleIndex angle;
	pair<int,int>  position;

	_POINT_TIP(){};

	_POINT_TIP(ShapeIndex s,AngleIndex a,pair<int,int> p):shape(s),angle(a),position(p){};

	bool operator==(const _POINT_TIP &t)const
	{
		return (shape==t.shape&&angle==t.angle&&position==t.position);
	}

	_POINT_TIP& operator=(const _POINT_TIP& _Right)
	{	
		shape = _Right.shape;
		angle = _Right.angle;
		position=_Right.position;
		return (*this);
	}

}PointTip;

//”Œœ∑ƒ£ Ω
typedef enum _GAME_MODEL
{
	MODEL_SLIP=1,	//ª¨∂Ø
	MODEL_POINT,	//µ„ª˜÷∏“˝
}GameModel;

//∑ΩøÈ—’…´◊¥Ã¨
typedef enum _BOX_COLOR_
{
	BOX_NORMAL=1,	//’˝≥£
	BOX_GRAY,	//ª“…´
}BoxColor;

//”Œœ∑π§≥Ã
typedef enum _GAME_PROJECT
{
	GAME_TETRIS,
	GAME_1010,
	GAME_START,
	GAME_ANGLE,
}GameProject;

#endif
