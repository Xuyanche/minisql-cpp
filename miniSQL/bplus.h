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

	//在i位置插入toInsert节点
	BPlusNode* insertNode(BPlusNode* toInsert, int insertPos);
	//删除下标为todelete的子节点
	BPlusNode* removeNode(int toDelete);


	//插入KeyValue，在执行这个操作之后需要让该节点的父亲节点对应key=该节点key[0]
	//remove同理
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


	//找到一个没有满的兄弟节点
	static BPlusNode* findSibling(BPlusNode* Parent, int thisNo);
	
	//找出一个存储数量超过一半的兄弟节点
	static BPlusNode* findSiblingOverHalf(BPlusNode* Parent, int thisNo, int& resultNo);
	
	//将n个数据从from里移动到to里， parent是他们共同的父亲节点
	static BPlusNode* moveData(BPlusNode* parent, BPlusNode* from,
		BPlusNode* to, int fPos, int n);
	static BPlusNode* splitNode(BPlusNode* Parent, BPlusNode* focus, int thisNo);
	static BPlusNode* mergeNode(BPlusNode* Parent, BPlusNode* less, BPlusNode* more, int lPos);



	int keyNum;//记录这个节点里有多少个有效的记录
	array<KeyType, M + 1> key;//多余的一个是用来存储中间量的，最后会被分走
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