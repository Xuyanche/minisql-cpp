#include "databaseManager.h"

static const int MAX_SEARCH_TABLENUM = 5;


DatabaseManager::DatabaseManager() {

}

DatabaseManager::~DatabaseManager() {
	int i;
	for (i = 0; i < tableVector.size(); i++) {
		delete tableVector.at(i);
	}
	for (i = 0; i < indexVector.size(); i++) {
		delete indexVector.at(i);
	}

}


void DatabaseManager::recieveOpcode(string opcode) {
	string temp, opcopy = opcode;
	temp = getNextWord(opcopy);
	if (temp == "00") {
		//create database
	}
	else if (temp == "01") {
		//delete database
	}
	else if (temp == "10") {
		dbCreateTable(opcopy);//create table ----
	}
	else if (temp == "11") {
		dbDropTable(opcopy);//drop table----
	}
	else if (temp == "20") {
		dbCreateIndex(opcode);//create index-----
	}
	else if (temp == "21") {
		dbDropIndex(opcopy);//drop index ----
	}
	else if (temp == "30") {
		dbSearch(opcopy);//select----
	}
	else if (temp == "40") {
		dbInsert(opcode);//insert log----
	}
	else if (temp == "41") {
		dbDelete(opcode);//delete log---
	}
	else if (temp == "99") {
		cout << "opcode: error in reading operation sentence" << endl;
	}



	return;


}


bool DatabaseManager::dbCreateTable(string opcode) {
	//opcode: 20 tablename attr0 type0 unique0....#main
	Table* newtable = new Table(opcode);
	if (newtable == NULL)
		return false;
	tableVector.push_back(newtable);
	return true;
}

bool DatabaseManager::dbCreateIndex(string opcode) {
	//opcode: 30 basetableName uniqueAttrName
	string tableName = getNextWord(opcode);
	Table* baseTable = dbFindTable(tableName);
	if (baseTable == NULL)
		return false;
	int attrNo = baseTable->getAttrNo(getNextWord(opcode));
	Index* newindex = new Index(baseTable, attrNo);
	if (newindex == NULL)
		return false;
	return true;
}


bool DatabaseManager::dbInsert(string opcode) {
	//opcode: 40 tableName attr1value attr2value attr2value...(没有回车)
	string tableName = getNextWord(opcode);
	Table* tableToOperate = dbFindTable(tableName);
	if (tableToOperate == NULL)
		return false;
	tableToOperate->tableInsert(opcode);
	return true;
}


bool DatabaseManager::dbDelete(string opcode) {
	//opcode: 41 tableName#attr1 lower1 higher1 equal1 equal2
	string tableName, opcopy = opcode;
	tableName = getNextWord(opcopy);
	Table* sourceTable = dbFindTable(tableName);
	if (sourceTable == NULL)
		return false;
	sourceTable->tableDelete(&dbSearch(opcode));
	return true;

}

bool DatabaseManager::dbDropIndex(string opcode) {
	stringstream ss;
	string tableName = getNextWord(opcode);
	
	int attrNo;
	ss << opcode;
	ss >> attrNo;
	for (int i = 0; i < indexVector.size(); i++) {
		if (indexVector.at(i)->baseTable->name == tableName && indexVector.at(i)->attrNo == attrNo) {
			delete indexVector.at(i);
			indexVector.at(i) = indexVector.at(indexVector.size() - 1);
			indexVector.at(indexVector.size() - 1) = NULL;
			return true;
		}
	}
	return false;
}


bool DatabaseManager::dbDropTable(string opcode) {
	//opcode: 21 tableName
	Table* find;
	for (int i = 0; i < tableVector.size(); i++) {
		if (tableVector.at(i)->name == opcode) {
			tableVector.at(i)->header.append("droped");
			delete tableVector.at(i);
			tableVector.at(i) = tableVector.at(tableVector.size() - 1);
			tableVector.at(tableVector.size() - 1) = NULL;
			return true;
		}
	}
	return false;
}



















