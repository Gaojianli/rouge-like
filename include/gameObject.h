#pragma once
enum class ObjectType {
	creature,
	item
};
class gameObject {
public:
	gameObject() = default;
	virtual ObjectType getType();
	virtual const char* getInfo();
	virtual void interactiveThis();
};