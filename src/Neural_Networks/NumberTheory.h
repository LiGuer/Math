#ifndef NUMBERTHEORY_H
#define NUMBERTHEORY_H
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "BigNum.h"
typedef unsigned long long INT64U;
typedef long long INT64S;
/*################################################################################################
[数论 Number Theory]
INT64S Factorial(INT64S n);										//阶乘
INT64S*FibonacciSequence(int N);								//Fibonacci数列
INT64S C		(INT64S n, INT64S m);							//组合
INT64S A		(INT64S n, INT64S m);							//排列
INT64S GCD		(INT64S a, INT64S b);							//最大公约数
INT64S LCM		(INT64S a, INT64S b);							//最小公倍数
INT64S GCDex	(INT64S a, INT64S b, INT64S& x, INT64S& y)		//拓展Euclid算法
bool   isPrime	(INT64S a);										//素数判断
INT64S PowMod	(INT64S a, INT64S k, INT64S m);					//幂次模
INT64S RSAPrivateKey(INT64S p, INT64S q, INT64S a);				//RSA密码
################################################################################################*/
namespace NumberTheory {
/******************************************************************************
*                    Factorial  阶乘
*	[定义]:
		n! = Π_(i=0)^n  i
		0! = 1
	[算法]:
		Euler积分: Γ(s) = ∫_0^+inf  x^(s-1)·e^-x  dx
		在整数点处取值: Γ(n + 1) = n!
******************************************************************************/
inline INT64S Factorial(INT64S n){
	return tgamma(n + 1);
	// return n == 0 ? 1 : n * Factorial(n - 1);
}
/*********************************************************************************
						组合 / 排列 
*	[定义]: 组合: 从n个不同元素中任取m个,不管其顺序合成一组的,所有组合的个数.记C_n^m.
			排列: 从n个不同元素中任取m个,按一定顺序合成一组的,所有组合的个数.记A_n^m.
*	[公式]: C_n^m = n! / ((n - m)!·m!)
			A_n^m = n! / (n - m)!
			杨辉恒等式: C_n^m = C_(n-1)^(m-1) + C_(n-1)^m
---------------------------------------------------------------------------------
*	[问题]: 求解C_n^m mod p
*	[公式]: Lucas定理: C_n^m mod p = C_(n/p)^(m/p)·C_(n mod p)^(m mod p) mod p
			将n, m 表示为p进制形式, ni, mi是该进制模式下的第i位.
			则 C_n^m = Π_(i=0)^k  C_ni^mi mod p
*********************************************************************************/
INT64S C(INT64S n, INT64S m) {
	return (INT64S)round(Factorial(n) / Factorial(m) / Factorial(n - m));
}

INT64S A(INT64S n, INT64S m) {
	return (INT64S)round(Factorial(n) / Factorial(n - m));
}

INT64S C_Lucas(INT64S n, INT64S m, INT64S p) {
	INT64S ans = 1;
	while (n | m) 
		ans *= C(n % p, m % p), n /= p, m /= p;
	return ans;
}

/*********************************************************************************
						Fibonacci数列
[定义]: F[n] = F[n-1] + F[n-2]		(n≥2, F[0]=0, F[1]=1)
[算法]: 递推式
*********************************************************************************/
INT64S* FibonacciSequence(int N) {
	INT64S* seq = (INT64S*)calloc(N, sizeof(INT64S));
	seq[0] = 0; 
	seq[1] = 1;
	for (int i = 2; i < N; i++) 
		seq[i] = seq[i - 1] + seq[i - 2];
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
[拓展Euclid算法]
	[算法]: 辗转相除法的拓展
			除计算a、b最大公约数, 还能找到满足a x + b y = gcd(a,b)的(x,y)的一组解(其中一个很可能是负数)
	[线性方程定理]: 
		非零正整数a b, 总存在x y满足 a x + b y = gcd(a ,b)
		若a b互质, 则 a x + b y = 1
*********************************************************************************/
INT64S GCDex(INT64S a, INT64S b, INT64S& x, INT64S& y){
	if (!b) { x = 1, y = 0; return a; }
	else{
		INT64S r = GCDex(b, a % b, y, x);
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
	for (INT64S i = 2; i <= sqrt(a); i++) 
		if (a % i == 0) 
			return false;
	return true;
}
bool* getPrime(INT64S n) {
	bool* prime = (bool*)malloc((n + 1) * sizeof(bool));
	for (int i = 2; i <= n; i++) 
		prime[i] = true;
	prime[1] = prime[0] = false;
	
	for (int i = 2; i <= n; i++) {
		if (prime[i])
			for (int j = i + i; j <= n; j += i) 
			prime[j] = false;
	}
}

/*********************************************************************************
[幂次模]
	[问题]: 求 b = a^k (mod m)
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
INT64S PowMod(INT64S a, INT64S k, INT64S m) {
	INT64S ans = 1;
	for (int i = 0; i < sizeof(k) * 8; i++) {
		if (k & ((INT64S)1 << i))
			ans = (ans * a) % m;
		a = (a * a) % m;
	}return ans;
}
/*********************************************************************************
[RSA 密码]
	[特征]: 非对称密码. 公钥加密, 私钥解密.
			大数质因数分解的时间复杂度极高.
	[步骤]:
		(1) 选2个大素数 p,q 
		(2) n = p q
			φ(n) = (p - 1)(q - 1)
		(3) 选公钥 a	(>1, 小于且互质于φ(n), ≠p、q)
		(4) 计算私钥 b	(a b = 1 mod φ(n))
			利用拓展Euclid算法
		(5) 公钥(n, a)		密文 = 明文^a (mod n)
			私钥(n, b)		明文 = 密文^b (mod n)
			加/解密, 利用幂次模算法
	[原理]:
		(1) Euler函数φ(n): 小于且互质于n的数的数目
			若n为素数, 则φ(n) = (n - 1)
		(2) a b = 1 mod φ(n)  =>  a b + c φ(n) = 1
			拓展Euclid算法, 可计算 a x + b y 的(x,y)的一组解
		(3) Euler公式: a n 互质 => a ^ φ(n) ≡ 1 (mod n)
			=> 明 ^ (kφ(n) + 1) = 明 ^ (ab) ≡ 1 (mod n)
			=> 若 密 = 明^a (mod n), 则 密^b = 明^(ab) = 明 (mod n)
	[例]:
		私钥: RSAPrivateKey(p, q, a)
		加密: PowMod(message, a, n);
		解密: PowMod(message, b, n);
*********************************************************************************/
INT64S RSAPrivateKey(INT64S p, INT64S q, INT64S a) {
	INT64S phi = (p - 1) * (q - 1), b, c;
	GCDex(phi, a, c, b);
	return b;
}
}
#endif
