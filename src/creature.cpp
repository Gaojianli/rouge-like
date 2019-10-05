#include "creature.h"
Creature::Creature(int x, int y, const char* name,attitudes attitude) :position({ x,y }), name(name), beAttacked(false), bePoisoned(false),attitude(attitude)
{

}

ObjectType Creature::getType()
{
	return ObjectType::creature;
}

void Creature::died() {
	std::cout << "I am died";
}

bool Creature::attack(Creature& beAttack) {
	beAttack.beAttacked = true;
	//calculate attack sum
	int attackSum = this->getAttack();
	//calculate defends sum
	int defenseSum = beAttack.getDefense();
	if (attackSum < defenseSum)
		return false;//nothing happened
	else
		beAttack.health -= attackSum - defenseSum;
	if (beAttack.health < 0)
		beAttack.died();
	return true;
}