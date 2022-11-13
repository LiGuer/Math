#ifndef FUNCTION_PLOYNOMIAL_FUNCTION_H
#define FUNCTION_PLOYNOMIAL_FUNCTION_H

#include <stdlib.h>
#include <float.h>
#include "../Matrix/Matrix.h"

using namespace Matrix;

namespace Function {

/* ------------------------------------------------
*
*		多项式函数
*
** ------------------------------------------------*/
	/*
	 *	多项式函数
	 */
	// 一元
	double PolynomialFunction(double x, Mat<>& a) {
		int f = 0;

		for (int i = 0; i < a.rows; i++) {
			f += a(i) * pow(x, 0);
		}
		return f;
	}

	// 多元
	double PolynomialFunction(Mat<>& x, Tensor<>& a) {

	}

	/*
	 *	线性函数
	 *	f = a' x + f
	 */
	// 一元
	double LinearFunction(double x, double a, double b) {
		return a * x + b;
	}

	// 多元
	double LinearFunction(double x, Mat<>& a, double b) {
		return dot(a, x) + b;
	}

	double LinearFunction(double x, Mat<>& a) {
		if (a.rows == x.rows + 1) {
			Mat<> x_(a.rows);
			x_(a.rows - 1) = 1;
			memcpy(x_.data, x.data, sizeof(double) * x.rows);

			return dot(a, x_);
		}
		else if (A.rows == x.rows) {
			return dot(a, x);
		}
		return DBL_MAX;
	}

	/*
	 *	二次函数
	 *	f = x' A x + b' x + c
	 */
	// 一元
	double QuadraticFunction(double x, double a, double b, double c) {
		return a * x * x + b * x + c;
	}

	// 多元
	double QuadraticFunction(Mat<>& x, Mat<>& A, Mat<>& b, double c) {
		Mat<> tmp;

		return dot(x, mul(tmp, A, x)) + dot(b, x) + c;
	}

	double QuadraticFunction(Mat<>& x, Mat<>& A) {
		if (A.rows == x.rows + 1) {
			Mat<> x_(A.rows), tmp;
			x_(A.rows - 1) = 1;
			memcpy(x_.data, x.data, sizeof(double) * x.rows);

			return dot(x_, mul(tmp, A, x_));
		}
		else if(A.rows == x.rows) {
			Mat<> tmp;

			return dot(x, mul(tmp, A, x));
		}
		return DBL_MAX;
	}

	/*
	 *	四次函数
	 */
	// 一元
	double QuarticFunction(double x, double a, double b, double c, double d, double e) {
		return a * pow(x, 4) + b * pow(x, 3) + c * pow(x, 2) + d * x + e;
	}

	// 多元
	double QuarticFunction(Mat<>& x, Tensor<>& a, Tensor<>& b, Mat<>& c, Mat<>& d, double e) {

		return 0;
	}

}

#endif