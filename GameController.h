#pragma once

#include <iostream>

#include "Field.h"
#include "Enemy.h"
#include "Color.h"

#include "functions.h"

using namespace std;

class GameController
{
	bool end, win;
	Field userField;

	Enemy enemy;

	Color color;
public: 
	GameController();

	void Start();
	void Battle();
	void End();
};