#include "bplus.h"
static int Unavailable = INT_MIN;
static Pair UnavailablePair{INT_MIN, INT_MIN};

//////////////////  B+Node //////////////////////////

BPlusNode::BPlusNode() {
	keyNum = 0;
	for (int i = 0; i < M + 1; i++) {
		key[i] = UnavailablePair;
		children[i] = NULL;
	}
	next = NULL;
}

//这个拷贝构建函数只是浅拷贝，不是深拷贝，指针指的位置都是没有复制的
//想要深拷贝应该用B+Tree里的拷贝函数
BPlusNode::BPlusNode(BPlusNode& B) {
	keyNum = B.keyNum;
	for (int i = 0; i < M + 1; i++) {
		key[i] = B.key[i];
		children[i] = B.children[i];
	}
	next = B.next;
}

//想要摧毁B+树应该使用B+树的析构函数/destroy()函数
BPlusNode::~BPlusNode() {
	for (int i = 0; i < M + 1; i++) {
		if (children[i] != NULL) {
			cout << "error: deleting B+Node" << endl;
		}
	}
}


BPlusNode* BPlusNode::findLeftMost() {
	BPlusNode* Tmp;
	Tmp = this;
	while (Tmp != NULL && Tmp->children[0] != NULL) {
		Tmp = Tmp->children[0];
	}
	return Tmp;
}


BPlusNode* BPlusNode::findRightMost() {
	BPlusNode* Tmp;
	Tmp = this;
	while (Tmp != NULL && Tmp->children[Tmp->keyNum - 1] != NULL) {
		Tmp = Tmp->children[Tmp->keyNum - 1];
	}
	return Tmp;
}



BPlusNode* BPlusNode::insertNode(BPlusNode* toInsert, int insertPos) {


	if (toInsert->isLeaf()) { 
		if (insertPos > 0) {
			children[insertPos - 1]->next = toInsert;
		}
		toInsert->next = children[insertPos];
	}

	for (int i = keyNum - 1; i >= insertPos; i--) {
		children[i + 1] = children[i];
	}
	children[insertPos] = toInsert;
	key[insertPos] = toInsert->key[0];
	keyNum++;

	return toInsert;
}


BPlusNode* BPlusNode::removeNode(int toDelete) {

	if (children[toDelete]->isLeaf() && toDelete > 0) {
		children[toDelete - 1]->next = children[toDelete + 1];
	}

	for (int i = toDelete; i < keyNum - 1; i++) {
		children[i] = children[i + 1];
		key[i] = key[i + 1];
	}
	children[keyNum] = NULL;
	key[keyNum] = UnavailablePair;
	keyNum--;

	return this;
}

BPlusNode* BPlusNode::insertKey(KeyType keyValue) {
	int i;
	for (i = keyNum - 1; i >= 0; i++) {
		key[i + 1] = key[i];
		if (key[i] < keyValue) {
			break;
		}
	}
	key[keyNum] = keyValue;
	keyNum++;
	return this;
}

BPlusNode* BPlusNode::removeKey(int toDelete) {
	for (int i = toDelete; i < keyNum - 1; i++) {
		key[i] = key[i + 1];
	}
	key[keyNum - 1] = UnavailablePair;
	keyNum--;
	return this;
}

bool BPlusNode::isLeaf() {
	if (children[0] != NULL) {
		return false;
	}
	else {
		return true;
	}
}


BPlusNode* BPlusNode::recursiveDestroy() {
	for (int i = 0; i < keyNum; i++) {
		if (children[i]->isLeaf()) {
			delete children[i];
		}
		else {
			children[i]->recursiveDestroy();
			children[i] = NULL;
		}
	}
	return this;
}

void BPlusNode::recursiveTravel() {
	for (int i = 0; i < keyNum; i++) {
		if (children[i]->isLeaf()) {
			children[i]->printNode();
		}
		else {
			children[i]->recursiveTravel();
		}
	}
	return;
}
	
void BPlusNode::printNode() {
	for (int i = 0; i < keyNum; i++) {
		cout << key[i];
	}
	cout << endl;
	return;
}



BPlusNode* BPlusNode::findSibling(BPlusNode* Parent, int thisNo) {
	BPlusNode* sibling = NULL;
	if (thisNo == 0) {
		if (Parent->children[1]->keyNum < M) {
			sibling = Parent->children[1];
		}
	}
	else if (Parent->children[thisNo - 1]->keyNum < M)
		sibling = Parent->children[thisNo - 1];
	else if (thisNo + 1 < Parent->keyNum && Parent->children[thisNo + 1]->keyNum < M) {
		sibling = Parent->children[thisNo + 1];
	}

	return sibling;
}


