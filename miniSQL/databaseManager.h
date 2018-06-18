#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "table.h"
#include "index.h"
#include "interpretor.h"





class DatabaseManager {
public:
	DatabaseManager();
	~DatabaseManager();

	void recieveOpcode(string opcode);


	bool dbCreateTable(string opcode);
	bool dbCreateIndex(string opcode);
	bool dbInsert(string opcode);
	bool dbDelete(string opcode);
	bool dbDropIndex(string opcode);
	bool dbDropTable(string opcode);
	Table dbSearch(string opcode);


	void dbCloseTable(string tableName);
	Table* dbOpenTable(string tableName);

	void getTableOpened();

	void selfCheck();

	Table* dbFindTable(string tableName);
	Index* dbFindIndex(string indexBaseTableName, int attrNoValue);

private:

	vector<Table*> tableVector;
	vector<Index*> indexVector;

};







#endif // !DATABASEMANAGER_H



