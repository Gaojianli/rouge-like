#include "mankind.h"
#include "weapons.h"
Mankind::Mankind(std::pair<int x,int y> postion,const char* name, Role role):Creature(postion.first,postion.second,name)
{
	switch (role)
	{
	case Role::magician:
		this->health = this->healthUpper = 114;
		this->mana = this->manaUpper = 514;
		this->power = 9;
		break;
	case Role::amazon:
		this->health = this->healthUpper = 514;
		this->mana = this->manaUpper = 0;
		this->power = 15;
		break;
	case Role::dwarf:
		this->health = this->healthUpper = 200;
		this->mana = this->manaUpper = 200;
		this->powerUpper = this->power = 10;
		break;
	case Role::harpy:
		this->health = this->healthUpper = 190;
		this->mana = this->manaUpper = 220;
		this->power = 12;
		break;
	case Role::monkey:
		this->health = this->healthUpper = 514;
		this->mana = this->manaUpper = 0;
		this->power = 1;
		break;
	case Role::warrior:
		this->health = this->healthUpper = 514;
		this->mana = this->manaUpper = 114;
		this->power = 10;
		break;
	}
}

//control a monster, return true if succeed
bool Mankind::conjure(Monster* object)
{
	if (this->mana > object->power) {//enough mana
		this->mana -= object->power;
		object->bePoisoned = 5;//control 5 rounds
	}
}

bool Mankind::attack(Creature& beAttack)
{
	beAttack.beAttacked = true;
	//calculate attack sum
	int attackSum = power;
	for (auto &i : backpack) {
		if (i.getItemType() == ItemType::weapons) {
			attackSum += static_cast<Weapons&>(i).attack;
		}
	}
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
