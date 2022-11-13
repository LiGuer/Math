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

	Complex& zero(Complex& a) {
		a.real = a.imag = 0;
		return a;
	}

	Complex& I(Complex& a) {
		a.real = 0;
		a.imag = 1;
		return a;
	}

}

#endif