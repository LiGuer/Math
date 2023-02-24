#ifndef MATRIX_OPERATE_BASICOPERATE_H
#define MATRIX_OPERATE_BASICOPERATE_H
#include "Mat.h"
#include "Init.h"

#include <vector>

using namespace std;

namespace Matrix {
/******************************************************************************
* 
*                    Basic operations of matrix
* 
******************************************************************************/

	/*---------------- negative ----------------*/
	inline Mat<>& negative(Mat<>& ans, Mat<>& a) {
		ans.alloc(a.rows, a.cols);

		for (int i = 0; i < a.size(); i++)
			ans[i] = -a[i];
		return ans;
	}

	inline Mat<>& operator-(Mat<>& a) {
		Mat<> ans;
		ans.alloc(a.rows, a.cols);

		for (int i = 0; i < a.size(); i++)
			ans[i] = -a[i];
		return ans;
	}

	/*---------------- transpose ----------------*/
	inline Mat<>& transpose(Mat<>& ans, Mat<>& a) {
		Mat<> ansTmp(a.cols, a.rows);

		for (int i = 0; i < a.rows; i++)
			for (int j = 0; j < a.cols; j++)
				ansTmp(j, i) = a(i, j);

		ans.eatMat(ansTmp);
		return ans;
	}

	/*---------------- addition ----------------*/
	inline Mat<>& add(Mat<>& ans, Mat<>& a, Mat<>& b) {
		if (a.rows != b.rows || a.cols != b.cols)
			exit(-1);

		ans.alloc(a.rows, a.cols);

		for (int i = 0; i < a.size(); i++)
			ans[i] = a[i] + b[i];
		return ans;
	}

	inline vector<double>& add(vector<double>& ans, vector<double>& a, vector<double>& b) {
		if (a.size() != b.size() || a.size() != ans.size())
			exit(-1);

		for (int i = 0; i < a.size(); i++)
			ans[i] = a[i] + b[i];
		return ans;
	}

	/*---------------- subtraction ----------------*/
	inline Mat<>& sub(Mat<>& ans, Mat<>& a, Mat<>& b) {
		if (a.rows != b.rows || a.cols != b.cols)
			exit(-1);
		ans.alloc(a.rows, a.cols);

		for (int i = 0; i < a.size(); i++)
			ans[i] = a[i] - b[i];
		return ans;
	}

	inline vector<double>& sub(vector<double>& ans, vector<double>& a, vector<double>& b) {
		if (a.size() != b.size() || a.size() != ans.size())
			exit(-1);

		for (int i = 0; i < a.size(); i++)
			ans[i] = a[i] - b[i];
		return ans;
	}

	/*---------------- multiplication ----------------*/
	inline Mat<>& mul(Mat<>& ans, Mat<>& a, Mat<>& b) {
		if (a.cols != b.rows)
			exit(-1);
		Mat<> ansTmp(a.rows, b.cols);

		for (int i = 0; i < a.rows; i++)
			for (int j = 0; j < b.cols; j++)
				for (int k = 0; k < a.cols; k++)
					ansTmp(i, j) += a(i, k) * b(k, j);

		ans.eatMat(ansTmp);
		return ans;
	}

	inline vector<double>& mul(vector<double>& ans, Mat<>& a, vector<double>& b) {
		if (a.cols != b.size())
			exit(-1);
		vector<double> ansTmp(a.rows, 0);

		for (int i = 0; i < a.rows; i++)
			for (int j = 0; j < a.cols; j++)
				ansTmp[i] += a(i, j) * b[j];

		ans = ansTmp;
		return ans;
	}

	// scalar multiplication
	inline Mat<>& mul(Mat<>& ans, const double a, Mat<>& b) {
		ans.alloc(b.rows, b.cols);

		for (int i = 0; i < ans.size(); i++)
			ans[i] = a * b[i];
		return ans;
	}

	inline vector<double>& mul(vector<double>& ans, const double a, vector<double>& b) {
		for (int i = 0; i < ans.size(); i++)
			ans[i] = a * b[i];
		return ans;
	}

	/*---------------- multiplication of elements, Hadamard product ----------------*/
	inline Mat<>& elementMul(Mat<>& ans, Mat<>& a, Mat<>& b) {
		if (a.rows != b.rows || a.cols != b.cols)
			exit(-1);
		ans.alloc(a.rows, a.cols);

		for (int i = 0; i < ans.size(); i++)
			ans[i] = a[i] * b[i];
		return ans;
	}

	inline vector<double>& elementMul(vector<double>& ans, vector<double>& a, vector<double>& b) {
		if (a.size() != b.size())
			exit(-1);

		for (int i = 0; i < ans.size(); i++)
			ans[i] = a[i] * b[i];
		return ans;
	}

	/*---------------- division of elements ----------------*/
	inline Mat<>& elementDiv(Mat<>& ans, Mat<>& a, Mat<>& b) {
		if (a.rows != b.rows || a.cols != b.cols)
			exit(-1);
		ans.alloc(a.rows, a.cols);

		for (int i = 0; i < ans.size(); i++)
			ans[i] = a[i] / b[i];
		return ans;
	}

	inline vector<double>& elementDiv(vector<double>& ans, vector<double>& a, vector<double>& b) {
		if (a.size() != b.size())
			exit(-1);

		for (int i = 0; i < ans.size(); i++)
			ans[i] = a[i] / b[i];
		return ans;
	}

}
#endif