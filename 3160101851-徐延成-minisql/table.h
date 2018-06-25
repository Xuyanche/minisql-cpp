#ifndef TABLE_H
#define TABLE_H
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <array>
#include <vector>
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
	Table(string header, int a);
	~Table();
	
	Table searchTable(int attrNo, string attrValue);//返回符合条件的tuple组成的虚拟表的指针
	Table searchTable(int attrNo, double lower, double higher, bool equal1, bool equal2);
	Table searchTable(int attrNo1, int attrNo2);//attrNo1Value=attrNo2Value
	Table selectTable(vector<int> attrNo);
	Table joinTable(Table& T);//笛卡儿积
	
	void tableInsert(string toAdd);//直接操作buffer
	void tableDelete(int attrNo, string attrValue);//直接操作自身buffer
	void tableDelete(Table* T);
	void readHeader();
	void regenerateHeader();
	string tableGetContent(int position);

	void tableRead();
	void tableWrite();

	Table& operator=(const Table& T);


	int getAttrNo(string attrNameValue);//返回的是下标
	int getTotalRecordNum();
	bool isToInsertUnique(string toInsert, int uniqueAttrNo);
	void tablePrint();

	//header格式如下：
	//01 tableName attr0 type0 unique0 attr1 type1 unique1 ... #main
	//unique为0或1，表示这一属性是否unique，main是一个数字表示主键的下标
	string header;
	string name;
	array<string, MAX_ATTR_NUM> attrName;
	array<string, MAX_ATTR_NUM> attrType;
	bool unique[MAX_ATTR_NUM];
	int main;
	bool dirtBit;
	string fileName;
	//只有少数方法可以获得filename：直接调用使用filename的构建函数
	//或是使一个table和有filename的表相等
	//由任何操作（search/select/join）得到的表都会失去filename
	//这个属性可以区分真表和假表

	BufferNode head;//这是一个无效的头节点
	

};


#endif // !TABLE_H