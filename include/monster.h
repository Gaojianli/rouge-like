#pragma once
#include "mankind.h"
class Monster :public Creature {
public:
	Monster();
	int beControlled;//0 is not be controlled, other is the lasting time
	void interactiveThis() override {};
	bool attack(Creature& beAttack);
};