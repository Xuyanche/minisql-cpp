#include "table.h"

Table::Table() {
	header = "";
	main = 0;
	name = "";
	fileName = "";
	dirtBit = false;
	attrName = new string[MAX_ATTR_NUM];
	attrType = new string[MAX_ATTR_NUM];
	for (int i = 0; i < MAX_ATTR_NUM; i++) {
		unique[i] = false;
	}
}

Table::Table(Table* headcopy) {
	header = headcopy->header;
	attrName = new string[MAX_ATTR_NUM];
	attrType = new string[MAX_ATTR_NUM];
	for (int i = 0; i < MAX_ATTR_NUM; i++) {
		attrName[i] = headcopy->attrName[i];
		attrType[i] = headcopy->attrType[i];
		unique[i] = headcopy->unique[i];
	}
	main = headcopy->main;
	name = headcopy->name + "*";
	fileName = headcopy->fileName;
	dirtBit = true;
}

Table::Table(const Table& T) {
	header = T.header;
	attrName = new string[MAX_ATTR_NUM];
	attrType = new string[MAX_ATTR_NUM];
	for (int i = 0; i < MAX_ATTR_NUM; i++) {
		attrName[i] =T.attrName[i];
		attrType[i] = T.attrType[i];
		unique[i] = T.unique[i];
	}
	main = T.main;
	name = T.name;
	fileName = T.fileName;
	dirtBit = true;
	head.makeCopy(T.head);
}


Table::Table(string fileNameValue) {
	header = "";
	main = 0;
	name = "";
	attrName = new string[MAX_ATTR_NUM];
	attrType = new string[MAX_ATTR_NUM];
	for (int i = 0; i < MAX_ATTR_NUM; i++) {
		unique[i] = false;
	}
	dirtBit = false;
	fileName = fileNameValue;
	tableRead();
}


Table::~Table() {
	delete[]attrName;
	delete[]attrType;
	head.clear();
}

void Table::tableInsert(string toAdd) {
	for (int i = 0; i < MAX_ATTR_NUM; i++) {
		if (attrName[i] == "") break;
		if (unique[i] == true) {
			//cout << "checking unique attr" << i << endl;
			if (!this->isToInsertUnique(toAdd, i)) {
				cout << "attr" << i << " should be unique" << endl;
				cout << "cannot insert" << endl;
				return;
			}
		}
	}
	head.insertLog(toAdd);
	dirtBit = true;
	return;
}

void Table::tableDelete(int attrNo, string attrValue) {
	head.deleteLog(attrNo, attrValue);
	dirtBit = true;
	return;
}

void Table::tableDelete(Table* T) {
	head.deleteLog(T->head);
	dirtBit = true;
	return;
}

string Table::tableGetContent(int position) {
	return head.getContent(position);
}




Table Table::searchTable(int attrNo, string attrValue) {
	Table result(this);
	int i = 0;
	BufferNode* focus = head.next;
	while (focus != NULL) {
		for (i = 0; i < focus->recordNum; i++) {
			//cout << BufferNode::getAttr(attrNo, focus->content[i])<<"|" << endl;
			if (attrValue == BufferNode::getAttr(attrNo, focus->content[i])) {
				result.tableInsert(focus->content[i]);
			}
		}
		focus = focus->next;
	}
	return result;
}



Table Table::searchTable(int attrNo, double lower, double higher, bool equal1, bool equal2) {
	Table result(this);
	string attrstring;
	stringstream ss;
	int i = 0;
	double attrvalue;
	BufferNode* focus = head.next;

	while (focus != NULL) {
		for (i = 0; i < focus->recordNum; i++) {
			attrstring = BufferNode::getAttr(attrNo, focus->content[i]);

			ss.str("");
			ss.clear();


			ss << attrstring;
			ss >> attrvalue;
			if (equal1 == false && equal2 == false) {
				if (attrvalue > lower && higher > attrvalue) {
					result.tableInsert(focus->content[i]);
				}
			}
			else if (equal1 == true && equal2 == false) {
				if (lower <= attrvalue && higher > attrvalue) {
					result.tableInsert(focus->content[i]);
				}
			}
			else if (equal1 == false && equal2 == true) {
				if (lower < attrvalue && higher >= attrvalue) {
					result.tableInsert(focus->content[i]);
				}
			}
			else if (equal1 == true && equal2 == true) {
				if (lower <= attrvalue && higher >= attrvalue) {
					result.tableInsert(focus->content[i]);
				}
			}

		}
		focus = focus->next;
	}
	return result;
}



