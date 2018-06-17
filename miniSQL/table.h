#ifndef TABLE_H
#define TABLE_H
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include "interpretor.h"
#include "buffer.h"
#include "bplus.h"
using namespace std;



static const int MAX_ATTR_NUM = 32;



class Table {
public:
	Table();
	Table(Table* headCopy);
	Table(const Table& T);//copy
	Table(string fileNameValue);
	~Table();
	
	Table searchTable(int attrNo, string attrValue);//返回符合条件的tuple组成的虚拟表的指针
	Table searchTable(int attrNo, double lower, double higher, bool equal1, bool equal2);
	Table joinTable(Table& T);//笛卡儿积
	
	void tableInsert(string toAdd);//直接操作buffer
	void tableDelete(int attrNo, string attrValue);//直接操作自身buffer
	void tableDelete(Table* T);
	void readHeader();
	string tableGetContent(int position);

	void tableRead();
	void tableWrite();




	int getAttrNo(string attrNameValue);//返回的是下标
	int getTotalRecordNum();
	bool isToInsertUnique(string toInsert, int uniqueAttrNo);
	void tablePrint();

	//header格式如下：
	//01 tableName attr0 type0 unique0 attr1 type1 unique1 ... #main
	//unique为0或1，表示这一属性是否unique，main是一个数字表示主键的下标
	string header;
	string name;
	string* attrName;
	string* attrType;
	int unique[MAX_ATTR_NUM];
	int main;
	bool dirtBit;
	string fileName;
	
	BufferNode head;//这是一个无效的头节点
	

};


#endif // !TABLE_H