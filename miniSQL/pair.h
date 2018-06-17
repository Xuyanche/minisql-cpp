#ifndef PAIR_H
#define PAIR_H
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

class Pair {
public:
	Pair();
	Pair(double keyValue, int positionValue);
	Pair(const Pair& p);
	~Pair();

	Pair& operator=(const Pair& p);
	bool operator==(Pair& p);
	bool operator>(Pair& p);
	bool operator<(Pair& p);
	bool operator!=(Pair& p);
	bool operator>=(Pair& p);
	bool operator<=(Pair& p);
	friend ostream & operator<<(ostream &out, Pair p);

	double key;
	int position;
};


#endif // !PAIR_H

