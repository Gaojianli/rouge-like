#include "game.h"
#include "bottle.h"
#include "monster.h"
#include "mankind.h"
#include "mainmap.h"
#include "weapons.h"
#include "curses/curses.h"
#include <cmath>
#include <comutil.h>
using std::string;
#define COLOR_NORMAL COLOR_PAIR(1)
#define COLOR_SELECTED COLOR_PAIR(2)
#define COLOR_INVALID COLOR_PAIR(3)

std::string ws2s(const std::wstring &ws)
{
	_bstr_t t = ws.c_str();
	char *pchar = (char *)t;
	string result = pchar;
	return result;
}
std::wstring s2ws(const string &s)
{
	_bstr_t t = s.c_str();
	wchar_t *pwchar = (wchar_t *)t;
	std::wstring result = pwchar;
	return result;
}

void Game::createPlayer()
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
	if (strlen(name) == 0)
	{
		move(LINES / 2 - 2, COLS / 2 - 4);
		goto inputName; //reinput
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
		"monkey"};
	for (int i = 0; i < 7; i++)
	{
		if (i == 0)
		{
			string toInsert = "*";
			toInsert += roles[i];
			mvprintw(6 + i, COLS / 2 - 16, toInsert.c_str()); //default select
		}
		else
			mvprintw(6 + i, COLS / 2 - 15, roles[i].c_str());
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
			move(6 + postion, 0);
			clrtoeol();
			mvprintw(6 + postion, COLS / 2 - 15, roles[postion].c_str());
			if (postion != 0)
				postion--;
			toInsert += roles[postion];
			mvprintw(6 + postion, COLS / 2 - 16, toInsert.c_str());
			refresh();
			break;
		case KEY_DOWN:
			move(6 + postion, 0);
			clrtoeol();
			mvprintw(6 + postion, COLS / 2 - 15, roles[postion].c_str());
			if (postion != 6)
				postion++;
			toInsert += roles[postion];
			mvprintw(6 + postion, COLS / 2 - 16, toInsert.c_str());
			refresh();
			break;
		case 13: //enter
			selected = true;
			break;
		default:
			break;
		}
	}
	player = std::make_shared<Player>(Player({rand() % 9, rand() % 9}, name, static_cast<Role>(postion)));
	delete[] name;
}

