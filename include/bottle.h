#pragma once
#include <exception>
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
	Bottle();
	Bottle(BottleType, unsigned increased);
	template<typename T>
	bool use(T& target);
	ItemType getItemType() override;
	const char* getInfo() override;
	// void interactiveThis() override;
};