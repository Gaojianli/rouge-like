#pragma once
#include "mankind.h"
class Player : public Mankind
{
public:
	int movePoints = 5;
	Player(std::pair<int, int> postion, const char *name, Role role);
	bool move(MoveDirection direction) override;
	void interactiveThis() override;
	const char* getInfo() override;
};