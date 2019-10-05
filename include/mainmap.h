//MainMap.h

#pragma once
#define MAP_SIZE 4
#include <iostream>
#include <vector>
#include <string>
#include "gameObject.h"
#include "creature.h"
#include "item.h"
#include "map.h"
class MainMap {
private:
	std::vector<std::vector<Map>> maps;
	int mainmap_x = 0;
	int mainmap_y = 0;
public:
	MainMap();
	bool SetMapLocation(int x, int y); // Set X and Y if Not out of range and return true, otherwise return false and do nothing
	bool isOutOfRange(int x,int y);
	int GetMapXLocation();
	int GetMapYLocation();
};