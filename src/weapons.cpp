#include "weapons.h"
Weapons::Weapons(WeaponsType type, int attack, int defense, unsigned power) {
	this->type = type;
	this->attack = attack;
	this->defense = defense;
	this->power = power;
}
ItemType Weapons::getItemType() {
	return ItemType::weapons;
}