#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "markup.h"

#include <stack>
#include <regex>

using namespace std;

class Document
{
private:
	Block* root;
public:
	Document();
	Document(const Document&);
	Document(Document&&) noexcept;
	explicit Document(istream& stream);
	explicit Document(const string& text);

	Document& operator=(const Document&);
	Document& operator=(Document&&) noexcept;

	bool empty() const;
	size_t count() const;
	void remove(size_t idx);
	void update(size_t idx, std::istream& stream);
	void update(size_t idx, const std::string& text);
	void clear();

	void load(std::istream& stream);
	void load(const std::string& text);

	// Output document in console. I wrote all for cout before noticed it should return string :<
	void render() const; 
};


