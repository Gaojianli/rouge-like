#pragma once
enum class ObjectType {
	creature,
	item,
	nothing
};
class gameObject {
public:
	gameObject() = default;
	virtual ObjectType getType()=0;
	virtual const char* getInfo()=0;
	virtual void interactiveThis()=0;
};