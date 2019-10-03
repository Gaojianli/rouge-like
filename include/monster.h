#include "creature.h"
#include "mankind.h"
#include <optional>
class Monster :Creature {
	int beControlled;//0 is not be controlled, other is the lasting time
	Mankind* owner;
};