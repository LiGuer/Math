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
	bool	sign = 0;
	int8u*	data = NULL;
	int64	byte = 0;
/******************************************************************************
*                    基础函数
-------------------------------------------------------------------------------
******************************************************************************/
	/*----------------构造/析构----------------*/
	BigNum()					{ zero(8); }
	BigNum(int64 _byte)			{ zero(_byte); }
	BigNum(const char* input)	{ zero(8); *this = input; }
   ~BigNum()					{ delete data; }
	/*----------------索引----------------*/
	int8u& operator[] (int64 index) { return data[index]; }
	int8u& operator() (int64 index) { return data[index]; }
	/*----------------归零----------------*/
	BigNum& alloc(int64 _byte){
		if(byte != _byte) data = (int8u*)malloc((byte = _byte) * sizeof(int8u)); 
		return *this;
	}
	BigNum& Realloc() {
		if (byte == 0) return alloc(8);
		realloc(data, (byte *= 2) * sizeof(int8u)); return *this;
	}
	BigNum& zero()				{ sign = 0; memset(data, 0, byte * sizeof(int8u)); return *this; }
	BigNum& zero(int64 _byte)	{ alloc(_byte); zero(); return *this; }
	/*----------------赋值----------------*/
	BigNum& operator= (BigNum& input) {
		sign = input.sign;
		byte = input.byte; alloc(byte);
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
		if (input[0] == '-') { sign = 1; input++; }
		//进制
		int8u base = 10;
		if (input[0] == '0' && input[1] == 'x') { base = 16; input += 2; }
		if (input[0] == '0' && input[1] == 'b') { base =  2; input += 2; }
		//数
		int8u t;
		switch (base)
		{
		case  2: 
			int64 len = strlen(input);
			zero((len + 3) / 4);
			for (int i = 0; i < byte; i++) {
				t = 0;
				for (int j = 0; j < 4; j++)
					t += input[len - 1 - i * 4 - j] - '0';
				data[i] += t;
			}
			break;
		case 10:
			zero(8);
			for (int i = 0; input[i]; i++) {
				t = input[i] - '0';
			}
			break;
		case 16: 
			int64 len = strlen(input);
			zero((len + 1) / 2);
			for (int i = len - 1; i >= 0; i--) {
					 if(input[i] >= '0' && input[i] <= '9') t = input[i] - '0';
				else if(input[i] >= 'A' && input[i] <= 'F') t = input[i] - 'A';
				else if(input[i] >= 'a' && input[i] <= 'f') t = input[i] - 'a';
				if (i % 2 == 1)t *= 16;
				data[(i - len - 1) / 2] += t;
			}
			break;
		}
		return *this;
	}
	/*----------------转字符串----------------*/
	char* toStr(int8u base = 10) {
		char* ans;
		switch (base)
		{
		case  2:
			return ans;
		case 10:		
			ans = (char*)malloc(100 * sizeof(char));
			int64 cur = byte - 1, ansCur = 0;
			return ans;
		case 16:
			ans = (char*)malloc(byte * sizeof(char));
			int64 cur = byte - 1, ansCur = 0;
			while (cur >= 0 && *(data + cur) == 0)cur--;	//找到data的最高位
			unsigned char t = 0;
			while (cur >= 0) {
				t = t * 2 + *(data + cur);
				if (cur % 4 == 0) {							//2转16，是四位合一位
					ans[ansCur++] = t;						//写16进制结果
					t = 0;
				}cur--;
			}
			for (int64 i = 0; i < ansCur; i++) 				//16进制转'0-9''A-F'的符号表示
				ans[i] += ans[i] <= 9 ? '0' : 'A' - 10;
			ans[ansCur] = '\0';
			return ans;
		}
	}
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
	BigNum& operator+= (BigNum& a) { add(*this, a); return *this; }
	BigNum& operator+= (int64& a) { 
	}
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
	BigNum& operator-= (BigNum& a) { sub(*this, a); return *this; }
	BigNum& operator-= (int64 a) { }
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
	BigNum& operator*= (int64 a){ }
	/*----------------除 [div]----------------*/
	BigNum& div(BigNum& b) { }
	BigNum& operator/= (BigNum& a){ }
	BigNum& operator/= (int64 a) { }
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