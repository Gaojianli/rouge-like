#pragma once
#include <iostream>
#include "gameObject.h"
enum class ItemType
{
	bottle,
	weapons,
	key
};
class Item : public gameObject
{
public:
	//type
	std::string name;
	ObjectType getType();
	virtual ItemType getItemType();
	virtual void newRoundStarts(){};
	std::string getInfo();
	void interactiveThis() override{};
};