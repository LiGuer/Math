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
==============================================================================*/
#ifndef NUMBERTHEORY_H
#define NUMBERTHEORY_H
#include <stdlib.h>
#include <stdio.h>
typedef unsigned long long INT64U;
typedef long long INT64S;
/*--------------------------------[ 最大公约数 ]--------------------------------
[算法]: 辗转相除法
**---------------------------------------------------------------------*/
INT64S GCD(INT64S a, INT64S b) { return a % b == 0 ? b : GCD(b, a % b); }
/*--------------------------------[ 拓展欧几里得算法 ]--------------------------------
[算法]: 辗转相除法 的 拓展
		除计算a、b最大公约数, 还能找到x y（其中一个很可能是负数）满足 a x + b y = gcd(a ,b)
[线性方程定理]: 非零正整数 a,b ,总存在 x,y 满足 a x + b y = gcd(a ,b)
				其中, a,b 互质时, a x + b y = 1
**----------------------------------------------------------------------------------*/
INT64S GCDEx(INT64S a, INT64S b, INT64S& x, INT64S& y){
	if (b == 0) { x = 1, y = 0; return a; }
	else{
		INT64S r = GCDEx(b, a % b, y, x);
		y -= x * (a / b);
		return r;
	}
}
/*--------------------------------[ Fibonacci Sequence ]--------------------------------
[定义]: F[n] = F[n-1] + F[n-2]		(n≥2, F[0]=0, F[1]=1)
**------------------------------------------------------------------------------------*/
INT64S* FibonacciSequence(int N) {
	INT64S* seq = (INT64S*)calloc(N, sizeof(INT64S));
	seq[0] = 0; seq[1] = 1;
	for (int i = 2; i < N; i++) seq[i] = seq[i - 1] + seq[i - 2];
	return seq;
}
/*--------------------------------[ 幂次模 ]--------------------------------
[问题]: bi = ai^k  (mod m) ,  幂次模问题
[算法]: 逐次平方法
[步骤]:
	[1] 将 k 二进制展开  k = u0·2⁰ + u1·2¹ + u2·2² + ... + ur·2^r
			//计算机里, k内存天然是二进制
	[2] 逐次平方制作模m的a幂次表, i∈[0,r] 
			a^(2^0) = a ≡ A0 (mod m)
			a^(2^i) ≡ (a^2^(i-1))² ≡ A²(i-1) ≡ Ai (mod m)
	[3] 乘积  A0^u0·A1^u1·...·Ar^ur  (mod m)
[证明]: a^k = a^(u0·2⁰ + u1·2¹ + u2·2² + ... + ur·2^r)
**---------------------------------------------------------------------*/
INT64S PowersModulo(INT64S a, INT64S k, INT64S m) {
	INT64S ans = 1;
	for (int i = 0; i < sizeof(k) * 8; i++) {
		if (k & ((INT64S)1 << i))ans = (ans * a) % m;
		a = (a * a) % m;
	}return ans;
}
/*--------------------------------[ RSA ]--------------------------------
*	设. 选取大质数: q, p  则. m = p q , 选取 k 与 m 互质
*	加密: bi = ai^k  (mod m) ,  幂次模问题
		[算法]: 平方法
*	解密: 解同余式 x^k  ≡ bi (mod m)	计算模m的k次根
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
		加密: PowersModulo(message, k, m);
		解密: PowersModulo(message, u, m);
	* 该算法简单，无显式程序
**---------------------------------------------------------------------*/
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
#endif
