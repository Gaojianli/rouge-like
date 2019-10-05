#include <list>
#include <memory>
#include <iostream>
#include "creature.h"
#include "curses/curses.h"
#include "map.h"
#include "player.h"
using namespace std;
class Game {
private:
	list<Creature*> characters;
	shared_ptr<Player> player;
	WINDOW* menubar, *map, *info, *status;
public:
	shared_ptr<Map> globalMap;
	Game()=default;
	void init();//create player
	void start();
	void drewMain();
};