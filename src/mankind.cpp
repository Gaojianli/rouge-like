#include "mankind.h"
#include "weapons.h"
#include <string>
#include "game.h"
extern std::shared_ptr<Game> game;
Mankind::Mankind(std::pair<int, int> position, const char *name, Role role, attitudes attitude) : Creature(position.first, position.second, name, attitude), role(role)
{
	switch (role)
	{
	case Role::magician:
		this->health = this->healthUpper = 114;
		this->mana = this->manaUpper = 514;
		this->power = 9;
		break;
	case Role::wizard:
		this->health = this->healthUpper = 140;
		this->mana = this->manaUpper = 400;
		this->power = 9;
		break;
	case Role::amazon:
		this->health = this->healthUpper = 514;
		this->mana = this->manaUpper = 0;
		this->power = 15;
		break;
	case Role::dwarf:
		this->health = this->healthUpper = 200;
		this->mana = this->manaUpper = 200;
		this->power = 10;
		break;
	case Role::harpy:
		this->health = this->healthUpper = 190;
		this->mana = this->manaUpper = 220;
		this->power = 12;
		break;
	case Role::monkey:
		this->health = this->healthUpper = 514;
		this->mana = this->manaUpper = 0;
		this->power = 1;
		break;
	case Role::warrior:
		this->health = this->healthUpper = 514;
		this->mana = this->manaUpper = 114;
		this->power = 10;
		break;
	}
}

//control a monster, return true if succeed
bool Mankind::conjure(Monster *object)
{
	if (this->mana > object->power)
	{ //enough mana
		this->mana -= object->power;
		object->beControlled = 5; //control 5 rounds
		return true;
	}
	else
		return false;
}

const char *Mankind::getInfo()
{
	const std::string roles[] = {
		"magician",
		"wizard",
		"warrior",
		"harpy",
		"amazon",
		"dwarf",
		"monkey"};
	/*
	example:A magician. Which power is 5. Very dangerous.
	*/
	std::string introduction = "A ";
	introduction += roles[(int)role];
	introduction += ". Which power is " + std::to_string(this->power);
	introduction += ". Very " + (attitude == attitudes::agressive) ? "dangerous." : "friendly.";
	return introduction.c_str();
}

int Mankind::getAttack()
{
	int attackSum = power;
	for (auto &i : backpack)
	{
		if (i.getItemType() == ItemType::weapons)
		{
			attackSum += static_cast<Weapons &>(i).attack;
		}
	}
	return attackSum;
}

int Mankind::getDefense()
{
	int defenseSum = power;
	for (auto &i : backpack)
	{
		if (i.getItemType() == ItemType::weapons)
		{
			defenseSum += static_cast<Weapons &>(i).defense;
		}
	}
	return defenseSum;
}

bool Mankind::attack(Creature &beAttack)
{
	beAttack.beAttacked = true;
	//calculate attack sum
	int attackSum = this->getAttack();
	//calculate defends sum
	int defenseSum = beAttack.getDefense();
	if (attackSum < defenseSum)
		return false; //nothing happened
	else
		beAttack.health -= attackSum - defenseSum;
	if (beAttack.health < 0)
		beAttack.died();
	for (auto &i : follower)
	{ //monster be controlled will attack same object
		i->attack(beAttack);
	}
	return true;
}

bool Mankind::pick(Item &toPick)
{
	if (backpack.size() <= 4) //backpack is full
	{
		this->backpack.push_back(toPick);
		return true;
	}
	else
		return false;
}