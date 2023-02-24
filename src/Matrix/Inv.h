#ifndef MATRIX_OPERATE_INV_H
#define MATRIX_OPERATE_INV_H
#include "Mat.h"
#include "Init.h"
#include "Decompose.h"
#include "BasicOperate.h"

namespace Matrix {

/*---------------- Inverse ----------------*/
	inline Mat<>& inv(Mat<>& ans, Mat<>& a) {
		if (a.rows != a.cols)
			exit(-1);

		Mat<> tmp(a.rows, a.cols);
		int n = a.rows;

		// LUP分解
		Mat<> L, U, P;
		LUP(a, L, U, P);
		
		//对每一列
		Mat<> b(n), x(n);
		for (int k = 0; k < n; k++) {
			b.zero();
			b[k] = 1;

			// 解线性方程组 //solve y
			for (int i = 0; i < n; i++) {
				x[i] = b[P[i]];		//yi
				for (int j = 0; j < i; j++)
					x[i] -= x[j] * L(i, j);
			}

			//solve x
			for (int i = n - 1; i >= 0; i--) {
				for (int j = i + 1; j < n; j++)
					x[i] -= x[j] * U(i, j);
				x[i] /= U(i, i);
			}

			//合并至结果
			for (int i = 0; i < a.rows; i++)
				tmp(i, k) = x[i];
		}

		ans.eatMat(tmp);
		return ans;
	}

/*---------------- Pseudo-inverse ----------------*/
	inline Mat<>& pinv(Mat<>& ans, Mat<>& a) {
		Mat<> aT;
		transpose(aT, a);
		mul(ans, aT, a);
		inv(ans, ans);
		mul(ans, ans, aT);

		return ans;
	}
}

#endif