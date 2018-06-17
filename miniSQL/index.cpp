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

Table& Index::indexSearch(double searchValue) {
	Pair p(searchValue, INT_MIN);
	Table result(baseTable);
	BPlusNode* node = Tree->treeSearch(p);
	int i;
	for (i = 0; i < node->keyNum; i++) {
		if (node->key[i] == p)
			break;
	}
	result.tableInsert(baseTable->tableGetContent(node->key[i].position));

	return result;
}

Table& Index::indexSearch(double lower, double higher) {
	Pair plower(lower, INT_MIN), phigher(higher, INT_MIN);
	Table result(baseTable);
	BPlusNode* node = Tree->treeSearch(plower);
	
	while (node != NULL) {
		for (int i = 0; i < node->keyNum; i++) {
			if (node->key[i] >= plower && node->key[i] <= phigher)
				result.tableInsert(baseTable->tableGetContent(node->key[i].position));
			else if (node->key[i] > phigher)
				break;
		}
		node = node->next;
	}


	return result;
}

