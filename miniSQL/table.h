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
	
	Table searchTable(int attrNo, string attrValue);//���ط���������tuple��ɵ�������ָ��
	Table searchTable(int attrNo, double lower, double higher, bool equal1, bool equal2);
	Table joinTable(Table& T);//�ѿ�����
	
	void tableInsert(string toAdd);//ֱ�Ӳ���buffer
	void tableDelete(int attrNo, string attrValue);//ֱ�Ӳ�������buffer
	void tableDelete(Table* T);
	void readHeader();
	string tableGetContent(int position);

	void tableRead();
	void tableWrite();




	int getAttrNo(string attrNameValue);//���ص����±�
	int getTotalRecordNum();
	bool isToInsertUnique(string toInsert, int uniqueAttrNo);
	void tablePrint();

	//header��ʽ���£�
	//01 tableName attr0 type0 unique0 attr1 type1 unique1 ... #main
	//uniqueΪ0��1����ʾ��һ�����Ƿ�unique��main��һ�����ֱ�ʾ�������±�
	string header;
	string name;
	string* attrName;
	string* attrType;
	int unique[MAX_ATTR_NUM];
	int main;
	bool dirtBit;
	string fileName;
	
	BufferNode head;//����һ����Ч��ͷ�ڵ�
	

};


#endif // !TABLE_H