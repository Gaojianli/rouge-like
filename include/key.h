#pragma once
#include "item.h"
enum class Directions
{
	up = 0,
	down,
	left,
	right,
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
	std::string getInfo() override;
};