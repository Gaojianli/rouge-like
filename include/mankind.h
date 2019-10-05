#pragma once
#include "creature.h"
#include "monster.h"
enum class Role {
	magician=0,
	wizard,
	warrior,
	harpy,
	amazon,
	dwarf,
	monkey
};
class Monster;
class Mankind:public Creature {
public:
	unsigned mana=0;
	unsigned manaUpper=0;
	std::vector<Item> backpack;
	Role role;
	std::vector<Monster*> follower;
	Mankind(std::pair<int, int> position,const char* name, Role role, attitudes attitude);
	bool conjure(Monster* object);
	const char* getInfo() override;
	void interactiveThis() override {};
	int getAttack() override;
	int getDefense() override;
};