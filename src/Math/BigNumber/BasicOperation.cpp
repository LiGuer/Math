#include "BigNum.h"

/*
 *  Basic operation
 */

bool cmp(BigNum& a) {
	int t1 =   usefulByte(), 
		t2 = a.usefulByte();

	if (t1 > t2) return  1;
	if (t1 < t2) return -1;

	char t;

	for (int i = t1 - 1; i >= 0; i--) {
		t = memcmp(data + i, a.data + i, 1);
		if (t != 0) return t;
	}

	return 0;
}

bool BigNum::operator>  (BigNum& a) { return cmp(a) >  0 ? true : false; }
bool BigNum::operator>= (BigNum& a) { return cmp(a) >= 0 ? true : false; }
bool BigNum::operator<  (BigNum& a) { return cmp(a) <  0 ? true : false; }
bool BigNum::operator<= (BigNum& a) { return cmp(a) <= 0 ? true : false; }
bool BigNum::operator== (BigNum& a) { return cmp(a) == 0 ? true : false; }
bool BigNum::operator!= (BigNum& a) { return cmp(a) != 0 ? true : false; }

// bit operation

BigNum& BigNum::Not(BigNum& a) {}
BigNum& BigNum::And(BigNum& a, BigNum& b) {}
BigNum& BigNum::Or (BigNum& a, BigNum& b) {}
BigNum& BigNum::Xor(BigNum& a, BigNum& b) {}

BigNum& BigNum::operator~ () {}
BigNum& BigNum::operator& (BigNum& a) {}
BigNum& BigNum::operator| (BigNum& a) {}
BigNum& BigNum::operator^ (BigNum& a) {}

// number operation

BigNum& BigNum::neg(BigNum& a) {}

BigNum& BigNum::add(BigNum& a, BigNum& b) {
}

BigNum& BigNum::sub(BigNum& a, BigNum& b) {}
BigNum& BigNum::mul(BigNum& a, BigNum& b) {}
BigNum& BigNum::div(BigNum& a, BigNum& b) {}
BigNum& BigNum::mod(BigNum& a, BigNum& b) {}

BigNum& BigNum::operator- () {}
BigNum& BigNum::operator+ (BigNum& a) {}
BigNum& BigNum::operator- (BigNum& a) {}
BigNum& BigNum::operator* (BigNum& a) {}
BigNum& BigNum:: operator/ (BigNum& a) {}
BigNum& BigNum::operator% (BigNum& a) {}




#endif