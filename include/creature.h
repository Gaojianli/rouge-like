#pragma once
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
	std::pair<int, int> position;
	std::string name;
	int health=100;
	int healthUpper=100;
	unsigned power=100;
	attitudes attitude;
	bool beAttacked;
	bool bePoisoned;
	ObjectType getType() override;
	virtual void died();
	virtual bool attack(Creature& beAttack);
	virtual int getAttack() = 0;//return the sum of the attack points of this creature
	virtual int getDefense() = 0;//return the sum of the defense points of this creature
protected:
	Creature(int x, int y, const char* name, attitudes attitude);
	//virtual void move(int x, int y);
};
