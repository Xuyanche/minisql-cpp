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
	bool adminopmode = false;

	cout << setfill('=') << setw(40) << "=" << endl;
	cout << "miniSQL by xuyancheng" << endl;
	cout << "3160101851" << endl;
	cout << setfill('=') << setw(40) << "=" << endl;

	while (true) {
		if (adminopmode) {
			cout << "admin mode" << endl;
			cout << ">>";
			getline(cin, input, '\n');
			if (input == "exit") {
				cout << "exiting" << endl;
				break;
			}
			else if (input == "greed is good") {
				adminopmode = false;
				continue;
			}
			DB.recieveOpcode(input);
			continue;
		}

		cout << endl << "system ready" << endl;
		cout << ">>";
		getline(cin, input, '\n');
		cout << endl << input << endl;
		if (input == "exit") {
			cout << "exiting" << endl;
			break;
		}
		else if (input == "greed is good") {
			adminopmode = true;
			continue;
		}
		opcode = interpret(input);
		cout << opcode << endl << endl;

		DB.recieveOpcode(opcode);
	}


	//opcode = interpret("insert 123 18 2.35");

	//opcode = interpret("delete 123 attr1>10");
	//opcode = interpret("select 123.attr1 456.attr2 from 123 456 where 123.attr1>5 and 465.attr2<=3");
	//opcode = interpret("create table tablename(attr1 int, attr2 float)");
	//opcode = interpret("select * from 123");
	//opcode = interpret("create index indexname on tablename attrname");
	//opcode = interpret("drop table tablename;");
	//opcode = interpret("drop index indexname");
	//cout << opcode << endl;
	//DB.recieveOpcode(opcode);
	//DB.dbFindTable("123")->tablePrint();

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





























