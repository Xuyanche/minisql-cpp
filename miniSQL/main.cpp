#include <iostream>
#include <iomanip>
#include <string>
#include "interpretor.h"
#include "table.h"
#include "buffer.h"
#include "bplus.h"
#include "index.h"
#include "databaseManager.h"
using namespace std;

const int MAX_TABLE_NUM = 32;




int main() {

	string opcode, input;
	DatabaseManager DB;	

	opcode = interpret("insert 123 18 2.35");
	//opcode = interpret("select 123.attr1 456.attr2 from 123 456 where 123.attr1>5 and 465.attr2<=3");
	//opcode = interpret("create table tablename(attr1 int, attr2 float main)");
	//opcode = interpret("create index indexname on tablename attrname");
	//opcode = interpret("drop table tablename;");
	//opcode = interpret("drop index indexname");
	cout << opcode << endl;
	DB.recieveOpcode(opcode);
	DB.dbFindTable("123")->tablePrint();

/*
	DB.dbInsert("123 16 5.3");
	DB.dbFindTable("123")->tablePrint();
	DB.dbFindTable("456")->tablePrint();
	cout << "===========================" << endl;
	//DB.dbSearch("123 456#123.attr1 1 5 0 0 123.attr2 2 4 0 0");
	DB.recieveOpcode("30 123 456#123.attr1 456.attr2#123.attr1 1 5 0 0 123.attr2 2 4 0 0");


	//DB.dbFindTable("123")->searchTable(0, 1, 5, false, false).tablePrint();










	
	Table* tptr;
	Table T("123.txt");
	tptr = &T;
	T.tablePrint();
	cout << T.name << endl;
	cout << T.head.next->recordNum << ";" << endl;
	cout << "===========================" << endl;
	T.searchTable(1, 2, 5, false, false).tablePrint();
	cout << "===========================" << endl;
	T.tableInsert("30 17.5");
	T.tablePrint();
	cout << "===========================" << endl;
	T.tableInsert("5 2");
	T.tablePrint();
	T.tableWrite();*/




	
	system("pause");
	return 0;
}





























