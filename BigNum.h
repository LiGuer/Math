/*
Copyright 2020-2021 LiGuer. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
	http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#ifndef BIGNUM_H
#define BIGNUM_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <algorithm>
/*################################################################################################
[大数类 Big Number]
	[核心数据]
bool	sign = 1;
int8u*	data = NULL;
int64	byte = 0;
	[基础函数]
################################################################################################*/
class BigNum
{
public:
	typedef long long int64;
	typedef unsigned char int8u;
#define BIGNUM_AUTOREALLOC 0
/******************************************************************************
*                    核心数据
*	特征: 堆叠顺序小低大高, 补码形式
******************************************************************************/
	bool	sign = 1;
	int8u*	data = NULL;
	int64	byte = 0;
/******************************************************************************
*                    基础函数
-------------------------------------------------------------------------------
// 基础函数
BigNum				()						//构造,析构
BigNum				(int64 _byte, bool _sign = 1)
BigNum				(const char* input)
~BigNum				()
BigNum& alloc		(int64 _byte)			//分配内存
BigNum& Realloc		(int64 _byte)
BigNum& zero		() 						//归零
BigNum& zero		(int64 _byte)
// 索引,赋值,输出
int8u&	operator[]	(int64 index) 			//索引
int8u&	operator()	(int64 index)
BigNum& operator=	(BigNum& input)  		//赋值
BigNum& operator=	(int64 input)
BigNum& operator=	(const char* input)
int usefulByte		()
int usefulBit		()
char* toStr			(int8u base = 10)		//输出字符串
// 位运算: 补与或非异或左移右移 [ & | ~ ^ << >> ]
BigNum&	comple		()  					//补码
BigNum&	icomple		()
BigNum	operator&	(BigNum& a)  			//与 &
BigNum&	operator&=	(BigNum& a)
BigNum	operator|	(BigNum& a) 			//或 |
BigNum& operator|=	(BigNum& a)
BigNum& operator~	() 						//非 ~
BigNum& operator^	(BigNum& a) 			//异或 ^
BigNum& operator^=	(BigNum& a)
BigNum& operator<<=	(int n)  				//左移右移 << >> 
BigNum& operator>>=	(int n)
// 比较运算: 大小等 [ > < = ]
char cmp(BigNum& a) 						
bool operator> 		(BigNum& a)
bool operator>=		(BigNum& a)
bool operator< 		(BigNum& a)
bool operator<=		(BigNum& a)
bool operator==		(BigNum& a)
// 数值运算: 负加减乘除余幂 [ + - * / % pow ]
BigNum	operator-	() 						//负 -
BigNum	operator+	(BigNum& a)  			//加 +
BigNum& add			(BigNum& a, BigNum& b)
BigNum& operator+=	(BigNum& a)
BigNum& operator+=	(int64 & a)
BigNum& operator++	()
BigNum  operator- 	(BigNum& a)				//减 -
BigNum& sub			(BigNum& a, BigNum& b)
BigNum& operator-=	(BigNum& a)
BigNum& operator-=	(int64 a)
BigNum& operator--	()
BigNum  operator* 	(BigNum& a) 			//乘 *
BigNum& mul			(BigNum& a, BigNum& b)
BigNum& operator*=	(BigNum& a)
BigNum& operator*=	(int64   a)
BigNum	operator/	(BigNum& a) 			//除 /
BigNum& div			(BigNum& a, BigNum& b)
BigNum& operator/=	(BigNum& a)
BigNum& operator/=	(int64   a)
BigNum	operator%	(BigNum& a) 			//余 %
BigNum& mod			(BigNum& a)
BigNum& operator%=	(BigNum& a)
BigNum& operator%=	(int64   a)
BigNum& pow			(BigNum& a) 			// 幂 pow
BigNum& pow			(int64 n)
******************************************************************************/
	/*----------------构造/析构----------------*/
	BigNum()							{ zero(8); }
	BigNum(int64 _byte, bool _sign = 1) { zero(_byte); sign = _sign; }
	BigNum(const char* input)			{ zero(8); *this = input; }
   ~BigNum()							{ delete data; }
	/*----------------索引----------------*/
	int8u& operator[] (int64 index) { return data[index]; }
	int8u& operator() (int64 index) { return data[index]; }
	/*----------------归零----------------*/
	BigNum& alloc(int64 _byte){
		if(byte != _byte) data = (int8u*)malloc((byte = _byte) * sizeof(int8u)); 
		return *this;
	}
	BigNum& Realloc(int64 _byte) {
		realloc(data, (byte = _byte) * sizeof(int8u)); return *this;
	}
	BigNum& zero()				{ memset(data, 0, byte * sizeof(int8u)); return *this; }
	BigNum& zero(int64 _byte)	{ alloc(_byte); zero(); return *this; }
	/*----------------赋值----------------*/
	BigNum& operator= (BigNum& input) {
		sign = input.sign;
		alloc(input.byte);
		memcpy(data, input.data, sizeof(int8u) * byte);
		return *this;
	}
	BigNum& operator= (int64 input) {
		alloc(8);
		memcpy(data, &input, sizeof(char) * 8);
		return *this;
	}
	BigNum& operator= (const char* input) {
		//符号
		bool negative = 0;
		if (input[0] == '-') { negative = 1; input++; }
		//进制
		int8u base = 10;
		if (input[0] == '0' && input[1] == 'x') { base = 16; input += 2; }
		if (input[0] == '0' && input[1] == 'b') { base =  2; input += 2; }
		//数
		int8u t;
		int64 len = strlen(input);
		switch (base)
		{
		case  2: 
			zero(std::max(byte, (len + 3) / 4));
			for (int i = 0; i < byte; i++) {
				t = 0;
				for (int j = 0; j < 4 && len - 1 - i * 4 - j >= 0; j++)
					t += input[len - 1 - i * 4 - j] - '0';
				data[i] += t;
			}
			break;
		case 10:
			int64 t;
			zero(std::max(byte, (int64)(len * log(10) / log(256)) + 1));
			for (int i = 0; input[i]; i++) {
				t = input[i] - '0';
				(*this *= 10) += t;
			}
			break;
		case 16: 
			zero(std::max(byte, (len + 1) / 2));
			for (int i = len - 1; i >= 0; i--) {
					 if(input[i] >= '0' && input[i] <= '9') t = input[i] - '0';
				else if(input[i] >= 'A' && input[i] <= 'F') t = input[i] - 'A';
				else if(input[i] >= 'a' && input[i] <= 'f') t = input[i] - 'a';
				if  ((len - 1 - i) % 2 == 1) t *= 16;
				data[(len - 1 - i) / 2] += t;
			}
			break;
		}
		if (negative) data[byte - 1] |= 0x80; comple();
		return *this;
	}
	/*----------------转字符串----------------*/
	int usefulByte() {
		int num = byte - 1;
		if (sign && data[byte - 1] == 0x80) num--;
		while (num > 0 && data[num] == 0)   num--;	//找到data的最高位
		return num + 1;
	}
	int usefulBit() {
		int num1 = usefulByte(), num2 = 7;
		while (num2 > 0 && !((data[num1 - 1] >> num2) & 1))  num2--;
		return (num1 - 1) * 8 + num2 + 1;
	}
	char* toStr(int8u base = 10) {
		icomple();
		char* str = NULL;
		int64 num = usefulByte();
		bool negative = 0; 
		if (sign && data[byte - 1] & 0x80) negative = 1;

		switch (base)
		{
		case  2:
			str = (char*)calloc((8 * num + 1 + negative ? 1 : 0), sizeof(char));
			if (negative) { str[0] = '-'; str++; }
			for (int i = 0; i < num; i++)
				for (int j = 0; j < 8; j++)
					str[8 * i + j] = ((data[num - 1 - i] >> (7 - j)) & 1) + '0';
			break;
		case 10:		
			str = (char*)calloc((num * log(256) / log(10) + 1 + negative ? 1 : 0), sizeof(char));
			if (negative) { str[0] = '-'; str++; }
			break;
		case 16:
			str = (char*)calloc((2 * num + 1 + negative ? 1 : 0), sizeof(char));
			if (negative) { str[0] = '-'; str++; }
			for (int i = 0; i < num; i++) {
				str[2 * i]      = data[num - 1 - i] / 0x10;	
				str[2 * i + 1]  = data[num - 1 - i] % 0x10;
				str[2 * i]     += str[2 * i]     <= 9 ? '0' : 'A' - 10;
				str[2 * i + 1] += str[2 * i + 1] <= 9 ? '0' : 'A' - 10;
			}
			break;
		}
		comple(); if (negative) str--; return str;
	}
	/*----------------补码----------------*/
	BigNum& comple() {
		if (sign && data[byte - 1] & 0x80) {
			for (int i = 0; i < byte; i++) data[i] = ~data[i];
			data[byte - 1] ^= 0x80;
			for (int i = 0; i < byte; i++) {
				if (data[i] == 0xFF) data[i] = 0;
				else { data[i] += 1; break; }
			}
		} return *this;
	}
	BigNum& icomple() {
		if (sign && data[byte - 1] & 0x80) {
			for (int i = 0; i < byte; i++) {
				if (data[i] == 0) data[i] = 0xFF;
				else { data[i] -= 1; break; }
			}
			for (int i = 0; i < byte; i++) data[i] = ~data[i];
			data[byte - 1] ^= 0x80;
		} return *this;
	}
	/*----------------与或非 异或 [ & | ~ ^ ]----------------*/
	BigNum operator& (BigNum& a) {

	}
	BigNum& operator&= (BigNum& a) {
		for (int i = 0; i < byte; i++) data[i] &= a.data[i];
		return *this;
	}
	BigNum operator| (BigNum& a) {

	}
	BigNum& operator|= (BigNum& a) {
		for (int i = 0; i < byte; i++) data[i] |= a.data[i];
		return *this;
	}
	BigNum& operator~ () {
		for (int i = 0; i < byte; i++) data[i] = ~data[i];
		return *this;
	}
	BigNum& operator^ (BigNum& a) {

	}
	BigNum& operator^= (BigNum& a) {
		for (int i = 0; i < byte; i++) data[i] ^= a.data[i];
		return *this;
	}
	/*----------------左移右移 [ << >> ]----------------*/
	BigNum& operator<<= (int n){
		int8u c1 = 0, c2 = 0, n1 = n / 8, n2 = n % 8;
		for (int64 i = 0; i < byte; i++) {
			c2 = (data[i] >> 8 - n2);
			data[i] <<= n2;
			data[i] += c1;
			c1 = c2;
		} 
		for (int64 i = byte - 1; i >= 0; i--) 
			data[i] = i >= n1 ? data[i - n1] : 0;
		return *this;
	}
	BigNum& operator>>= (int n) {
		int8u c1 = 0, c2 = 0, n1 = n / 8, n2 = n % 8;;
		for (int64 i = byte - 1; i >= 0; i--) {
			c2 = (data[i] << 8 - n2);
			data[i] >>= n2;
			data[i] += c1;
			c1 = c2;
		}
		for (int64 i = 0; i < byte; i++)
			data[i] = i + n1 < byte ? data[i + n1] : 0;
		return *this;
	}
	/*----------------比较 [ > < = ]----------------*/
	char cmp(BigNum& a) {
		int t1 = usefulByte(), t2 = a.usefulByte();
		if (t1 > t2) return  1;
		if (t1 < t2) return -1;
		char t;
		for (int i = t1 - 1; i >= 0; i--) {
			t = memcmp(data + i, a.data + i, 1);
			if (t != 0) return t;
		} return 0;
	}
	bool operator>  (BigNum& a) { return cmp(a) >  0 ? true : false; }
	bool operator>= (BigNum& a) { return cmp(a) >= 0 ? true : false; }
	bool operator<  (BigNum& a) { return cmp(a) <  0 ? true : false; }
	bool operator<= (BigNum& a) { return cmp(a) <= 0 ? true : false; }
	bool operator== (BigNum& a) { return cmp(a) == 0 ? true : false; }
	/*----------------负 [ - ]----------------*/
	BigNum operator- () {
		BigNum ans; ans = *this;
		if (sign) {
			if (ans.data[byte - 1] & 0x80) { ans.icomple(); ans.data[byte - 1] ^= 0x80; }
			else { ans.data[byte - 1] |= 0x80;  ans.comple(); }
		}
		return ans;
	}
	/*----------------加减乘除余幂 [ + - * / % pow ]----------------*/
	// +
	BigNum operator+ (BigNum& a) {
		BigNum tmp(byte);
		int8u c = 0; int t;	//c: 进位
		for (int64 i = 0; i < byte; i++) {
			t = data[i] + a[i] + c;
			tmp[i] = t % 0x100;
			c = t > 0xFF ? 1 : 0;
		}
		return tmp;
	}
	BigNum& add(BigNum& a, BigNum& b) {
		int8u c = 0; int t;	//c: 进位
		for (int64 i = 0; i < a.byte; i++) {
			t = a[i] + b[i] + c;
			data[i] = t % 0x100;
			c = t > 0xFF ? 1 : 0;
		}
		return *this;
	}
	BigNum& operator+= (BigNum& a) { return add(*this, a); }
	BigNum& operator+= (int64 & a) { return *this; }
	BigNum& operator++ () { 
		for (int i = 0; i < byte; i++) {
			if (data[i] == 0xFF) data[i] = 0;
			else { data[i] += 1; break; }
		} return *this;
	}
	// -
	BigNum  operator-  (BigNum& a) {
		BigNum tmp(byte);
		int8u c = 0; int t;		//借位
		for (int64 i = 0; i < byte; i++) {
			t = data[i] - a[i] - c;
			tmp[i] = t > 0 ? t : t + 0x100;
			c = t >= 0 ? 0 : 1;
		}
		return tmp;
	}
	BigNum& sub(BigNum& a, BigNum& b) { 
		int8u c = 0; int t;		//借位
		for (int64 i = 0; i < byte; i++) {
			t = a[i] - b[i] - c; 
			data[i] = t > 0 ? t : t + 0x100;
			c = t >= 0 ? 0 : 1;
		}return *this;
	}
	BigNum& operator-= (BigNum& a) { return sub(*this, a); }
	BigNum& operator-= (int64 a) { return *this; }
	BigNum& operator-- () {
		for (int i = 0; i < byte; i++) {
			if (data[i] == 0) data[i] = 0xFF;
			else { data[i] -= 1; break; }
		} return *this;
	}
	// *
	BigNum  operator*  (BigNum& a) {
		BigNum t, ans(byte); t = a;
		for (int64 i = 0; i < byte; i++)
			for (int j = 0; j < 8; j++) {
				if (data[i] & (1 << j)) ans += t;
				t <<= 1;
			}
		return ans;
	}
	BigNum& mul(BigNum& a, BigNum& b) {
		BigNum t, ans(byte); t = b;
		for (int64 i = 0; i < byte; i++) 
			for (int j = 0; j < 8; j++) {
				if (a.data[i] & (1 << j)) ans += t; 
				t <<= 1;
			}
		return *this = ans;
	}
	BigNum& operator*= (BigNum& a) { return mul(*this, a); }
	BigNum& operator*= (int64   a) { return *this; }
	// /
	BigNum operator/ (BigNum& a) {
		int n1 = usefulBit(), n2 = a.usefulBit();
		BigNum t1, t2, ans(byte); t1 = *this, t2 = a; 
		t2 <<= n1 - n2; 
		for (int i = n1 - 1; i >= n2 - 1; i--) {
			if (t1 >= t2) {
				int i_ = i - n2 + 1;
				ans[i_ / 8] |= (1 << (i_ % 8));
				t1 -= t2;
			}
			t2 >>= 1;
		}
		return ans;
	}
	BigNum& div(BigNum& a, BigNum& b) { return *this; }
	BigNum& operator/= (BigNum& a) { return div(*this, a); }
	BigNum& operator/= (int64   a) { return *this; }
	// %
	BigNum operator% (BigNum& a) { 
		int n1 = usefulBit(), n2 = a.usefulBit();
		BigNum t1, t2; t1 = *this, t2 = a;
		t2 <<= n1 - n2;
		for (int i = n1 - 1; i >= n2 - 1; i--) {
			if (t1 >= t2) t1 -= t2;
			t2 >>= 1;
		}
		return t1;
	}
	BigNum& mod(BigNum& a) { }
	BigNum& operator%= (BigNum& a) { return mod(a); }
	BigNum& operator%= (int64   a) { return *this; }
	// 幂
	BigNum& pow(BigNum& a) {

	}
	BigNum& pow(int64 n) {
		if (n == 0) {
			zero(); data[0] = 1; return *this;
		}
		BigNum t; t = *this;
		while (--n) *this *= t;
		return *this;
	}
};

/*################################################################################################
[大浮点数类 Big Float]

################################################################################################*/
class BigFloat {
public:
	typedef long long		int64;
	typedef unsigned char	int8u;
/******************************************************************************
*                    核心数据
******************************************************************************/
BigNum num, index;
/******************************************************************************
*                    核心操作
******************************************************************************/
BigFloat() { ; }
BigFloat(double x) {

}
BigFloat(const char* x) {

}
};
#endif