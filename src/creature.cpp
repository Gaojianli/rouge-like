#include "creature.h"
#include "game.h"
extern std::shared_ptr<Game> game;
Creature::Creature(int x, int y, const char* name,attitudes attitude) :position({ x,y }), name(name), beAttacked(false), bePoisoned(false),attitude(attitude)
{

}

ObjectType Creature::getType()
{
	return ObjectType::creature;
}

void Creature::died() {
	game->globalMap->eraseGameObjectat(position.first, position.second);
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

void Creature::setPosition(unsigned x, unsigned y)
{
	if (x < 9 && y < 9)
		this->position = { x,y };
}