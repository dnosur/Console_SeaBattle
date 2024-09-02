#pragma once

#include <map>
#include <iostream>
#include <Windows.h>

#include "Color.h"
#include "functions.h"

using namespace std;

class Field
{
	int field[10][10];

	const int field_width = 10;
	const int field_height = 10;

	//MouseCoords	FieldCoords
	map<COORD, COORD> coords;

	Color color;

	int shipsCount;

	void Set(SHORT& y, SHORT& x, int& num);
public: 
	Field();

	void ClearField();
	void ClearColors(bool fog = false);

	void Generate();

	void SetField(int x, int y, int num);
	int GetField(int x, int y);
	int GetField(COORD mouseCoords);

	bool ShootTo(int x, int y);

	map<COORD, COORD> GetCoords();
	COORD GetCoordBy(COORD fieldCoord);
	COORD GetFieldCoord(COORD mouseCoord);

	bool ContainsMouseCoord(COORD mouseCoord);

	int GetFieldWidth();
	int GetFieldHeight();

	void SetShipsCount(int shipsCount);
	int GetShipsCount();

	void Draw(COORD StartPos, COORD EndPos, int num);

	void SetShip(COORD StartPos, COORD EndPos, int ship_size);
	bool CanSetShip(COORD StartPos, COORD EndPos);

	void ShowField(bool fog = false, int startX = 0, int startY = 0);

	void operator = (Field& field);
};

bool operator <(const COORD& left, const COORD& right);