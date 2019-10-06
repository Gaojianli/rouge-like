#include "monster.h"
#include "weapons.h"
#include <string>
std::string Monster::getInfo()
{
	const std::string monsterTypes[] = {
		"slime",
		"skeleton",
		"dragon",
		"snake",
		"tarrasque"};
	std::string introduction = "A ";
	introduction += monsterTypes[(int)type];
	introduction += ". Which power is " + std::to_string(this->power);
	introduction += ". Very " + (attitude == attitudes::agressive) ? "dangerous." : "friendly.";
	return introduction;
}

Monster::Monster(std::pair<int, int> position, const char *name, MonsterType type, attitudes attitude)
	: Creature(position.first, position.second, name, attitude), type(type), beControlled(0)
{
}

int Monster::getAttack()
{
	return this->power;
}

int Monster::getDefense()
{
	return this->power;
}
CreatureType Monster::getCreatureType() {
	return CreatureType::monster;
}