#pragma once
#include "item.h"
enum class Directions
{
	up = 0,
	down,
	right,
	left,
	win
};
class Key : public Item
{
public:
	bool used = false;
	Directions direction;
	unsigned step;
	Key(Directions direction, unsigned step);
	bool useIt();
	void newRoundStarts();
	ItemType getItemType();
	const char *getInfo() override;
};