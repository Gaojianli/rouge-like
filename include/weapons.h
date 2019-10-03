#include "item.h"
enum class Weapons
{
	cane,
	sword,
	shield
};
class Weapons : public Item {
public:
	Weapons type;
	int attack;
	int defense;
	unsigned power;
	Weapons(Weapons type, int attack, int defense, unsigned power);
};