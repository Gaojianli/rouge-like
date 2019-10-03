#pragma once
enum class ObjectType {
	creature,
	item,
	nothing
};
class gameObject {
public:
	gameObject() = default;
	virtual ObjectType getType();
	virtual const char* getInfo();
	virtual void interactiveThis();
};