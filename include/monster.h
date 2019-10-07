#pragma once
#include "mankind.h"
enum class MonsterType
{
	slime = 0,
	skeleton,
	dragon,
	snake,
	tarrasque
};
class Mankind;
class Monster : public Creature
{
public:
	Monster(std::pair<int, int> position, const char *name, MonsterType type, attitudes attitude);
	int beControlled = 0;
	; //0 is not be controlled, other is the lasting time
	void interactiveThis() override{};
	MonsterType type;
	Mankind* master;
	int getAttack() override;
	int getDefense() override;
	CreatureType getCreatureType() override;
	std::string getInfo() override;
};