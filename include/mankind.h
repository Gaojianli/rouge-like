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
private:
	unsigned manaUpper=0;
public:
	unsigned mana=0;
	std::vector<Item*> backpack;
	bool pick(Item* toPick);
	Role role;
	std::vector<Monster*> follower;
	Mankind(std::pair<int, int> position,const char* name, Role role, attitudes attitude);
	bool conjure(Monster* object);
	bool attack(Creature& beAttack) override;
	CreatureType getCreatureType() override;
	std::string getInfo() override;
	void interactiveThis() override {};
	int getAttack() override;
	int getDefense() override;
	unsigned getMana();
	unsigned getManaUpper();
	void changeMana(int amount);
};