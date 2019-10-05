#pragma once
#include "item.h"
enum class WeaponsType
{
	cane = 0,
	sword,
	shield
};
class Weapons : public Item
{
public:
	WeaponsType type;
	int attack;
	int defense;
	unsigned mana;
	Weapons(WeaponsType type, int attack, int defense, unsigned power);
	ItemType getItemType();
	const char *getInfo() override;
};