#include "Field.h"
#include <vector>

// -1 - Мимо
// -2 - Попал

void Field::Set(SHORT& y, SHORT& x, int& num)
{
	field[y][x] = num;
	
	if(y - 1 >= 0 && (field[y - 1][x] == 0 || field[y - 1][x] < -3)) 
		field[y - 1][x] = -3; //Up
	if(y + 1 < field_height && (field[y + 1][x] == 0 || field[y + 1][x] < -3)) 
		field[y + 1][x] = -3; //Down

	if (x - 1 >= 0 && (field[y][x - 1] == 0 || field[y][x - 1] < -3)) 
		field[y][x - 1] = -3; //Left
	if (x + 1 < field_width && (field[y][x + 1] <= 0 || field[y][x + 1] < -3)) 
		field[y][x + 1] = -3; //Right

	if (x - 1 >= 0 && y - 1 >= 0 && (field[y - 1][x - 1] == 0 || field[y - 1][x - 1] < -3)) 
		field[y - 1][x - 1] = -3; //Left up
	if (x + 1 < field_width && y + 1 < field_height && (field[y + 1][x + 1] == 0 || field[y + 1][x + 1] < -3)) 
		field[y + 1][x + 1] = -3; //Right down

	if (x - 1 >= 0 && y + 1 < field_height && (field[y + 1][x - 1] == 0 || field[y + 1][x - 1] < -3)) 
		field[y + 1][x - 1] = -3; //Left donw
	if (x + 1 < field_width && y - 1 >=0 && (field[y - 1][x + 1] <= 0 || field[y - 1][x + 1] < -3)) 
		field[y - 1][x + 1] = -3; //Right up
}

Field::Field()
{
	ClearField();
}

void Field::ClearField()
{
	shipsCount = 0;
	for (int i = 0; i < field_height; i++) {
		for (int j = 0; j < field_width; j++) {
			field[i][j] = 0;
		}
	}
}

void Field::ClearColors(bool fog)
{
	int mainWaterColor = (!fog) ? color.LightBlue : color.DarkGray;
	int shipsColor = (!fog) ? color.Yellow : color.DarkGray;

	color.SetColor(mainWaterColor);

	for (map<COORD, COORD>::const_iterator i = coords.begin(); i != coords.end(); i++) {
		gotoxy(i->first.X, i->first.Y);

		if (!field[i->second.Y][i->second.X] || field[i->second.Y][i->second.X] == -3) cout << ((!fog) ? "~ " : "* ");

		if (field[i->second.Y][i->second.X] > 0) {
			color.SetColor(shipsColor);
			cout << ((!fog) ? "^ " : "* ");
			color.SetColor(mainWaterColor);
		}

		if (field[i->second.Y][i->second.X] == -1) {
			color.SetColor(color.LightGray);
			cout << "o	";
			color.SetColor(mainWaterColor);
		}

		if (field[i->second.Y][i->second.X] == -2) {
			color.SetColor(color.Red);
			cout << "X	";
			color.SetColor(mainWaterColor);
		}
	}
}

void Field::Generate()
{
	ClearField();

	int shipSize = 1;
	vector<COORD> freeCoords;

	auto clearBusy = [](vector<COORD>& freeCoords, int (&field)[10][10]) {
		vector<COORD> temp;

		for (int i = 0; i < freeCoords.size(); i++) {
			if (field[freeCoords[i].Y][freeCoords[i].X] > 0 || field[freeCoords[i].Y][freeCoords[i].X] == -3) {
				continue;
			}

			temp.push_back(freeCoords[i]);
		}

		freeCoords.clear();
		freeCoords.swap(temp);
	};

	for (SHORT i = 0; i < field_height; i++) {
		for (SHORT j = 0; j < field_width; j++) {
			freeCoords.push_back({ j, i });
		}
	}

	COORD currPos;
	COORD endPos;

	while (shipsCount < 10) {

		for (SHORT i = 0; i < 5 - shipSize;) {
			int it = rand() % freeCoords.size();
			currPos = freeCoords[it];

			endPos = currPos;
			endPos.X = currPos.X + (shipSize - 1);

			if (CanSetShip(currPos, endPos)) { //Right
				SetShip(currPos, endPos, shipSize);
				i++;

				clearBusy(freeCoords, field);
				continue;
			}

			endPos = currPos;
			endPos.X = currPos.X - (shipSize - 1);

			if (CanSetShip(currPos, endPos)) { //Left
				SetShip(currPos, endPos, shipSize);
				i++;

				clearBusy(freeCoords, field);
				continue;
			}

			endPos = currPos;
			endPos.Y = currPos.Y + (shipSize - 1);

			if (CanSetShip(currPos, endPos)) { //Down
				SetShip(currPos, endPos, shipSize);
				i++;

				clearBusy(freeCoords, field);
				continue;
			}

			endPos = currPos;
			endPos.Y = currPos.Y - (shipSize - 1);

			if (CanSetShip(currPos, endPos)) { //Up
				SetShip(currPos, endPos, shipSize);
				i++;

				clearBusy(freeCoords, field);
				continue;
			}
		}

		shipSize++;
	}
}

