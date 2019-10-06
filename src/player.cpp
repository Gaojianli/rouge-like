#include "player.h"
#include "game.h"
extern std::shared_ptr<Game> game;
Player::Player(std::pair<int, int> postion, const char* name, Role role) : Mankind(postion, name, role, attitudes::agressive)
{
}

void Player::interactiveThis() {}

bool Player::move(MoveDirection direction)
{
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
	auto gates = game->globalMap->getGates();
	if (movePoints == 0) {//can't move again this round
		return false;
	}
	if (newX > 8 || newX < 0)
	{ //detect door
		if (newY == 4)
		{ //door
			if (gates[newX > 8 ? 2 : 3])
			{
				position = { newX > 8 ? 0 : 8, newY }; //new position in new door
				movePoints--;
				return true;
			}
		}
		return false;
	}
	if (newY > 8 || newY < 0)
	{
		if (newX == 4)
		{
			if (gates[newY > 8 ? 0 : 1])
			{
				position = { newX, newY > 8 ? 0 : 8 };
				movePoints--;
				return true;
			}
		}
		return false;
	}
	if (game->globalMap->isOutOfRange(newX, newY) || game->globalMap->getLocationType(newX, newY) != ObjectType::nothing)
	{
		return false; //out of range
	}
	else
	{
		position = { newX, newY };
		movePoints--;
		return true;
	}
}

const char* Player::getInfo()
{
	return "Player";
}