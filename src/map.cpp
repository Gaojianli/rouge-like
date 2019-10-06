#include "map.h"
#include <vector>
#include <string>
#include <algorithm>
#include "gameObject.h"

Map::Map()
{
	for (int i = 0; i < 9; i++)
	{
		std::vector<gameObject *> thisrow;
		for (int j = 0; j < 9; j++)
		{
			thisrow.push_back(nullptr);
		}
		mapcontent.push_back(thisrow);
	}
}
void Map::setPortal(int enable_portal)
{ // Use switch to avoid illegal value
	switch (enable_portal)
	{
	case 1:
	{
		portal_stat = 1;
	}
	case 2:
	{
		portal_stat = 2;
	}
	case 3:
	{
		portal_stat = 3;
	}
	case 4:
	{
		portal_stat = 4;
	}
	default:
		break;
	}
}
void Map::setDisabledGate(int disabled_gate)
{
	switch (disabled_gate)
	{
	case 0:
	{
		enable_gate[0] = false;
	}
	case 1:
	{
		enable_gate[1] = false;
	}
	case 2:
	{
		enable_gate[2] = false;
	}
	case 3:
	{
		enable_gate[3] = false;
	}
	default:
		break;
	}
}
bool *Map::getGates()
{
	return enable_gate;
}
int Map::getPortal()
{
	return 0;
	//return enable_portal;
}
std::vector<gameObject *> &Map::getSameRoomObjectList()
{
	return objectlist;
}
void Map::distributeThings(const std::vector<gameObject *> &items)
{
	for (int i = 0; i < items.size(); i++)
	{
		int rand_x = (std::rand() % 9), rand_y = (std::rand() % 9);
		if (nullptr == ((mapcontent[rand_y])[rand_x]))
		{
			((mapcontent[rand_y])[rand_x]) = items[i];
			objectlist.push_back(items[i]);
			if (ObjectType::creature == items[i]->getType())
			{
				Creature *cret = static_cast<Creature *>(items[i]);
				cret->setPosition(rand_y, rand_x);
			}
		}
		else
		{
			i--;
		}
	}
}
void Map::randomSetThings(gameObject* item) {
	while (true) {
		int rand_x = (std::rand() % 9), rand_y = (std::rand() % 9);
		if (nullptr == ((mapcontent[rand_y])[rand_x]))
		{
			((mapcontent[rand_y])[rand_x]) = item;
			objectlist.push_back(item);
			if (ObjectType::creature == item->getType())
			{
				Creature* cret = static_cast<Creature*>(item);
				cret->setPosition(rand_y, rand_x);
			}
			break;
		}
	}
}
void Map::eraseGameObjectAt(int x, int y)
{
	if (nullptr == (mapcontent[y])[x])
	{
		objectlist.erase(std::find(objectlist.begin(), objectlist.end(), (mapcontent[y])[x]));
	}
	(mapcontent[y])[x] = nullptr;
}
std::vector<std::string> Map::drawablemap()
{
	std::vector<std::string> drawable;
	for (auto &i : mapcontent)
	{
		std::string thisLine;
		for (auto &j : i)
		{
			if (j == nullptr)
			{
				thisLine += "  ";
			}
			else if (ObjectType::creature == j->getType())
			{
				thisLine += "()";
			}
			else if (ObjectType::item == j->getType())
			{
				thisLine += "<>";
			}
		}
		drawable.push_back(thisLine);
	}
	std::reverse(drawable.begin(),drawable.end());
	return drawable;
}
void Map::setGameObjectat(int x, int y, gameObject *gameobj)
{
	objectlist.push_back(gameobj);
	(mapcontent[y])[x] = gameobj;
}

bool Map::isOutOfRange(int x, int y)
{
	if (x < 0 || x > 8 || y < 0 || y > 8)
	{
		return true;
	}
	else
	{
		return false;
	}
}
ObjectType Map::getLocationType(int x, int y)
{
	if (nullptr == (mapcontent[y])[x])
	{
		return ObjectType::nothing;
	}
	else
	{
		return (mapcontent[y])[x]->getType();
	}
}
Creature *Map::getLocationCreature(int x, int y)
{
	return dynamic_cast<Creature *>(((mapcontent[y])[x]));
}
Item *Map::getLocationItem(int x, int y)
{
	return dynamic_cast<Item *>(((mapcontent[y])[x]));
}
std::string Map::getLocationInfo(int x, int y)
{
	if (nullptr == (mapcontent[y])[x])
	{
		return "Nothing special there";
	}
	return std::string(((mapcontent[y])[x])->getInfo());
}
void Map::interactiveLocation(int x, int y)
{
	if (nullptr == (mapcontent[y])[x])
	{
	}
	else
	{
		((mapcontent[y])[x])->interactiveThis();
	}
}