void Field::SetField(int x, int y, int num)
{
	if (x >= 0 && x < field_width && y >= 0 && y < field_height) {
		field[y][x] = num;
	}
}
int Field::GetField(int x, int y)
{
	if (x >= 0 && x < field_width && y >= 0 && y < field_height) return field[y][x];
}
int Field::GetField(COORD mouseCoords)
{
	if (coords.size()) {
		COORD fieldCoord = coords[mouseCoords];

		return field[fieldCoord.Y][fieldCoord.X];
	}
	return 0;
}

bool Field::ShootTo(int x, int y)
{
	if (x >= 0 && x < field_width && y >= 0 && y < field_height && field[y][x] != -1 && field[y][x] != -2 && coords.size()) {
		field[y][x] = ((field[y][x] > 0) ? -2 : -1);

		COORD coord = { x, y };
		coord = GetCoordBy(coord);
		gotoxy(coord.X, coord.Y);

		if (field[y][x] == -1 || field[y][x] == -3) {
			color.SetColor(color.LightGray);
			cout << "o ";
			color.ResetColor();

			return false;
		}

		if (field[y][x] == -2) {
			color.SetColor(color.Red);
			cout << "X ";
			color.ResetColor();

			return true;
		}

		return false;
	}

	return false;
}

map<COORD, COORD> Field::GetCoords()
{
	return coords;
}

COORD Field::GetCoordBy(COORD fieldCoord)
{
	for (map<COORD, COORD>::const_iterator i = coords.begin(); i != coords.end(); i++) {
		if (i->second.X == fieldCoord.X && i->second.Y == fieldCoord.Y) {
			return i->first;
		}
	}
}

COORD Field::GetFieldCoord(COORD mouseCoord)
{
	return coords[mouseCoord];
}

bool Field::ContainsMouseCoord(COORD mouseCoord)
{
	for (map<COORD, COORD>::const_iterator it = coords.begin(); it != coords.end(); it++) {
		if (it->first.X == mouseCoord.X && it->first.Y == mouseCoord.Y) return true;
	}

	return false;
}

int Field::GetFieldWidth()
{
	return field_width;
}

int Field::GetFieldHeight()
{
	return field_height;
}

void Field::SetShipsCount(int shipsCount)
{
	if(shipsCount <= 10 && shipsCount >= 0) this->shipsCount = shipsCount;
}

int Field::GetShipsCount()
{
	return shipsCount;
}

void Field::Draw(COORD StartPos, COORD EndPos, int num)
{
	bool canSet = CanSetShip(StartPos, EndPos);
	color.SetColor(((canSet) ? color.Green : color.Red));

	COORD coord;

	if (StartPos.X > EndPos.X) { //Left
		for (int i = StartPos.X; i >= EndPos.X; i--) {
			if (i < 0) break;

			coord = GetCoordBy({ (SHORT)i, StartPos.Y });
			gotoxy(coord.X, coord.Y);
			cout << num;
		}
	}

	if (StartPos.X < EndPos.X) { //Right
		for (int i = StartPos.X; i <= EndPos.X; i++) {
			if (i >= field_width) break;

			coord = GetCoordBy({ (SHORT)i, StartPos.Y });
			gotoxy(coord.X, coord.Y);
			cout << num;
		}
	}

	if (StartPos.Y < EndPos.Y) { //Down

		for (int i = StartPos.Y; i <= EndPos.Y; i++) {
			if (i >= field_height) break;

			coord = GetCoordBy({ StartPos.X, (SHORT)i });
			gotoxy(coord.X, coord.Y);
			cout << num;
		}
	}

	if (StartPos.Y > EndPos.Y) { //Up
		for (int i = StartPos.Y; i >= EndPos.Y; i--) {
			if (i < field_height) break;

			coord = GetCoordBy({ StartPos.X, (SHORT)i });
			gotoxy(coord.X, coord.Y);
			cout << num;
		}
	}

	if (StartPos.X && StartPos.Y && StartPos.X == EndPos.X && StartPos.Y == EndPos.Y) {
		coord = GetCoordBy({ StartPos.X, StartPos.Y });
		gotoxy(coord.X, coord.Y);
		cout << num;
	}

	color.ResetColor();
}

