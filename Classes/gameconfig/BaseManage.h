#pragma once

#include"ConfigState.h"

class BaseManage {
public:
	GameState _gamestate;
	int _score;
	int _target;
	int _level;
	int _stepnumber;
	ShapeIndex _nextBox[3];

public:
	virtual GameState getGameState() { return _gamestate; }
	virtual int getmScore() { return _score; }
	virtual int getmTarget() { return _target; }
	virtual int getmLevel() { return _level; }
	virtual int getmStepNumber() { return _stepnumber; }
	virtual ShapeIndex getNextBox(int i) { return _nextBox[i]; }

	virtual void start() {}		//¼ÌÐø
	virtual void pause() {}	//ÔÝÍ£
	virtual void Resurrection() {}	//¸´»î
};