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