void showItemInWin(WINDOW *win, Item *item)
{
	if (item == nullptr)
	{
		mvwaddstr(win, 0, 0, "       ");
		mvwaddstr(win, 1, 0, " Empty ");
		mvwaddstr(win, 2, 0, "       ");
		mvwaddstr(win, 3, 0, "       ");
		return;
	}
	switch (item->getItemType())
	{
	case ItemType::bottle:
		switch (dynamic_cast<Bottle *>(item)->type)
		{
		case BottleType::bloodBottle:
			mvwaddstr(win, 0, 0, "Blood  ");
			mvwaddstr(win, 1, 0, " Bottle");
			mvwaddstr(win, 2, 0, "       ");
			mvwprintw(win, 3, 0, "HP: %d", dynamic_cast<Bottle *>(item)->increased);
			break;
		case BottleType::manaBottle:
			mvwaddstr(win, 0, 0, "Mana   ");
			mvwaddstr(win, 1, 0, " Bottle");
			mvwaddstr(win, 2, 0, "       ");
			mvwprintw(win, 3, 0, "MP: %d", dynamic_cast<Bottle *>(item)->increased);
			break;
		case BottleType::poison:
			mvwaddstr(win, 0, 0, "Poison ");
			mvwaddstr(win, 1, 0, " Bottle");
			mvwaddstr(win, 2, 0, "       ");
			mvwprintw(win, 3, 0, "R: %d", dynamic_cast<Bottle *>(item)->increased);
			break;
		default:
			break;
		}
		break;
	case ItemType::key:
		switch (dynamic_cast<Key *>(item)->direction)
		{
		case Directions::up:
			mvwaddstr(win, 0, 0, "Key to ");
			mvwaddstr(win, 1, 0, "   up  ");
			mvwprintw(win, 2, 0, "step: %d", dynamic_cast<Key *>(item)->step);
			mvwprintw(win, 3, 0, "Used: %s", dynamic_cast<Key *>(item)->used ? "Y" : "N");
			break;
		case Directions::down:
			mvwaddstr(win, 0, 0, "Key to ");
			mvwaddstr(win, 1, 0, " down  ");
			mvwprintw(win, 2, 0, "step: %d", dynamic_cast<Key *>(item)->step);
			mvwprintw(win, 3, 0, "Used: %s", dynamic_cast<Key *>(item)->used ? "Y" : "N");
			break;
		case Directions::left:
			mvwaddstr(win, 0, 0, "Key to ");
			mvwaddstr(win, 1, 0, " left  ");
			mvwprintw(win, 2, 0, "step: %d", dynamic_cast<Key *>(item)->step);
			mvwprintw(win, 3, 0, "Used: %s", dynamic_cast<Key *>(item)->used ? "Y" : "N");
			break;
		case Directions::right:
			mvwaddstr(win, 0, 0, "Key to ");
			mvwaddstr(win, 1, 0, " right ");
			mvwprintw(win, 2, 0, "step: %d", dynamic_cast<Key *>(item)->step);
			mvwprintw(win, 3, 0, "Used: %s", dynamic_cast<Key *>(item)->used ? "Y" : "N");
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
		switch (dynamic_cast<Weapons *>(item)->type)
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
		mvwprintw(win, 1, 0, "ATK:%d", dynamic_cast<Weapons *>(item)->attack);
		mvwprintw(win, 2, 0, "DEF:%d", dynamic_cast<Weapons *>(item)->defense);
		mvwprintw(win, 3, 0, "MP :%d", dynamic_cast<Weapons *>(item)->mana);
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
	if (backpackWin != nullptr)
	{
		deleteMenu(backpackWin, 9);
	}
	backpackWin = new WINDOW *[9];
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
	for (auto i : {1, 2, 3, 4})
		wborder(backpackWin[i], '|', '|', '-', '-', '+', '+', '+', '+');
	int count = 0;
	for (auto i : player->backpack)
		showItemInWin(backpackWin[5 + (count++)], i);
	for (; count < 4; count++)
		showItemInWin(backpackWin[5 + count], nullptr);
	wrefresh(backpackWin[0]);
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
	const char *roles[] = {
		"Magician",
		"Wizard",
		"Warrior",
		"Harpy",
		"Amazon",
		"Dwarf",
		"Monkey"};
	if (playerStatusWin != nullptr)
	{
		delwin(playerStatusWin);
		touchwin(stdscr);
		refresh();
	}
	playerStatusWin = subwin(status, 11, 18, 2, 81);
	wborder(playerStatusWin, '|', '|', '-', '-', '+', '+', '+', '+');
	mvwprintw(playerStatusWin, 2, 1, "Name: %s", player->name.c_str());
	mvwprintw(playerStatusWin, 3, 1, "Role: %s", roles[static_cast<int>(player->role)]);
	mvwprintw(playerStatusWin, 4, 1, "HP: %d/%d", player->health, player->healthUpper);
	mvwprintw(playerStatusWin, 5, 1, "MP: %d/%d", player->mana, player->getManaUpper());
	mvwprintw(playerStatusWin, 6, 1, "ATK/DEF: %d/%d", player->getAttack(), player->getDefense());
	mvwprintw(playerStatusWin, 7, 1, "Move Points: %d", player->movePoints);
	wrefresh(playerStatusWin);
}

void Game::drawPlayer()
{
	if (playerWin != nullptr)
	{
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
WINDOW **Game::drawMenu(bool *menuEnable)
{
	const char *muneStr[]{
		"Investigate",
		"Attack",
		"Conjure",
		"Pick up",
		"Backpack",
		"Next round",
		"Help",
		"Exit"};
	int i;
	WINDOW **items = new WINDOW *[9];

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
	for (i = 0; i < 8; i++)
	{
		wprintw(items[i + 1], muneStr[i]);
		if (!menuEnable[i])
			wbkgd(items[i + 1], COLOR_INVALID);
	}
	i = 0;
	while (!menuEnable[i++])
		;
	wbkgd(items[i], COLOR_SELECTED);
	wrefresh(items[0]);
	return items;
}

MenuType Game::scrollMenu(WINDOW **items, int count, bool *menuEnable)
{
	int key;
	int selected = 0;
	while (!menuEnable[selected++])
		;
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
				while (!menuEnable[(++selected) % count])
					;
			}
			else
			{
				selected += count;
				while (!menuEnable[(--selected) % count])
					;
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
		else
		{
			return MenuType::Close;
		}
	}
}

int Game::scrollBackpack()
{
	int key;
	int selected = 0;
	size_t count = player->backpack.size();
	wbkgd(backpackWin[selected + 5], COLOR_SELECTED);
	wnoutrefresh(backpackWin[selected + 5]);
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
		else
		{
			wbkgd(backpackWin[selected + 5], COLOR_NORMAL);
			return -1;
		}
	}
}

bool Game::useOrThrowBackpack(int backpackIndex)
{
	auto item = new WINDOW *[3];
	item[0] = newwin(4, 7, 3, 38);
	wborder(item[0], '|', '|', '-', '-', '+', '+', '+', '+');
	item[1] = subwin(item[0], 1, 5, 4, 39);
	item[2] = subwin(item[0], 1, 5, 5, 39);
	waddstr(item[1], "Use");
	waddstr(item[2], "Throw");

	wrefresh(item[0]);
	int selected = 0;
	bool canUse = true;
	auto playerItem = player->backpack[backpackIndex];
	if (
		playerItem->getItemType() == ItemType::weapons ||
		(playerItem->getItemType() == ItemType::key && dynamic_cast<Key *>(playerItem)->used))
	{
		wbkgd(item[1], COLOR_INVALID);
		wnoutrefresh(item[1]);
		wbkgd(item[2], COLOR_SELECTED);
		wnoutrefresh(item[2]);
		canUse = false;
		selected = 1;
	}
	else
	{
		wbkgd(item[1], COLOR_SELECTED);
		wnoutrefresh(item[1]);
	}
	int key;
	while (true)
	{
		key = getch();
		switch (key)
		{
		case KEY_UP:
		case KEY_DOWN:
			wbkgd(item[selected + 1], COLOR_NORMAL);
			wnoutrefresh(item[selected + 1]);
			if (canUse)
			{
				selected = (selected + 1) % 2;
			}
			wbkgd(item[selected + 1], COLOR_SELECTED);
			wnoutrefresh(item[selected + 1]);
			doupdate();
			break;
		case 13:
			deleteMenu(item, 3);
			if (selected == 0)
			{
				useItem(backpackIndex);
			}
			else
			{
				throwItem(backpackIndex);
			}
			drawBackPack();
			return true;
		default:
			deleteMenu(item, 3);
			return false;
		}
	}
}

Directions Game::scrollDirections(bool *directionsEnable)
{
	auto directionsWin = newwin(5, 10, 3, 38);
	auto direction = new WINDOW *[5];
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
	for (auto i : {Directions::up, Directions::down, Directions::left, Directions::right})
		if (!directionsEnable[static_cast<int>(i)])
			wbkgd(direction[static_cast<int>(i)], COLOR_INVALID);
	wbkgd(direction[static_cast<int>(Directions::win)], COLOR_SELECTED);
	wrefresh(directionsWin);
	int key;
	while (true)
	{
		key = getch();
		if (key == KEY_DOWN || key == KEY_UP || key == KEY_LEFT || key == KEY_RIGHT)
		{
			wbkgd(direction[static_cast<int>(selected)], COLOR_NORMAL);
			wnoutrefresh(direction[static_cast<int>(selected)]);
			switch (key)
			{
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
		else
		{
			deleteMenu(direction, 5);
			return Directions::win;
		}
	}
}

void Game::deleteMenu(WINDOW **items, int count)
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

void Game::addInfo(const char *message)
{
	auto messageStr = string(message);
	while (messageStr.length() % 96 != 0)
	{
		messageStr += " ";
	}
	for (size_t i = 0; i < messageStr.length() / 96; i++)
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

void Game::addInfo(const wchar_t *message)
{
	auto messageStr = std::wstring(message);
	while (messageStr.length() % 96 != 0)
	{
		messageStr += L" ";
	}
	for (size_t i = 0; i < messageStr.length() / 96; i++)
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

void Game::printHelp()
{
	addInfo("Legends:");
	addInfo("()   --Mankind");
	addInfo("{}   --Monster");
	addInfo("<>   --Items");
	addInfo("n/nn --Doors");
	addInfo("/\\   --Player");
	addInfo("X    --Magic tunnel");
	addInfo(" ");
	addInfo("Helps:");
	addInfo("w/a/s/d --Move player");
	addInfo("m       --Show/Hide menu");
}
