#pragma once
#include "mankind.h"
enum class MoveDirection {
	up,
	down,
	left,
	right
};
class Player:public Mankind {
public:
	Player(std::pair<int, int> postion, const char* name, Role role);
	bool move(MoveDirection direction);
	void interactiveThis() override;
};