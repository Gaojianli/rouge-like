#pragma once
#include "item.h"
#include "creature.h"
#include "monster.h"
enum class BottleType {
	bloodBottle,
	manaBottle,
	poison
};
class Bottle :public Item {
public:
	unsigned increased;
	BottleType type;
	bottle(BottleType, unsigned increased);
	template<typename T>
	bool use(T& target);
	ItemType getItemType();
};