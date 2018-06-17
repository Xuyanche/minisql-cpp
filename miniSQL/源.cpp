BufferNode* readFile(string FileName);

//这个函数的参数不能是dummyheader，注意。
void writeFile(BufferNode* head);

void bufferSearch(BufferNode* head, int attrNo, string attrValue, Table* result);





BufferNode* readFile(string fileName) {
	BufferNode* head, *focus;

	head = new BufferNode(fileName, 0);
	focus = head;
	while (focus->recordNum < BUFFER_CAPACITY) {
		focus->next = new BufferNode(focus);
		focus = focus->next;
	}
	return head;
}


void writeFile(BufferNode* head) {
	ofstream fs;
	fs.open(head->fileName, fstream::app);
	BufferNode *focus = head;
	while (focus != NULL) {
		for (int i = 0; i < focus->recordNum; i++) {
			fs << focus->content[1] << endl;
		}
		focus = focus->next;
	}
	fs.close();
	return;
}


//注意，这里的head指针不是一个dummyheader，这是因为在table里存储的是一个bufferNode实体
//attrNo是需要查询的属性在属性列表中的下标，从0开始
void bufferSearch(BufferNode* head, int attrNo, string attrValue, Table* result) {
	BufferNode* focus = head;
	string attrStored;

	for (int i = 0; i < focus->recordNum; i++) {
		attrStored = BufferNode::getAttr(attrNo, focus->content[i]);
		if (attrStored == attrValue) {
			result->tableInsert(focus->content[i]);
		}
	}
	return;
}


string BufferNode::getAttr(int attrNo, string s) {
	string temp;
	string s1 = s;
	for (int i = 0; i <= attrNo; i++) {
		temp = getNextWord(s1);
	}
	return temp;
}



/*
文件的格式应当是
01 tableName attr1 int 1 attr2 float 0 atr3 5 0#attr1\n
1 1.2 aaaaa\n
2 1.3 bbbbb\n
.....
*/


////
//**需要考察一下end需不需要+1**
////
static const int BUFFER_CAPACITY = 32;

BufferNode::BufferNode() {
	dirtBit = false;
	lock = false;
	next = NULL;
	previous = NULL;
	fileName = "";
	recordNum = 0;
	start = 0;
	end = 0;
	content = new string[BUFFER_CAPACITY];
}

BufferNode::BufferNode(string fileNameValue, long startValue) {
	ifstream fs;
	string setting;
	char temp[BUFFER_CAPACITY];
	int length;

	fs.open(fileNameValue, ifstream::in);
	getline(fs, setting, '\n');
	fs.ignore();


	start = startValue;
	dirtBit = false;
	lock = false;
	fileName = fileNameValue;
	previous = NULL;
	next = NULL;


	fs.open(fileName, fstream::in);
	fs.seekg(start, ios::beg); //move the get pointer to start
	recordNum = 0;
	end = start;
	while (recordNum < BUFFER_CAPACITY) {
		getline(fs, content[recordNum], '\n');
		end = end + 1 + content[recordNum].length();
		recordNum++;
	}
	recordNum++;
	fs.close();
}

BufferNode::BufferNode(BufferNode* lastNode) {
	ifstream fs;

	start = lastNode->end + 1;
	dirtBit = false;
	lock = false;
	fileName = lastNode->fileName;
	previous = lastNode;
	next = NULL;

	content = new string[BUFFER_CAPACITY];



	fs.open(fileName, fstream::in);
	fs.seekg(start, ios::beg); //move the get pointer to start
	recordNum = 0;
	end = start;
	while (recordNum < BUFFER_CAPACITY) {
		getline(fs, content[recordNum], '\n');
		end = end + 1 + content[recordNum].length();
		recordNum++;
	}
	recordNum++;
	fs.close();
}

BufferNode::BufferNode(BufferNode* lastNode, string toAdd) {
	ifstream fs;

	start = lastNode->end + 1;
	dirtBit = true;
	lock = false;
	fileName = lastNode->fileName;
	previous = lastNode;
	next = NULL;

	content = new string[BUFFER_CAPACITY];
	content[0] = toAdd;
	recordNum = 1;
	end = start + content[0].length() + 1;
}



BufferNode::~BufferNode() {
	delete[]content;
	if (next != NULL) {
		cout << "error: delete BufferNode" << endl;
		cout << "May cause memory leak" << endl;
	}
}

void BufferNode::clear() {
	BufferNode* temp = NULL;
	while (next != NULL) {
		if (next->next != NULL) {
			temp = next->next;
			delete next;
			next = temp;
		}
		else {
			delete next;
			next = NULL;
		}
	}
	return;
}


========================================================
=========================================================


Table::Table() {
	header = "";
	primary = "";
	tableName = "";
	isTemp = false;
	attrType = new string[maxAttrNum];
	attrName = new string[maxAttrNum];
	for (int i = 0; i < maxAttrNum; i++) {
		unique[i] = false;
	}

}

Table::Table(string op) {
	int i = 0;
	string temp;
	string seperator;
	attrType = new string[maxAttrNum];
	attrName = new string[maxAttrNum];
	isTemp = false;

	cout << "in create table" << endl;
	header = op;
	temp = getNextWord(op);
	temp = getNextWord(op);
	while (true) {

		if (seperator == "#") {
			temp = getNextWord(op);
			primary = temp;
			break;
		}
		temp = getNextWord(op, seperator);
		attrName[i] = temp;
		temp = getNextWord(op, seperator);
		attrType[i] = temp;
		temp = getNextWord(op, seperator);

		if (temp == "1") {
			unique[i] = true;
		}
		else if (temp == "0") {
			unique[i] = false;
		}
		i++;
	}


}

Table::~Table() {
	delete[] attrName;
	delete[] attrType;
	bufferHead.clear();
	Destroy(indexRoot);
}




void Table::writeTable() {
	ofstream fs;
	string header;
	int i = 0;
	//the header is the op
	//then follows the real value:
	//attr1,attr2,attr3,...attrN#
	//there is no return or other things in between



	if (!isTemp) {
		fs.open(tableName + ".txt", fstream::out);
		fs << header;
		fs.close();
	}
	else {
		fs.open(tableName + "temp.txt", fstream::out);
		fs << header;
		fs.close();
	}


	writeFile(bufferHead.next);




	return;

}

void Table::tablePrint() {
	int i = 0;

	cout << "tableName: " << tableName << endl;
	for (i; i < maxAttrNum; i++) {
		cout << i << " " << attrName[i] << " " << attrType[i] << " " << unique[i] << endl;
	}
	cout << "primary: " << primary << endl;
	return;
}

int Table::isAttr(string attrNameValue) {
	int i = 0;
	for (i; i < maxAttrNum; i++) {
		if (attrName[i] == attrNameValue) {
			return i;
		}
	}
	return -1;
}

void Table::tableInsert(string attrString) {
	int mainKeyValue = 0;
	stringstream ss;
	//通过一定的手段拿到主键的值
	//这部分还没有写

	Insert(indexRoot, mainKeyValue);
	bufferHead.next->appendLog(attrString);
	return;
}



Table* Table::queryOneAttr(string attrNameValue, string attrValue) {
	Table* result;
	int attrNo;
	result = new Table(header);
	result->isTemp = true;

	for (attrNo = 0; attrNo < maxAttrNum; attrNo++) {
		if (attrName[attrNo] == attrNameValue) {
			break;
		}
	}
	bufferSearch(bufferHead.next, attrNo, attrValue, result);
	bufferSearch()
		return result;
}





