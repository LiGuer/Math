#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ll long long
#define MAXN 1000
class Num
{
private:
public:
	bool* num = (bool*)malloc(MAXN);
	Num(const char* input);
	void add(Num* b);	//[add]加
	void sub(Num* b);	//[sub]减
	void mul(Num* b);	//[mul]乘
	void div(Num* b);
	void pow(ll n);		//[pow] 乘方
	void leftShift();	//左移
	void rightShift();	//右移
	char* hex();
	char* dec();
};

Num::Num(const char* inputStr)
{
	memset(num, 0, sizeof(bool) * MAXN);
	ll cur = 0;
	while (*(inputStr + cur++) != '\0');
	char* input = (char*)malloc(MAXN / 3);
	memcpy(input, inputStr, sizeof(char) * cur);
	cur--;
	for (ll i = 0; i < cur; i++)*(input + i) -= '0';
	for (ll i = 0; i <= (cur - 1) / 2; i++) {	//高低位反转
		unsigned char t = input[i];
		input[i] = input[cur - 1 - i];
		input[cur - 1 - i] = t;
	}
	int numCur = 0;
	while (cur > 0) {
		bool c = 0;
		for (ll i = cur - 1; i >= 0; i--) {
			unsigned char t = *(input + i) + c * 10;
			c = t % 2;
			*(input + i) = t / 2;
		}
		num[numCur++] = c;
		while (*(input + cur - 1) == 0)cur--;
	}
}
void Num::add(Num* b)	//[add]加
{
	bool c = 0;	//进位标志
	for (ll i = 0; i < MAXN; i++) {
		unsigned char t = num[i] + b->num[i] + c;
		num[i] = t % 2;	c = t / 2;
	}
}

void Num::sub(Num* b)	//[sub]减
{
	bool c = 0;	//借位标志
	for (ll i = 0; i < MAXN; i++) {
		char t = num[i] - b->num[i] - c;
		c = t < 0 ? 1 : 0;
		num[i] = t < 0 ? t + 2 : t;
	}
}

void Num::mul(Num* b)	//[mul]乘
{
	Num* t = new Num("0");
	Num* ans = new Num("0");
	memcpy(t, b, sizeof(bool) * MAXN);
	for (ll i = 0; i < MAXN; i++) {
		if (i != 0)t->leftShift();		//每次左移一位
		if (num[i] == 1)ans->add(t);	//并加至ans
	}
	memcpy(num, ans->num, sizeof(ans->num));
}
void Num::pow(ll n)	//[pow]乘方
{
	if (n == 0) {
		memset(num, 0, sizeof(bool) * MAXN);
		*num = 1; return;
	}
	Num* t = new Num("0");
	memcpy(t, num, sizeof(bool) * MAXN);
	while (--n) {
		mul(t);
	}
}

void Num::leftShift()	//左移
{
	for (ll i = MAXN - 1; i > 0; i--) {
		num[i] = num[i - 1];
	}num[0] = 0;
}
void Num::rightShift()	//右移
{
	for (ll i = 0; i < MAXN - 1; i++) {
		num[i] = num[i + 1];
	}num[MAXN - 1] = 0;
}

char* Num::hex()	//十六进制输出
{
	char* ans = (char*)malloc(MAXN);		//新建存储十六进制结果的内存块
	ll cur = MAXN - 1, ansCur = 0;
	while (cur >= 0 && *(num + cur) == 0)cur--;	//找到num的最高位
	unsigned char t = 0;
	while (cur >= 0) {
		t = t * 2 + *(num + cur);
		if (cur % 4 == 0) {		//二转十六，是四位合一位
			ans[ansCur++] = t;	//写十六进制结果
			t = 0;
		}cur--;
	}
	for (ll i = 0; i < ansCur; i++) {	//十六进制转'0-9''A-F'的符号表示
		if (ans[i] <= 9)ans[i] += '0';
		else ans[i] += 'A' - 10;
	}
	ans[ansCur]='\0';	//尾'\0'
	return ans;
}
char* Num::dec()		//十进制输出
{
	char ans[100];
	ll cur = MAXN - 1, ansCur = 0;
	return ans;
}

void main()
{
	Num* a = new Num("12345");
	Num* b = new Num("903");
	a->mul(b);
	printf("%s\n", a->hex());
	b->add(a);
	printf("%s\n", b->hex());
}