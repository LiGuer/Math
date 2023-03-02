#ifndef FUNCTION_SOLVE_PLOYNOMIAL_EQUATION_H
#define FUNCTION_SOLVE_PLOYNOMIAL_EQUATION_H

#include <stdlib.h>
#include <float.h>
#include <complex>
#include "../Matrix/Matrix.h"

using namespace std;
using namespace Matrix;

namespace Function {

/* ------------------------------------------------
*
*		求解 多项式方程
*
** ------------------------------------------------*/

	/*
	 * 解线性方程
	 */
	// a x + b = 0
	inline double solveLinearEquation(double a, double b) {
		return -b / a;
	}

	// A x = b
	inline Mat<>& solveLinearEquation(Mat<>& A, Mat<>& b, Mat<>& ans) {
		Mat<> Apinv;
		pinv(Apinv, A);
		mul(ans, Apinv, b);
		return ans;
	}

	/*
	 * 解二次方程
	 */
	// a x^2 + b x + c = 0
	inline void solveQuadraticEquation(double a, double b, double c, complex<double>& root_1, complex<double>& root_2) {
		if (a == 0) {
			root_1 = solveLinearEquation(b, c);
			root_2 = DBL_MAX;
			return;
		}
		
		complex<double> delta = b * b - 4 * a * c;
		root_1 = (-b + pow(delta, 1. / 2.)) / (2 * a);
		root_2 = (-b - pow(delta, 1. / 2.)) / (2 * a);
		return;
	}

	inline int solveQuadraticEquation_real(double a, double b, double c, double& root_1, double& root_2) {
		root_1 = root_2 = DBL_MAX;

		if (a == 0) {
			root_1 = solveLinearEquation(b, c);
			return 1;
		}
		
		double delta = b * b - 4 * a * c;

		if (delta < 0) 
			return 0;

		else if (delta == 0) {
			root_1 = -b / (2 * a);
			return 1;
		}
		else {
			delta = sqrt(delta);
			roots[0] = (-b + delta) / (2 * a);
			roots[1] = (-b - delta) / (2 * a);
			return 2;
		}
		return 0;
	}

	/*
	 * 解3次方程
	 */
	inline void solveCubicEquation(double a, double b, double c, double d, complex<double> roots[3]) {
		if (a == 0 || b == 0) {
			roots[0] = solveLinearEquation(c, d);
			roots[1] = roots[2] = DBL_MAX;
		}

		if (a == 0) {
			solveQuadraticEquation(b, c, d, roots);
		}

		complex<double>
			D1 = ((3.0 * c / a) - (pow(b, 2.0) / pow(a, 2.0))) / 3.0,
			D2 = (((2.0 * pow(b, 3.0)) / pow(a, 3.0)) - ((9.0 * b * c) / pow(a, 2.0)) + (27.0 * d / a)) / 27.0,
			D  = (pow(D2, 2.0) / 4.0 + pow(D1, 3.0) / 27.0);
	}

	/*
	 * 解四次方程
	 */
	// a x^4 + b x^3 + c x^2 + d x + e = 0
	inline void solveQuarticEquation(
		complex<double> a, complex<double> b, complex<double> c, complex<double> d, complex<double> e, 
		complex<double>& root_1, complex<double>& root_2, complex<double>& root_3, complex<double>& root_4
	) {
		b /= a,
		c /= a,
		d /= a,
		e /= a;

		complex<double>
			Q1 = c * c - 3. * b * d + 12. * e,
			Q2 = 2. * c * c * c - 9. * b * c * d + 27. * d * d + 27. * b * b * e - 72. * c * e,
			Q3 = 8. * b * c - 16. * d - 2. * b * b * b,
			Q4 = 3. * b * b - 8. * c,

			Q5 = pow(Q2 / 2. + pow(Q2 * Q2 / 4. - Q1 * Q1 * Q1, 1. / 2.), 1. / 3.),
			Q6 = (Q1 / Q5 + Q5) / 3.,
			Q7 = 2. * pow(Q4 / 12. + Q6, 1. / 2);

		root_1 = (-b - Q7 - pow(4. * Q4 / 6. - 4. * Q6 - Q3 / Q7, 1. / 2.)) / 4.;
		root_2 = (-b - Q7 + pow(4. * Q4 / 6. - 4. * Q6 - Q3 / Q7, 1. / 2.)) / 4.;
		root_3 = (-b + Q7 - pow(4. * Q4 / 6. - 4. * Q6 + Q3 / Q7, 1. / 2.)) / 4.;
		root_4 = (-b + Q7 + pow(4. * Q4 / 6. - 4. * Q6 + Q3 / Q7, 1. / 2.)) / 4.;
	}

	inline void solveQuarticEquation(const complex<double> coeff[5], complex<double> roots[4]) {
		complex<double>
			a = coeff[4],
			b = coeff[3] / a,
			c = coeff[2] / a,
			d = coeff[1] / a,
			e = coeff[0] / a,

			Q1 = c * c - 3. * b * d + 12. * e,
			Q2 = 2. * c * c * c - 9. * b * c * d + 27. * d * d + 27. * b * b * e - 72. * c * e,
			Q3 = 8. * b * c - 16. * d - 2. * b * b * b,
			Q4 = 3. * b * b - 8. * c,

			Q5 = pow(Q2 / 2. + pow(Q2 * Q2 / 4. - Q1 * Q1 * Q1, 1. / 2.), 1. / 3.),
			Q6 = (Q1 / Q5 + Q5) / 3.,
			Q7 = 2. * pow(Q4 / 12. + Q6, 1. / 2);

		roots[0] = (-b - Q7 - pow(4. * Q4 / 6. - 4. * Q6 - Q3 / Q7, 1. / 2.)) / 4.;
		roots[1] = (-b - Q7 + pow(4. * Q4 / 6. - 4. * Q6 - Q3 / Q7, 1. / 2.)) / 4.;
		roots[2] = (-b + Q7 - pow(4. * Q4 / 6. - 4. * Q6 + Q3 / Q7, 1. / 2.)) / 4.;
		roots[3] = (-b + Q7 + pow(4. * Q4 / 6. - 4. * Q6 + Q3 / Q7, 1. / 2.)) / 4.;
	}

}

#endif