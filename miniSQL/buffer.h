#ifndef BUFFER_H
#define BUFFER_H
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include "interpretor.h"
using namespace std;


static const int BUFFER_CAPACITY = 32;


class BufferNode {
public:
	BufferNode();
	BufferNode(istream& fs);
	BufferNode(BufferNode* lastNode, string toAdd);
	BufferNode(BufferNode& BN);
	~BufferNode();

	void clear();
	static string getAttr(int attrNo, string s);//从一tuple里抽取出一个属性
	
	void insertLog(string toAdd);//把记录插入到末尾
	void deleteLog(int attrNo, string attrValue);
	void deleteLog(BufferNode& deleteHead);
	//void updateLog(int attrNo, string oldValue, string newValue);

	void bufferRead(istream& fs);//这时的fs已经越过了header的位置,使用前先清空
	void bufferWrite(ostream& fs);//全部写入文件,已经写入了header
	long getOffset();
	void bufferPrint();
	int getBufferNum();
	string getContent(int position);//是排在一行的下标
	int getTotalRecordNum();



	void rearrange();
	void makeCopy(const BufferNode& copyHead);

	BufferNode* next;
	BufferNode* previous;
	int length;
	string* content;//content是一个指针，对应的是一组string共32个，每一个string是一个tuple
	
	int recordNum;
	bool dirtBit;
	bool lock;
};



#endif // !BUFFER_H

