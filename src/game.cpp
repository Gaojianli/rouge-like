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
}