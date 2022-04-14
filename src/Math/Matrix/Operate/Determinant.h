#ifndef MATRIX_OPERATE_DETERMINANT_H
#define MATRIX_OPERATE_DETERMINANT_H
#include "../Mat.h"
#include "../Init.h"
#include "Basic.h"

namespace Matrix {
/******************************************************************************
* 
*                    行列式
* 
******************************************************************************/
	double det(Mat<>& a);

	/*----------------余子式 [ comi ]----------------*/
	inline double comi(Mat<>& a, int i0, int j0) {
		Mat<> tmp(a.rows - 1, a.cols - 1);
		for (int i = 0; i < a.rows; i++) {
			for (int j = 0; j < a.cols; j++) {
				if (i == i0 || j == j0)
					continue;

				tmp(i < i0 ? i : i - 1, j < j0 ? j : j - 1) = a(i, j);
			}
		}
		return det(a);
	}

	/*----------------行列式 [ det |x| ]----------------*/
	inline double det(Mat<>& a) {
		if (a.rows != a.cols)
			exit(-1);
		//加速
		if (a.rows == 1)
			return a[0];

		if (a.rows == 2)
			return a(0, 0) * a(1, 1) - a(0, 1) * a(0, 1);

		double ans;
		memset(&ans, 0, sizeof(double));
		if (a.rows == 3) {
			double t;
			for (int i = 0; i < 3; i++) {
				t = 1;

				for (int j = 0; j < 3; j++)
					t *= a(j, (j + i) % 3);
				ans += t;

				for (int j = 0; j < 3; j++)
					t *= a(j, (2 - j + i) % 3);
				ans -= t;
			}
			return ans;
		}
		//普适
		for (int i = 0; i < a.rows; i++)
			ans += a(i, 0) * (i % 2 == 0 ? 1 : -1) * comi(a, i, 0);
		return ans;
	}

	/*--------------伴随矩阵 [ adjugate A* ]----------------*/
	inline Mat<>& adjugate(Mat<>& ans, Mat<>& a) {
		Mat<> ansTmp(a.rows, a.cols);
		for (int i = 0; i < a.rows; i++)
			for (int j = 0; j < a.cols; j++)
				ansTmp(i, j) = ((i + j) % 2 == 0 ? 1 : -1) * comi(a, i, j);

		ans.eatMat(ansTmp);
		return ans;
	}

}

#endif
