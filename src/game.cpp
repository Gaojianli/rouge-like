#include "game.h"
#include "curses/curses.h"
using std::string;
void Game::init()
{
	initscr();
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	init_pair(2, COLOR_BLUE, COLOR_WHITE);
	init_pair(3, COLOR_WHITE, COLOR_RED);
	init_pair(4, COLOR_WHITE, COLOR_GREEN);
	init_pair(5, COLOR_WHITE, COLOR_YELLOW);
	curs_set(0);
	raw();
	noecho();
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
	curs_set(1);//show the curs
	echo();
	auto name = new char[20];
	flushinp();//flush the input
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
		"monkey"
	};
	for (int i = 0; i < 7; i++) {
		if (i == 0) {
			string toInsert = "*";
			toInsert += roles[i];
			mvprintw(5 + i, COLS / 2 - 21, toInsert.c_str());//default select
		}
		else
			mvprintw(5 + i, COLS / 2 - 20, roles[i].c_str());
	}
	int ch;
	int postion = 0;
	keypad(stdscr, TRUE);
	bool selected = false;
	curs_set(0);
	while (!selected) {
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
		case 13://enter
			selected = true;
			break;
		default:
			break;
		}
	}
	player = std::make_shared<Player>(Player({ 0,0 }, name, static_cast<Role>(postion)));
	delete[] name;
	move(2, 0);
	clrtobot();
	refresh();
	drawMain();
}

void Game::drawMain() {
	resize_term(34,100);
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

void Game::addInfo(const char* message)
{
	auto messageStr = string(message);
	while (messageStr.length() % 96 != 0)
	{
		messageStr += " ";
	}
	for (int i = 0; i < messageStr.length() / 96; i ++) {
		infoList[header++] = messageStr.substr(i * 96, 96);
		header %= 17;
	}
	for (int i = 0; i < 17; i++) {
		wmove(info, i + 1, 1);
		waddstr(info, infoList[(i + header) % 17].c_str());
	}
	wrefresh(info);
}

void Game::nextRound()
{

}