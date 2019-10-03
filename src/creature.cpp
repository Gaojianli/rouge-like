#include "creature.h"
Creature::Creature(int x, int y, const char* name,attitudes attitude) :postion({ x,y }), name(name), beAttacked(false), bePoisoned(false),attitude(attitude)
{

}