void Field::SetShip(COORD StartPos, COORD EndPos, int ship_size)
{
	if (CanSetShip(StartPos, EndPos)) {
		if (StartPos.X > EndPos.X) { //Left
			for (SHORT i = StartPos.X; i >= EndPos.X; i--) {
				Set(StartPos.Y, i, ship_size);
			}
		}

		if (StartPos.X < EndPos.X) { //Right
			for (SHORT i = StartPos.X; i <= EndPos.X; i++) {
				Set(StartPos.Y, i, ship_size);
			}
		}

		if (StartPos.Y < EndPos.Y) { //Down
			for (SHORT i = StartPos.Y; i <= EndPos.Y; i++) {
				Set(i, StartPos.X, ship_size);
			}
		}

		if (StartPos.Y > EndPos.Y) { //Up
			for (SHORT i = StartPos.Y; i >= EndPos.Y; i--) {
				Set(i, StartPos.X, ship_size);
			}
		}

		if (StartPos.X && StartPos.Y && StartPos.X == EndPos.X && StartPos.Y == EndPos.Y) {
			Set(StartPos.Y, StartPos.X, ship_size);
		}

		shipsCount++;
	}
}

bool Field::CanSetShip(COORD StartPos, COORD EndPos)
{
	if (StartPos.X > EndPos.X) { //Left
		if (EndPos.X < 0) return false;

		for (int i = StartPos.X; i >= EndPos.X; i--) {
			if (field[StartPos.Y][i] > 0 || field[StartPos.Y][i] == -3) return false;
		}

		return true;
	}

	if (StartPos.X < EndPos.X) { //Right
		if (EndPos.X >= field_width) return false;

		for (int i = StartPos.X; i <= EndPos.X; i++) {
			if (field[StartPos.Y][i] > 0 || field[StartPos.Y][i] == -3) return false;
		}

		return true;
	}

	if (StartPos.Y < EndPos.Y) { //Down
		if (EndPos.Y >= field_width) return false;

		for (int i = StartPos.Y; i <= EndPos.Y; i++) {
			if (field[i][StartPos.X] > 0 || field[i][StartPos.X] == -3) return false;
		}

		return true;
	}

	if (StartPos.Y > EndPos.Y) { //Up
		if (EndPos.Y < 0) return false;

		for (int i = StartPos.Y; i >= EndPos.Y; i--) {
			if (field[i][StartPos.X] > 0 || field[i][StartPos.X] == -3) return false;
		}

		return true;
	}

	if (StartPos.X && StartPos.Y && StartPos.X == EndPos.X && StartPos.Y == EndPos.Y && field[StartPos.Y][StartPos.X] != -3) 
		return true;

	return false;
}

void Field::ShowField(bool fog, int startX, int startY)
{
	if (startX && startY) gotoxy(startX, startY);
	else cout << "\n\n\t\t";

	int mainWaterColor = (!fog) ? color.LightBlue : color.DarkGray;
	int shipsColor = (!fog) ? color.Yellow : color.DarkGray;

	color.SetColor(mainWaterColor);

	for (int i = 0; i < field_height; i++) {
		for (int j = 0; j < field_width; j++) {

			COORD coord = { j, i };
			coords[getxy()] = coord;

			if(!field[i][j] || field[i][j] == -3)
				cout << ((!fog) ? "~ " : "* ");

			if (field[i][j] > 0) {
				color.SetColor(shipsColor);
				cout << ((!fog) ? "^ " : "* ");
				color.SetColor(mainWaterColor);
			}

			if (field[i][j] == -1) {
				color.SetColor(color.LightGray);
				cout << "o ";
				color.SetColor(mainWaterColor);
			}

			if (field[i][j] == -2) {
				color.SetColor(color.Red);
				cout << "X ";
				color.SetColor(mainWaterColor);
			}

		}
		cout << endl;
		if (startX && startY) gotoxy(startX, getxy().Y);
	}

	color.ResetColor();
}

void Field::operator=(Field& field)
{
	for (int i = 0; i < field_height; i++) {
		for (int j = 0; j < field_width; j++) {
			SetField(j, i, field.GetField(j, i));
		}
	}

	coords = field.GetCoords();
	shipsCount = field.GetShipsCount();
}

bool operator<(const COORD& left, const COORD& right)
{
	if (left.Y < right.Y)
	{
		return true;
	}
	else if (right.Y < left.Y)
	{
		return false;
	}
	else if (left.X < right.X)
	{
		return true;
	}
	else /* (right.x < left.x) */
	{
		return false;
	}
}
