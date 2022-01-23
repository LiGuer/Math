#ifndef DECOMPOSE_H
#define DECOMPOSE_H
#include "Matrix.h"
/*
 * 矩阵分解
 */

void LUP(Mat<>& a, Mat<>& U, Mat<>& L, Mat<int>& P);    // LUP 上下三角分解
                                                        // LU  上下三角对角分解
                                                        // GGT 对称三角分解
                                                        // QR  正交三角分解
                                                        // SVG 奇异值分解
                                                        // FG  满秩分解

#endif