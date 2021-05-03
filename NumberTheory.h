/*
Copyright 2020 LiGuer. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
	http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
[Reference]:
[1] Thanks for Artiprocher at https://www.zhihu.com/question/379824357/answer/1088257294
==============================================================================*/
#ifndef NUMBERTHEORY_H
#define NUMBERTHEORY_H
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
typedef unsigned long long INT64U;
typedef long long INT64S;
namespace NumberTheory {
/*********************************************************************************
						组合 / 排列 
*	[定义]: 组合: 从n个不同元素中任取m个,不管其顺序合成一组的,所有组合的个数.记C_n^m.
			排列: 从n个不同元素中任取m个,按一定顺序合成一组的,所有组合的个数.记A_n^m.
*	[公式]: C_n^m = n! / ((n - m)!·m!)
			A_n^m = n! / (n - m)!
			杨辉恒等式: C_n^m = C_(n-1)^(m-1) + C_(n-1)^m
*	[算法]:
		欧拉积分: Γ(s) = ∫_0^+inf  x^(s-1)·e^-x  dx
		在整数点处取值: Γ(n + 1) = n!
---------------------------------------------------------------------------------
*	[问题]: 求解C_n^m mod p
*	[公式]: Lucas定理: C_n^m mod p = C_(n/p)^(m/p)·C_(n mod p)^(m mod p) mod p
			将n, m 表示为p进制形式, ni, mi是该进制模式下的第i位.
			则 C_n^m = Π_(i=0)^k  C_ni^mi mod p
*********************************************************************************/
INT64S C(INT64S n, INT64S m) {
	return (INT64S)round(tgamma(n + 1) / tgamma(m + 1) / tgamma(n - m + 1));
}
INT64S A(INT64S n, INT64S m) {
	return (INT64S)round(tgamma(n + 1) / tgamma(n - m + 1));
}
INT64S C_Lucas(INT64S n, INT64S m, INT64S p) {
	INT64S ans = 1;
	while (n | m) ans *= C(n % p, m % p), n /= p, m /= p;
	return ans;
}
/*********************************************************************************
						Fibonacci 数列
[定义]: F[n] = F[n-1] + F[n-2]		(n≥2, F[0]=0, F[1]=1)
[算法]: 递推式
*********************************************************************************/
INT64S* FibonacciSequence(int N) {
	INT64S* seq = (INT64S*)calloc(N, sizeof(INT64S));
	seq[0] = 0; 
	seq[1] = 1;
	for (int i = 2; i < N; i++) seq[i] = seq[i - 1] + seq[i - 2];
	return seq;
}
/*********************************************************************************
						最大公约数 / 最小公倍数
[算法]: 辗转相除法 GCD(a,b) == GCD(b, a mod b)
[算法]: 最小公倍数 = a·b / GCD(a,b)
[注]: 先除后乘，减少中间数位数
*********************************************************************************/
INT64S GCD(INT64S a, INT64S b) { return a % b == 0 ? b : GCD(b, a % b); }
INT64S LCM(INT64S a, INT64S b) { return a / GCD(a, b) * b; }
/*********************************************************************************
						拓展欧几里得算法
[算法]: 辗转相除法 的 拓展
		除计算a、b最大公约数, 还能找到x y（其中一个很可能是负数）满足 a x + b y = gcd(a ,b)
[线性方程定理]: 非零正整数 a,b ,总存在 x,y 满足 a x + b y = gcd(a ,b)
				其中, a,b 互质时, a x + b y = 1
*********************************************************************************/
INT64S GCDEx(INT64S a, INT64S b, INT64S& x, INT64S& y){
	if (!b) { x = 1, y = 0; return a; }
	else{
		INT64S r = GCDEx(b, a % b, y, x);
		y -= a / b * x;
		return r;
	}
}
/*********************************************************************************
						素数判断 / 素数筛
*	[定义]: 素数: 只被1和本身整除的数. (1不是素数)
	[算法]: 素数判断: 遍历测试所有整数直至sqrt(a), 
			因为a不会再被更大数整除, 能和更大数相乘等于a的小整数已被测试过。
			时间复杂度 O(sqrt(n))
	[算法]: 素数筛: 从小到大将素数的倍数筛掉, 时间复杂度 O(n log(n))
*********************************************************************************/
bool isPrime(INT64S a) {
	if (a == 1)return false;
	for (INT64S i = 2; i <= sqrt(a); i++) if (a % i == 0) return false;
	return true;
}
bool* getPrime(INT64S n) {
	bool* prime = (bool*)malloc((n + 1) * sizeof(bool));
	for (int i = 2; i <= n; i++) prime[i] = true;
	prime[1] = prime[0] = false;
	for (int i = 2; i <= n; i++) {
		if (prime[i])
			for (int j = i + i; j <= n; j += i) prime[j] = false;
	}
}
/*********************************************************************************
						幂次模
[问题]: 求 bi = ai^k  (mod m) ,  幂次模问题
[算法]: 逐次平方法
[步骤]:
	[1] 将 k 二进制展开  k = u0·2⁰ + u1·2¹ + u2·2² + ... + ur·2^r
			(计算机里, k内存天然是二进制)
	[2] 逐次平方制作模m的a幂次表, i∈[0,r]
			a^(2^0) = a ≡ A0 (mod m)
			a^(2^i) ≡ (a^2^(i-1))² ≡ A²(i-1) ≡ Ai (mod m)
	[3] 乘积  A0^u0·A1^u1·...·Ar^ur  (mod m)
[证明]: a^k = a^(u0·2⁰ + u1·2¹ + u2·2² + ... + ur·2^r)
*********************************************************************************/
INT64S PowersModulo(INT64S a, INT64S k, INT64S m) {
	INT64S ans = 1;
	for (int i = 0; i < sizeof(k) * 8; i++) {
		if (k & ((INT64S)1 << i))ans = (ans * a) % m;
		a = (a * a) % m;
	}return ans;
}
/*********************************************************************************
						RSA加密
*	设. 选取大质数: q, p  则. m = p q , 选取 k 与 m 互质
*	[加密]: bi = ai^k  (mod m) ,  幂次模问题
		[算法]: 平方法
*	[解密]: 解同余式 x^k  ≡ bi (mod m)	计算模m的k次根
		[核心基础]:
			[欧拉公式]: 若 a 与 m 互质,
						则 a^Φ(m) ≡ 1 (mod n)
						* Φ(x): [1,x)中,与x互质的数个数
			[线性方程定理]
		[步骤]: 若 b 与 m 互质, k 与 Φ(m) 互质,
			[1] 计算Φ(m)
				* Φ(m) = Φ(p)Φ(q)		* 当 m = p q,且p,q互质
						= (p - 1)(q - 1)	* x质数,则Φ(x) = x - 1
			[2] 求满足 k u - Φ(m) v = 1 的 u,v   * 线性方程定理
				即. k u ≡ 1 (mod Φ(m))	即. u 是 k (mod Φ(m))的逆
				[算法]: 拓展欧几里得算法
			[3] x = b^u (mod m)
		[证明]: 只要证, x = b^u 是 x^k  ≡ bi (mod m)的解
			x^k = (b^u)^k = b^(1 + Φ(m) v) = b·b^Φ(m)^v
			欧拉公式,有 b^Φ(m) = 1 (mod m)
			x^k (mod m) = b (mod m)		得证
	* k: 公钥		u: 私钥
*	该算法简单，无显式程序
		加密: PowersModulo(message, k, m);
		解密: PowersModulo(message, u, m);
*********************************************************************************/
INT64S RSAdecrypt(INT64S k, INT64S p, INT64S q) {
	INT64S mPhi = (p - 1)*(q - 1);
	INT64S u, v;
	GCDEx(mPhi, k, v, u);
	printf("u:%lld\n", u);
	u = 1;
	while ((k * u) % mPhi != 1)u++;	//u:私钥,模逆元
	printf("u:%lld\n", u);
	return u;
}
}
#endif
