#include "buffer.h"

BufferNode::BufferNode() {
	next = NULL;
	previous = NULL;
	length = 0;
	content = new string[BUFFER_CAPACITY];
	recordNum = 0;
	dirtBit = false;
	lock = false;
}



BufferNode::BufferNode(istream& fs) {
	next = NULL;
	previous = NULL;
	length = 0;
	content = new string[BUFFER_CAPACITY];
	recordNum = 0;
	dirtBit = false;
	lock = false;
	for (recordNum; recordNum < BUFFER_CAPACITY; recordNum++) {
		getline(fs, content[recordNum], '\n');
		//cout << "reading";
		length += content[recordNum].length();
		if (fs.eof())break;
	}
	
	recordNum++;
}



BufferNode::BufferNode(BufferNode* lastNode, string toAdd) {
	next = NULL;
	previous = lastNode;
	length = toAdd.length();
	content = new string[BUFFER_CAPACITY];
	recordNum = 1;
	dirtBit = false;
	lock = false;
	content[0] = toAdd;

}

BufferNode::BufferNode(BufferNode& BN) {
	this->makeCopy(BN);
}


BufferNode::~BufferNode() {
	delete[]content;
}


void BufferNode::clear() {
	BufferNode* focus = next;

	while (focus != NULL && focus->next != NULL) {
		focus->next->previous = this;
		next = focus->next;
		delete focus;
		focus = next;
	}
	return;
}

void BufferNode::insertLog(string toAdd) {
	BufferNode* focus = next;

	if (next == NULL) {
		next = new BufferNode(this, toAdd);
		return;
	}

	while (focus != NULL && focus->next != NULL) {
		if (focus->recordNum < BUFFER_CAPACITY) {
			break;
		}
		focus = focus->next;
	}

	if (focus != NULL && focus->recordNum < BUFFER_CAPACITY ) {
		focus->content[focus->recordNum] = toAdd;
		focus->recordNum++;
		focus->length += toAdd.length();
	}
	else if (focus != NULL) {
		focus->next = new BufferNode(focus, toAdd);
	}
	return;
}

void BufferNode::deleteLog(int attrNo, string attrValue) {
	BufferNode* focus = next;
	if (next == NULL) {
		return;
	}
	while (focus != NULL) {
		for (int i = 0; i < focus->recordNum; i++) {
			if (getAttr(attrNo, focus->content[i]) == attrValue) {
				focus->content[i] = "";
			}
		}

		focus->rearrange();
		focus = focus->next;
	}

	return;
}

void BufferNode::deleteLog(BufferNode deleteHead) {
	BufferNode* focus = next, *deleteFocus = deleteHead.next;
	bool findFlag = false;
	if (next == NULL || deleteHead.next == NULL) {
		return;
	}
	
	while (deleteFocus != NULL) {
		for (int i = 0; i < deleteFocus->recordNum; i++) {
			focus = next;
			while (focus != NULL) {
				for (int j = 0; j < focus->recordNum; j++) {
					if (focus->content[j] == deleteFocus->content[i]) {
						focus->content[j] = "";
						break;
					}
				}
				focus = focus->next;
			}
		}
		deleteFocus = deleteFocus->next;
	}
	focus = next;
	while (focus != NULL) {
		focus->rearrange();
		focus = focus->next;
	}
	return;
}



string BufferNode::getContent(int position) {
	int i = 0;
	BufferNode* focus = next;
	while (focus != NULL) {
		if (i + focus->recordNum > position) {//i+recordNum是下一个节点的第一个content的下标
			return focus->content[position - i];
		}
		i += focus->recordNum;
		focus = focus->next;
	}
	return "";
}




void BufferNode::bufferRead(istream& fs) {
	BufferNode* focus = this, *temp = NULL;
	while (!fs.eof()) {
		temp = new BufferNode(fs);
		temp->previous = focus;
		focus->next = temp;
		focus = focus->next;
		temp = NULL;
	}
	return;
}

void BufferNode::bufferWrite(ostream& fs) {
	BufferNode* focus = next;
	while (focus != NULL) {
		for (int i = 0; i < focus->recordNum; i++) {
			fs << focus->content[i];
			if (focus->next == NULL && i == focus->recordNum - 1);
			else
				fs << endl;
		}
		focus = focus->next;
	}

	return;
}


long BufferNode::getOffset() {
	long offset = 0;
	BufferNode* focus = previous;
	while (focus != NULL) {
		offset += focus->length;
		focus = focus->previous;
	}
	return offset;
}


string BufferNode::getAttr(int attrNo, string s) {
	string temp, seperator;
	string delim = " \n#";
	string s1 = s;
	for (int i = 0; i <= attrNo; i++) {
		temp = getNextWord(s1, delim, seperator);
	}
	return temp;
}




int BufferNode::getTotalRecordNum() {
	BufferNode* focus = next;
	int i = 0;
	while (focus != NULL) {
		i += focus->recordNum;
		focus = focus->next;
	}

	return i;

}


void BufferNode::bufferPrint() {
	for (int i = 0; i < recordNum; i++) {
		cout << content[i] << endl;
	}
	if (next != NULL) {
		next->bufferPrint();
	}
	return;
}

int BufferNode::getBufferNum() {
	int i = 0;
	BufferNode* focus = this;
	while (focus != NULL) {
		i++;
		focus = focus->next;
	}
	return i;
}

void BufferNode::rearrange() {
	string* save;
	int i, j;
	save = new string[BUFFER_CAPACITY];
	length = 0;
	for (i = 0; i < BUFFER_CAPACITY; i++) {
		save[i] = content[i];
		length += content[i].length();
	}
	j = 0;
	for (i = 0; i < BUFFER_CAPACITY; i++) {
		if (save[i] != "") {
			content[j] = save[i];
			j++;
		}
	}
	recordNum = j;
	for (j; j < BUFFER_CAPACITY; j++) {
		content[j] = "";
	}
	delete[]save;
	return;
}





void BufferNode::makeCopy(const BufferNode& copyHead) {
	this->clear();
	BufferNode* focus = this, *copyFocus = copyHead.next, *lastFocus = NULL;

	length = copyHead.length;
	recordNum = copyHead.recordNum;
	dirtBit = copyHead.dirtBit;
	lock = copyHead.lock;
	for (int i = 0; i < BUFFER_CAPACITY; i++)
		content[i] = copyHead.content[i];
	if (copyFocus != NULL)
		next = new BufferNode();
	focus = next;
	lastFocus = this;
	while (copyFocus != NULL) {
		focus->length = copyFocus->length;
		focus->recordNum = copyFocus->recordNum;
		focus->dirtBit = copyFocus->dirtBit;
		focus->lock = copyFocus->lock;
		for (int i = 0; i < BUFFER_CAPACITY; i++)
			focus->content[i] = copyFocus->content[i];
		focus->previous = lastFocus;
		if (copyFocus->next != NULL) {
			focus->next = new BufferNode();
			lastFocus = focus;
			focus = focus->next;
			copyFocus = copyFocus->next;
		}
		else {
			break;
		}
	}

	return;
}








