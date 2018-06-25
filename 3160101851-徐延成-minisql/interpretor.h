#ifndef INTERPRETOR_H
#define INTERPRETOR_H
#include <iostream>
#include <iomanip>
#include <string>
#include<vector>
#include <sstream>
using namespace std;

string getInst();
string stringCheck(string type, string toBeChecked);

string interpret(string inst);

string conditionInterpret(string &inst);



string getNextWord(string& readString, string& seperator);
string getNextWord(string& readString);
string getNextWord(string& readString, string delim, string& seperator);
string peekNextWord(string readString);




#endif // !INTERPRETOR_H
