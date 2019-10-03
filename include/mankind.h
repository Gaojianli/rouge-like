#pragma once
#include "creature.h"
#include "monster.h"
enum class Role {
	magician,
	wizard,
	warrior,
	harpy,
	amazon,
	dwarf,
	monkey
};
class Mankind:public Creature {
public:
	unsigned mana;
	unsigned manaUpper;
	std::vector<Item> backpack;
	std::vector<Monster* > follower;
	Mankind(std::pair<int x, int y> postion,const char* name, Role role);
	bool conjure(Monster* object);
	bool attack(Creature& beAttack);
};