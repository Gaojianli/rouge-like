#pragma once
#include <iostream>
#include <vector>
#include "gameObject.h"
#include "item.h"
enum class CreatureType {
	human,
	monster
};
enum class attitudes
{
	friendly,
	agressive
};
enum class MoveDirection
{
	up = 0,
	down,
	left,
	right
};
class Creature : public gameObject
{
public:
	std::pair<int, int> position;
	std::string name;
	int health = 100;
	unsigned healthUpper = 100;
	unsigned power = 10;
	attitudes attitude;
	bool beAttacked;
	bool isDied = false;
	int bePoisoned;
	ObjectType getType() override;
	virtual void died();
	virtual CreatureType getCreatureType() = 0;
	virtual bool attack(Creature &beAttack);
	void setPosition(unsigned x, unsigned y);
	virtual int getAttack() = 0;  //return the sum of the attack points of this creature
	virtual int getDefense() = 0; //return the sum of the defense points of this creature
	virtual bool move(MoveDirection direction);

protected:
	Creature(int x, int y, const char *name, attitudes attitude);
};