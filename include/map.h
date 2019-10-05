#pragma once
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
	void setPortal(int enable_portal); // Enable certain portal, see argument usage above
	void setDisabledGate(int disabled_gate); // Disables certain gate, see args above
	bool * getGates(); // Get gates status
	int getPortal(); // Get portal_stat
	void distributeThings(const std::vector<gameObject*> & items); // Automatically distribute objects on the map
	void eraseGameObjectat(int x, int y); // Erase object on certain location of the map
	Item* pickUpObject(int x, int y); // Return a ptr of an object and remove it from the map
	bool moveObject(int src_x, int src_y, int dst_x, int dst_y); // Move object form (src_x,src_y) to (dst_x,dst_y), if (dst_x,dst_y) have item, return false and do nothing
	std::vector<std::string> drawablemap(); // Generate map for ui
	void setGameObjectat(int x, int y,gameObject * gameobj); // Set a game oject at certain location
	bool isOutOfRange(int x, int y); // Judge if a location is out of map
	ObjectType getLoactionInfo(int x, int y); // Get objecttype of certain location
	Creature * getLocationCreature(int x, int y); // If it's a creature , return it otherwise return a nullptr(dynamic cast)
	Item * getLocationItem(int x, int y); // same to above
	std::string getLocationInfo(int x, int y); // Return a string of object info, If nothing there, give out a "Nothing special there"
	void interactiveLocation(int x, int y); // Interactive with sth, silent fail
};