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
	WINDOW *menubar, *map, *info, *status, *playerWin;
	std::wstring infoList[17];
	int header = 0;
	MainMap mainmap;
	void drawMain();
	void drawMap();
	void drawBackPack();
	void drawPlayer();
	WINDOW** drawMenu(bool* menuEnable);
	MenuType scrollMenu(WINDOW** items, int count, bool* menuEnable);
	void deleteMenu(WINDOW** items, int count);
	bool isAround(ObjectType target);
	bool canControlAround();
	void nextRound();
public:
	std::shared_ptr<Map> globalMap;
	Game() = default;
	void init(); //create player
	void start();
	void addInfo(const char *message);
	void addInfo(const wchar_t * message);
};