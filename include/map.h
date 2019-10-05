#include <iostream>
#include <vector>
#include <string>
#include "gameObject.h"
#include "creature.h"
#include "item.h"
class Map {
private:
	std::vector<std::vector<gameObject*>> mapcontent; // A 9*9 map of items and monsters
	bool enable_gate[4] = { true,true,true,true }; // Flags of gate, 0: Gate up, 1: Gate down, 2: Gate left, 3: Gate right
	int portal_stat = 0; // 0 for disable,1 for top left, 2 for top right, etc
public:
	Map() = default;
	void setPortal(int enable_portal); 
	void setDisabledGate(int disabled_gate);
	bool * getGates();
	int getPortal();
	void distributeThings(const std::vector<gameObject*> & items);
	void eraseGameObjectat(int x, int y);
	std::vector<std::string> drawablemap();
	void setGameObjectat(int x, int y,gameObject * gameobj);
	bool isOutOfRange(int x, int y);
	ObjectType getLoactionInfo(int x, int y);
	Creature * getLocationCreature(int x, int y);
	Item * getLocationItem(int x, int y);
	std::string getLocationInfo(int x, int y);
	void interactiveLocation(int x, int y);
};