#include "markup.h"

Block::Block()
{
	alignMode = 0;
	borderChar = ' ';
	borderWidth = 0;
	marginTop = 0;
	marginRight = 0;
	marginBot = 0;
	marginLeft = 0;
	paddingTop = 0;
	paddingRight = 0;
	paddingBot = 0;
	paddingLeft = 0;
	maskChar = ' ';
	contentWidth = 0;

	alignLeft = 0;
	alignRight = 0;

	parent = nullptr;
}

void Block::setParent(Block* newParent)
{
	parent = newParent;
}
Block* Block::getParent()
{
	return parent;
}

void Block::addComponent(Component* component) {
	components.push_back(component);
}
void Block::removeComponent(Component* component)
{
	auto it = find(components.begin(), components.end(), component);
	if (it != components.end())
	{
		components.erase(it);
	}
}
void Block::replaceComponent(Component* target, Component* source)
{
	auto it = find(components.begin(), components.end(), target);
	if (it != components.end())
	{
		components.emplace(it, source);
		removeComponent(target);
	}
}

short Block::updateWidth()
{
	short updatedWidth = contentWidth;
	short newWidth;
	for (const auto& component : components) {
		newWidth = component->updateWidth();
		if (newWidth > updatedWidth)
		{
			updatedWidth = newWidth;
		}
	}
	contentWidth = updatedWidth;
	return contentWidth + borderWidth * 2 + marginLeft + marginRight + paddingLeft + paddingRight;
}
void Block::updateAlign()
{
	if (parent != nullptr)
	{
		short parentBlockWidth = parent->contentWidth;
		short blockWidth = getBlockWidth();

		switch (parent->alignMode)
		{
		case 1: // left
			alignLeft = 0;
			alignRight = parentBlockWidth - blockWidth;
			break;
		case 2: // center
			alignLeft = alignRight = (parentBlockWidth - blockWidth) / 2;
			if ((parentBlockWidth - blockWidth) % 2 != 0)
			{
				alignLeft += 1;
			}
			break;
		case 3: // right
			alignLeft = parentBlockWidth - blockWidth;
			alignRight = 0;
			break;
		default:
			contentWidth = parent->contentWidth - borderWidth * 2 - paddingLeft - paddingRight - marginLeft - marginRight;
			blockWidth = getBlockWidth();
			alignLeft = 0;
			alignRight = parentBlockWidth - blockWidth;
			break;
		}
	}
	for (const auto& component : components)
	{
		component->updateAlign();
	}
}
char Block::findMask() const
{
	Block* parents = parent;
	char mask;
	while (parents != nullptr)
	{
		if (parents->maskChar != ' ')
			return parents->maskChar;
		parents = parents->parent;
	}
	return ' ';
}

void Block::printBorder() const
{
	char mask = findMask();
	if (mask == ' ')
	{
		for (size_t i = 0; i < borderWidth; i++)
		{
			cout << borderChar;
		}
	}
	else
	{
		for (size_t i = 0; i < borderWidth; i++)
		{
			cout << mask;
		}
	}

}
void Block::printLeftBorders() const
{
	if (parent != nullptr)
	{
		parent->printLeftBorders();
	}
	for (size_t i = 0; i < marginLeft; i++)
	{
		cout << ' ';
	}
	cout << setfill(' ') << setw(alignLeft) << "";
	printBorder();
	for (size_t i = 0; i < paddingLeft; i++)
	{
		cout << ' ';
	}
}
void Block::printRightBorders() const
{
	for (size_t i = 0; i < paddingRight; i++)
	{
		cout << ' ';
	}
	printBorder();
	cout << setfill(' ') << setw(alignRight) << "";
	for (size_t i = 0; i < marginRight; i++)
	{
		cout << ' ';
	}
	if (parent != nullptr)
	{
		parent->printRightBorders();
	}
}

void Block::render() const
{
	char mask = findMask();

	for (size_t i = 0; i < marginTop; i++)
	{
		if (parent->parent != nullptr)
		{
			parent->printLeftBorders();
			cout << setfill(' ') << setw(alignLeft + getBlockWidth() + alignRight) << "";
			parent->printRightBorders();
			cout << endl;
		}
		else
		{
			break;
		}
	}
	for (size_t i = 0; i < borderWidth; i++)
	{
		parent->printLeftBorders();
		cout << setfill(' ') << setw(alignLeft) << "";
		for (size_t j = 0; j < marginLeft; j++)
		{
			cout << ' ';
		}
		if (mask == ' ')
		{
			for (size_t j = 0; j < contentWidth + borderWidth * 2 + paddingLeft + paddingRight; j++)
			{
				cout << borderChar;
			}
		}
		else
		{
			for (size_t j = 0; j < contentWidth + borderWidth * 2 + paddingLeft + paddingRight; j++)
			{
				cout << mask;
			}
		}
		for (size_t j = 0; j < marginRight; j++)
		{
			cout << ' ';
		}
		cout << setfill(' ') << setw(alignRight) << "";
		parent->printRightBorders();
		cout << endl;
	}
	for (size_t i = 0; i < paddingTop; i++)
	{
		printLeftBorders();
		cout << setfill(' ') << setw(contentWidth) << "";
		printRightBorders();
		cout << endl;
	}

	for (const auto& component : components)
	{
		component->render();
	}

	for (size_t i = 0; i < paddingBot; i++)
	{
		printLeftBorders();
		cout << setfill(' ') << setw(contentWidth) << "";
		printRightBorders();
		cout << endl;
	}
	for (size_t i = 0; i < borderWidth; i++)
	{
		parent->printLeftBorders();
		cout << setfill(' ') << setw(alignLeft) << "";
		for (size_t j = 0; j < marginLeft; j++)
		{
			cout << ' ';
		}
		if (mask == ' ')
		{
			for (size_t j = 0; j < contentWidth + borderWidth * 2 + paddingLeft + paddingRight; j++)
			{
				cout << borderChar;
			}
		}
		else
		{
			for (size_t j = 0; j < contentWidth + borderWidth * 2 + paddingLeft + paddingRight; j++)
			{
				cout << mask;
			}
		}
		for (size_t j = 0; j < marginRight; j++)
		{
			cout << ' ';
		}
		cout << setfill(' ') << setw(alignRight) << "";
		parent->printRightBorders();
		cout << endl;
	}
	for (size_t i = 0; i < marginBot; i++)
	{
		if (parent->parent != nullptr)
		{
			parent->printLeftBorders();
			cout << setfill(' ') << setw(alignLeft + getBlockWidth() + alignRight) << "";
			parent->printRightBorders();
			cout << endl;
		}
		else
		{
			break;
		}
	}
}

