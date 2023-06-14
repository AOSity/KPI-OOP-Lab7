#include "Document.h"

regex lineRegex("[$].*");
regex blockBeginRegex("[{]");
regex blockEndRegex("[}]");
regex alignRegex("#align (center|left|right)", regex::icase);
regex borderRegex("#border [0-9]*( .)?", regex::icase);
regex marginRegex("#margin [0-9]* [0-9]* [0-9]* [0-9]*", regex::icase);
regex paddingRegex("#padding [0-9]* [0-9]* [0-9]* [0-9]*", regex::icase);
regex maskRegex("#mask .", regex::icase);
regex widthRegex("#width [0-9]*", regex::icase);

Block* istreamToBlock(istream& stream)
{
	Block* root = new Block();
	stack<Block*> blocksStack;
	blocksStack.push(root);

	string line;
	Block* newBlock;
	Line* newLine;
	smatch match;
	string word;

	while (getline(stream, line)) {
		if (regex_search(line, match, blockBeginRegex))		// {
		{
			newBlock = new Block();
			newBlock->setParent(blocksStack.top());
			blocksStack.top()->addComponent(newBlock);
			blocksStack.push(newBlock);
			newBlock = nullptr;
		}
		else if (regex_search(line, match, blockEndRegex))	// }
		{
			blocksStack.pop();
		}	
		else if (regex_search(line, match, alignRegex))		// #align <type>
		{
			if (regex_search(line, match, regex("left", regex::icase)))
			{
				blocksStack.top()->setAlign(1);
			}
			else if (regex_search(line, match, regex("center", regex::icase)))
			{
				blocksStack.top()->setAlign(2);
			}
			else if (regex_search(line, match, regex("right", regex::icase)))
			{
				blocksStack.top()->setAlign(3);
			}			
		}
		else if (regex_search(line, match, borderRegex))	// #border <size> <char>
		{
			stringstream liness(match.str());
			liness >> line;

			liness >> line;
			short width = stoi(line);
			liness >> line;
			char symbol = ' ';
			if (line.size() > 0)
				symbol = line[0];
			blocksStack.top()->setBorder(width, symbol);
		}
		else if (regex_search(line, match, marginRegex))	// #margin <top> <right> <bottom> <left>
		{
			stringstream liness(match.str());
			liness >> line;

			liness >> line;
			short top = stoi(line);
			liness >> line;
			short right = stoi(line);
			liness >> line;
			short bottom = stoi(line);
			liness >> line;
			short left = stoi(line);

			blocksStack.top()->setMargin(top, right, bottom, left);
		}
		else if (regex_search(line, match, paddingRegex))	// #padding <top> <right> <bottom> <left>
		{
			stringstream liness(match.str());
			liness >> line;

			liness >> line;
			short top = stoi(line);
			liness >> line;
			short right = stoi(line);
			liness >> line;
			short bottom = stoi(line);
			liness >> line;
			short left = stoi(line);

			blocksStack.top()->setPadding(top, right, bottom, left);
		}
		else if (regex_search(line, match, maskRegex))		// #mask <char>
		{
			stringstream liness(match.str());
			liness >> line;

			liness >> line;
			char symbol = ' ';
			if (line.size() > 0)
				symbol = line[0];

			blocksStack.top()->setMask(symbol);
		}
		else if (regex_search(line, match, widthRegex))		// #width <num>
		{
			stringstream liness(match.str());
			liness >> line;

			liness >> line;
			short width = stoi(line);

			blocksStack.top()->setWidth(width);
		}
		else if (regex_search(line, match, lineRegex))		// $<text>
		{
			line.erase(0, 1);
			newLine = new Line(line);
			newLine->setParent(blocksStack.top());

			blocksStack.top()->addComponent(newLine);
			if (blocksStack.top()->getWidth() < line.size())
			{
				blocksStack.top()->setWidth(line.size());
			}
		}

		if (blocksStack.size() <= 1)
		{
			break;
		}
	}
	
	root->updateWidth();
	root->updateAlign();

	return root;
}

Document::Document()
{
	root = nullptr;
}
Document::Document(const Document& otherDocument)
{
	root = otherDocument.root->getCopy();
}
Document::Document(Document&& otherDocument) noexcept
{
	root = otherDocument.root;
	otherDocument.root = nullptr;
}
Document::Document(istream& stream)
{
	root = istreamToBlock(stream);
}
Document::Document(const string& text)
{
	istringstream stream(text);
	root = istreamToBlock(stream);
}

Document& Document::operator=(const Document& otherDocument)
{
	root = otherDocument.root->getCopy();
	return *this;
}
Document& Document::operator=(Document&& otherDocument) noexcept
{
	root = otherDocument.root;
	otherDocument.root = nullptr;
	return *this;
}

bool Document::empty() const
{
	if (root == nullptr)
		return true;
	return false;
}
size_t Document::count() const
{
	if (root != nullptr)
	{
		vector<Component*> components = root->getComponents();
		return components.size();
	}
	return 0;
}
void Document::remove(size_t idx)
{
	vector<Component*> components = root->getComponents();
	if (idx >= 0 && idx < components.size())
	{
		components[idx]->getParent()->removeComponent(components[idx]);
	}
}
void Document::update(size_t idx, istream& stream)
{
	vector<Component*> components = root->getComponents();
	Block* newBlock = istreamToBlock(stream);
	newBlock->getComponents()[0]->setParent(components[idx]->getParent());
	if (idx >= 0 && idx < components.size())
	{
		components[idx]->getParent()->replaceComponent(components[idx], newBlock->getComponents()[0]);
	}
	root->updateWidth();
	root->updateAlign();
}
void Document::update(size_t idx, const string& text)
{
	istringstream stream(text);
	update(idx, stream);
}
void Document::clear()
{
	if (root != nullptr)
		delete root;
	root = nullptr;
}

void Document::load(istream& stream)
{
	clear();
	root = istreamToBlock(stream);
}
void Document::load(const string& text)
{
	clear();
	istringstream stream(text);
	root = istreamToBlock(stream);
}

void Document::render() const
{
	if (root != nullptr)
		root->render();
}