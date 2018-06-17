#include "pair.h"




Pair::Pair() {
	key = INT_MIN;
	position = INT_MIN;
}

Pair::Pair(double keyValue, int positionValue) {
	key = keyValue;
	position = positionValue;
}

Pair::Pair(const Pair& p) {
	key = p.key;
	position = p.position;
}

Pair::~Pair() {
	//什么也不做
}

Pair& Pair::operator=(const Pair& p) {
	this->key = p.key;
	this->position = p.position;
	return *this;
}

bool Pair::operator==(Pair& p) {
	if (key == p.key) {
		return true;
	}
	else {
		return false;
	}
}


bool Pair::operator<(Pair& p) {
	if (key < p.key) {
		return true;
	}
	else {
		return false;
	}
}


bool Pair::operator>(Pair& p) {
	if (key > p.key) {
		return true;
	}
	else {
		return false;
	}
}
bool Pair::operator!=(Pair& p) {
	if (*this == p) {
		return false;
	}
	else {
		return true;
	}
}
bool Pair::operator>=(Pair& p) {
	if (*this > p || *this == p) {
		return true;
	}
	else {
		return false;
	}
}
bool Pair::operator<=(Pair& p) {
	if (*this < p || *this == p) {
		return true;
	}
	else {
		return false;
	}
}


ostream & operator<<(ostream &out, Pair p) {
	out << p.key << "," << p.position << ";";
	return out;
}




