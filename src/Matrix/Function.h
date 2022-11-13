#ifndef MATRIX_FUNCTION_H
#define MATRIX_FUNCTION_H
#include "Mat.h"
#include "Init.h"

namespace Matrix {
	/*----------------º¯Êý²Ù×÷ [ function ]----------------*/
	template<typename F>
	Mat<>& function(Mat<>& ans, Mat<>& x, F&& f) {
		ans.alloc(x.rows, x.cols);
		for (int i = 0; i < x.size(); i++)
			ans[i] = f(x[i]);
		return *this;
	}

	template<typename F>
	Mat<>& function(Mat<>& ans, F&& f) {
		for (int i = 0; i < ans.size(); i++)
			ans[i] = f(ans[i]);
		return *this;
	}

	template<typename F>
	Mat<>& functionIndex(Mat<>& ans, Mat<>& x, F&& f) {
		ans.alloc(x.rows, x.cols);
		for (int i = 0; i < x.size(); i++)
			ans[i] = f(x[i], i);
		return *this;
	}

	template<typename F>
	Mat<>& functionIndex(Mat<>& ans, F&& f) {
		for (int i = 0; i < ans.size(); i++)
			ans[i] = f(ans[i], i);
		return *this;
	}
}

#endif