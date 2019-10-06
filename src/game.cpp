#include "game.h"
#include "bottle.h"
#include "monster.h"
#include "mankind.h"
#include "mainmap.h"
#include "weapons.h"
#include "curses/curses.h"
#include <cmath>
#include <functional>
#include <comutil.h>
using std::string;
string ws2s(const std::wstring& ws) {
	_bstr_t t = ws.c_str();
	char* pchar = (char*)t;
	string result = pchar;
	return result;
}
std::wstring s2ws(const string& s) {
	_bstr_t t = s.c_str();
	wchar_t* pwchar = (wchar_t*)t;
	std::wstring result = pwchar;
	return result;
}

#define COLOR_NORMAL COLOR_PAIR(1)
#define COLOR_SELECTED COLOR_PAIR(2)
#define COLOR_INVALID COLOR_PAIR(3)

void Game::init()
{
	initscr();
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK); // normal
	init_pair(2, COLOR_BLACK, COLOR_WHITE); // selected
	init_pair(3, COLOR_RED, COLOR_BLACK);   // invalid
	init_pair(4, COLOR_WHITE, COLOR_GREEN);
	init_pair(5, COLOR_WHITE, COLOR_YELLOW);
	curs_set(0);
	raw();
	noecho();
	// Generate items
	std::vector<gameObject*> itemToDistribute;
	for (int i = 0; i < std::rand() % 8 + 16; i++) // Add bottle to item list
	{
		itemToDistribute.push_back(new Bottle());
	}
	for (int j = 0; j < 5; j++) // Add keys to item list
	{
		itemToDistribute.push_back(new Key(static_cast<Directions>(j), 1));
	}
	for (int k = 0; k < 12; k++) // Add sword to item list
	{
		switch (std::rand() % 3)
		{
		case 0: {
			itemToDistribute.push_back(new Weapons(WeaponsType::sword, std::rand() % 5 + 5, 0, 0));
			break;
		}
		case 1: {
			itemToDistribute.push_back(new Weapons(WeaponsType::shield, 0, std::rand() % 3 + 5, 0));
			break;
		}
		case 2: {
			itemToDistribute.push_back(new Weapons(WeaponsType::cane, std::rand() % 3 + 3, 0, std::rand() % 20 + 20));
			break;
		}
		default:
			break;
		}
	}
	for (int l = 0; l < 3; l++) // Add Monster to item list
	{
		switch (std::rand() % 5)
		{
		case 0:
		{
			Monster* m = new Monster({ 0, 0 }, ("Slime" + std::to_string(l)).c_str(), MonsterType::slime, static_cast<attitudes>(std::rand() % 2));
			itemToDistribute.push_back(m);
			characters.push_back(m);
			break;
		}
		case 1:
		{
			Monster* m = new Monster({ 0, 0 }, ("Skeleton" + std::to_string(l)).c_str(), MonsterType::skeleton, static_cast<attitudes>(std::rand() % 2));
			itemToDistribute.push_back(m);
			characters.push_back(m);
			break;
		}
		case 2:
		{
			Monster* m = new Monster({ 0, 0 }, ("Dragon" + std::to_string(l)).c_str(), MonsterType::dragon, static_cast<attitudes>(std::rand() % 2));
			itemToDistribute.push_back(m);
			characters.push_back(m);
			break;
		}
		case 3:
		{
			Monster* m = new Monster({ 0, 0 }, ("Snake" + std::to_string(l)).c_str(), MonsterType::snake, static_cast<attitudes>(std::rand() % 2));
			itemToDistribute.push_back(m);
			characters.push_back(m);
			break;
		}
		case 4:
		{
			Monster* m = new Monster({ 0, 0 }, ("Tarrasque" + std::to_string(l)).c_str(), MonsterType::tarrasque, static_cast<attitudes>(std::rand() % 2));
			itemToDistribute.push_back(m);
			characters.push_back(m);
			break;
		}
		default:
			break;
		}
	}
	for (int m = 0; m < 3; m++) // Generate three human NPC
	{
		Mankind* p = new Mankind({ 0, 0 }, (std::string("Human ") + std::to_string(m)).c_str(), static_cast<Role>(std::rand() % 7), static_cast<attitudes>(std::rand() % 2));
		itemToDistribute.push_back(p);
		characters.push_back(p);
	}
	// Init Maps.
	globalMainMap = std::make_shared<MainMap>(MainMap());
	// Roll Map.
	globalMainMap->SetMapLocation(std::rand() % 4, std::rand() % 4);// Roll first rom
	globalMap = std::make_shared<Map>(globalMainMap->GetCurrentMap());

	// Send Items to Map

	for (auto i : itemToDistribute) {
		(globalMainMap->GetMapAt(std::rand() % 4, std::rand() % 4)).randomSetThings(i);
	}
}

