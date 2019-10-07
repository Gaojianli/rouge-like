#include "creature.h"
#include "game.h"
extern std::shared_ptr<Game> game;
Creature::Creature(int x, int y, const char *name, attitudes attitude) : position({x, y}), name(name), beAttacked(false), bePoisoned(false), attitude(attitude)
{
}

ObjectType Creature::getType()
{
	return ObjectType::creature;
}

void Creature::died()
{
	game->globalMap->eraseGameObjectAt(position.first, position.second, false);
	game->characters.remove(this);
	game->addInfo((this->name + " died.").c_str());
	delete this;
}

bool Creature::attack(Creature &beAttack)
{
	beAttack.beAttacked = true;
	//calculate attack sum
	int attackSum = this->getAttack();
	//calculate defends sum
	int defenseSum = beAttack.getDefense();
	if (attackSum < defenseSum)
		return false; //nothing happened
	else
		beAttack.health -= attackSum - defenseSum;
	if (beAttack.health < 0)
		beAttack.died();
	return true;
}

void Creature::setPosition(unsigned x, unsigned y)
{
	if (x < 9 && y < 9)
		this->position = {x, y};
}

bool Creature::move(MoveDirection direction)
{
	//NPC can't go out of the room.
	int newX = position.first;
	int newY = position.second;
	switch (direction)
	{
	case MoveDirection::up:
		newY++;
		break;
	case MoveDirection::down:
		newY--;
		break;
	case MoveDirection::left:
		newX--;
		break;
	case MoveDirection::right:
		newX++;
		break;
	}
	if (game->globalMap->isOutOfRange(newX, newY))
	{
		return false; //out of range
	}
	if (game->globalMap->getLocationType(newX, newY) != ObjectType::nothing) {
		return false; // on other things
	}
	else
	{
		position = {newX, newY};
		return true;
	}
}