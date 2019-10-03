#include "bottle.h"
Bottle::Bottle(BottleType type, unsigned increased) {
	this->increased = increased;
	this->type = type;
}
ItemType Bottle::getItemType() {
	return ItemType::bottle;
}
template<typename T>
bool Bottle::use(T& target) {
	{
		try {
			if constexpr (std::is_same_v<Creature, std::decay_t<T>>) {
				switch (this->type)
				{
				case BottleType::bloodBottle:
					target.health += this->increased;
					target.health = target.health > target.healthUpper ? target.healthUpper : target.health;
					return true;
				case BottleType::manaBottle:
					if constexpr (std::is_same_v<Monster, std::decay_t<T>>) {
						return false;
					}
					else {
						target.mana += this->increased;
						target.mana = target.mana > target.manaUpper ? target.manaUpper : target.mana;
						return true;
					}
				case BottleType::poison:
					target.bePoisoned += this->increased;
				default:
					return false;
				}
			}
		}catch (std::exception& e) {
			cout << e.what() << endl;
			return false
		}
	}
}