Table Table::joinTable(Table& T) {
	Table result;
	int i, j;
	for (i = 0; i < MAX_ATTR_NUM; i++) {
		if (attrName[i] == "")
			break;
		result.attrName[i] = name + "." + attrName[i];
		result.attrType[i] = attrType[i];
		result.unique[i] = unique[i];
	}
	for (j = 0; j < MAX_ATTR_NUM; j++) {
		if (T.attrName[j] == "")
			break;
		if (i >= MAX_ATTR_NUM)
			break;
		result.attrName[i] = T.name + "." + T.attrName[j];
		result.attrType[i] = T.attrType[j];
		result.unique[i] = T.unique[i];
		i++;
	}

	result.main = this->main;
	result.fileName = "";
	result.dirtBit = true;
	result.name = name + T.name;

	for (i = 0; i < getTotalRecordNum(); i++) {
		for (j = 0; j < T.getTotalRecordNum(); j++) {
			result.tableInsert(tableGetContent(i) + T.tableGetContent(j));
		}
	}
	

	return result;

}


void Table::tableRead() {
	ifstream fs;
	fs.open(fileName);
	getline(fs, header, '\n');
	readHeader();
	head.clear();//如果head还在存储东西，把他清除
	head.bufferRead(fs);
	fs.close();
	return;
}

void Table::tableWrite() {
	ofstream fs;
	fs.open(fileName);
	fs << header << endl;
	head.bufferWrite(fs);
	fs.close();
	return;
}



int Table::getAttrNo(string attrNameValue) {
	int i = 0;
	for (i; i < MAX_ATTR_NUM; i++) {
		if (attrName[i] == attrNameValue) {
			return i;
		}
	}
	return -1;
}

void Table::readHeader() {
	int i = 0, u;
	string copy = header, temp, seperator;
	stringstream ss;
	temp = getNextWord(copy);
	name = getNextWord(copy);
	for (i; i < MAX_ATTR_NUM; i++) {
		attrName[i] = getNextWord(copy, seperator);
		attrType[i]= getNextWord(copy, seperator);
		temp = getNextWord(copy, seperator);
		ss.clear();
		ss.str("");
		ss << temp;
		ss >> u;
		if (u == 1)
			unique[i] = true;
		else 
			unique[i] = false;
		if (seperator == "#") break;
	}
	ss.str("");
	ss.clear();
	temp = getNextWord(copy);
	ss << temp;
	ss >> main;
	return;
}

int Table::getTotalRecordNum() {
	return head.getTotalRecordNum();
}



bool Table::isToInsertUnique(string toInsert, int uniqueAttrNo) {
	string uniqueValue = BufferNode::getAttr(uniqueAttrNo, toInsert);
	if (this->searchTable(uniqueAttrNo, uniqueValue).getTotalRecordNum() == 0) {
		return true;
	}
	else {
		return false;
	}
}



void Table::tablePrint() {
	cout << header << endl;
	for (int i = 0; i < MAX_ATTR_NUM; i++) {
		cout << attrName[i] << ";";
	}
	cout << endl;
	for (int i = 0; i < MAX_ATTR_NUM; i++) {
		cout << attrType[i] << ";";
	}
	cout << endl;
	for (int i = 0; i < getTotalRecordNum(); i++) {
		cout << tableGetContent(i) << endl;
	}
	return;
}

