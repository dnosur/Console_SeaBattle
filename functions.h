#pragma once
#include <windows.h>
#include <string>
#include <conio.h>

#include <map>

void clear();
void pause();

void gotoxy(int X, int Y);

COORD getxy();
void getxy(int& x, int& y);

template<typename K, typename V>
int erase_by_value(std::map<K, V>& mapOfElemen, V value)
{
    int totalDeletedElements = 0;
    auto it = mapOfElemen.begin();

    while (it != mapOfElemen.end())
    {

        if (it->second == value)
        {
            totalDeletedElements++;

            it = mapOfElemen.erase(it);
        }
        else
        {
            // Go to next entry in map
            it++;
        }
    }
    return totalDeletedElements;
};