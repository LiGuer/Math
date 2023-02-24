#ifndef MATRIX_OPERATE_INNER_H
#define MATRIX_OPERATE_INNER_H
#include "Mat.h"
#include "Init.h"
#include "BasicOperate.h"

namespace Matrix {
/******************************************************************************
* 
*                    内积空间
* 
******************************************************************************/

	/*---------------- inner product  ----------------*/
	inline double dot(Mat<>& a, Mat<>& b) {
		if (a.rows != b.rows || a.cols != b.cols)
			exit(-1);
		double ans = 0;

		for (int i = 0; i < a.size(); i++)
			ans += a[i] * b[i];
		return ans;
	}

	inline double dot(vector<double>& a, vector<double>& b) {
		if (a.size() != b.size())
			exit(-1);
		double ans = 0;

		for (int i = 0; i < a.size(); i++)
			ans += a[i] * b[i];
		return ans;
	}

	/*---------------- norm ----------------*/
	inline double norm(Mat<>& a) {
		return sqrt(dot(a, a));
	}

	inline double norm(Mat<>& a, const char ctrl) {
		return sqrt(dot(a, a)); //####
	}

	inline double norm(vector<double>& a) {
		return sqrt(dot(a, a));
	}

	inline double norm(vector<double>& a, const int n) {
		return sqrt(dot(a, a)); //####
	}

	/*---------------- normalize -----------------*/
	inline Mat<>& normalize(Mat<>& a) {
		double t = norm(a);
		if (t == 0)
			return a;

		for (int i = 0; i < a.size(); i++)
			a[i] /= t;

		return a;
	}

	inline vector<double>& normalize(vector<double>& a) {
		double t = norm(a);
		if (t == 0)
			return a;

		for (int i = 0; i < a.size(); i++)
			a[i] /= t;

		return a;
	}
}

#endif