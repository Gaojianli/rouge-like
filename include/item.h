#pragma once
#include <iostream>
#include "gameObject.h"
enum class ItemType {
	bottle,
	weapons,
	key
};
class Item:public gameObject {
	//type
	std::string name;
	ObjectType getType();
	virtual ItemType getItemType();
};