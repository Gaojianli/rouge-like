#include "game.h"
#include "bottle.h"
#include "monster.h"
#include "mankind.h"
#include "mainmap.h"
#include "key.h"
#include "curses/curses.h"
#include <cmath>
using std::string;
void Game::init()
{
	initscr();
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);
	init_pair(3, COLOR_WHITE, COLOR_RED);
	init_pair(4, COLOR_WHITE, COLOR_GREEN);
	init_pair(5, COLOR_WHITE, COLOR_YELLOW);
	curs_set(0);
	raw();
	noecho();
	// Generate items
	std::pair<int, int> zeropair = std::make_pair(0, 0);
	int bottlenum = (std::rand() % 8 + 16);
	std::vector<gameObject *> item_to_distribute;
	for (int i = 0; i < bottlenum; i++) // Add bottle to item list
	{
		item_to_distribute.push_back(new Bottle());
	}
	for (int j = 0; j < 5; j++) // Add keys to item list
	{
		item_to_distribute.push_back(new Key(static_cast<Directions>(j), 1));
	}
	for (int k = 0; k < 3; k++) // Add Monster to item list
	{
		switch (std::rand() % 5)
		{
		case 0:
		{
			item_to_distribute.push_back(new Monster(zeropair, ("Slime" + std::to_string(k)).c_str(), MonsterType::slime, static_cast<attitudes>(std::rand() % 2)));
			break;
		}
		case 1:
		{
			item_to_distribute.push_back(new Monster(zeropair, ("Skeleton" + std::to_string(k)).c_str(), MonsterType::skeleton, static_cast<attitudes>(std::rand() % 2)));
			break;
		}
		case 2:
		{
			item_to_distribute.push_back(new Monster(zeropair, ("Dragon" + std::to_string(k)).c_str(), MonsterType::dragon, static_cast<attitudes>(std::rand() % 2)));
			break;
		}
		case 3:
		{
			item_to_distribute.push_back(new Monster(zeropair, ("Snake" + std::to_string(k)).c_str(), MonsterType::snake, static_cast<attitudes>(std::rand() % 2)));
			break;
		}
		case 4:
		{
			item_to_distribute.push_back(new Monster(zeropair, ("Tarrasque" + std::to_string(k)).c_str(), MonsterType::tarrasque, static_cast<attitudes>(std::rand() % 2)));
			break;
		}
		default:
			break;
		}
	}
	globalMap = std::make_shared<Map>(Map());
}

