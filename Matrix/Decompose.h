#ifndef DECOMPOSE_H
#define DECOMPOSE_H
#include "Matrix.h"
#include "Transform.h"
/*
 * 矩阵分解
 */
namespace Matrix{
void LUP(Mat<>& A, Mat<>& U, Mat<>& L, Mat<int>& P);	// LUP 上下三角分解
														// LU  上下三角对角分解
														// GGT 对称三角分解
void QR (Mat<>& A, Mat<>& Q, Mat<>& R);					// QR  正交三角分解
void SVG(Mat<>& A, Mat<>& U, Mat<>& S, Mat<>& V);		// SVG 奇异值分解
														// FG  满秩分解
}

#endif