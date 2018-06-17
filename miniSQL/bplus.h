#ifndef BPLUS_H
#define BPLUS_H
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <array>
#include "pair.h"
using namespace std;
#define KeyType Pair

static const int M = 8;
static const int LIMIT_M_2 = M / 2;



class BPlusNode {
public:
	BPlusNode();
	BPlusNode(BPlusNode& B);
	~BPlusNode();

	//��iλ�ò���toInsert�ڵ�
	BPlusNode* insertNode(BPlusNode* toInsert, int insertPos);
	//ɾ���±�Ϊtodelete���ӽڵ�
	BPlusNode* removeNode(int toDelete);


	//����KeyValue����ִ���������֮����Ҫ�øýڵ�ĸ��׽ڵ��Ӧkey=�ýڵ�key[0]
	//removeͬ��
	BPlusNode* insertKey(KeyType keyValue);
	BPlusNode* removeKey(int toDelete);
	bool isLeaf();
	BPlusNode* recursiveDestroy();
	void recursiveTravel();
	void printNode();
	BPlusNode* findLeftMost();
	BPlusNode* findRightMost();
	KeyType findKey(KeyType keyValue);


	static BPlusNode* recursiveInsert(BPlusNode* T, KeyType Key, int i, BPlusNode* Parent);
	static BPlusNode* recursiveRemove(BPlusNode* parent, KeyType keyValue, int fPos, BPlusNode* focus);


	//�ҵ�һ��û�������ֵܽڵ�
	static BPlusNode* findSibling(BPlusNode* Parent, int thisNo);
	
	//�ҳ�һ���洢��������һ����ֵܽڵ�
	static BPlusNode* findSiblingOverHalf(BPlusNode* Parent, int thisNo, int& resultNo);
	
	//��n�����ݴ�from���ƶ���to� parent�����ǹ�ͬ�ĸ��׽ڵ�
	static BPlusNode* moveData(BPlusNode* parent, BPlusNode* from,
		BPlusNode* to, int fPos, int n);
	static BPlusNode* splitNode(BPlusNode* Parent, BPlusNode* focus, int thisNo);
	static BPlusNode* mergeNode(BPlusNode* Parent, BPlusNode* less, BPlusNode* more, int lPos);



	int keyNum;//��¼����ڵ����ж��ٸ���Ч�ļ�¼
	array<KeyType, M + 1> key;//�����һ���������洢�м����ģ����ᱻ����
	BPlusNode* children[M + 1];
	BPlusNode* next;

};


class BPlusTree {
public:
	BPlusTree();
	BPlusTree(BPlusTree& T);
	~BPlusTree();


	void treeInsert(KeyType keyValue);
	void treeRemove(KeyType keyValue);
	KeyType treeSearch(KeyType keyValue);
	void travel();
	void clearTree();


	BPlusNode* root;


private:
	void copyTree(BPlusTree& T);
};























#endif // !BPLUS_H