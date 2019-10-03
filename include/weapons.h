#pragma once
#include "item.h"
enum class WeaponsType
{
	cane,
	sword,
	shield
};
class Weapons : public Item {
public:
	WeaponsType type;
	int attack;
	int defense;
	unsigned power;
	Weapons(WeaponsType type, int attack, int defense, unsigned power);
	ItemType getItemType();
};