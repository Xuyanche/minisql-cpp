#include "interpretor.h"


static string englishLowerCharacter = "abcdefghijklmnopqrstuvwxyz";
static string englishUpperCharacter = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static string number = "1234567890";


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
		allow = englishLowerCharacter + englishUpperCharacter + number + " ,().<>=*;";
		string valid = englishLowerCharacter + englishUpperCharacter + number + "_*.";
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
	else if (type == "value") {
		allow = number + ".";
		for (i = 0; i < toBeChecked.length(); i++) {
			if (allow.find(toBeChecked[i]) == string::npos) {
				cout << "invalid character found" << endl;
				toBeChecked.erase(i, 1);
				i--;
			}
		}
	}


	return toBeChecked;
}

/*
1)创建数据库：00 数据库名；
2)删除数据库：01 数据库名；
3)创建表：10 表名 属性1 类型1（如果是CHAR则只保存数组的长度） 是否为UNIQUE（是为1,否为0）属性2 类型2 是否为UNIQUE…… # PRIMARYKEY的列名（用空格隔开）；
4)删除表：11 表名；
5)创建索引：20 索引名 表名 属性列表（可能有多个，用空格隔开）；
6)删除索引：21 索引名；
7)选择语句：30 # 表名 # 选择的属性列表(如果为*,则存储的为*) # 条件1 # 条件2 #……（条件内部没有空格）；
8)插入记录：40 表名 # 记录(只有一条记录，按照属性顺序，用','隔开，中间没有空格) ；
9)删除记录：41 表名 # 条件1 # 条件2 # ……（条件内部没有空格）；
10)退出系统：50；
11)执行脚本文件："99"（因为解析的过程中已经调用了执行的函数，传递给API时已经执行完了）
PS: 所有属性用一个空格隔开，其中的“#”和“，”两边也是有空格的，故属性可用splite函数分离获得

***** # , or other things like them have no space besides them *****


B+树中间块结构设计如下：
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
	string allow = englishLowerCharacter + englishUpperCharacter + number + "_.*";
	string word;

	//delete not allowed chracter in the front
	while (allow.find(readString[0]) == string::npos) {
		readString.erase(0, 1);
	}

	int temp = readString.find_first_not_of(allow);
	if (temp == string::npos) {
		word = readString;
		readString = "";
		seperator = "s";
		return word;
	}
	word = readString.substr(0, temp);

	if (temp == readString.length() - 1) {
		seperator = readString[temp];
		readString = "";
		return word;
	}
	
	//cout << "2" << endl;
	if (temp + 1 < readString.length() && temp > 0) {
		seperator = readString[temp];
		readString = readString.substr(temp + 1, string::npos);
		
	}
	//cout << word << endl;
	return word;
}

string getNextWord(string& readString) {
	string allow = englishLowerCharacter + englishUpperCharacter + number + "_.*";
	string word;

	//delete not allowed chracter in the front
	while (allow.find(readString[0]) == string::npos) {
		readString.erase(0, 1);
	}
	int temp = readString.find_first_not_of(allow);
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

string peekNextWord(string readString) {
	string allow = englishLowerCharacter + englishUpperCharacter + number + "_.*";
	string word;
	int temp = readString.find_first_not_of(allow);

	if (temp == string::npos) {
		word = readString;
		readString = "";
		return word;
	}
	word = readString.substr(0, temp);
	return word;
}




string interpret(string input) {
	input = stringCheck("inst", input);
	string opcode, temp, seperator;
	string firstword = getNextWord(input), secondword, main;
	vector<string> attrname;
	if (firstword == "create") {
		secondword = getNextWord(input);
		if (secondword == "table") {
			//create table tablename(attrlist);
			opcode = "10 ";
			opcode.append(getNextWord(input));//tablename
			while (true) {
				if (input == "")
					break;
				temp = getNextWord(input);
				if (main == "")
					main = temp;
				opcode.append(" " + temp);//attrname
				attrname.push_back(temp);
				opcode.append(" " + getNextWord(input, seperator));//attrtype
				if (seperator == ",") {
					opcode.append(" 0");
					continue;
				}
				else if (peekNextWord(input) == "unique") {
					opcode.append(" 1");//unique=1
					getNextWord(input);
				}
				else if (peekNextWord(input) == "main") {
					opcode.append(" 1");//unique=1
					getNextWord(input);
					main = temp;
				}
				else {
					opcode.append(" 0");//unique=0
				}
			}

			for (int i = 0; i < attrname.size(); i++) {
				if (attrname.at(i) == main)
					main = to_string(i);
			}

			opcode.append("#" + main);
		}
		else if (secondword == "index") {
			//create index indexname on tablename(attrlist);
			opcode = "20";
			opcode.append(" " + getNextWord(input));//indexname
			getNextWord(input);//on
			opcode.append(" " + getNextWord(input));//basetablename
			while (true) {
				if (input == "")
					break;
				temp = getNextWord(input);
				opcode.append(" " + temp);//attrname
			}
		}
		else {
			opcode = "99";
		}
	}
	else if (firstword == "drop") {
		secondword = getNextWord(input);
		if (secondword == "table") {
			//drop tablename
			opcode = "11 " + input;
		}
		else if (secondword == "index") {
			//drop indexname
			opcode = "21 " + input;
		}
		else {
			opcode = "99";
		}
	}
	else if (firstword == "insert") {
		//insert tablename tuple;
		opcode = "40 " + input;
	}
	else if (firstword == "select") {
		opcode = "30";
		temp = "#";
		while (true) {
			if (peekNextWord(input) == "from" || input == "")
				break;
			if (temp.size() > 1)
				temp.append(" ");
			temp.append(getNextWord(input));//attrlist
		}
		getNextWord(input);//from
		while (true) {
			if (peekNextWord(input) == "where" || input == "")
				break;
			opcode.append(" " + getNextWord(input));//tablename
		}
		opcode.append(temp + "#");
		if (input == "")
			return opcode;
		getNextWord(input);//where
		int conditionflag = 0;
		while (true) {
			if (conditionflag == 1)
				opcode.append(" ");
			else
				conditionflag = 1;
			if (input == "")
				break;
			opcode.append(conditionInterpret(input));
			if (peekNextWord(input) == "and")
				getNextWord(input);
		}

	}
	else if (firstword == "delete") {
		opcode.append("41 " + getNextWord(input));//tablename
		int a = 0;
		while (true) {
			if (a == 0) {
				a = 1;
				opcode.append("#");
			}
			else {
				opcode.append(" ");
			}
			if (input == "")
				break;
			opcode.append(conditionInterpret(input));
		}
	}
	else {
		opcode = "99";
	}

	return opcode;

}


string conditionInterpret(string &inst) {
	//只能是一个属性和固定值比
	string operat = "<>=", instcopy;
	string attrname, lower, upper, equal1, equal2, op;
	int temp = inst.find("and");//用and分割

	if (temp == string::npos) {
		instcopy = inst;
		inst = "";
	}
	else {
		instcopy = inst.substr(0, temp);
		inst = inst.substr(temp, string::npos);
	}

	
	temp = instcopy.find_first_of(operat);
	attrname = instcopy.substr(0, temp);
	instcopy = instcopy.substr(temp, string::npos);
	temp = instcopy.find_first_not_of(operat);
	op = instcopy.substr(0, temp);
	instcopy = instcopy.substr(temp, string::npos);
	if (op == ">") {
		equal1 = "0";
		equal2 = "0";
		upper = "NOT_EXIST";
		lower = getNextWord(instcopy);
	}
	else if (op == "<") {
		equal1 = "0";
		equal2 = "0";
		lower = "NOT_EXIST";
		upper = getNextWord(instcopy);
	}
	else if (op == "<=") {
		equal1 = "0";
		equal2 = "1";
		lower = "NOT_EXIST";
		upper = getNextWord(instcopy);
	}
	else if (op == ">=") {
		equal1 = "1";
		equal2 = "0";
		upper = "NOT_EXIST";
		lower = getNextWord(instcopy);
	}
	else {
		return "";
	}

	return attrname + " " + lower + " " + upper + " " + equal1 + " " + equal2;
}


