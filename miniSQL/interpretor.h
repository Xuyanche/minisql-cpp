#ifndef INTERPRETOR_H
#define INTERPRETOR_H
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
using namespace std;

string getInst();
string stringCheck(string type, string toBeChecked);

string interpreter(string inst);
string createInterpret(string inst);
string selectInterpret(string inst);
void insertInterpret(string inst);
string dropInterpret(string inst);
string deleteInterpret(string inst);



string getNextWord(string& readString, string& seperator);
string getNextWord(string& readString);
string getNextWord(string& readString, string delim, string& seperator);





#endif // !INTERPRETOR_H
