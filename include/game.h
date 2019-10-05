#include <list>
#include <memory>
#include <iostream>
#include "creature.h"
#include "curses/curses.h"
#include "map.h"
#include "player.h"

class Game {
private:
	std::list<Creature*> characters;
	std::shared_ptr<Player> player;
	WINDOW* menubar, *map, *info, *status;
	std::string infoList[17];
	int header = 0;
public:
	std::shared_ptr<Map> globalMap;
	Game()=default;
	void init();//create player
	void start();
	void drawMain();
	void nextRound();
	void addInfo(const char* message);
};