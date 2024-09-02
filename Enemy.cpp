#include "Enemy.h"

void Enemy::UnshootedRemoveAt(int id)
{
	vector<COORD> temp;

	for (int i = 0; i < unshooted.size(); i++) {
		if (i != id) temp.push_back(unshooted[i]);
	}

	unshooted.swap(temp);
}

void Enemy::UnshootedRemoveAt(COORD coord)
{
	vector<COORD> temp;

	for (int i = 0; i < unshooted.size(); i++) {
		if (unshooted[i].X != coord.X || unshooted[i].Y != coord.Y) temp.push_back(unshooted[i]);
	}

	unshooted.swap(temp);
}

void Enemy::UnshootedRemoveAt(int x, int y)
{
	vector<COORD> temp;

	for (int i = 0; i < unshooted.size(); i++) {
		if (unshooted[i].X != x || unshooted[i].Y != y) temp.push_back(unshooted[i]);
	}

	unshooted.swap(temp);
}

void Enemy::ShootAI()
{
	if (aiShoot[0].X + 1 < userField.GetFieldWidth()) {
		for (SHORT i = aiShoot[0].X + 1; i < userField.GetFieldWidth(); i++) {

			aiShoot.push_back({ i, aiShoot[0].Y });

			if (userField.GetField(i, aiShoot[0].Y) > 0) {
				currentShipSize--;

				if (!currentShipSize) return;

				continue;
			}

			break;
		}
	}

	if (aiShoot[0].X - 1 >= 0) {
		for (SHORT i = aiShoot[0].X - 1; i >= 0; i--) {

			aiShoot.push_back({ i, aiShoot[0].Y });

			if (userField.GetField(i, aiShoot[0].Y) > 0) {
				currentShipSize--;

				if (!currentShipSize) return;

				continue;
			}

			break;
		}
	}

	if (aiShoot[0].Y + 1 < userField.GetFieldHeight()) {
		for (SHORT i = aiShoot[0].Y + 1; i < userField.GetFieldHeight(); i++) {

			aiShoot.push_back({ aiShoot[0].X, i });

			if (userField.GetField(aiShoot[0].X, i) > 0) {
				currentShipSize--;

				if (!currentShipSize) return;

				continue;
			}

			break;
		}
	}

	if (aiShoot[0].Y - 1 >= 0) {
		for (SHORT i = aiShoot[0].Y - 1; i >= 0; i--) {

			aiShoot.push_back({ aiShoot[0].X, i });

			if (userField.GetField(aiShoot[0].X, i) > 0) {
				currentShipSize--;

				if (!currentShipSize) return;

				continue;
			}

			break;
		}
	}
}

void Enemy::CyrcleAI()
{
	for (int i = 0; i < aiShoot.size(); i++) {
		if (userField.GetField(aiShoot[i].X, aiShoot[i].Y) == -2) Cyrcle(aiShoot[i].X, aiShoot[i].Y);
	}

	aiShoot.clear();
	aiIterator = 0;
}

void Enemy::Cyrcle(int x, int y)
{
	if (y - 1 >= 0 && userField.GetField(x, y - 1) <= 0 && userField.GetField(x, y - 1) != -2)
		UnshootedRemoveAt(x, y - 1); //Up
	if (y + 1 < userField.GetFieldHeight() && userField.GetField(x, y + 1) <= 0 && userField.GetField(x, y + 1) != -2)
		UnshootedRemoveAt(x, y + 1); //Down
	if (x - 1 >= 0 && userField.GetField(x - 1, y) <= 0 && userField.GetField(x - 1, y) != -2)
		UnshootedRemoveAt(x - 1, y); //Left
	if (x + 1 < userField.GetFieldWidth() && userField.GetField(x + 1, y) <= 0 && userField.GetField(x + 1, y) != -2)
		UnshootedRemoveAt(x+1, y); //Right
	if (x - 1 >= 0 && y - 1 >= 0 && userField.GetField(x - 1, y - 1) <= 0 && userField.GetField(x - 1, y - 1) != -2)
		UnshootedRemoveAt(x - 1, y - 1);
	if (x + 1 < userField.GetFieldWidth() && y + 1 < userField.GetFieldHeight() && userField.GetField(x + 1, y + 1) <= 0 && userField.GetField(x + 1, y + 1) != -2)
		UnshootedRemoveAt(x + 1, y + 1); //Right down
	if (x - 1 >= 0 && y + 1 < userField.GetFieldHeight() && userField.GetField(x + 1, y - 1) <= 0 && userField.GetField(x + 1, y - 1) != -2)
		UnshootedRemoveAt(x + 1, y - 1); //Left donw
	if (x + 1 < userField.GetFieldWidth() && y - 1 >= 0 && userField.GetField(x - 1, y + 1) <= 0 && userField.GetField(x - 1, y + 1) != -2)
		UnshootedRemoveAt(x - 1, y + 1); //Right up
}

Enemy::Enemy()
{
	win = false;
}

Enemy::Enemy(Field& userField)
{
	win = false;
	SetUserField(userField);
}

void Enemy::SetUserField(Field& userField)
{
	this->userField = userField;
}

Field& Enemy::GetEnemyField()
{
	return field;
}

bool Enemy::IsWin()
{
	return win;
}

bool Enemy::Shoot()
{
	if (!aiShoot.size()) {
		int field_id = rand() % unshooted.size();

		int fieldNum = userField.GetField(unshooted[field_id].X, unshooted[field_id].Y);
		bool isShooted = userField.ShootTo(unshooted[field_id].X, unshooted[field_id].Y);

		if (isShooted && fieldNum - 1 > 0) {
			aiShoot.clear();
			aiShoot.push_back(unshooted[field_id]);

			currentShipSize = fieldNum - 1;
			aiIterator = 1;
			ShootAI();
		}
		
		if (fieldNum == 1) {
			userField.SetShipsCount(userField.GetShipsCount() - 1);

			win = !userField.GetShipsCount();

			Cyrcle(unshooted[field_id].X, unshooted[field_id].Y);
		}

		UnshootedRemoveAt(field_id);

		return !isShooted;
	}
	else {
		bool isShooted = userField.ShootTo(aiShoot[aiIterator].X, aiShoot[aiIterator].Y);
		UnshootedRemoveAt(aiShoot[aiIterator]);

		aiIterator++;

		if (aiIterator >= aiShoot.size()) {
			userField.SetShipsCount(userField.GetShipsCount() - 1);

			win = !userField.GetShipsCount();

			CyrcleAI();
		}

		return !isShooted;
	}

	return true;
}

void Enemy::Start()
{
	field.Generate();
	field.ShowField(true, 50, 10);

	win = false;

	unshooted.clear();
	for (SHORT i = 0; i < field.GetFieldHeight(); i++) {
		for (SHORT j = 0; j < field.GetFieldWidth(); j++) {
			unshooted.push_back({ j, i });
		}
	}
}