BPlusNode* BPlusNode::findSiblingOverHalf(BPlusNode* Parent, int thisNo, int& resultNo) {
	BPlusNode* sibling = NULL;

	if (thisNo == 0) {
		if (Parent->children[1]->keyNum > M/2) {
			sibling = Parent->children[1];
			resultNo = 1;
		}
	}
	else if (Parent->children[thisNo - 1]->keyNum > M / 2) {
		sibling = Parent->children[thisNo - 1];
		resultNo = thisNo - 1;
	}
	else if (thisNo + 1 < Parent->keyNum && Parent->children[thisNo + 1]->keyNum < M) {
		sibling = Parent->children[thisNo + 1];
		resultNo = thisNo + 1;
	}
	
	return sibling;
}


BPlusNode* BPlusNode::moveData(BPlusNode* parent, BPlusNode* from, BPlusNode* to, int fPos, int n) {
	KeyType TmpKey;
	BPlusNode* Child;
	int j, fromInFront;

	fromInFront = 0;
	if (from->key[0] < to->key[0])
		fromInFront = 1;

	j = 0;
	if (fromInFront) {//from在to前面，即tPos = fPos + 1 时
		if (!from->isLeaf()) {
			while (j < n) {
				Child = from->children[from->keyNum - 1];
				from->removeNode(from->keyNum - 1);
				to->insertNode(Child, 0);//插入到最前面的位置
				
				j++;
			}
		}
		else {
			while (j < n) {
				TmpKey = from->key[from->keyNum - 1];
				from->removeKey(from->keyNum - 1);
				parent->key[fPos] = from->key[0];
				to->insertKey(TmpKey);
				parent->key[fPos + 1] = to->key[0];

				j++;
			}
		}
		parent->key[fPos + 1] = to->key[0];
		// 将树叶节点重新连接
		if (from->keyNum > 0)
			from->findRightMost()->next = to->findLeftMost();

	}
	else {//from在to后面，即tPos = fPos - 1时，此时把from的前几个数据移至to的最后几位
		if (!from->isLeaf()) {
			while (j < n) {
				Child = from->children[0];
				from->removeNode(0);
				to->insertNode(Child, to->keyNum);
				j++;
			}
		}
		else {
			while (j < n) {
				TmpKey = from->key[0];
				from->removeKey(0);
				to->insertKey(TmpKey);
				j++;
			}
		}
		parent->key[fPos] = from->key[0];
		if (from->keyNum > 0)
			to->findRightMost()->next = to->findLeftMost();
	}

	return parent;
}


BPlusNode* BPlusNode::splitNode(BPlusNode* Parent, BPlusNode* focus, int thisNo) {
	int j, k, Limit;
	BPlusNode* NewNode;

	NewNode = new BPlusNode();

	k = 0;
	j = focus->keyNum / 2;
	Limit = focus->keyNum;
	while (j < Limit) {
		if (!focus->isLeaf()) {
			NewNode->children[k] = focus->children[j];
			focus->children[j] = NULL;
		}
		NewNode->key[k] = focus->key[j];
		focus->key[j] = UnavailablePair;
		NewNode->keyNum++; focus->keyNum--;
		j++; k++;
	}

	if (Parent != NULL)
		Parent->insertNode(NewNode, thisNo + 1);
	else {
		// 如果是X是根，那么创建新的根并返回
		Parent = new BPlusNode();
		Parent->insertNode(focus, 0);
		Parent->insertNode(NewNode, 1);
		return Parent;
	}

	return focus;
}

BPlusNode* BPlusNode::mergeNode(BPlusNode* parent, BPlusNode* less, BPlusNode* more, int lPos) {
	int Limit;

	if (more->keyNum > LIMIT_M_2) {
		// 从S中移动一个元素到X中
		moveData(parent, less, more, lPos, 1);
	}
	else {
		//将X全部元素移动到S中，并把X删除
		Limit = less->keyNum;
		moveData(parent, less, more, lPos, Limit);
		parent->removeNode(lPos);
		delete less;
		less = NULL;
	}

	return parent;
}



