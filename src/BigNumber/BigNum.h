#ifndef BIGNUM_H
#define BIGNUM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <vector>

using namespace std;

/*
 * Big Number
 */

class BigNum
{
public:
	typedef long long int64;
	typedef unsigned long long int64u;
	typedef unsigned int int32u;
	typedef unsigned char int8u;

	/*
	 *  核心数据
	 *	特征: 堆叠顺序小低大高, 补码形式
	 */
	bool sign = 1;
	vector<int64u> data;

	/*-------- 构造/析构 --------*/
	BigNum()							{ zero(64); }
	BigNum(int64 byte, bool _sign = 1)  { zero(byte); sign =_sign; }
	//BigNum(const char* input)			{ zero(64); *this = input; }

	/*-------- 索引 --------*/
	int64u& operator[] (int64 index) { return data[index]; }
	int64u& operator() (int64 index) { return data[index]; }

	/*-------- 归零 --------*/
	BigNum& zero() {
		int n = data.size();

		for (int i = 0; i <= n; i++)
			data[i] = 0;

		return *this;
	}

	BigNum& zero(int64 bits){
		data.clear();

		for (int i = 0; i <= bits / 64; i++)
			data.push_back(0);

		return *this;
	}

	/*--------赋值--------*/
	BigNum& operator= (BigNum& input) {
		sign = input.sign;
		data = input.data;
		return *this;
	}

	BigNum& operator= (int64 input) {
		zero();
		data[0] = input;

		return *this;
	}

	/*
	 * Basic operation 
	 */

	// compare

	bool cmp (BigNum& a);

	bool operator>  (BigNum& a);
	bool operator>= (BigNum& a);
	bool operator<  (BigNum& a);
	bool operator<= (BigNum& a);
	bool operator== (BigNum& a);
	bool operator!= (BigNum& a);

	// bit operation

	BigNum& Not (BigNum& a);
	BigNum& And (BigNum& a, BigNum& b);
	BigNum& Or  (BigNum& a, BigNum& b);
	BigNum& Xor (BigNum& a, BigNum& b);

	BigNum& operator~ ();
	BigNum& operator& (BigNum& a);
	BigNum& operator| (BigNum& a);
	BigNum& operator^ (BigNum& a);

	// number operation

	BigNum& neg(BigNum& a);
	BigNum& add(BigNum& a, BigNum& b);
	BigNum& sub(BigNum& a, BigNum& b);
	BigNum& mul(BigNum& a, BigNum& b);
	BigNum& div(BigNum& a, BigNum& b);
	BigNum& mod(BigNum& a, BigNum& b);

	BigNum& operator- ();
	BigNum& operator+ (BigNum& a);
	BigNum& operator- (BigNum& a);
	BigNum& operator* (BigNum& a);
	BigNum& operator/ (BigNum& a);
	BigNum& operator% (BigNum& a);

};


#endif