void Game::start()
{
	attrset(A_REVERSE);
	for (int i = 0; i < COLS; ++i)
		mvaddch(0, i, ' ');
	mvprintw(0, COLS / 2 - 6, "Rouge-like");
	attrset(A_NORMAL);
	mvprintw(LINES / 2 - 2, COLS / 2 - 27, "Please input your name:");
	refresh();
	curs_set(1); //show the curs
	echo();
	auto name = new char[20];
	flushinp(); //flush the input
	getnstr(name, 20);
	move(LINES / 2 - 2, 0);
	clrtoeol();
	noecho();
	mvprintw(4, COLS / 2 - 27, "Please select a role:");
	const string roles[] = {
		"magician",
		"wizard",
		"warrior",
		"harpy",
		"amazon",
		"dwarf",
		"monkey"};
	for (int i = 0; i < 7; i++)
	{
		if (i == 0)
		{
			string toInsert = "*";
			toInsert += roles[i];
			mvprintw(5 + i, COLS / 2 - 21, toInsert.c_str()); //default select
		}
		else
			mvprintw(5 + i, COLS / 2 - 20, roles[i].c_str());
	}
	int ch;
	int postion = 0;
	keypad(stdscr, TRUE);
	bool selected = false;
	curs_set(0);
	while (!selected)
	{
		ch = getch();
		string toInsert = "*";
		switch (ch)
		{
		case KEY_UP:
			move(5 + postion, 0);
			clrtoeol();
			mvprintw(5 + postion, COLS / 2 - 20, roles[postion].c_str());
			if (postion != 0)
				postion--;
			toInsert += roles[postion];
			mvprintw(5 + postion, COLS / 2 - 21, toInsert.c_str());
			refresh();
			break;
		case KEY_DOWN:
			move(5 + postion, 0);
			clrtoeol();
			mvprintw(5 + postion, COLS / 2 - 20, roles[postion].c_str());
			if (postion != 6)
				postion++;
			toInsert += roles[postion];
			mvprintw(5 + postion, COLS / 2 - 21, toInsert.c_str());
			refresh();
			break;
		case 13: //enter
			selected = true;
			break;
		default:
			break;
		}
	}
	player = std::make_shared<Player>(Player({0, 0}, name, static_cast<Role>(postion)));
	delete[] name;
	move(2, 0);
	clrtobot();
	refresh();
	drawMain();
	drawMap();
	WINDOW **menu = nullptr;
	bool menuEnable[7] = { false };
	MenuType menuChoose = MenuType::Attack;
	while (true)
	{
		ch = getch();
		switch (ch)
		{
		case 'M':
		case 'm':
			menuEnable[static_cast<int>(MenuType::Investigation)] = 
			menu = drawMenu();
			menuChoose = scrollMenu(menu, 7);
			deleteMenu(menu, 8);
		default:
			break;
		}
	}
}
/*
	Investigation
	Attack
	Control
	Pick up
	Backpack
	Help
	Exit
*/
WINDOW **Game::drawMenu()
{
	const char *muneStr[]{
		"Investigation",
		"Attack",
		"Control",
		"Pick up",
		"Backpack",
		"Help",
		"Exit"};
	int i;
	WINDOW **items;
	items = (WINDOW **)malloc(8 * sizeof(WINDOW *));

	items[0] = newwin(9, 15, 3, 83);
	wborder(items[0], '|', '|', '-', '-', '+', '+', '+', '+');
	items[1] = subwin(items[0], 1, 13, 4, 84);
	items[2] = subwin(items[0], 1, 13, 5, 84);
	items[3] = subwin(items[0], 1, 13, 6, 84);
	items[4] = subwin(items[0], 1, 13, 7, 84);
	items[5] = subwin(items[0], 1, 13, 8, 84);
	items[6] = subwin(items[0], 1, 13, 9, 84);
	items[7] = subwin(items[0], 1, 13, 10, 84);
	for (i = 0; i < 7; i++)
		wprintw(items[i + 1], muneStr[i]);
	wbkgd(items[1], COLOR_PAIR(2));
	wrefresh(items[0]);
	return items;
}
MenuType Game::scrollMenu(WINDOW **items, int count)
{
	int key;
	int selected = 0;
	while (1)
	{
		key = getch();
		if (key == KEY_DOWN || key == KEY_UP)
		{
			wbkgd(items[selected + 1], COLOR_PAIR(1));
			wnoutrefresh(items[selected + 1]);
			if (key == KEY_DOWN)
			{
				selected = (selected + 1) % count;
			}
			else
			{
				selected = (selected + count - 1) % count;
			}
			wbkgd(items[selected + 1], COLOR_PAIR(2));
			wnoutrefresh(items[selected + 1]);
			doupdate();
		}
		else if (key == 13)
		{
			return MenuType(selected);
		}
		else {
			return MenuType::Close;
		}
	}
}
void Game::deleteMenu(WINDOW **items, int count)
{
	int i;
	for (i = 0; i < count; i++)
		delwin(items[i]);
	free(items);
	touchwin(stdscr);
	refresh();
}
void Game::drawMain()
{
	resize_term(34, 100);
	clrtobot();
	//bkgd(COLOR_PAIR(1));
	menubar = subwin(stdscr, 1, 100, 0, 0);
	//wbkgd(menubar, COLOR_PAIR(4));
	map = subwin(stdscr, 11, 20, 2, 1);
	//wbkgd(map, COLOR_PAIR(3));
	status = subwin(stdscr, 11, 77, 2, 22);
	//wbkgd(status, COLOR_PAIR(5));
	info = subwin(stdscr, 19, 98, 14, 1);
	wborder(map, '|', '|', '-', '-', '+', '+', '+', '+');
	wborder(status, '|', '|', '-', '-', '+', '+', '+', '+');
	wborder(info, '|', '|', '-', '-', '+', '+', '+', '+');
	//wbkgd(info, COLOR_PAIR(5));
	refresh();
}
bool Game::canInvestigation()
{
	auto playerPosition = player->position;
	return false;
}
bool Game::canAttack()
{
	return false;
}
bool Game::canPickUp()
{
	return false;
}
bool Game::canControl()
{
	return false;
}
void Game::addInfo(const char *message)
{
	auto messageStr = string(message);
	while (messageStr.length() % 96 != 0)
	{
		messageStr += " ";
	}
	for (int i = 0; i < messageStr.length() / 96; i++)
	{
		infoList[header++] = messageStr.substr(i * 96, 96);
		header %= 17;
	}
	for (int i = 0; i < 17; i++)
	{
		wmove(info, i + 1, 1);
		waddstr(info, infoList[(i + header) % 17].c_str());
	}
	wrefresh(info);
}

