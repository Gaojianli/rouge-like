#include <iostream>
#include "gameObject.h"
class item:public gameObject {
	//type
	std::string name;
	ObjectType getType();
};