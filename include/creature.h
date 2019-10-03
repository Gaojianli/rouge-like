#include <iostream>
#include <vector>
#include "gameObject.h"
#include "item.h"
enum attitudes {
	friendly,
	agressive
};
class Creature:public gameObject{
	std::pair<int, int> postion;
	std::string name;
	unsigned health;
	unsigned power;
	attitudes attitude;
	bool beAttacked;
	bool bePoisoned;
public:
	Creature();
	ObjectType getType() {
		return ObjectType::creature;
	}
	//virtual void move(int x, int y);
};
