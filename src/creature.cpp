#include "creature.h"
Creature::Creature(int x, int y, const char* name) :postion({ x,y }), name(name), beAttacked(false), bePoisoned(false)
{

}