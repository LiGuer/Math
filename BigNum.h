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
#include <math.h>
#include <algorithm>
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
*	特征: 堆叠顺序小低大高, 补码
******************************************************************************/
	bool	sign = 1;
	int8u*	data = NULL;
	int64	byte = 0;
/******************************************************************************
*                    基础函数
-------------------------------------------------------------------------------
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
	BigNum& Realloc() {
		if (byte == 0) return alloc(8);
		realloc(data, (byte *= 2) * sizeof(int8u)); return *this;
	}
	BigNum& zero()				{ memset(data, 0, byte * sizeof(int8u)); return *this; }
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
			zero((len + 3) / 4);
			for (int i = 0; i < byte; i++) {
				t = 0;
				for (int j = 0; j < 4 && len - 1 - i * 4 - j >= 0; j++)
					t += input[len - 1 - i * 4 - j] - '0';
				data[i] += t;
			}
			break;
		case 10:
			int64 t;
			zero(len * log(10) / log(256) + 1);
			for (int i = 0; input[i]; i++) {
				t = input[i] - '0';
				(*this *= 10) += t;
			}
			break;
		case 16: 
			zero((len + 1) / 2);
			for (int i = len - 1; i >= 0; i--) {
					 if(input[i] >= '0' && input[i] <= '9') t = input[i] - '0';
				else if(input[i] >= 'A' && input[i] <= 'F') t = input[i] - 'A';
				else if(input[i] >= 'a' && input[i] <= 'f') t = input[i] - 'a';
				if ((len - 1 - i) % 2 == 1) t *= 16;
				data[(len - 1 - i) / 2] += t;
			}
			break;
		}
		if (negative) data[byte - 1] |= 0x80; complement();
		return *this;
	}
	/*----------------转字符串----------------*/
	int usefulByte() {
		int num = byte - 1;
		if (sign && data[byte - 1] == 0x80) num--;
		while (num > 0 && data[num] == 0)   num--;	//找到data的最高位
		return num + 1;
	}
	char* toStr(int8u base = 10) {
		icomplement();
		char* str = NULL;
		int64 num = usefulByte();
		switch (base)
		{
		case  2:
			str = (char*)calloc((8 * num + 1), sizeof(char));
			for (int i = 0; i < num; i++)
				for (int j = 0; j < 8; j++)
					str[2 * i + j] = ((data[num - 1 - i] >> (7 - j)) & 1) + '0';
			break;
		case 10:		
			str = (char*)calloc((num * log(256) / log(10) + 1), sizeof(char));
			break;
		case 16:
			str = (char*)calloc((2 * num + 1), sizeof(char));
			for (int i = 0; i < num; i++) {
				str[2 * i]      = data[num - 1 - i] / 0x10;	
				str[2 * i + 1]  = data[num - 1 - i] % 0x10;
				str[2 * i]     += str[2 * i]     <= 9 ? '0' : 'A' - 10;
				str[2 * i + 1] += str[2 * i + 1] <= 9 ? '0' : 'A' - 10;
			}
			break;
		}
		complement(); return str;
	}
	/*----------------补码----------------*/
	BigNum& complement() {
		if (sign && data[byte - 1] & 0x80) {
			for (int i = 0; i < byte; i++) data[i] = ~data[i];
			data[byte - 1] ^= 0x80;
			for (int i = 0; i < byte; i++) {
				if (data[i] == 0xFF) data[i] = 0;
				else { data[i] += 1; break; }
			}
		} return *this;
	}
	BigNum& icomplement() {
		if (sign && data[byte - 1] & 0x80) {
			for (int i = 0; i < byte; i++) {
				if (data[i] == 0) data[i] = 0xFF;
				else { data[i] -= 1; break; }
			}
			for (int i = 0; i < byte; i++) data[i] = ~data[i];
			data[byte - 1] ^= 0x80;
		} return *this;
	}
	/*----------------加减乘除 [ + - * / ]----------------*/
	BigNum& add(BigNum& a, BigNum& b) {
		alloc(std::max(a.byte, b.byte));
		bool c = 0; int t;	//c: 进位
		for (int64 i = 0; i < byte; i++) {
			t = a[i] + b[i] + (c ? 1 : 0);
			data[i] = t % 0x100;
			c = t > 0xFF ? 1 : 0;
		}return *this;
	}
	BigNum& operator+= (BigNum& a) { return add(*this, a); }
	BigNum& operator+= (int64 & a) { return *this; }
	BigNum& sub(BigNum& a, BigNum& b) { return *this; }
	BigNum& operator-= (BigNum& a) { return sub(*this, a); }
	BigNum& operator-= (int64 a) { return *this; }
	BigNum& mul(BigNum& a, BigNum& b) {
		BigNum t, ans("0");
		t = b;
		for (int64 i = 0; i < byte; i++) {
			if (i != 0) t.leftShift();
			if (data[i] == 1) ans += t;
		}
		memcpy(data, ans.data, sizeof(ans.data));
		return *this;
	}
	BigNum& operator*= (BigNum& a) { return mul(*this, a); }
	BigNum& operator*= (int64 a){ return *this; }
	BigNum& div(BigNum& a, BigNum& b) { return *this; }
	BigNum& operator/= (BigNum& a){ return div(*this, a); }
	BigNum& operator/= (int64 a) { return *this; }
	/*----------------余 [mod]----------------*/
	BigNum& mod(BigNum& b) { }
	/*----------------乘方 [pow]----------------*/
	BigNum& pow(int64 n) {
		if (n == 0) {
			memset(data, 0, sizeof(bool) * byte);
			*data = 1; return *this;
		}
		BigNum t("0"); t = *this;
		while (--n)  *this *= t;
		return *this;
	}
	/*----------------左移右移 [ << >> ]----------------*/
	void leftShift() {
		for (int64 i = 0; i < byte; i++) {
			if (i != byte - 1)data[i + 1] = data[i] / 0x80;
			data[i] = (data[i] % 0x80) * 2;
		}
	}
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