Table DatabaseManager::dbSearch(string opcode) {
	//opcode: 30 tableName tableName2#attr1 lower1 higher1 equal11 equal12
	//如果指定的是相等的值，那么higher就是NOT_EXIST
	
	string attrName, lowerstring, higherstring, equal1string, equal2string, seperator;
	int attrNovalue, lowervalue, highervalue;
	bool equal1value, equal2value;
	stringstream ss;
	array<Table*, MAX_SEARCH_TABLENUM> sourceTable;
	Table temp;
	Index* tempIndex = NULL;
	int i = 0;

	sourceTable.fill(NULL);

	while (i < MAX_SEARCH_TABLENUM) {
		sourceTable[i] = dbFindTable(getNextWord(opcode, seperator));
		i++;
		if (seperator == "#" || sourceTable[i-1] == NULL)
			break;
	}
	if (i > 1) {
		temp = sourceTable[0]->joinTable(*sourceTable[1]);
		for (i = 2; i < MAX_SEARCH_TABLENUM; i++) {
			if (sourceTable[i] == NULL)
				break;
			temp = temp.joinTable(*sourceTable[i]);
		}
	}
	else if (i == 1)
		temp = *sourceTable[0];
	else
		return temp;
	while (true) {
		attrName = getNextWord(opcode);
		lowerstring = getNextWord(opcode);
		higherstring = getNextWord(opcode);
		equal1string = getNextWord(opcode);
		equal2string = getNextWord(opcode);
		attrNovalue = temp.getAttrNo(attrName);
		ss.str("");
		ss.clear();
		ss << lowerstring;
		ss >> lowervalue;
		ss.str("");
		ss.clear();
		ss << higherstring;
		ss >> highervalue;
		equal1value = (equal1string == "1") ? true : false;
		equal2value = (equal2string == "1") ? true : false;
		//如果有相应的index，那么通过index搜索
		tempIndex = dbFindIndex(temp.name, attrNovalue);
		if (tempIndex != NULL) {
			if (higherstring != "NOT_EXIST")
				temp = tempIndex->indexSearch(lowervalue);
			else
				temp = tempIndex->indexSearch(lowervalue, highervalue);
		}
		else {
			//如果没有相应的index，那么就直接搜索
			if (higherstring != "NOT_EXIST")
				temp = temp.searchTable(attrNovalue, lowervalue, highervalue, equal1value, equal2value);
			else
				temp = temp.searchTable(attrNovalue, lowerstring);
		}
		attrName = "";
		lowerstring = "";
		higherstring = "";
		equal1string = "";
		equal2string = "";
		if (opcode == "")
			break;
	}

	temp.tablePrint();
	
	return temp;
}



Table* DatabaseManager::dbOpenTable(string tableName) {
	Table* newtable = new Table(tableName + ".txt");
	tableVector.push_back(newtable);
	return newtable;
}

void DatabaseManager::dbCloseTable(string tableName) {
	Table* find;
	for (int i = 0; i < tableVector.size(); i++) {
		if (tableVector.at(i)->name == tableName) {
			delete tableVector.at(i);
			tableVector.at(i) = tableVector.at(tableVector.size() - 1);
			tableVector.at(tableVector.size() - 1) = NULL;
			return;
		}
	}
	return;
}

void DatabaseManager::getTableOpened() {
	int i;
	cout << "tableName:" << endl;
	for (i = 0; i < tableVector.size(); i++) {
		cout << tableVector.at(i)->name << endl;
	}
	cout << "index:" << endl;
	for (i = 0; i < indexVector.size(); i++) {
		cout << indexVector.at(i)->baseTable->name << ";" << indexVector.at(i)->attrNo << endl;
	}
	return;
}

//等待完成，再打开的表过多的时候关闭一些
void DatabaseManager::selfCheck() {
	return;

}



Table* DatabaseManager::dbFindTable(string tableName) {
	Table* find;
	for (int i = 0; i < tableVector.size(); i++) {
		if (tableVector.at(i)->name == tableName)
			return tableVector.at(i);
	}
	find = dbOpenTable(tableName);
	return find;
}

Index* DatabaseManager::dbFindIndex(string indexBaseTableName, int attrNoValue) {
	for (int i = 0; i < indexVector.size(); i++) {
		if (indexVector.at(i)->baseTable->name == indexBaseTableName && indexVector.at(i)->attrNo == attrNoValue)
			return indexVector.at(i);
	}
	return NULL;
}



