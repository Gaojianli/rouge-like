#pragma once
#include <list>
#include <memory>
#include <iostream>
#include "creature.h"
#include "map.h"
#include "player.h"
using namespace std;
class Creature;
class Game {
private:
	list<Creature*> characters;
	shared_ptr<Player> player;
public:
	shared_ptr<Map> globalMap;
	Game()=default;
	void init();//create player
	void start();
};