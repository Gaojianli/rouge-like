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
	introduction += ", Health is ";
	introduction += std::to_string(this->health) + "/" + std::to_string(this->healthUpper);
	introduction += ".Very ";
	introduction += (attitude == attitudes::agressive) ? "agressive." : "friendly.";
	return introduction;
}

Monster::Monster(std::pair<int, int> position, const char *name, MonsterType type, attitudes attitude)
	: Creature(position.first, position.second, name, attitude), type(type), beControlled(0),master(nullptr)
{
	switch (type)
	{
	case MonsterType::slime:
		this->healthUpper = this->health = 20;
		this->power = 4;
		break;
	case MonsterType::skeleton:
		this->healthUpper = this->health = 50;
		this->power = 8;
		break;
	case MonsterType::dragon:
		this->healthUpper = this->health = 100;
		this->power = 15;
		break;
	case MonsterType::snake:
		this->healthUpper = this->health = 40;
		this->power = 6;
		break;
	case MonsterType::tarrasque:
		this->healthUpper = this->health = 70;
		this->power = 9;
		break;
	default:
		break;
	}
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