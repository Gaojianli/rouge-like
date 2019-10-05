#include "key.h"
ItemType Key::getItemType() {
	return ItemType::key;
}
void Key::newRoundStarts() {
	this->used = false;
}
Key::Key(Directions direction, unsigned step) {
	this->direction = direction;
	this->step = step;
	this->used = false;
}
const char* Key::getInfo() {
	const char* ordinal[] = {
		"first",
		"secound",
		"third",
		"fourth",
		"fifth",
		"sixth",
		"seventh",
		"eighth",
		"ninth",
		"tenth"
	};
	const std::string directionsStr[] = {
		"up",
		"down",
		"right",
		"left"
	};
	std::string describe = "";
	if (this->direction == Directions::win) {
		describe += "This is the key to win.";
		return describe.c_str();
	}
	describe += "This is a key that can teleport to the ";
	describe += ordinal[this->step];
	describe += " room ";
	describe += directionsStr[static_cast<int>(this->direction)] + '.';
	return describe.c_str();
}
bool Key::useIt() {
	if (this->used == false) {
		this->used = true;
		return true;
	}
	else {
		return false;
	}
}