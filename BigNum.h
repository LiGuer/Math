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
	BigNum() {
		byte = 8;
		data = (int8u*)malloc(byte * sizeof(int8u));
	}
	BigNum(int64 _byte) {
		_byte = byte;
		data = (int8u*)malloc(byte * sizeof(int8u));
	}
	BigNum(const char* input) {
		byte = 8;
		data = (int8u*)malloc(byte * sizeof(int8u));
		int64 cur = 0;
		while (input[cur++] != '\0'); cur--;
		//for (int64 i = 0; i < cur; i++)input[i] -= '0';
		for (int64 i = 0; i <= (cur - 1) / 2; i++) {	//高低位反转
			unsigned char t = input[i];
			//input[i] = input[cur - 1 - i];
			//input[cur - 1 - i] = t;
		}
		int dataCur = 0;
		while (cur > 0) {
			bool c = 0;
			for (int64 i = cur - 1; i >= 0; i--) {
				unsigned char t = *(input + i) + c * 10;
				c = t % 2;
				//*(input + i) = t / 2;
			}
			data[dataCur++] = c;
			while (*(input + cur - 1) == 0)cur--;
		}
	}
/******************************************************************************
*                    基础函数
-------------------------------------------------------------------------------
******************************************************************************/
	int8u operator[] (int64 index) { return data[index]; }
	/*----------------加 [add]----------------*/
	BigNum& add(BigNum& a, BigNum& b) {
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
	/*----------------减 [sub]----------------*/
	BigNum& sub(BigNum& a, BigNum& b) {
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
	/*----------------除 [div]----------------*/
	BigNum& div(BigNum& b) { }
	BigNum& operator/= (BigNum& a){ }
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
#endif