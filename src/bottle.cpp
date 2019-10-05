#include "bottle.h"
<<<<<<< HEAD
#include "game.h"
extern std::shared_ptr<Game> game;
=======
Bottle::Bottle() {
	int type = (std::rand() % 3);
	switch (type)
	{
	case 0: {
		this->type = BottleType::bloodBottle;
		this->increased = (std::rand() % 20 + 20);
		break;
	}
	case 1: {
		this->type = BottleType::manaBottle;
		this->increased = std::rand() % 30 + 50;
		break;
	}
	case 2: {
		this->type = BottleType::poison;
		this->increased = std::rand() % 3 + 3;
		break;
	}
	default:
		break;
	}
}
>>>>>>> df9e42076d81c7fa4a34c8ca7aa2b804c61eba85
Bottle::Bottle(BottleType type, unsigned increased) {
	this->increased = increased;
	this->type = type;
}
ItemType Bottle::getItemType() {
	return ItemType::bottle;
}
const char* Bottle::getInfo() {
	std::string describe = "";
	switch (this->type)
	{
	case BottleType::bloodBottle:
		describe += "A bottle full of blood.You can use it to restore ";
		describe += this->increased;
		describe += " health.";
		break;
	case BottleType::manaBottle:
		describe += "A bottle full of mana.You can use it to restore ";
		describe += this->increased;
		describe += " mana.";
	case BottleType::poison:
		describe += "A bottle full of poison.You can use it to "; 
		describe += "enchant the target to poison for ";
		describe += this->increased;
		describe += " rounds.";
	default:
		break;
	}
	return describe.c_str();
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
					target.beAttacked = true;
				default:
					return false;
				}
			}
		}catch (std::exception& e) {
			game->addInfo(e.what());
			return false;
		}
	}
}