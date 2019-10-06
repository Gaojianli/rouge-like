#include "weapons.h"
#include <string>
Weapons::Weapons(WeaponsType type, int attack, int defense, unsigned power)
{
	this->type = type;
	this->attack = attack;
	this->defense = defense;
	this->mana = power;
}
ItemType Weapons::getItemType()
{
	return ItemType::weapons;
}
std::string Weapons::getInfo()
{
	const std::string weaponsTypeStr[] = {
		"cane",
		"sword",
		"shield"};
	const std::string weaponsEffect[] = {
		"mana",
		"attack",
		"defense"};
	std::string describe = "";
	describe += "This is a " + weaponsTypeStr[static_cast<int>(this->type)] + ".";
	describe += "It can provide ";
	if (this->attack > 0)
	{
		describe += std::to_string(this->attack);
		describe += " attack ";
	}
	if (this->defense > 0)
	{
		describe += std::to_string(this->defense);
		describe += " defense ";
	}
	if (this->mana > 0)
	{
		describe += std::to_string(this->mana);
		describe += " mana ";
	}
	return describe;
}