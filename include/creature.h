#include <iostream>
#include <vector>
#include "gameObject.h"
#include "item.h"
enum attitudes {
	friendly,
	agressive
};
class Creature:public gameObject{
public:
	std::pair<int, int> postion;
	std::string name;
	unsigned health=100;
	unsigned healthUpper=100;
	unsigned power=100;
	attitudes attitude;
	bool beAttacked;
	bool bePoisoned;
	Creature(int x, int y, const char* name);
	ObjectType getType() {
		return ObjectType::creature;
	}
	//virtual void move(int x, int y);
};
