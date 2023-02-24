#ifndef MATRIX_OPERATE_SUM_H
#define MATRIX_OPERATE_SUM_H
#include "Mat.h"
#include "Init.h"
#include "BasicOperate.h"

namespace Matrix {
	/******************************************************************************
	*
	*                    求和
	*
	******************************************************************************/

	/*---------------- max/min ----------------*/
	inline double max(Mat<>& a) {
		double ans = a[0];
		for (int i = 1; i < a.size(); i++)
			ans = ans >= a[i] ? ans : a[i];
		return ans;
	}

	inline double min(Mat<>& a) {
		double ans = a[0];
		for (int i = 1; i < a.size(); i++)
			ans = ans <= a[i] ? ans : a[i];
		return ans;
	}

	inline double max(vector<double>& a) {
		double ans = a[0];
		for (int i = 1; i < a.size(); i++)
			ans = ans >= a[i] ? ans : a[i];
		return ans;
	}

	inline double min(vector<double>& a) {
		double ans = a[0];
		for (int i = 1; i < a.size(); i++)
			ans = ans <= a[i] ? ans : a[i];
		return ans;
	}

	inline double max(Mat<>& a, int& index) {
		double ans = a[0];
		index = 0;

		for (int i = 1; i < a.size(); i++)
			if (ans < a[i]) {
				ans = a[i];
				index = i;
			}
		return ans;
	}

	inline double min(Mat<>& a, int& index) {
		double ans = a[0];
		index = 0;

		for (int i = 1; i < a.size(); i++)
			if (ans > a[i]) {
				ans = a[i];
				index = i;
			}
		return ans;
	}

	// 行/列
	inline Mat<>& max(Mat<>& ans, Mat<>& a, int index) {
		if (index == 0) {
			ans.alloc(a.rows);

			for (int x = 0; x < a.rows; x++)
				ans[x] = a(x, 0);

			for (int x = 0; x < a.rows; x++)
				for (int y = 0; y < a.cols; y++)
					ans(x) = ans(x) >= a(x, y) ? ans(x) : a(x, y);
			return ans;
		}
		else {
			ans.alloc(1, a.cols);

			for (int y = 0; y < a.cols; y++)
				ans[y] = a(0, y);

			for (int x = 0; x < a.rows; x++)
				for (int y = 0; y < a.cols; y++)
					ans(y) = ans(y) >= a(x, y) ? ans(y) : a(x, y);
			return ans;
		}
	}

	inline Mat<>& min(Mat<>& ans, Mat<>& a, int index) {
		if (index == 0) {
			ans.alloc(a.rows);

			for (int x = 0; x < a.rows; x++)
				ans[x] = a(x, 0);

			for (int x = 0; x < a.rows; x++)
				for (int y = 0; y < a.cols; y++)
					ans(x) = ans(x) <= a(x, y) ? ans(x) : a(x, y);
			return ans;
		}
		else {
			ans.alloc(1, a.cols);

			for (int y = 0; y < a.cols; y++)
				ans[y] = a(0, y);

			for (int x = 0; x < a.rows; x++)
				for (int y = 0; y < a.cols; y++)
					ans(y) = ans(y) <= a(x, y) ? ans(y) : a(x, y);
			return ans;
		}
	}

	/*---------------- 求和 ----------------*/
	inline double sum(Mat<>& a) {
		double ans = 0;
		for (int i = 1; i < a.size(); i++)
			ans += a[i];
		return ans;
	}

	inline Mat<>& sum(Mat<>& ans, Mat<>& a, int dim) {
		if (dim == 0) {				//对每一列求和
			Mat<> ansTmp(1, a.cols);
			for (int j = 0; j < a.cols; j++)
				for (int i = 0; i < a.rows; i++)
					ansTmp[j] += a(i, j);

			ans.eatMat(ansTmp);
			return ans;
		}

		if (dim == 1) {				//对每一行求和
			Mat<> ansTmp(a.rows);
			for (int i = 0; i < a.rows; i++)
				for (int j = 0; j < a.cols; j++)
					ansTmp[i] += a(i, j);

			ans.eatMat(ansTmp);
			return ans;
		}
		exit(-1);
		return ans;
	}

	/*---------------- 求积 ----------------*/
	inline double product(Mat<>& a) {
		double ans = 0;
		for (int i = 1; i < a.size(); i++)
			ans *= a[i];
		return ans;
	}

}

#endif