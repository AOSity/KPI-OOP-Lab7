#pragma once
#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

class Block;

class Component {
public:
	virtual void setParent(Block* parent) = 0;
	virtual short updateWidth() = 0;
	virtual void updateAlign() = 0;
	virtual void render() const = 0;
	virtual Component* getCopy() = 0;
	virtual Block* getParent() = 0;
	virtual ~Component() {}
};

class Block : public Component {
private:
	Block* parent;
	vector<Component*> components;
	unsigned short alignMode;
	char borderChar;
	unsigned short borderWidth;
	unsigned short marginTop, marginRight, marginBot, marginLeft;
	unsigned short paddingTop, paddingRight, paddingBot, paddingLeft;
	char maskChar;
	unsigned short contentWidth;

	unsigned short alignLeft, alignRight;

public:
	friend class Line;

	Block();

	void setParent(Block* newParent);
	Block* getParent();

	void addComponent(Component* component);
	void removeComponent(Component* component);
	void replaceComponent(Component* target, Component* source);

	short updateWidth() override;
	void updateAlign();
	char findMask() const;

	void printBorder() const;
	void printLeftBorders() const;
	void printRightBorders() const;

	void render() const override;

	void setAlign(short mode);
	void setBorder(short width, char symbol);
	void setMargin(short top, short right, short bot, short left);
	void setPadding(short top, short right, short bot, short left);
	void setMask(char symbol);
	void setWidth(short width);

	short getWidth() const;
	short getBlockWidth() const;
	Block* getCopy();
	vector<Component*> getComponents();
};

class Line : public Component {
private:
	Block* parent;
	string line;
	unsigned short alignLeft, alignRight;
public:
	Line();
	Line(string text);

	void setParent(Block* newParent);

	short updateWidth() override;
	void updateAlign();
	char findMask() const;

	void render() const override;

	Line* getCopy();
	Block* getParent();
};

