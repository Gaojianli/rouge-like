#include <iostream>
#include <vector>
#include "item.h"
enum attitudes {
	friendly,
	agressive
};
class Creature {
	std::pair<int, int> postion;
	std::string name;
	unsigned health;
	unsigned power;
	attitudes attitude;
	bool beAttacked;
	bool bePoisoned;
	//virtual void move(int x, int y);
};