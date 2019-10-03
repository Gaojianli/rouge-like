#include <list>
#include <memory>
#include <iostream>
#include "creature.h"
#include "map.h"
#include "player.h"
using namespace std;
class Game {
private:
	list<Creature*> characters;
	shared_ptr<Player> player;
	shared_ptr<Map> globalMap;
public:
	Game()=default;
	void init();//create player
	void start();
};