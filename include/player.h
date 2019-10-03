#pragma once
#include "mankind.h"
class Player:public Mankind {
public:
	Player(std::pair<int, int> postion, const char* name, Role role);
	void interactiveThis() override;
};