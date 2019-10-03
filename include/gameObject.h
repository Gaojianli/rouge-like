enum class ObjectType {
	creature,
	item
};
class gameObject {
protected:
	gameObject() = default;
	virtual ObjectType getType();
	virtual const char* getInfo();
	virtual void interactiveThis();
};