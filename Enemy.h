#pragma once

#include "Field.h"

#include <vector>

class Enemy
{
	Field field;
	Field userField;

	vector<COORD> unshooted;

	vector<COORD> aiShoot;
	int aiIterator;
	int currentShipSize;

	bool win;

	void UnshootedRemoveAt(int id);
	void UnshootedRemoveAt(COORD coord);
	void UnshootedRemoveAt(int x, int y);

	void ShootAI();

	void CyrcleAI();
	void Cyrcle(int x, int y);
public:
	Enemy();
	Enemy(Field& userField);

	void SetUserField(Field& userField);

	Field& GetEnemyField();

	bool IsWin();

	bool Shoot();
	void Start();
};

