#include "creature.h"
Creature::Creature(int x, int y, const char* name,attitudes attitude) :postion({ x,y }), name(name), beAttacked(false), bePoisoned(false),attitude(attitude)
{

}

const char* Creature::getInfo() {
	return NULL;
}
ObjectType Creature::getType()
{
	return ObjectType::creature;
}

void Creature::died() {
	std::cout << "I am died";
}