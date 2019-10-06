#include <list>
#include <memory>
#include <iostream>
#include "creature.h"
#include "curses/curses.h"
#include "map.h"
#include "mainmap.h"
#include "player.h"

enum class MenuType
{
	Investigation = 0,
	Attack,
	PickUp,
	Control,
	Backpack,
	Help,
	Exit,
	Close
};

class Game
{
private:
	std::list<Creature *> characters;
	std::shared_ptr<Player> player;
	WINDOW *menubar, *map, *info, *status;
	std::string infoList[17];
	int header = 0;
	MainMap mainmap;

public:
	std::shared_ptr<Map> globalMap;
	Game() = default;
	void init(); //create player
	void start();
	void drawMain();
	WINDOW **drawMenu(bool *menuEnable);
	MenuType scrollMenu(WINDOW **items, int count, bool *menuEnable);
	bool isAround(ObjectType target);
	bool canControlAround();
	void deleteMenu(WINDOW **items, int count);
	void nextRound();
	void addInfo(const char *message);
	void drawMap();
};