//tPos是T在Parent里的位置
BPlusNode* BPlusNode::recursiveInsert(BPlusNode* T, KeyType keyValue, int tPos, BPlusNode* Parent) {
	int j, Limit;
	BPlusNode* Sibling;
	j = 0;
	while (j < T->keyNum && keyValue >= T->key[j]) {
		if (keyValue == T->key[j]) //重复值不插入
			return T;
		j++;
	}
	if (j != 0 && T->children[0] != NULL)
		j--;


	if (T->isLeaf()) {
		T->insertKey(keyValue);
		if (Parent != NULL)
			Parent->key[tPos] = T->key[0];	
	}
	else
		T->children[j] = recursiveInsert(T->children[j], keyValue, j, T);

	Limit = M;

	if (T->keyNum > Limit) {//如果需要分裂节点
		if (Parent == NULL) {
			T = splitNode(Parent, T, tPos);
		}
		else {
			Sibling = findSibling(Parent, tPos);
			if (Sibling != NULL) {//将T的一个元素（Key或者Child）移动的Sibing中
				moveData(Parent, T, Sibling, tPos, 1);
			}
			else {// 分裂节点
				T = splitNode(Parent, T, tPos);
			}
		}
	}

	if (Parent != NULL)
		Parent->key[tPos] = T->key[0];

	return T;
}

BPlusNode* BPlusNode::recursiveRemove(BPlusNode* parent, KeyType keyValue, int fPos, BPlusNode* focus) {
	int j, NeedAdjust;
	BPlusNode* Sibling, *Tmp;

	Sibling = NULL;

	// 查找分支
	j = 0;
	while (j < focus->keyNum && keyValue >= focus->key[j]) {
		if (keyValue == focus->key[j])
			break;
		j++;
	}

	if (focus->isLeaf()) { 
		if (keyValue != focus->key[j] || j == focus->keyNum)//没找到
			return focus;
	}
	else
		if (j == focus->keyNum || keyValue < focus->key[j]) j--;

	if (focus->isLeaf()) { //树叶
		focus->removeKey(j);
		parent->key[fPos] = focus->key[0];
	}
	else {
		focus->children[j] = recursiveRemove(focus, keyValue, j, focus->children[j]);
	}

	NeedAdjust = 0;
	if (parent == NULL && focus->children[0] != NULL && focus->keyNum < 2)//树的根或者是一片树叶，或者其儿子数在2到M之间
		NeedAdjust = 1;
	else if (parent != NULL && focus->children[0] != NULL && focus->keyNum < LIMIT_M_2)//除根外，所有非树叶节点的儿子数在M/2到M之间,向上取整
		NeedAdjust = 1;
	else if (parent != NULL && focus->children[0] == NULL && focus->keyNum < LIMIT_M_2)//（非根）树叶中关键字的个数也在[M/2]和M之间
		NeedAdjust = 1;

	//调整节点
	if (NeedAdjust) {
		if (parent == NULL) { //根
			if (focus->children[0] != NULL && focus->keyNum < 2) {
				Tmp = focus;
				focus = focus->children[0];
				delete Tmp;
				return focus;
			}

		}
		else {
			// 查找兄弟节点，其关键字数目大于M/2
			Sibling = findSiblingOverHalf(parent, fPos, j);
			if (Sibling != NULL) {
				moveData(parent, Sibling, focus, j, 1);
			}
			else {
				if (fPos == 0)
					Sibling = parent->children[1];
				else
					Sibling = parent->children[fPos - 1];

				parent = mergeNode(parent, focus, Sibling, fPos);
				focus = parent->children[fPos];
			}
		}

	}
	return focus;
}


BPlusNode* BPlusNode::findKey(KeyType keyValue) {
	int i;

	for (i = 0; i < keyNum; i++) {
		if (children[i]->key[0] > keyValue)
			break;
	}
	i--;

	if (children[i]->isLeaf()) {
		for (int j = 0; j < children[i]->keyNum; j++) {
			if (children[i]->key[j] == keyValue) {
				return children[i];
			}
		}
	}
	else {
		return children[i]->findKey(keyValue);
	}
	
}






/////////////////// B+tree /////////////////


BPlusTree::BPlusTree() {
	root = new BPlusNode();
}

BPlusTree::BPlusTree(BPlusTree& T) {
	copyTree(T);
}


BPlusTree::~BPlusTree() {
	clearTree();
}


void BPlusTree::treeInsert(KeyType keyValue) {
	root = BPlusNode::recursiveInsert(root, keyValue, Unavailable, NULL);
	return;
}


void BPlusTree::clearTree() {
	root->recursiveDestroy();
	return;
}


void BPlusTree::treeRemove(KeyType keyValue) {
	root = BPlusNode::recursiveRemove(NULL, keyValue, 0, root);
	return;
}

BPlusNode* BPlusTree::treeSearch(KeyType keyValue) {
	return root->findKey(keyValue);
}


void BPlusTree::copyTree(BPlusTree& T) {



	return;
}


void BPlusTree::travel() {
	root->recursiveTravel();
	cout << endl;
}


