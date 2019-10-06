#include "item.h"
ObjectType Item::getType()
{
	return ObjectType::item;
}

ItemType Item::getItemType()
{
	return ItemType();
}

std::string Item::getInfo()
{
	return std::string("Item");
}