void Game::drawMap()
{
	auto item = std::vector<gameObject *>{
		new Bottle(BottleType::bloodBottle, 10),
		new Bottle(BottleType::bloodBottle, 10),
		new Bottle(BottleType::bloodBottle, 10),
	};
	globalMap->distributeThings(item);
	auto mapStr = globalMap->drawablemap();
	auto gates = globalMap->getGates();
	for (int i = 0; i < 9; i++)
	{
		wmove(map, i + 1, 1);
		waddstr(map, mapStr[i].c_str());
	}
	if (gates[0])
	{
		wmove(map, 0, 9);
		waddstr(map, "nn");
	}
	if (gates[1])
	{
		wmove(map, 10, 9);
		waddstr(map, "nn");
	}
	if (gates[2])
	{
		wmove(map, 5, 0);
		waddstr(map, "n");
	}
	if (gates[3])
	{
		wmove(map, 5, 19);
		waddstr(map, "n");
	}
	wrefresh(map);
}
void Game::nextRound()
{
	for (auto &charac : characters)
	{
		for (int i = 4; i > 0; i--)
		{
			if (charac->move(static_cast<MoveDirection>(std::rand() % 4)))
				i++; //move failed, try again;
		}
		if (charac->attitude == attitudes::agressive || charac->beAttacked == true)
		{ //attack randomly
		}
		if (charac->bePoisoned > 0)
		{
			charac->health -= charac->bePoisoned * 2; //health loss of ponison
			charac->bePoisoned--;
		}
		if (auto monsterChar = dynamic_cast<Monster *>(charac); monsterChar != nullptr)
		{
			//if it is monster, rest controlled rounds decrease
			monsterChar->beControlled--;
		}
	}
	auto sameRoomCharacters = globalMap->getSameRoomObjectList(); //all objects in same room
	for (auto &obj : sameRoomCharacters)
	{
		if (obj->getType() == ObjectType::creature)
		{
			if (auto creatureObj = dynamic_cast<Creature *>(obj); creatureObj->name != player->name)
			{
				//temp function to calculate distance
				auto calDistance = [](std::pair<int, int> postionA, std::pair<int, int> postionB) {
					auto sumX = powf(postionA.first - postionB.first, 2);
					auto sumY = powf(postionA.second - postionB.second, 2);
					return sqrt(sumX + sumY);
				};
				for (int i = 4; i > 0; i--)
				{
					if (creatureObj->move(static_cast<MoveDirection>(std::rand() % 4)))
						i++; //move failed, try again;
				}
				if (creatureObj->attitude == attitudes::agressive || creatureObj->beAttacked == true)
				{ //attack randomly
					if (calDistance(creatureObj->position, player->position) == 1)
					{ //can attack player
						creatureObj->attack(*player);
					}
				}
				//NPC pick item
				if (auto mankindObj = dynamic_cast<Mankind *>(creatureObj); mankindObj != nullptr)
				{
					if (globalMap->getLocationType(creatureObj->position.first + 1, creatureObj->position.second) == ObjectType::item)
					{
						mankindObj->pick(*globalMap->getLocationItem(creatureObj->position.first + 1, creatureObj->position.second));
						globalMap->eraseGameObjectAt(creatureObj->position.first + 1, creatureObj->position.second);
					}
					else if (globalMap->getLocationType(creatureObj->position.first - 1, creatureObj->position.second) == ObjectType::item)
					{
						mankindObj->pick(*globalMap->getLocationItem(creatureObj->position.first - 1, creatureObj->position.second));
						globalMap->eraseGameObjectAt(creatureObj->position.first - 1, creatureObj->position.second);
					}
					else if (globalMap->getLocationType(creatureObj->position.first, creatureObj->position.second + 1) == ObjectType::item)
					{
						mankindObj->pick(*globalMap->getLocationItem(creatureObj->position.first, creatureObj->position.second + 1));
						globalMap->eraseGameObjectAt(creatureObj->position.first, creatureObj->position.second + 1);
					}
					else if (globalMap->getLocationType(creatureObj->position.first, creatureObj->position.second - 1) == ObjectType::item)
					{
						mankindObj->pick(*globalMap->getLocationItem(creatureObj->position.first, creatureObj->position.second - 1));
						globalMap->eraseGameObjectAt(creatureObj->position.first, creatureObj->position.second - 1);
					}
				}
			}
		}
	}
}