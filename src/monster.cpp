#include "monster.h"
bool Monster::attack(Creature& beAttack)
{
	beAttack.beAttacked = true;
	int attackSum = power;
	//calculate defends sum
	int defenseSum = beAttack.power;
	if (auto beAttackMankind = dynamic_cast<Mankind&>(beAttack); beAttackMankind != nullptr) {//attack monster
		for (auto i& : beAttackMankind.backpack) {
			if (i.getItemType() == ItemType::weapons) {
				defenseSum += static_cast<Weapons&>(i).defense;
			}
		}
	}
	if (attackSum < defenseSum)
		return false;//nothing happend;
	else
		beAttack.health -= attackSum - defenseSum;
	if (beAttack.health < 0)
		beAttack.died();
	return true;
}