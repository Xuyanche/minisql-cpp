#include "table.h"

Table::Table() {
	header = "";
	main = -1;
	name = "";
	fileName = "";
	dirtBit = false;
	for (int i = 0; i < MAX_ATTR_NUM; i++) {
		unique[i] = false;
	}
}

Table::Table(Table* headcopy) {
	header = headcopy->header;
	for (int i = 0; i < MAX_ATTR_NUM; i++) {
		attrName[i] = headcopy->attrName[i];
		attrType[i] = headcopy->attrType[i];
		unique[i] = headcopy->unique[i];
	}
	main = headcopy->main;
	name = headcopy->name + "*";
	fileName = "";
	dirtBit = true;
}

Table::Table(const Table& T) {
	header = T.header;
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

Table::Table(string header, int a) {
	this->header = header;
	main = -1;
	for (int i = 0; i < MAX_ATTR_NUM; i++) {
		unique[i] = false;
	}
	readHeader();
	dirtBit = false;
	fileName = name + ".txt";
}


Table::Table(string fileNameValue) {
	header = "";
	main = -1;
	name = "";
	for (int i = 0; i < MAX_ATTR_NUM; i++) {
		unique[i] = false;
	}
	dirtBit = false;
	fileName = fileNameValue;
	tableRead();
}


Table::~Table() {
	if (fileName != "")
		this->tableWrite();
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
	result.regenerateHeader();
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
	result.regenerateHeader();
	return result;
}

Table Table::searchTable(int attrNo1, int attrNo2) {
	Table result(this);
	int i = 0;
	BufferNode* focus = head.next;
	string attr1Value, attr2Value;
	while (focus != NULL) {
		for (i = 0; i < focus->recordNum; i++) {
			attr1Value = BufferNode::getAttr(attrNo1, focus->content[i]);
			attr2Value = BufferNode::getAttr(attrNo2, focus->content[i]);
			if (attr1Value == attr2Value) {
				result.tableInsert(focus->content[i]);
			}
		}
		focus = focus->next;
	}

	result.regenerateHeader();
	return result;


}


Table Table::selectTable(vector<int> attrNo) {
	Table result;
	string newcontent, origincontent;
	int i, j;
	result.name = name + "selected";
	for (i = 0; i < attrNo.size(); i++) {
		if (attrNo.at(i) < 0) {
			return *this;
		}
		result.attrName[i] = attrName[attrNo.at(i)];
		result.attrType[i] = attrType[attrNo.at(i)];
		result.unique[i] = unique[attrNo.at(i)];
	}
	result.main = -1;
	result.regenerateHeader();
	result.dirtBit = true;
	result.fileName = "";

	for (i = 0; i < getTotalRecordNum(); i++) {
		newcontent = "";
		origincontent = tableGetContent(i);
		for (j = 0; j < attrNo.size(); j++) {
			if (attrNo.at(j) < 0) {
				result = *this;
				return result;
			}
			newcontent.append(BufferNode::getAttr(attrNo.at(j), origincontent));
			if (j < attrNo.size() - 1)
				newcontent.append(" ");
		}
		result.tableInsert(newcontent);
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
		result.unique[i] = false;
	}
	for (j = 0; j < MAX_ATTR_NUM; j++) {
		if (T.attrName[j] == "")
			break;
		if (i >= MAX_ATTR_NUM)
			break;
		result.attrName[i] = T.name + "." + T.attrName[j];
		result.attrType[i] = T.attrType[j];
		result.unique[i] = false;
		i++;
	}

	result.main = -1;
	result.fileName = "";
	result.dirtBit = true;
	result.name = name + T.name;
	result.regenerateHeader();

	for (i = 0; i < getTotalRecordNum(); i++) {
		for (j = 0; j < T.getTotalRecordNum(); j++) {
			result.tableInsert(tableGetContent(i) + " " + T.tableGetContent(j));
		}
	}
	

	return result;

}


void Table::regenerateHeader() {
	string newheader = "01", mainstring;
	stringstream ss;
	newheader.append(" " + name);
	for (int i = 0; i < MAX_ATTR_NUM; i++) {
		if (attrName[i] == "")
			break;
		newheader.append(" " + attrName[i] + " " + attrType[i] + " " + ((unique[i]) ? "1" : "0"));
	}
	newheader.append("#");
	ss << main;
	ss >> mainstring;
	newheader.append(mainstring);
	header = newheader;
	return;
}




void Table::tableRead() {
	ifstream fs;
	fs.open(fileName);
	getline(fs, header, '\n');
	cout << header << endl;
	readHeader();
	head.clear();//如果head还在存储东西，把他清除
	head.bufferRead(fs);
	fs.close();
	return;
}

void Table::tableWrite() {
	ofstream fs;
	if (fileName == "") {
		cout << "this table (" << name << ") cannot be written" << endl;
		return;
	}
	fs.open(fileName, fstream::out);
	regenerateHeader();
	fs << header << endl;
	head.bufferWrite(fs);
	fs.close();
	return;
}


Table& Table::operator=(const Table& T) {
	header = T.header;
	for (int i = 0; i < MAX_ATTR_NUM; i++) {
		attrName[i] = T.attrName[i];
		attrType[i] = T.attrType[i];
		unique[i] = T.unique[i];
	}
	main = T.main;
	name = T.name;
	fileName = T.fileName;
	dirtBit = true;
	head.makeCopy(T.head);

	return *this;
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
	cout << "main=" << main << endl;
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

