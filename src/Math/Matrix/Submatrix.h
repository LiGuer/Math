#ifndef MATRIX_SUBMATRIX_H
#define MATRIX_SUBMATRIX_H
#include "Mat.h"

namespace Matrix {
/******************************************************************************
* 
*                    ×Ó¾ØÕó
* 
******************************************************************************/

	/*----------------¶Á/Ð´ ×Ó¾ØÕó----------------*/
	inline Mat<>& block(Mat<>& ans, Mat<>& a, int rowSt, int rowEd, int colSt, int colEd) {
		Mat<> ansTmp(rowEd - rowSt + 1, colEd - colSt + 1);
		for (int i = rowSt; i <= rowEd; i++)
			for (int j = colSt; j <= colEd; j++)
				ansTmp(i - rowSt, j - colSt) = a(i, j);
		ans.eatMat(ansTmp);
		return ans;
	}

	inline Mat<>& setBlock(Mat<>& a, Mat<>& b, int rowSt, int colSt, int rowEd = -1, int colEd = -1) {
		if (rowEd == -1) {
			for (int i = 0; i < b.rows; i++)
				for (int j = 0; j < b.cols; j++)
					a(i + rowSt, j + colSt) = b(i, j);
		}
		else {
			for (int i = rowSt; i <= rowEd; i++)
				for (int j = colSt; j <= colEd; j++)
					a(i, j) = b(i - rowSt, j - colSt);
		}
		return a;
	}

	/*----------------Æ´½Ó----------------*/
	inline Mat<>& rowStack(Mat<>& ans, Mat<>& a, Mat<>& b) {
		if (a.cols != b.cols)
			exit(-1);

		Mat<> ansTmp(a.rows + b.rows, a.cols);

		for (int i = 0; i < ansTmp.rows; i++)
			for (int j = 0; j < ansTmp.cols; j++)
				ansTmp(i, j) = i < a.rows ? a(i, j) : b(i - a.rows, j);

		ans.eatMat(ansTmp);
		return ans;
	}

	inline Mat<>& rowStack(Mat<>& ans, std::initializer_list<Mat<>> list) {
		return ans;
	}

	inline Mat<>& colStack(Mat<>& ans, Mat<>& a, Mat<>& b) {
		if (a.rows != b.rows) 
			exit(-1);

		Mat<> ansTmp(a.rows, a.cols + b.cols);

		for (int i = 0; i < ansTmp.rows; i++)
			for (int j = 0; j < ansTmp.cols; j++)
				ansTmp(i, j) = j < a.cols ? a(i, j) : b(i, j - a.cols);

		ans.eatMat(ansTmp);
		return ans;
	}

	inline Mat<>& colStack(Mat<>& ans, std::initializer_list<Mat<>> list) {
		return ans;
	}

	/*----------------¸´ÖÆÍØÕ¹----------------*/
	inline Mat<>& colRepeat(Mat<>& ans, Mat<>& a, int repeatNum) {
		if (a.cols != 1) 
			exit(-1);

		Mat<> ansTmp(a.rows, repeatNum);

		for (int i = 0; i < a.rows; i++)
			for (int j = 0; j < repeatNum; j++)
				ansTmp(i, j) = a[i];
		return ans.eatMat(ansTmp);
	}


}
#endif