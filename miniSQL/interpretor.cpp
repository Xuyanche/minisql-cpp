#include "interpretor.h"


static string englishLowerCharacter = "abcdefghijklmnopqrstuvwxyz";
static string englishUpperCharacter = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static string number = "1234567890";

/*


string getInst() {
	string inst;
	getline(cin, inst, ';');
	inst = stringCheck("inst", inst);
	return inst;
}
*/
string stringCheck(string type, string toBeChecked) {
	int i;
	string allow;

	if (type == "tableName" || type == "indexName" || type == "attrName") {
		allow = englishLowerCharacter + englishUpperCharacter + number + "_";
		for (i = 0; i < toBeChecked.length(); i++) {
			if (allow.find(toBeChecked[i]) == string::npos) {
				cout << "invalid character found" << endl;
				toBeChecked.erase(i, 1);
				i--;
			}
		}
	}
	else if (type == "inst") {
		allow = englishLowerCharacter + englishUpperCharacter + number + "_(),*+-/|&;\n\t ";
		string valid = englishLowerCharacter + englishUpperCharacter + number;
		for (i = 0; i < toBeChecked.length(); i++) {
			if (allow.find(toBeChecked[i]) == string::npos) {
				toBeChecked.erase(i, 1);
				cout << "invalid character found" << endl;
				i--;
			}
		}
		//replace \n and \t
		for (i = 0; i < toBeChecked.length(); i++) {
			if (toBeChecked[i] == '\n' || toBeChecked[i] == '\t') {
				toBeChecked.replace(i, 1, " ");
			}
		}
		//delete " "
		for (i = 0; i < toBeChecked.length(); i++) {
			if (toBeChecked[i] == ' ') {
				if (i == 0 || i == toBeChecked.length() - 1) {
					//cout << i << endl;
					toBeChecked.erase(i, 1);
					//cout << toBeChecked << endl;
					i--;
				}
				else if (valid.find(toBeChecked[i - 1]) == string::npos || valid.find(toBeChecked[i + 1]) == string::npos) {
					toBeChecked.erase(i, 1);
					i--;
				}
			}
		}
	}


	return toBeChecked;
}

/*
1)�������ݿ⣺00 ���ݿ�����
2)ɾ�����ݿ⣺01 ���ݿ�����
3)������10 ���� ����1 ����1�������CHAR��ֻ��������ĳ��ȣ� �Ƿ�ΪUNIQUE����Ϊ1,��Ϊ0������2 ����2 �Ƿ�ΪUNIQUE���� # PRIMARYKEY���������ÿո��������
4)ɾ����11 ������
5)����������20 ������ ���� �����б������ж�����ÿո��������
6)ɾ��������21 ��������
7)ѡ����䣺30 # ���� # ѡ��������б�(���Ϊ*,��洢��Ϊ*) # ����1 # ����2 #�����������ڲ�û�пո񣩣�
8)�����¼��40 ���� # ��¼(ֻ��һ����¼����������˳����','�������м�û�пո�) ��
9)ɾ����¼��41 ���� # ����1 # ����2 # �����������ڲ�û�пո񣩣�
10)�˳�ϵͳ��50��
11)ִ�нű��ļ���"99"����Ϊ�����Ĺ������Ѿ�������ִ�еĺ��������ݸ�APIʱ�Ѿ�ִ�����ˣ�
PS: ����������һ���ո���������еġ�#���͡���������Ҳ���пո�ģ������Կ���splite����������

***** # , or other things like them have no space besides them *****


B+���м��ṹ������£�
*/


