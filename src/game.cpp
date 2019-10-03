#include "game.h"
#include "curses/curses.h"
void Game::init()
{
	initscr();
	raw();
	noecho();
}

void Game::start()
{
	mvprintw(LINES / 2, COLS / 2 - 27, "Please input your name:");
	refresh();
	curs_set(1);//show the curs
	echo();
	auto name = new char[20];
	flushinp();//flush the input
	getnstr(name, 20);
	player = make_shared<Player>(Player({ 0,0 }, name, Role::wizard));
}