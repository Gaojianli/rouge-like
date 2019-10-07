#pragma once
#include <list>
#include <memory>
#include <iostream>
#include "creature.h"
#include "curses/curses.h"
#include "map.h"
#include "mainmap.h"
#include "player.h"
#include "key.h"

enum class MenuType
{
	Investigate = 0,
	Attack,
	Control,
	PickUp,
	Backpack,
	NextRound,
	Help,
	Exit,
	Close
};

class Game
{
private:
	std::shared_ptr<Player> player;
	WINDOW *menubar, *map, *info, *status, *playerWin, **backpackWin, *playerStatusWin;
	std::wstring infoList[17];
	int header = 0;
	bool wined = false;
	unsigned roundNumber = 1;
	MainMap mainmap;
	void drawMain();
	void drawMap();
	void drawBackPack();
	void drawPlayerStatus();
	void drawPlayer();
	WINDOW** drawMenu(bool* menuEnable);
	MenuType scrollMenu(WINDOW** items, int count, bool* menuEnable);
	int scrollBackpack();
	Directions scrollDirections(bool* directionsEnable);
	bool useOrThrowBackpack(int backpackIndex);
	void useItem(int backpackIndex);
	void throwItem(int backpackIndex);
	void gotoWin();
	void deleteMenu(WINDOW** items, int count);
	bool isAround(ObjectType target);
	bool canControlAround();
	void nextRound();
	bool conjoure();
	void pickup();
	void printHelp();
	void investigate();
	bool attack();
public:
	Map* globalMap;
	std::list<Creature*> characters;
	std::shared_ptr<MainMap> globalMainMap;
	Game() = default;
	void init(); //create player
	void start();
	void addInfo(const char *message);
	void addInfo(const wchar_t * message);
};