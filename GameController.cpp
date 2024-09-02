#include "GameController.h"

GameController::GameController()
{
	end = win = false;
}

void GameController::Start()
{
	clear();

	end = false;
	win = false;

	userField.ShowField(false, 10, 10);

	map<int, int> ships;
	int maxShipSize = 4;
	
	int selectedShip = 1;
	bool horizontal = true;

	auto initShipsMap = [](map<int, int>& ships, int& maxShipSize) {
		for (int i = 0; i < maxShipSize; i++) {
			ships[i + 1] = maxShipSize - i;
		}
	};

	auto setBottomMenu = [](Field& userField, Color& color) {
		gotoxy(10, 25);

		color.SetColor(color.LightCyan);
		cout << "Enter - Set ship	";

		color.SetColor(color.LightMagenta);
		cout << "R - Reverse ship	";

		color.SetColor(color.Brown);
		cout << "C - Clear field	   ";

		color.SetColor(color.Cyan);
		cout << "F - Auto set		";

		if (userField.GetShipsCount() == 10) color.SetColor(color.Green);
		else color.SetColor(color.Red);
		cout << "Space - Continue";
	};

	initShipsMap(ships, maxShipSize);

	COORD currPos = {5, 9};
	COORD endPos = {-1, -1};

	setBottomMenu(userField, color);

	bool exit = false;
	int press = 0;

	while (!exit) {
		switch (press)
		{
		case 13: { //Enter
			if (userField.GetShipsCount() < 10 && endPos.X >= 0 && endPos.Y >=0 && selectedShip && ships[selectedShip] > 0 && userField.CanSetShip(currPos, endPos)) {
				userField.SetShip(currPos, endPos, selectedShip);
				ships[selectedShip] -= 1;

				if (ships[selectedShip] <= 0) {
					ships.erase(selectedShip);
					selectedShip++;

					if (selectedShip == 5) {
						setBottomMenu(userField, color);
					}
				}
			}
			break;
		}
		case 32: { //Space
			if (userField.GetShipsCount() >= 10) exit = true;
			break;
		}
		case 72: { //Up
			if (currPos.Y <= 0) break;
			currPos.Y--;

			userField.ClearColors();

			endPos = { (SHORT)((horizontal) ? currPos.X + selectedShip - 1 : currPos.X), (SHORT)((horizontal) ? currPos.Y : currPos.Y + selectedShip - 1) };
			userField.Draw(currPos, endPos, selectedShip);

			break;
		}
		case 75: { //Left
			if (currPos.X <= 0) break;
			currPos.X--;

			userField.ClearColors();

			endPos = { (SHORT)((horizontal) ? currPos.X + selectedShip - 1 : currPos.X), (SHORT)((horizontal) ? currPos.Y : currPos.Y + selectedShip - 1) };
			userField.Draw(currPos, endPos, selectedShip);
			break;
		}
		case 77: { //Right
			if (currPos.X + 1 >= userField.GetFieldWidth()) break;
			currPos.X++;

			userField.ClearColors();

			endPos = { (SHORT)((horizontal) ? currPos.X + selectedShip - 1 : currPos.X), (SHORT)((horizontal) ? currPos.Y : currPos.Y + selectedShip - 1) };
			userField.Draw(currPos, endPos, selectedShip);
			break;
		}
		case 80: { //Down
			if (currPos.Y + 1 >= userField.GetFieldHeight()) break;
			currPos.Y++;

			userField.ClearColors();

			endPos = { (SHORT)((horizontal) ? currPos.X + selectedShip - 1 : currPos.X), (SHORT)((horizontal) ? currPos.Y : currPos.Y + selectedShip - 1) };
			userField.Draw(currPos, endPos, selectedShip);
			break;
		}
		case 99: { //C - clear
			userField.ClearField();
			userField.ClearColors();

			selectedShip = 1;

			setBottomMenu(userField, color);
			initShipsMap(ships, maxShipSize);
			break;
		}
		case 102: { //F - Generate
			userField.Generate();
			userField.ClearColors();

			setBottomMenu(userField, color);
			ships.clear();
			break;
		}
		case 114: { //R - reverse
			horizontal = !horizontal;

			userField.ClearColors();

			endPos = { (SHORT)((horizontal) ? currPos.X + selectedShip - 1 : currPos.X), (SHORT)((horizontal) ? currPos.Y : currPos.Y + selectedShip - 1) };
			userField.Draw(currPos, endPos, selectedShip);
			break;
		}
		default:
			break;
		}

		press = _getch();
		//userField.ClearColors();
	}

	userField.ClearColors();
	Battle();
}

