/*
Copyright 2020,2021 LiGuer. All Rights Reserved.
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
/*********************************************************************************
						大数类
*********************************************************************************/
class BigNum
{
public:
	typedef long long int64;
	typedef unsigned char int8u;
/******************************************************************************
*                    核心数据
*	Data堆叠顺序: 小低大高
******************************************************************************/
	bool sign = 0;
	int8u* data;
	int64 byte = 0;
/******************************************************************************
*                    基础函数
-------------------------------------------------------------------------------
******************************************************************************/
	BigNum() { zero(8); }
	BigNum(int64 _byte) { zero(_byte); }
	BigNum(const char* input) {
		zero(8);
		int8u t;
		if (input[0] == '-') { sign = 1; input++; }
		for (int i = 0; input[i]; i++) {
			t = input[i] - '0';
			((*this) *= 10) += t;
		}
	}
	~BigNum() { delete data; }
/******************************************************************************
*                    基础操作
-------------------------------------------------------------------------------
******************************************************************************/
	int8u operator[] (int64 index) { return data[index]; }
	/*----------------零----------------*/
	BigNum& alloc(int64 _byte){
		_byte = byte;
		data = (int8u*)malloc(byte * sizeof(int8u));
	}
	BigNum& Realloc() {
		if (byte == 0)return alloc(8);
		byte *= 2;
		realloc(data, byte * sizeof(int8u));
	}
	BigNum& zero() { sign = 0; memset(data, 0, byte * sizeof(int8u)); }
	BigNum& zero(int64 _byte) { alloc(_byte); zero(); }
	/*----------------加 [add]----------------*/
	BigNum& add(BigNum& a, BigNum& b) {
		if (a.sign == 1 && b.sign == 0) { sign = 0; return subU(b, a); }
		if (a.sign == 0 && b.sign == 1) { sign = 0; return subU(a, b); }
		sign = (a.sign == 1 && b.sign == 1) ? 1 : 0;return addU(a, b);
	}
	BigNum& addU(BigNum& a, BigNum& b) {
		int8u c = 0;	//进位标志
		int t;
		for (int64 i = 0; i < byte; i++) {
			t = a[i] + b[i] + c;
			data[i] = t % 0xFF;
			c = t / 0xFF;
		}return *this;
	}
	BigNum& operator+= (BigNum& a) {
		int8u c = 0;	//进位标志
		int t;
		for (int64 i = 0; i < byte; i++) {
			t = data[i] + a[i] + c;
			data[i] = t % 0xFF;
			c = t / 0xFF;
		}return *this;
	}
	BigNum& operator+= (int a) { }
	/*----------------减 [sub]----------------*/
	BigNum& sub(BigNum& a, BigNum& b) {
		if (a.sign == 0 && b.sign == 1) { sign = 0; return addU(a, b); }
		if (a.sign == 1 && b.sign == 0) { sign = 1; return addU(a, b); }
		sign = (a.sign == 1 && b.sign == 1) ? 1 : 0;return subU(a, b);
	}
	BigNum& subU(BigNum& a, BigNum& b) {
		int8u c = 0;		//借位标志
		int t;
		for (int64 i = 0; i < byte; i++) {
			t = a[i] - b[i] - c;
			data[i] = t > 0 ? t : t + 0xFF;
			c = t > 0 ? 0 : 1;
		}return *this;
	}
	BigNum& operator-= (BigNum& a) {
		int8u c = 0;		//借位标志
		int t;
		for (int64 i = 0; i < byte; i++) {
			t = data[i] - a[i] - c;
			data[i] = t > 0 ? t : t + 0xFF;
			c = t > 0 ? 0 : 1;
		}return *this;
	}
	BigNum& operator-= (int a) { }
	/*----------------乘 [mul]----------------*/
	BigNum& mul(BigNum& b) {
		BigNum t("0"), ans("0");
		//memcpy(t, b, sizeof(bool) * byte);
		for (int64 i = 0; i < byte; i++) {
			if (i != 0)t.leftShift();		//每次左移一位
			//if (data[i] == 1)ans.add(t);	//并加至ans
		}
		memcpy(data, ans.data, sizeof(ans.data));
		return *this;
	}
	BigNum& operator*= (BigNum& a){ }
	BigNum& operator*= (int a){ }
	/*----------------除 [div]----------------*/
	BigNum& div(BigNum& b) { }
	BigNum& operator/= (BigNum& a){ }
	BigNum& operator/= (int a) { }
	/*----------------余 [mod]----------------*/
	BigNum& mod(BigNum& b) { }
	/*----------------乘方 [pow]----------------*/
	BigNum& pow(int64 n) {
		if (n == 0) {
			memset(data, 0, sizeof(bool) * byte);
			*data = 1; return;
		}
		BigNum t("0");
		//memcpy(t, data, sizeof(bool) * byte);
		while (--n)  mul(t);
	}
	/*----------------左移 []----------------*/
	void leftShift() {
		for (int64 i = 0; i < byte; i++) {
			if (i != byte - 1)data[i + 1] = data[i] / 0x80;
			data[i] = (data[i] % 0x80) * 2;
		}
	}
	/*----------------右移 []----------------*/
	void rightShift() {
		for (int64 i = 0; i < byte; i++) {
			if (i != 0)data[i - 1] += 0x80 * (data[i] % 2);
			data[i] /= 2;
		}
	}
	/*----------------十六进制输出 ----------------*/
	char* hex() {
		char* ans = (char*)malloc(byte);		//新建存储十六进制结果的内存块
		int64 cur = byte - 1, ansCur = 0;
		while (cur >= 0 && *(data + cur) == 0)cur--;	//找到data的最高位
		unsigned char t = 0;
		while (cur >= 0) {
			t = t * 2 + *(data + cur);
			if (cur % 4 == 0) {		//二转十六，是四位合一位
				ans[ansCur++] = t;	//写十六进制结果
				t = 0;
			}cur--;
		}
		for (int64 i = 0; i < ansCur; i++) {	//十六进制转'0-9''A-F'的符号表示
			if (ans[i] <= 9)ans[i] += '0';
			else ans[i] += 'A' - 10;
		}
		ans[ansCur] = '\0';	//尾'\0'
		return ans;
	}
	/*----------------十进制输出 ----------------*/
	char* dec() {
		char ans[100];
		int64 cur = byte - 1, ansCur = 0;
		return ans;
	}
};
/*********************************************************************************
						大浮点数类
*********************************************************************************/
class BigFloat {
public:
	typedef long long		int64;
	typedef unsigned char	int8u;
	/******************************************************************************
	*                    核心数据
	******************************************************************************/
	BigNum num, index;
	BigFloat() { ; }
	BigFloat(double x) {

	}
	BigFloat(const char* x) {

	}
};
#endif