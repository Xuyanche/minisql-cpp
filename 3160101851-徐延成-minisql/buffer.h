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
	static string getAttr(int attrNo, string s);//��һtuple���ȡ��һ������
	
	void insertLog(string toAdd);//�Ѽ�¼���뵽ĩβ
	void deleteLog(int attrNo, string attrValue);
	void deleteLog(BufferNode& deleteHead);
	//void updateLog(int attrNo, string oldValue, string newValue);

	void bufferRead(istream& fs);//��ʱ��fs�Ѿ�Խ����header��λ��,ʹ��ǰ�����
	void bufferWrite(ostream& fs);//ȫ��д���ļ�,�Ѿ�д����header
	long getOffset();
	void bufferPrint();
	int getBufferNum();
	string getContent(int position);//������һ�е��±�
	int getTotalRecordNum();



	void rearrange();
	void makeCopy(const BufferNode& copyHead);

	BufferNode* next;
	BufferNode* previous;
	int length;
	string* content;//content��һ��ָ�룬��Ӧ����һ��string��32����ÿһ��string��һ��tuple
	
	int recordNum;
	bool dirtBit;
	bool lock;
};



#endif // !BUFFER_H