void Game::start()
{
	resize_term(34, 100);
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
inputName:
	getnstr(name, 20);
	if (strlen(name) == 0) {
		move(LINES / 2 - 2, COLS / 2 - 4);
		goto inputName;//reinput
	}
	move(LINES / 2 - 2, 0);
	clrtobot();
	noecho();
	mvprintw(4, COLS / 2 - 27, "Please select a role:");
	const string roles[] = {
		"magician",
		"wizard",
		"warrior",
		"harpy",
		"amazon",
		"dwarf",
		"monkey" };
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
	player = std::make_shared<Player>(Player({ rand() % 9, rand() % 9 }, name, static_cast<Role>(postion)));
	delete[] name;
	move(2, 0);
	clrtobot();
	refresh();
	drawMain();
	printHelp();
	WINDOW** menu = nullptr;
	bool menuEnable[8] = { true,true,true,true,true,true,true,true };
	bool moveStatus = true;
	while (true)
	{
		drawMap();
		drawPlayer();
		drawPlayerStatus();
		drawBackPack();
		moveStatus = false;
		auto oldPosition = player->position;
		int backpackScroll = -1;
		ch = getch();
		switch (ch)
		{
		case 'M':
		case 'm':
			menuEnable[static_cast<int>(MenuType::Attack)] = isAround(ObjectType::creature);
			menuEnable[static_cast<int>(MenuType::PickUp)] = isAround(ObjectType::item);
			menuEnable[static_cast<int>(MenuType::Control)] = canControlAround();
			menuEnable[static_cast<int>(MenuType::Investigate)] = (menuEnable[static_cast<int>(MenuType::Attack)] || menuEnable[static_cast<int>(MenuType::PickUp)]);
			menu = drawMenu(menuEnable);
			switch (scrollMenu(menu, 8, menuEnable))
			{
			case MenuType::Backpack:
				drawBackPack();
				backpackScroll = scrollBackpack();
				break;
			case MenuType::PickUp:

				break;
			case MenuType::Investigate:
				investigate();
				break;
			case MenuType::Control:
			case MenuType::NextRound:
				nextRound();
				drawMap();
				break;
			case MenuType::Help:
				printHelp();
				break;
			case MenuType::Exit:
				exit(0);
				break;
			default:
				break;
			}
			deleteMenu(menu, 9);
			break;
		case 'W': // move
		case 'w':
			moveStatus = player->move(MoveDirection::up);
			break;
		case 'S':
		case 's':
			moveStatus = player->move(MoveDirection::down);
			break;
		case 'A':
		case 'a':
			moveStatus = player->move(MoveDirection::left);
			break;
		case 'D':
		case 'd':
			moveStatus = player->move(MoveDirection::right);
			break;
		default:
			break;
		}
		if (moveStatus) {
			auto x = globalMainMap->GetMapXLocation(), y = globalMainMap->GetMapYLocation();
			if (abs(oldPosition.first - player->position.first) > 1) {
				if (player->position.first == 0) {
					// right
					if (!globalMainMap->isOutOfRange(x + 1, y)) {
						globalMainMap->SetMapLocation(x + 1, y);
						globalMap = std::make_shared<Map>(globalMainMap->GetCurrentMap());
					}
				}
				else {
					// left
					if (!globalMainMap->isOutOfRange(x - 1, y)) {
						globalMainMap->SetMapLocation(x - 1, y);
						globalMap = std::make_shared<Map>(globalMainMap->GetCurrentMap());
					}
				}
			}
			else if (abs(oldPosition.second - player->position.second) > 1) {
				if (player->position.second == 0) {
					// up
					if (!globalMainMap->isOutOfRange(x, y + 1)) {
						globalMainMap->SetMapLocation(x, y + 1);
						globalMap = std::make_shared<Map>(globalMainMap->GetCurrentMap());
					}
				}
				else {
					// down
					if (!globalMainMap->isOutOfRange(x, y - 1)) {
						globalMainMap->SetMapLocation(x, y - 1);
						globalMap = std::make_shared<Map>(globalMainMap->GetCurrentMap());
					}
				}
			}
		}

	}
}
void showItemInWin(WINDOW* win, Item* item) {
	if (item == nullptr) {
		mvwaddstr(win, 0, 0, "       ");
		mvwaddstr(win, 1, 0, " Empty ");
		mvwaddstr(win, 2, 0, "       ");
		mvwaddstr(win, 3, 0, "       ");
		return;
	}
	switch (item->getItemType())
	{
	case ItemType::bottle:
		switch (dynamic_cast<Bottle*>(item)->type)
		{
		case BottleType::bloodBottle:
			mvwaddstr(win, 0, 0, "Blood  ");
			mvwaddstr(win, 1, 0, " Bottle");
			mvwaddstr(win, 2, 0, "       ");
			mvwprintw(win, 3, 0, "HP: %d", dynamic_cast<Bottle*>(item)->increased);
		case BottleType::manaBottle:
			mvwaddstr(win, 0, 0, "Mana   ");
			mvwaddstr(win, 1, 0, " Bottle");
			mvwaddstr(win, 2, 0, "       ");
			mvwprintw(win, 3, 0, "MP: %d", dynamic_cast<Bottle*>(item)->increased);
		case BottleType::poison:
			mvwaddstr(win, 0, 0, "Poison ");
			mvwaddstr(win, 1, 0, " Bottle");
			mvwaddstr(win, 2, 0, "       ");
			mvwprintw(win, 3, 0, "R: %d", dynamic_cast<Bottle*>(item)->increased);
		default:
			break;
		}
		break;
	case ItemType::key:
		switch (dynamic_cast<Key*>(item)->direction)
		{
		case Directions::up:
			mvwaddstr(win, 0, 0, "Key to ");
			mvwaddstr(win, 1, 0, "   up  ");
			mvwprintw(win, 2, 0, "step: %d", dynamic_cast<Key*>(item)->step);
			mvwprintw(win, 3, 0, "Used: %s", dynamic_cast<Key*>(item)->used ? "Y" : "N");
			break;
		case Directions::down:
			mvwaddstr(win, 0, 0, "Key to ");
			mvwaddstr(win, 1, 0, " down  ");
			mvwprintw(win, 2, 0, "step: %d", dynamic_cast<Key*>(item)->step);
			mvwprintw(win, 3, 0, "Used: %s", dynamic_cast<Key*>(item)->used ? "Y" : "N");
			break;
		case Directions::left:
			mvwaddstr(win, 0, 0, "Key to ");
			mvwaddstr(win, 1, 0, " left  ");
			mvwprintw(win, 2, 0, "step: %d", dynamic_cast<Key*>(item)->step);
			mvwprintw(win, 3, 0, "Used: %s", dynamic_cast<Key*>(item)->used ? "Y" : "N");
			break;
		case Directions::right:
			mvwaddstr(win, 0, 0, "Key to ");
			mvwaddstr(win, 1, 0, " right ");
			mvwprintw(win, 2, 0, "step: %d", dynamic_cast<Key*>(item)->step);
			mvwprintw(win, 3, 0, "Used: %s", dynamic_cast<Key*>(item)->used ? "Y" : "N");
			break;
		case Directions::win:
			mvwaddstr(win, 0, 0, "       ");
			mvwaddstr(win, 1, 0, "Key to ");
			mvwaddstr(win, 2, 0, "  WIN  ");
			mvwaddstr(win, 3, 0, "       ");
			break;
		default:
			break;
		}
		break;
	case ItemType::weapons:
		switch (dynamic_cast<Weapons*>(item)->type)
		{
		case WeaponsType::cane:
			mvwaddstr(win, 0, 0, "  Cane ");
			break;
		case WeaponsType::shield:
			mvwaddstr(win, 0, 0, "Shield ");
			break;
		case WeaponsType::sword:
			mvwaddstr(win, 0, 0, " Sword ");
			break;
		default:
			break;
		}
		mvwprintw(win, 1, 0, "ATK:%d", dynamic_cast<Weapons*>(item)->attack);
		mvwprintw(win, 2, 0, "DEF:%d", dynamic_cast<Weapons*>(item)->defense);
		mvwprintw(win, 3, 0, "MP :%d", dynamic_cast<Weapons*>(item)->mana);
		break;
	default:
		break;
	}
}
/*
	1 2
	3 4
*/
void Game::drawBackPack()
{
	if (backpackWin != nullptr) {
		deleteMenu(backpackWin, 9);
	}
	backpackWin = new WINDOW * [9];
	backpackWin[0] = subwin(status, 11, 17, 2, 65);
	wborder(backpackWin[0], '|', '|', '-', '-', '+', '+', '+', '+');
	backpackWin[1] = subwin(backpackWin[0], 6, 9, 2, 65);
	backpackWin[2] = subwin(backpackWin[0], 6, 9, 2, 73);
	backpackWin[3] = subwin(backpackWin[0], 6, 9, 7, 65);
	backpackWin[4] = subwin(backpackWin[0], 6, 9, 7, 73);

	backpackWin[5] = subwin(backpackWin[1], 4, 7, 3, 66);
	backpackWin[6] = subwin(backpackWin[2], 4, 7, 3, 74);
	backpackWin[7] = subwin(backpackWin[3], 4, 7, 8, 66);
	backpackWin[8] = subwin(backpackWin[4], 4, 7, 8, 74);
	for (auto i : { 1,2,3,4 }) wborder(backpackWin[i], '|', '|', '-', '-', '+', '+', '+', '+');
	int count = 0;
	for (auto i : player->backpack) showItemInWin(backpackWin[5 + (count++)], &i);
	for (; count < 4; count++) showItemInWin(backpackWin[5 + count], nullptr);
	wrefresh(backpackWin[0]);
}
void showItemInWin(WINDOW* win, Item& item) {
	switch (item.getItemType())
	{
	case ItemType::bottle:
		mvwaddstr(win, 0, 0, "");
		break;
	case ItemType::key:
		break;
	case ItemType::weapons:
		break;
	default:
		break;
	}
}
/*
	name: name
	role: role
	HP: health/healthUpper
	MP: mana/manaUpper
	ATK/DEF: getAttack/getDefense
	Move Points: movePoints
*/
void Game::drawPlayerStatus()
{
	const char* roles[] = {
		"Magician",
		"Wizard",
		"Warrior",
		"Harpy",
		"Amazon",
		"Dwarf",
		"Monkey" };
	if (playerStatusWin != nullptr) {
		delwin(playerStatusWin);
		touchwin(stdscr);
		refresh();
	}
	playerStatusWin = subwin(status, 11, 18, 2, 81);
	wborder(playerStatusWin, '|', '|', '-', '-', '+', '+', '+', '+');
	mvwprintw(playerStatusWin, 2, 1, "Name: %s", player->name.c_str());
	mvwprintw(playerStatusWin, 3, 1, "Role: %s", roles[static_cast<int>(player->role)]);
	mvwprintw(playerStatusWin, 4, 1, "HP: %d/%d", player->health, player->healthUpper);
	mvwprintw(playerStatusWin, 5, 1, "MP: %d/%d", player->mana, player->manaUpper);
	mvwprintw(playerStatusWin, 6, 1, "ATK/DEF: %d/%d", player->getAttack(), player->getDefense());
	mvwprintw(playerStatusWin, 7, 1, "Move Points: %d", player->movePoints);
	wrefresh(playerStatusWin);
}
void Game::drawPlayer()
{
	if (playerWin != nullptr) {
		delwin(playerWin);
		touchwin(stdscr);
		refresh();
	}
	playerWin = subwin(map, 1, 2, 11 - player->position.second, 2 + player->position.first * 2);
	wprintw(playerWin, "/\\");
	wrefresh(playerWin);
}

