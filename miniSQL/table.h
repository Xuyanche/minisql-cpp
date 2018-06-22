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
	
	Table searchTable(int attrNo, string attrValue);//���ط���������tuple��ɵ�������ָ��
	Table searchTable(int attrNo, double lower, double higher, bool equal1, bool equal2);
	Table searchTable(int attrNo1, int attrNo2);//attrNo1Value=attrNo2Value
	Table selectTable(vector<int> attrNo);
	Table joinTable(Table& T);//�ѿ�����
	
	void tableInsert(string toAdd);//ֱ�Ӳ���buffer
	void tableDelete(int attrNo, string attrValue);//ֱ�Ӳ�������buffer
	void tableDelete(Table* T);
	void readHeader();
	void regenerateHeader();
	string tableGetContent(int position);

	void tableRead();
	void tableWrite();

	Table& operator=(const Table& T);


	int getAttrNo(string attrNameValue);//���ص����±�
	int getTotalRecordNum();
	bool isToInsertUnique(string toInsert, int uniqueAttrNo);
	void tablePrint();

	//header��ʽ���£�
	//01 tableName attr0 type0 unique0 attr1 type1 unique1 ... #main
	//uniqueΪ0��1����ʾ��һ�����Ƿ�unique��main��һ�����ֱ�ʾ�������±�
	string header;
	string name;
	array<string, MAX_ATTR_NUM> attrName;
	array<string, MAX_ATTR_NUM> attrType;
	bool unique[MAX_ATTR_NUM];
	int main;
	bool dirtBit;
	string fileName;
	//ֻ�������������Ի��filename��ֱ�ӵ���ʹ��filename�Ĺ�������
	//����ʹһ��table����filename�ı����
	//���κβ�����search/select/join���õ��ı���ʧȥfilename
	//������Կ����������ͼٱ�

	BufferNode head;//����һ����Ч��ͷ�ڵ�
	

};


#endif // !TABLE_H