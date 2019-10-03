#include "monster.h"
#include "weapons.h"
bool Monster::attack(Creature& beAttack)
{
	beAttack.beAttacked = true;
	int attackSum = power;
	//calculate defends sum
	int defenseSum = beAttack.power;
	try {
		auto beAttackMankind = dynamic_cast<Mankind&>(beAttack);//attack monster
		for (auto &i : beAttackMankind.backpack) {
			if (i.getItemType() == ItemType::weapons) {
				defenseSum += static_cast<Weapons&>(i).defense;
			}
		}
	}
	catch (std::bad_cast) {
		defenseSum = beAttack.power;
	}
	if (attackSum < defenseSum)
		return false;//nothing happend;
	else
		beAttack.health -= attackSum - defenseSum;
	if (beAttack.health < 0)
		beAttack.died();
	return true;
}