/*
string interpreter(string inst) {
	

	string result, instcopy = inst;
	string temp = getNextWord(instcopy);
	if (temp == "create") {
		result = createInterpret(instcopy);
	}
	else if (temp == "select") {
		result = selectInterpret(instcopy);
	}
	else if (temp == "insert") {
		//insertInterpret(instcopy);
	}
	else if (temp == "drop") {
		result = dropInterpret(instcopy);
	}
	else if (temp == "delete") {
		result = deleteInterpret(instcopy);
	}

	//cout << result << endl;
	return result;
}



string selectInterpret(string inst) {
	string result, temp, instcopy = inst;
	result = "30 ";
	while (1) {
		temp = getNextWord(instcopy);
		if (temp == "from") {
			break;
		}
		else {
			result = result + temp + " ";
		}
	}
	return inst;
}

string createInterpret(string inst) {
	string temp, seperator, result;
	string instcopy = inst;
	string primaryKeyInfo, lastName;
	int counter = 0;
	temp = getNextWord(instcopy);
	if (temp == "table") {
		cout << "inst create table" << endl;
		result = "10 " + getNextWord(instcopy);
	}
	else if (temp == "index") {
		cout << "inst create index" << endl;
		result = "20 " + getNextWord(instcopy);
	}
	else if (temp == "database") {
		cout << "inst create database" << endl;
		result = "00 " + getNextWord(instcopy);
		return result;
	}
	else {
		cout << "unkown inst" << endl;
		return "99";
	}

	while (1) {
		temp = getNextWord(instcopy, seperator);

		if (temp == "primary" || temp == "key") {
			if (temp == "primary") {
				primaryKeyInfo = "#" + lastName;
			}
			continue;
		}


		if (counter == 0) { //name follows
			lastName = temp;
			result = result + " " + temp;
		}
		else if (counter == 2) { //uniqueness follows
			if (temp == "unique") {
				result.append(" 1");
			}
			else {
				result.append(" 0");
			}
		}
		else if (counter == 1) { //typename follows
			if (temp == "int" || temp == "float") {
				result = result + " " + temp;
			}
			else if (temp == "char") {
				cout << "in char" << endl;
				result = result + " " + getNextWord(instcopy);
			}
		}
		else if (temp == "") {

		}
		else { //counter is not 1,2,3
			cout << "error create 2" << endl;
			cout << result << endl;
			return "99";
		}
		//cout << "counter" << counter << endl;
		counter++;
		if (seperator == "," || temp == "") { //see if uniqueness is skiped
			if (counter == 2) {
				result.append(" 0");
			}
			else if (counter == 0 || counter == 1) {
				cout << "error create 1" << endl;
				//cout << result << endl;
				return "99";
			}
			if (temp == "") {
				break;
			}
			counter = 0;
		}
	}
	result.append(primaryKeyInfo);
	return result;
}
*/



string getNextWord(string& readString, string& seperator) {
	string allow = englishLowerCharacter + englishUpperCharacter + number + "_.";
	int temp = readString.find_first_not_of(allow);
	string word;

	//delete not allowed chracter in the front
	while (allow.find(readString[0]) == string::npos) {
		readString.erase(0, 1);
	}


	if (temp == string::npos) {
		word = readString;
		readString = "";
		seperator = "s";
		return word;
	}
	word = readString.substr(0, temp);
	//cout << "2" << endl;
	if (temp + 1 < readString.length() && temp > 0) {
		seperator = readString[temp];
		readString = readString.substr(temp + 1, string::npos);
		
	}
	//cout << word << endl;
	return word;
}


string getNextWord(string& readString) {
	string allow = englishLowerCharacter + englishUpperCharacter + number + "_.";
	string word;
	int temp = readString.find_first_not_of(allow);

	//delete not allowed chracter in the front
	while (allow.find(readString[0]) == string::npos) {
		readString.erase(0, 1);
	}
	if (temp == string::npos) {
		word = readString;
		readString = "";
		return word;
	}
	word = readString.substr(0, temp);
	//cout << "1" << endl;
	readString = readString.substr(temp + 1, string::npos);
	//cout << word << endl;
	return word;
}


string getNextWord(string& readString, string delim, string& seperator) {
	string word;
	int temp = readString.find_first_of(delim);

	if (temp == string::npos) {
		word = readString;
		readString = "";
		seperator = "s";
		return word;
	}
	else {
		seperator = readString.substr(temp, 1);
		word = readString.substr(0, temp);
		readString = readString.substr(temp + 1, string::npos);
	}
	return word;
}























/*
void insertInterpret(string inst) {
	//insert TableName value(attr1, attr2, attr3);
	string instcopy = inst, temp;
	string result = "40 ";

	temp = getNextWord(instcopy);
	result = result + temp + "#";

	temp = getNextWord(instcopy);
	temp = getNextWord(instcopy);

	while (temp != "") {
		result = result + " " + temp;
		temp = getNextWord(instcopy);
	}
	


	return;

}
*/


void distribute(string opcode) {
	

	string opcopy = opcode;


	return;
}





