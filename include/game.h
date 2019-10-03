#include <list>
#include "creature.h"
#include "map.h"
using namespace std;
class Game {
	list<Creature> characters;
	Creature player;
	Map globalMap;
	Game();
	void init();//create player
	void start();
};