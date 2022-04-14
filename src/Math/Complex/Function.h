#ifndef COMPLEX_FUNCTION_H
#define COMPLEX_FUNCTION_H

#include "Complex.h"
#include "Operate.h"

namespace ComplexLib {

	/*
	 * 基础函数
	 */

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
