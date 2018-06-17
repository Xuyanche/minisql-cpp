#include <iostream>
#include <iomanip>
#include <string>
#include "interpretor.h"
#include "table.h"
#include "buffer.h"
#include "bplus.h"
#include "index.h"
using namespace std;

const int MAX_TABLE_NUM = 32;




int main() {


	Table* tptr;
	Table T("123.txt");
	tptr = &T;
	T.tablePrint();
	cout << T.name << endl;
	cout << T.unique[0] << ";" << T.unique[1] << endl;
	cout << "===========================" << endl;
	T.searchTable(1, 2, 5, false, false).tablePrint();
	cout << "===========================" << endl;
	T.tableInsert("30 17.5");
	T.tablePrint();
	cout << "===========================" << endl;
	T.tableInsert("5 2");
	T.tablePrint();
	T.tableWrite();




	
	system("pause");
	return 0;
}



void SQLinit() {






}





























