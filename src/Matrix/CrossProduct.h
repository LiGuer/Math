#ifndef MATRIX_OPERATE_CROSSPRODUCT_H
#define MATRIX_OPERATE_CROSSPRODUCT_H
#include "Mat.h"
#include "Init.h"
#include "BasicOperate.h"

namespace Matrix {
	/******************************************************************************
	*
	*                    叉乘
	*
	******************************************************************************/

	inline Mat<>& cross(Mat<>& ans, Mat<>& a, Mat<>& b) {
		if (a.rows != b.rows)
			exit(-1);

		Mat<> ansTmp(a.rows, a.cols);
		ansTmp[0] = a[1] * b[2] - a[2] * b[1];
		ansTmp[1] = a[2] * b[0] - a[0] * b[2];
		ansTmp[2] = a[0] * b[1] - a[1] * b[0];

		ans.eatMat(ansTmp);
		return ans;
	}

	inline Mat<>& cross_(Mat<>& ans, Mat<>& a, Mat<>& b) {
		if (a.rows != b.rows)
			exit(-1);

		ans.alloc(a.rows, a.cols);
		ans[0] = a[1] * b[2] - a[2] * b[1];
		ans[1] = a[2] * b[0] - a[0] * b[2];
		ans[2] = a[0] * b[1] - a[1] * b[0];
		return ans;
	}

}

#endif