/*
	Investigate
	Attack
	Control
	Pick up
	Backpack
	Help
	Exit
*/
WINDOW** Game::drawMenu(bool* menuEnable)
{
	const char* muneStr[]{
		"Investigate",
		"Attack",
		"Conjure",
		"Pick up",
		"Backpack",
		"Next round",
		"Help",
		"Exit" };
	int i;
	WINDOW** items = new WINDOW * [9];

	items[0] = newwin(11, 16, 2, 22);
	wborder(items[0], '|', '|', '-', '-', '+', '+', '+', '+');
	items[1] = subwin(items[0], 1, 13, 3, 23);
	items[2] = subwin(items[0], 1, 13, 4, 23);
	items[3] = subwin(items[0], 1, 13, 5, 23);
	items[4] = subwin(items[0], 1, 13, 6, 23);
	items[5] = subwin(items[0], 1, 13, 7, 23);
	items[6] = subwin(items[0], 1, 13, 8, 23);
	items[7] = subwin(items[0], 1, 13, 9, 23);
	items[8] = subwin(items[0], 1, 13, 10, 23);
	for (i = 0; i < 8; i++) {
		wprintw(items[i + 1], muneStr[i]);
		if (!menuEnable[i]) wbkgd(items[i + 1], COLOR_INVALID);
	}
	i = 0;
	while (!menuEnable[i++]);
	wbkgd(items[i], COLOR_SELECTED);
	wrefresh(items[0]);
	return items;
}

