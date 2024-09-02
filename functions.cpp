#include "functions.h"

void clear() {
	system("cls");
}

void pause() {
	_getch();
}

void gotoxy(int X, int Y)
{
	COORD coord = { X, Y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void getxy(int& x, int& y) {
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!GetConsoleScreenBufferInfo(hStd, &screenBufferInfo))
		printf("GetConsoleScreenBufferInfo (%d)\n", GetLastError());

	x = screenBufferInfo.dwCursorPosition.X;
	y = screenBufferInfo.dwCursorPosition.Y;
}

COORD getxy() {
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!GetConsoleScreenBufferInfo(hStd, &screenBufferInfo))
		printf("GetConsoleScreenBufferInfo (%d)\n", GetLastError());

	return screenBufferInfo.dwCursorPosition;
}