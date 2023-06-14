#include <iostream>
#include <fstream>

#include "Document.h"

int main()
{
	ifstream file;
	file.open("input.txt");
	Document doc(file);
	cout << "Document elements count: " << doc.count() << endl;
	doc.render();
	Document docCopy(doc);

	string newBlock = "{\n#align center\n#border 1 %\n#padding 2 5 2 5\n$Hello!\n\}\n";
	doc.remove(4);
	doc.update(3, newBlock);
	cout << "Updated document elements count: " << doc.count() << endl;
	doc.render();

	doc.clear();
	cout << "Cleared document elements count: " << doc.count() << ", is empty: " << doc.empty() << endl;
	doc.render();

	doc.load(newBlock);
	cout << "New loaded document:" << endl;
	doc.render();

	cout << "Copy of document before all changes: " << endl;
	docCopy.render();
	
	doc = move(docCopy);
	cout << "Moved copy to document. Document elements count: " << doc.count() << ", copy elements count: " << docCopy.count() << endl;

	// All other features are implemented and working.
}