void GameController::Battle()
{
	HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD InputRecord;
	DWORD Events;
	COORD coord;

	DWORD prev_mode;
	GetConsoleMode(hin, &prev_mode);
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_EXTENDED_FLAGS | (prev_mode & ~ENABLE_QUICK_EDIT_MODE));

	SetConsoleMode(hin, ENABLE_MOUSE_INPUT);

	enemy.SetUserField(userField);
	enemy.Start();

	auto userShoot = [](Enemy& enemy, COORD& coord, vector<COORD>& currentShipCoords, bool& isKill) {
		COORD fieldCoord = enemy.GetEnemyField().GetFieldCoord(coord);
		int field_num = enemy.GetEnemyField().GetField(fieldCoord.X, fieldCoord.Y);

		if (field_num == -2 || field_num == -1) return false;

		bool isHit = enemy.GetEnemyField().ShootTo(fieldCoord.X, fieldCoord.Y);
		int killedField = 1;

		currentShipCoords.clear();
		currentShipCoords.push_back(fieldCoord);

		//Kill check

		if (field_num > 0 && field_num == killedField) {
			isKill = true;
			return !isHit;
		}

		//Right
		for (SHORT i = fieldCoord.X + 1; i < enemy.GetEnemyField().GetFieldWidth(); i++) {
			if (enemy.GetEnemyField().GetField(i, fieldCoord.Y) == -2) {
				killedField++;

				currentShipCoords.push_back({ i, fieldCoord.Y });

				if (field_num > 0 && field_num <= killedField) {
					isKill = true;
					return !isHit;
				}
			}

			if (enemy.GetEnemyField().GetField(i, fieldCoord.Y) > 0) return !isHit;

			if (enemy.GetEnemyField().GetField(i, fieldCoord.Y) == -1 || 
				enemy.GetEnemyField().GetField(i, fieldCoord.Y) == -3) break;
		}

		//Left
		for (SHORT i = fieldCoord.X - 1; i >= 0; i--) {
			if (enemy.GetEnemyField().GetField(i, fieldCoord.Y) == -2) {
				killedField++;

				currentShipCoords.push_back({ i, fieldCoord.Y });

				if (field_num > 0 && field_num <= killedField) {
					isKill = true;
					return !isHit;
				}
			}

			if (enemy.GetEnemyField().GetField(i, fieldCoord.Y) > 0) return !isHit;

			if (enemy.GetEnemyField().GetField(i, fieldCoord.Y) == -1 ||
				enemy.GetEnemyField().GetField(i, fieldCoord.Y) == -3) break;
		}

		//Up
		for (SHORT i = fieldCoord.Y - 1; i >= 0; i--) {
			if (enemy.GetEnemyField().GetField(fieldCoord.X, i) == -2) {
				killedField++;

				currentShipCoords.push_back({ fieldCoord.X, i });

				if (field_num > 0 && field_num <= killedField) {
					isKill = true;
					return !isHit;
				}
			}

			if (enemy.GetEnemyField().GetField(fieldCoord.X, i) > 0) return !isHit;

			if (enemy.GetEnemyField().GetField(fieldCoord.X, i) == -1 ||
				enemy.GetEnemyField().GetField(fieldCoord.X, i) == -3) break;
		}

		//Down
		for (SHORT i = fieldCoord.Y + 1; i < enemy.GetEnemyField().GetFieldHeight(); i++) {
			if (enemy.GetEnemyField().GetField(fieldCoord.X, i) == -2) {
				killedField++;

				currentShipCoords.push_back({ fieldCoord.X, i });

				if (field_num > 0 && field_num == killedField) {
					isKill = true;
					return !isHit;
				}
			}

			if (enemy.GetEnemyField().GetField(fieldCoord.X, i) > 0) return !isHit;

			if (enemy.GetEnemyField().GetField(fieldCoord.X, i) == -1 ||
				enemy.GetEnemyField().GetField(fieldCoord.X, i) == -3) break;
		}

		return !isHit;
	};

	bool move = true;
	vector<COORD> currentShipCoords;

	while (!end) {
		gotoxy(0, 0);

		if (move) {
			ReadConsoleInput(hin, &InputRecord, 1, &Events);

			bool isKill = false;

			if (InputRecord.Event.MouseEvent.dwButtonState == 1)
			{
				coord.X = InputRecord.Event.MouseEvent.dwMousePosition.X;
				coord.Y = InputRecord.Event.MouseEvent.dwMousePosition.Y;

				move = !userShoot(enemy, coord, currentShipCoords, isKill);
			}

			if (isKill) {
				for (COORD coord : currentShipCoords) {
					if (coord.Y - 1 >= 0 && enemy.GetEnemyField().GetField(coord.X, coord.Y - 1) == -3)
						enemy.GetEnemyField().SetField(coord.X, coord.Y - 1, -1); //Up
					if (coord.Y + 1 < enemy.GetEnemyField().GetFieldHeight() && enemy.GetEnemyField().GetField(coord.X, coord.Y + 1) == -3)
						enemy.GetEnemyField().SetField(coord.X, coord.Y + 1, -1); //Down
					if (coord.X - 1 >= 0 && enemy.GetEnemyField().GetField(coord.X - 1, coord.Y) == -3)
						enemy.GetEnemyField().SetField(coord.X - 1, coord.Y, -1); //Left
					if (coord.X + 1 < enemy.GetEnemyField().GetFieldWidth() && enemy.GetEnemyField().GetField(coord.X + 1, coord.Y) == -3)
						enemy.GetEnemyField().SetField(coord.X + 1, coord.Y, -1); //Right

					if (coord.X - 1 >= 0 && coord.Y - 1 >= 0 && enemy.GetEnemyField().GetField(coord.X - 1, coord.Y - 1) == -3)
						enemy.GetEnemyField().SetField(coord.X - 1, coord.Y - 1, -1); //Left up
					if (coord.X + 1 < enemy.GetEnemyField().GetFieldWidth() && coord.X + 1 < enemy.GetEnemyField().GetFieldHeight() && enemy.GetEnemyField().GetField(coord.X + 1, coord.Y + 1) == -3)
						enemy.GetEnemyField().SetField(coord.X + 1, coord.Y + 1, -1); //Right down

					if (coord.X - 1 >= 0 && coord.Y + 1 < enemy.GetEnemyField().GetFieldHeight() && enemy.GetEnemyField().GetField(coord.X - 1, coord.Y + 1) == -3)
						enemy.GetEnemyField().SetField(coord.X - 1, coord.Y + 1, -1); //Left donw
					if (coord.X + 1 < enemy.GetEnemyField().GetFieldWidth() && coord.Y - 1 >= 0 && enemy.GetEnemyField().GetField(coord.X + 1, coord.Y - 1) == -3)
						enemy.GetEnemyField().SetField(coord.X + 1, coord.Y - 1, -1); //Right up
				}

				enemy.GetEnemyField().SetShipsCount(enemy.GetEnemyField().GetShipsCount() - 1);

				if (enemy.GetEnemyField().GetShipsCount() <= 0) {
					end = true;
					win = true;

					continue;
				}

				enemy.GetEnemyField().ClearColors(true);

				continue;
			}

			if (move) continue;
		}
		else {
			move = enemy.Shoot();

			if (enemy.IsWin()) {
				end = true;
				win = false;

				continue;
			}
		}

		Sleep(1000);
	}

	End();
}

void GameController::End()
{
	userField.ClearField();
	enemy.GetEnemyField().ClearField();
	clear();

	color.SetColor((win) ? color.Green : color.Red);
	cout << "\n\t\t\tGAME OVER!\n";

	color.SetColor(color.DarkGray);
	cout << "\t\t\tYou " << ((win) ? "win" : "lose") << "\n";
	cout << "\t\t\tPress 'r' to restart";

	int press = 0;
	press = _getch();
	clear();

	if (press == 114) Start();
}
