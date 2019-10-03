#pragma once
#include <iostream>
#include "gameObject.h"
enum class ItemType {
	bottle,
	weapons,
	key
};
class Item:public gameObject {
public:
	//type
	std::string name;
	ObjectType getType();
	virtual ItemType getItemType();
	virtual void newRoundStarts() {};
	const char* getInfo() override { return NULL; };
	void interactiveThis() override {};
};