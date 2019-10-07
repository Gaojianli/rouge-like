#include "bottle.h"
#include "game.h"

extern std::shared_ptr<Game> game;
Bottle::Bottle()
{
	switch (std::rand() % 3)
	{
	case 0:
	{
		this->type = BottleType::bloodBottle;
		this->increased = (std::rand() % 20 + 20);
		break;
	}
	case 1:
	{
		this->type = BottleType::manaBottle;
		this->increased = std::rand() % 30 + 50;
		break;
	}
	case 2:
	{
		this->type = BottleType::poison;
		this->increased = std::rand() % 3 + 3;
		break;
	}
	default:
		this->type = BottleType::bloodBottle;
		this->increased = std::rand() % 20 + 20;
		break;
	}
}

bool Bottle::use(Creature* target)
{
	try
	{

		switch (this->type)
		{
		case BottleType::bloodBottle:
			target->health += this->increased;
			target->health = target->health > int(target->healthUpper) ? target->healthUpper : target->health;
			return true;
		case BottleType::manaBottle:
			if (target->getCreatureType() == CreatureType::monster)
			{
				return false;
			}
			else
			{
				auto mankind = dynamic_cast<Mankind*>(target);
				mankind->mana += this->increased;
				mankind->mana = mankind->mana > mankind->getManaUpper() ? mankind->getManaUpper() : mankind->mana;
				return true;
			}
		case BottleType::poison:
			target->bePoisoned += this->increased;
			target->beAttacked = true;
			target->attitude = attitudes::agressive;
			return true;
			break;
		default:
			return false;
		}
	}
	catch (std::exception& e)
	{
		game->addInfo(e.what());
		return false;
	}
}
Bottle::Bottle(BottleType type, unsigned increased)
{
	this->increased = increased;
	this->type = type;
}
ItemType Bottle::getItemType()
{
	return ItemType::bottle;
}
std::string Bottle::getInfo()
{
	std::string describe = "";
	switch (this->type)
	{
	case BottleType::bloodBottle:
		describe += "A bottle full of blood.You can use it to restore ";
		describe += std::to_string(this->increased);
		describe += " health.";
		break;
	case BottleType::manaBottle:
		describe += "A bottle full of mana.You can use it to restore ";
		describe += std::to_string(this->increased);
		describe += " mana.";
		break;
	case BottleType::poison:
		describe += "A bottle full of poison.You can use it to ";
		describe += "enchant the target to poison for ";
		describe += std::to_string(this->increased);
		describe += " rounds.";
		break;
	default:
		break;
	}
	return describe;
}
