#ifndef INDEX_H
#define INDEX_H
#include <sstream>
#include "bplus.h"
#include "table.h"
using namespace std;



class Index {
public:
	Index();
	Index(Table* T, int attrNoValue);
	~Index();


	void refresh();//��������B+��
	int indexSearch(double searchValue);
	int* indexSearch(double lower, double higher);





	Table* baseTable;
	int attrNo;
	BPlusTree* Tree;


};






#endif // !INDEX_H
