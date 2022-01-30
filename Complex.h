#ifndef COMPLEX_H
#define COMPLEX_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

class Complex {
public:

	double real = 0, imag = 0;

	// 赋值 / 取值
	Complex& operator=(Complex& a) {
		real = a.real;
		imag = a.imag;
		return *this;
	}

	Complex& set(double _real, double _imag) {
		real = _real;
		imag = _imag;
		return *this;
	}

	void get(double& _real, double& _imag) {
		_real = real;
		_imag = imag;
	}

};

namespace ComplexLib {

/* 
 * 基础构造
 */

Complex& zero(Complex& a){
    a.real = a.imag = 0;
    return a;
}

Complex& I(Complex& a){
    a.real = 0;
    a.imag = 1;
    return a;
}


/* 
 * 基础运算
 */
 //模
double abs(Complex& a) {
	return sqrt(a.real * a.real + a.imag * a.imag);
}

//幅角
double arg(Complex& a) {
	return atan(a.imag / a.real);
}

//负
Complex& operator-(Complex& a) {
	Complex ans;

	ans.real = -a.real;
	ans.imag = -a.imag;
	return ans;
}

//加
Complex& operator+(Complex& a, Complex& b) {
	Complex ans;

    ans.real = a.real + b.real;
    ans.imag = a.imag + b.imag;
    return ans;
}

Complex& operator+(Complex& a, double b) {
	Complex ans;

	ans.real = a.real + b;
	ans.imag = a.imag;
	return ans;
}

Complex& operator+(double a, Complex& b) {
	Complex ans;

	ans.real = b.real + a;
	ans.imag = b.imag;
	return ans;
}

//减
Complex& operator-(Complex& a, Complex& b) {
	Complex ans;

    ans.real = a.real - b.real;
    ans.imag = a.imag - b.imag;
    return ans;
}

Complex& operator-(Complex& a, double b) {
	Complex ans;

	ans.real = a.real + b;
	ans.imag = a.imag;
	return ans;
}

Complex& operator-(double a, Complex& b) {
	Complex ans;

	ans.real = a - b.real;
	ans.imag =   - b.imag;
	return ans;
}

//乘
Complex& operator*(Complex& a, Complex b) {
	Complex ans;

	ans.real = a.real * b.real - a.imag * b.imag;
	ans.imag = a.real * b.imag + a.imag * b.real;
    return ans;
}

Complex& operator*(Complex& a, double b) {
	Complex ans;

	ans.real = a.real * b;
	ans.imag = a.imag * b;
	return ans;
}

Complex& operator*(double a, Complex& b) {
	Complex ans;

	ans.real = a * b.real;
	ans.imag = a * b.imag;
	return ans;
}
//除
Complex& operator/(Complex& a, Complex& b) {
	Complex ans;

    double t =  b.real * b.real + b.imag * b.imag;
	ans.real = (a.real * b.real + a.imag * b.imag) / t;
	ans.imag = (a.imag * b.real - a.real * b.imag) / t;
    return ans;
}

Complex& operator/(Complex& a, double b) {
	Complex ans;

	ans.real = a.real / b;
	ans.imag = a.imag / b;
	return ans;
}

Complex& operator/(double a, Complex& b) {
	Complex ans;

	double t = b.real * b.real + b.imag * b.imag;
	ans.real =  a * b.real / t;
	ans.imag = -a * b.imag / t;
	return ans;
}

//共轭
Complex& conj (Complex& a){
	Complex ans;

    ans.real =  a.real;
    ans.imag = -a.imag;
    return ans;
}

//幂  de Moiver公式
Complex& Pow(Complex& a, double p) {
	Complex ans;

	double r = pow(abs(a), p), theta = p * arg(a);
	ans.real = r * cos(theta);
	ans.imag = r * sin(theta);
	return ans;
}

Complex& Pow(Complex& a, Complex& p) {
	Complex ans;

	return ans;
}

//指数函数
Complex& Exp(Complex& a) {
	Complex ans;

	ans.real = exp(a.real) * cos(a.imag);
	ans.imag = exp(a.real) * sin(a.imag);
	return ans;
}

//对数函数
Complex& Ln(Complex& a) {
	Complex ans;

	ans.real = log(abs(a));
	ans.imag = arg(a);
	return ans;
}

//三角函数、双曲函数
Complex& Cos(Complex& a) {
	Complex ans, i;
	I(i);

	ans = (Exp(a * i) + Exp(-a * i)) / 2;
	return ans;
}

Complex& Sin(Complex& a) {
	Complex ans, i;
	I(i);

	ans = (Exp(a * i) - Exp(-a * i)) / (2 * i);
	return ans;
}

Complex& Cosh(Complex& a) {
	Complex ans, i;
	I(i);

	ans = (Exp(a) + Exp(-a)) / 2;
	return ans;
}

Complex& Sinh(Complex& a) {
	Complex ans, i;
	I(i);

	ans = (Exp(a) - Exp(-a)) / 2;
	return ans;
}

}

#endif