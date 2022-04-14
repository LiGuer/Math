#ifndef MATRIX_SOLVE_EQUATIONS_H
#define MATRIX_SOLVE_EQUATIONS_H
#include "Mat.h"
#include "Init.h"
#include "Operate/Basic.h"
#include "Decompose.h"

namespace Matrix {

	/******************************************************************************
	*
	*                    解方程组
	*
	******************************************************************************/
	inline Mat<>& solveEquations(Mat<>& a, Mat<>& b, Mat<>& x) {
		int n = a.rows;
		x.zero(n);

		//[1] LUP分解
		Mat<> U, L, P;
		LUP(a, U, L, P);

		//[2] LUP - Solve  [3] solve y
		for (int i = 0; i < n; i++) {
			x[i] = b[P[i]];		//yi
			for (int j = 0; j < i; j++)
				x[i] -= x[j] * L(i, j);
		}

		//[4] solve x
		for (int i = n - 1; i >= 0; i--) {
			for (int j = i + 1; j < n; j++)
				x[i] -= x[j] * U(i, j);
			x[i] /= U(i, i);
		}

		return x;
	}

}

#endif