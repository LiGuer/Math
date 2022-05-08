#ifndef MATRIX_OPERATE_INNER_H
#define MATRIX_OPERATE_INNER_H
#include "../Mat.h"
#include "../Init.h"
#include "Basic.h"

namespace Matrix {
/******************************************************************************
* 
*                    内积空间
* 
******************************************************************************/

	/*----------------内积 [ dot · ]----------------*/
	inline double dot(Mat<>& a, Mat<>& b) {
		if (a.rows != b.rows || a.cols != b.cols)
			exit(-1);
		double ans = 0;

		for (int i = 0; i < a.size(); i++)
			ans += a[i] * b[i];
		return ans;
	}

	/*----------------范数 [ norm ||x|| ]----------------*/
	inline double norm(Mat<>& a) {
		return sqrt(dot(a, a));
	}

	inline double norm(Mat<>& a, const char* ctrl) {
		return sqrt(dot(a, a)); //####
	}

	/*---------------- 归一化 -----------------*/
	inline Mat<>& normalize(Mat<>& a) {
		double t = norm(a);
		if (t == 0)
			return a;

		for (int i = 0; i < a.size(); i++)
			a[i] /= t;

		return a;
	}
}

#endif