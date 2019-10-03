#pragma once
#include "item.h"
enum class Directions
{
	east,
	south,
	west,
	north
};
class Key : public Item {
	bool used;
public:
	Directions direction;
	unsigned step;
	Key(Directions direction, unsigned step);
	bool useIt();
	void newRoundStarts();
	ItemType getItemType();
};