void Block::setAlign(short mode)
{
	if (0 <= mode && mode <= 3)
		alignMode = mode;
}
void Block::setBorder(short width, char symbol)
{
	if (width >= 0)
		borderWidth = width;
	borderChar = symbol;
}
void Block::setMargin(short top, short right, short bot, short left)
{
	if (top >= 0)
		marginTop = top;
	if (right >= 0)
		marginRight = right;
	if (bot >= 0)
		marginBot = bot;
	if (left >= 0)
		marginLeft = left;
}
void Block::setPadding(short top, short right, short bot, short left)
{
	if (top >= 0)
		paddingTop = top;
	if (right >= 0)
		paddingRight = right;
	if (bot >= 0)
		paddingBot = bot;
	if (left >= 0)
		paddingLeft = left;
}
void Block::setMask(char symbol)
{
	maskChar = symbol;
}
void Block::setWidth(short width)
{
	contentWidth = width;
}
short Block::getWidth() const
{
	return contentWidth;
}
short Block::getBlockWidth() const
{
	return contentWidth + borderWidth * 2 + paddingLeft + paddingRight + marginLeft + marginRight;
}

Block* Block::getCopy()
{
	Block* rootCopy = new Block;
	rootCopy->setParent(nullptr);
	rootCopy->setAlign(alignMode);
	rootCopy->setBorder(borderWidth, borderChar);
	rootCopy->setMargin(marginTop, marginRight, marginBot, marginLeft);
	rootCopy->setPadding(paddingTop, paddingRight, paddingBot, paddingLeft);
	rootCopy->setMask(maskChar);
	rootCopy->setWidth(contentWidth);
	rootCopy->alignLeft = alignLeft;
	rootCopy->alignRight = alignRight;
	for (const auto& component : components)
	{
		Component* componentCopy = component->getCopy();
		componentCopy->setParent(rootCopy);
		rootCopy->addComponent(componentCopy);
	}
	return rootCopy;
}
vector<Component*> Block::getComponents()
{
	vector<Component*> allComponents;
	for (Component* component : components)
	{
		allComponents.push_back(component);
		if (typeid(*component) == typeid(Block))
		{

			vector<Component*> newComponents = ((Block*)component)->getComponents();
			for (Component* newComponent : newComponents)
			{
				allComponents.push_back(newComponent);
			}
		}
	}
	return allComponents;
}

Line::Line()
{
	parent = nullptr;
	alignLeft = 0;
	alignRight = 0;
}
Line::Line(string text)
{
	line = text;
	parent = nullptr;
	alignLeft = 0;
	alignRight = 0;
}

void Line::setParent(Block* newParent)
{
	parent = newParent;
}

short Line::updateWidth()
{
	return line.size();
}
void Line::updateAlign()
{
	switch (parent->alignMode)
	{
	case 1: // left
		alignLeft = 0;
		alignRight = parent->contentWidth - line.size();
		break;
	case 2: // center
		alignLeft = alignRight = (parent->contentWidth - line.size()) / 2;
		if ((parent->contentWidth - line.size()) % 2 != 0)
		{
			alignLeft += 1;
		}
		break;
	case 3: // right
		alignLeft = parent->contentWidth - line.size();
		alignRight = 0;
		break;
	default:
		alignLeft = 0;
		alignRight = parent->contentWidth - line.size();
		break;
	}
}

void Line::render() const
{
	parent->printLeftBorders();
	char mask = findMask();
	if (mask == ' ')
	{
		cout << left << setfill(' ') << setw(alignLeft) << "" << line << setw(alignRight) << "";
	}
	else
	{
		string maskedLine = "";
		for (size_t i = 0; i < line.size(); i++)
		{
			maskedLine += mask;
		}
		cout << left << setfill(' ') << setw(alignLeft) << "" << maskedLine << setw(alignRight) << "";
	}
	parent->printRightBorders();
	cout << endl;
}

Line* Line::getCopy()
{
	Line* lineCopy = new Line;
	lineCopy->setParent(nullptr);
	lineCopy->line = line;
	lineCopy->alignLeft = alignLeft;
	lineCopy->alignRight = alignRight;
	return lineCopy;
}

Block* Line::getParent()
{
	return parent;
}

char Line::findMask() const
{
	Block* parents = parent;
	char mask;
	while (parents != nullptr)
	{
		if (parents->maskChar != ' ')
			return parents->maskChar;
		parents = parents->parent;
	}
	return ' ';
}


