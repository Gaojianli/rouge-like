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
	Weapons type;
	int attack;
	int defense;
	unsigned power;
	Weapons(Weapons type, int attack, int defense, unsigned power);
};