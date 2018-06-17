#include "index.h"


Index::Index() {
	baseTable = NULL;
	attrNo = 0;
	Tree = new BPlusTree();
}

Index::Index(Table* T, int attrNoValue) {
	Pair p;
	string contentReading;
	double keyRead;
	stringstream ss;
	baseTable = T;
	attrNo = attrNoValue;
	Tree = new BPlusTree();
	

	for (int i = 0; i < T->getTotalRecordNum(); i++) {
		contentReading = T->tableGetContent(i);
		ss.str("");
		ss.clear();
		ss << BufferNode::getAttr(attrNo, contentReading);
		ss >> keyRead;
		p.key = keyRead;
		p.position = i;
		cout << p << endl;
		Tree->treeInsert(p);
	}
}

Index::~Index() {
	delete Tree;
}


void Index::refresh() {
	Pair p;
	string contentReading;
	double keyRead;
	stringstream ss;
	delete Tree;
	Tree = new BPlusTree();

	for (int i = 0; i < baseTable->getTotalRecordNum(); i++) {
		contentReading = baseTable->tableGetContent(i);
		ss.str("");
		ss.clear();
		ss << BufferNode::getAttr(attrNo, contentReading);
		ss >> keyRead;
		p.key = keyRead;
		p.position = i;
		cout << p << endl;
		Tree->treeInsert(p);
	}
	return;
}

int Index::indexSearch(double searchValue) {
	Pair p(searchValue, INT_MIN);
	BPlusTree temp;

	return Tree->treeSearch(p).position;
}



