#include "map.h"
#include <vector>
#include <string>
#include "gameObject.h"
Map::Map() {
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
void Map::setPortal(int enable_portal) { // Use switch to avoid illegal value
	switch (enable_portal)
	{
	case 1: {
		portal_stat = 1;
	}
	case 2: {
		portal_stat = 2;
	}
	case 3: {
		portal_stat = 3;
	}
	case 4: {
		portal_stat = 4;
	}
	default:
		break;
	}
}
void Map::setDisabledGate(int disabled_gate) {
	switch (disabled_gate)
	{
	case 0: {
		enable_gate[0] = false;
	}
	case 1: {
		enable_gate[1] = false;
	}
	case 2: {
		enable_gate[2] = false;
	}
	case 3: {
		enable_gate[3] = false;
	}
	default:
		break;
	}
}
bool* Map::getGates() {
	return enable_gate;
}
int Map::getPortal() {
	return 0;
	//return enable_portal;
}
void Map::distributeThings(const std::vector<gameObject*>& items) {
	for (int i = 0; i < items.size(); i++) {
		int rand_x = (std::rand() % 9), rand_y = (std::rand() % 9);
		if (nullptr == ((mapcontent[rand_x])[rand_y])) {
			((mapcontent[rand_x])[rand_y]) = items[i];
			if(ObjectType::creature==items[i]->getType()){
				Creature* cret = static_cast<Creature*>(items[i]);
				cret->setPosition(rand_x, rand_y);
			}
		}
		else {
			i--;
		}
	}
}
void Map::eraseGameObjectat(int x, int y) {
	(mapcontent[x])[y] = nullptr;
}
std::vector<std::string> Map::drawablemap() {
	std::vector<std::string> drawable;
	for (auto& i : mapcontent)
	{
		std::string thisLine;
		for (auto& j : i) {
			if (j == nullptr) {
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
	return drawable;
}
void Map::setGameObjectat(int x, int y, gameObject* gameobj) {
	(mapcontent[x])[y] = gameobj;
}

bool Map::isOutOfRange(int x, int y)
{
	if (x < 0 || x > 8 || y < 0 || y > 8) {
		return true;
	}
	else
	{
		return false;
	}
}
ObjectType Map::getLoactionInfo(int x, int y) {
	if (nullptr == (mapcontent[x])[y]) {
		return ObjectType::nothing;
	}
	else {
		return (mapcontent[x])[y]->getType();
	}
}
Creature* Map::getLocationCreature(int x, int y) {
	return dynamic_cast<Creature*>(((mapcontent[x])[y]));
}
Item* Map::getLocationItem(int x, int y) {
	return dynamic_cast<Item*>(((mapcontent[x])[y]));
}
std::string Map::getLocationInfo(int x, int y) {
	if (nullptr == (mapcontent[x])[y]){
		return "Nothing special there";
}
	return std::string(((mapcontent[x])[y])->getInfo());
}
void Map::interactiveLocation(int x, int y){
	if (nullptr == (mapcontent[x])[y]) {}
	else {
		((mapcontent[x])[y])->interactiveThis();
	}
}