MenuType Game::scrollMenu(WINDOW** items, int count, bool* menuEnable)
{
	int key;
	int selected = 0;
	while (!menuEnable[selected++]);
	selected -= 1;
	while (1)
	{
		key = getch();
		if (key == KEY_DOWN || key == KEY_UP)
		{
			wbkgd(items[selected + 1], COLOR_NORMAL);
			wnoutrefresh(items[selected + 1]);
			if (key == KEY_DOWN)
			{
				while (!menuEnable[(++selected) % count]);
			}
			else
			{
				selected += count;
				while (!menuEnable[(--selected) % count]);
			}
			selected %= count;
			wbkgd(items[selected + 1], COLOR_SELECTED);
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
int Game::scrollBackpack()
{
	int key;
	int selected = 0;
	int count = player->backpack.size();
	wbkgd(backpackWin[selected + 5], COLOR_SELECTED);
	while (1)
	{
		key = getch();
		if (key == KEY_DOWN || key == KEY_UP || key == KEY_LEFT || key == KEY_RIGHT)
		{
			wbkgd(backpackWin[selected + 5], COLOR_NORMAL);
			wnoutrefresh(backpackWin[selected + 5]);
			if (key == KEY_DOWN || key == KEY_UP)
			{
				selected = (selected + 2) % 4 < count ? (selected + 2) % 4 : selected;
			}
			else if (key == KEY_LEFT)
			{
				selected = (selected + 3) % 4 < count ? (selected + 3) % 4 : selected;
			}
			else
			{
				selected = (selected + 1) % 4 < count ? (selected + 1) % 4 : selected;
			}
			wbkgd(backpackWin[selected + 5], COLOR_SELECTED);
			wnoutrefresh(backpackWin[selected + 5]);
			doupdate();
		}
		else if (key == 13)
		{
			wbkgd(backpackWin[selected + 5], COLOR_NORMAL);
			return selected;
		}
		else {
			wbkgd(backpackWin[selected + 5], COLOR_NORMAL);
			return -1;
		}
	}
}
Directions Game::scrollDirections(bool* directionsEnable)
{
	auto directionsWin = newwin(5, 10, 3, 38);
	auto direction = new WINDOW * [5];
	wborder(directionsWin, '|', '|', '-', '-', '+', '+', '+', '+');
	direction[static_cast<int>(Directions::up)] = subwin(directionsWin, 1, 2, 4, 42);
	direction[static_cast<int>(Directions::down)] = subwin(directionsWin, 1, 2, 6, 42);
	direction[static_cast<int>(Directions::left)] = subwin(directionsWin, 1, 2, 5, 40);
	direction[static_cast<int>(Directions::right)] = subwin(directionsWin, 1, 2, 5, 44);
	direction[static_cast<int>(Directions::win)] = subwin(directionsWin, 1, 2, 5, 42);
	
	waddwstr(direction[static_cast<int>(Directions::up)], L"��");
	waddwstr(direction[static_cast<int>(Directions::down)], L"��");
	waddwstr(direction[static_cast<int>(Directions::left)], L"��");
	waddwstr(direction[static_cast<int>(Directions::right)], L"��");
	Directions selected = Directions::win;
	for (auto i : { Directions::up , Directions::down, Directions::left, Directions::right })
		if (!directionsEnable[static_cast<int>(i)]) wbkgd(direction[static_cast<int>(i)], COLOR_INVALID);
	wbkgd(direction[static_cast<int>(Directions::win)], COLOR_SELECTED);
	wrefresh(directionsWin);
	int key;
	while (true) {
		key = getch();
		if (key == KEY_DOWN || key == KEY_UP || key == KEY_LEFT || key == KEY_RIGHT)
		{
			wbkgd(direction[static_cast<int>(selected)], COLOR_NORMAL);
			wnoutrefresh(direction[static_cast<int>(selected)]);
			switch (key) {
			case KEY_UP:
				selected = directionsEnable[static_cast<int>(Directions::up)] ? Directions::up : selected;
				break;
			case KEY_DOWN:
				selected = directionsEnable[static_cast<int>(Directions::down)] ? Directions::down : selected;
				break;
			case KEY_LEFT:
				selected = directionsEnable[static_cast<int>(Directions::left)] ? Directions::left : selected;
				break;
			case KEY_RIGHT:
				selected = directionsEnable[static_cast<int>(Directions::right)] ? Directions::right : selected;
				break;
			}
			wbkgd(direction[static_cast<int>(selected)], COLOR_SELECTED);
			wnoutrefresh(direction[static_cast<int>(selected)]);
			doupdate();
		}
		else if (key == 13)
		{
			deleteMenu(direction, 5);
			return selected;
		}
		else {
			deleteMenu(direction, 5);
			return Directions::win;
		}
	}
}
void Game::deleteMenu(WINDOW** items, int count)
{
	int i;
	for (i = 0; i < count; i++)
		delwin(items[i]);
	delete[] items;
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

template<typename Callback>
bool _isAround(std::shared_ptr<Map> globalMap, std::shared_ptr<Player> player, Callback _callback) {
	const int directionTable[4][2] = { {1,0},{-1,0},{0,-1},{0,1} };
	auto playerPosition = player->position;
	auto x = playerPosition.first, y = playerPosition.second;
	auto flag = false;
	for (auto direction : directionTable) {
		if (!globalMap->isOutOfRange(x + direction[0], y + direction[1])) {
			_callback(x + direction[0], y + direction[1], flag);
		}
	}
	return flag;
}

bool Game::isAround(ObjectType target) {
	return _isAround(globalMap, player, [&](int x, int y, bool& flag) {
		if (globalMap->getLocationType(x, y) == target) {
			flag = true;
		}
		});
}

bool Game::canControlAround()
{
	return _isAround(globalMap, player, [&](int x, int y, bool& flag) {
		if (globalMap->getLocationType(x, y) == ObjectType::creature) {
			auto creature = globalMap->getLocationCreature(x, y);
			auto monster = dynamic_cast<Monster*>(creature);
			if (monster != nullptr && monster->beControlled == 0) {
				flag = true;
			}
		}
		});
}

void Game::addInfo(const char* message)
{
	auto messageStr = string(message);
	while (messageStr.length() % 96 != 0)
	{
		messageStr += " ";
	}
	for (int i = 0; i < messageStr.length() / 96; i++)
	{
		infoList[header++] = s2ws(messageStr.substr(i * 96, 96));
		header %= 17;
	}
	for (int i = 0; i < 17; i++)
	{
		wmove(info, i + 1, 1);
		waddwstr(info, infoList[(i + header) % 17].c_str());
	}
	wrefresh(info);
}

void Game::addInfo(const wchar_t* message)
{
	auto messageStr = std::wstring(message);
	while (messageStr.length() % 96 != 0)
	{
		messageStr += L" ";
	}
	for (int i = 0; i < messageStr.length() / 96; i++)
	{
		infoList[header++] = messageStr.substr(i * 96, 96);
		header %= 17;
	}
	for (int i = 0; i < 17; i++)
	{
		wmove(info, i + 1, 1);
		waddwstr(info, infoList[(i + header) % 17].c_str());
	}
	wrefresh(info);
}

void Game::drawMap()
{
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
	player->movePoints = 5;//reset move points

	//regenerate bottles
	globalMainMap->GetMapAt(rand() % 4, rand() % 4)
		.randomSetThings(new Bottle(static_cast<BottleType>(std::rand() % 3), 10));
	globalMainMap->GetMapAt(rand() % 4, rand() % 4)
		.randomSetThings(new Bottle(static_cast<BottleType>(std::rand() % 3), 10));

	for (auto& charac : characters)
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
		if (auto monsterChar = dynamic_cast<Monster*>(charac); monsterChar != nullptr)
		{
			//if it is monster, rest controlled rounds decrease
			monsterChar->beControlled--;
		}
	}
	auto sameRoomCharacters = globalMap->getSameRoomObjectList(); //all objects in same room
	for (auto& obj : sameRoomCharacters)
	{
		if (obj->getType() == ObjectType::creature)
		{
			if (auto creatureObj = dynamic_cast<Creature*>(obj); creatureObj->name != player->name)
			{
				//temp function to calculate distance
				auto calDistance = [](std::pair<int, int> postionA, std::pair<int, int> postionB) {
					auto sumX = powf(float(postionA.first - postionB.first), float(2.0));
					auto sumY = powf(float(postionA.second - postionB.second), float(2.0));
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
				if (auto mankindObj = dynamic_cast<Mankind*>(creatureObj); mankindObj != nullptr)
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

void Game::printHelp()
{
	addInfo("Legends:");
	addInfo("()   --Mankind");
	addInfo("{}   --Monster");
	addInfo("<>   --Items");
	addInfo("n/nn --Doors");
	addInfo("/\\   --Player");
	addInfo(" ");
	addInfo("Helps:");
	addInfo("w/a/s/d --Move player");
	addInfo("m       --Show/Hide menu");
}

void Game::investigate() {
	const int directionTable[4][2] = { {1,0},{-1,0},{0,-1},{0,1} };
	auto playerPosition = player->position;
	auto x = playerPosition.first, y = playerPosition.second;
	auto flag = false;
	for (auto direction : directionTable) {
		if (!globalMap->isOutOfRange(x + direction[0], y + direction[1])) {
			if (auto objectType = globalMap->getLocationType(x + direction[0], y + direction[1]); objectType != ObjectType::nothing) {
				gameObject* toInvestigate;
				if (objectType == ObjectType::creature)
					toInvestigate = globalMap->getLocationCreature(x + direction[0], y + direction[1]);
				else
					toInvestigate = globalMap->getLocationItem(x + direction[0], y + direction[1]);
				addInfo("Investigate result:");
				addInfo(toInvestigate->getInfo().c_str());
			}
		}
	}
}