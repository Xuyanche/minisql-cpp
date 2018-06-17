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
		//create table
	}
	else if (temp == "11") {
		//drop table
	}
	else if (temp == "20") {
		//create index
	}
	else if (temp == "21") {
		//drop index
	}
	else if (temp == "30") {
		//select
	}
	else if (temp == "40") {
		//insert log
	}
	else if (temp == "41") {
		//delete log
	}
	else if (temp == "99") {
		//error
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
	//opcode: 41

}



void DatabaseManager::dbSearch(string opcode) {
	//opcode: 30 tableName tableName2#attr1 lower1 higher1 equal11 equal12
	//如果指定的是相等的值，那么higher就是NOT_EXIST
	
	array<string, MAX_SEARCH_TABLENUM> tableName;
	string attrName, lowerstring, higherstring, equal1string, equal2string;
	int attrNovalue, lowervalue, highervalue;
	bool equal1value, equal2value;
	stringstream ss;
	array<Table*, MAX_SEARCH_TABLENUM> sourceTable;
	Table* temp = NULL;






	if (tableToOperate == NULL)
		return;
	temp = tableToOperate;
	while (true) {
		attrName = getNextWord(opcode);
		lowerstring = getNextWord(opcode);
		higherstring = getNextWord(opcode);
		equal1string = getNextWord(opcode);
		equal2string = getNextWord(opcode);
		if (equal2string == "")
			break;
		attrNovalue = temp->getAttrNo(attrName);
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

		if (higherstring == "NOT_EXIST")
			temp = &(temp->searchTable(attrNovalue, lowervalue, highervalue, equal1value, equal2value));
		else
			temp = &(temp->searchTable(attrNovalue, lowerstring));

		attrName = "";
		lowerstring = "";
		higherstring = "";
		equal1string = "";
		equal2string = "";
	}


	temp->tablePrint();
	
	return;

}











Table* DatabaseManager::dbFindTable(string tableName) {
	for (int i = 0; i < tableVector.size(); i++) {
		if (tableVector.at(i)->name == tableName)
			return tableVector.at(i);
	}
	return NULL;
}

Index* DatabaseManager::dbFindIndex(string indexBaseTableName, int attrNoValue) {
	for (int i = 0; i < indexVector.size(); i++) {
		if (indexVector.at(i)->baseTable->name == indexBaseTableName && indexVector.at(i)->attrNo == attrNoValue)
			return indexVector.at(i);
	}
	return NULL;
}



