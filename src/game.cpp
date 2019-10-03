#include "..\include\game.h"
#include "curses/curses.h"
Game::Game() {

}

void Game::init()
{
	initscr();
	raw();
	noecho();
}

void Game::start()
{
}