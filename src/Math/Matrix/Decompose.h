#ifndef MATRIX_DECOMPOSE_H
#define MATRIX_DECOMPOSE_H
#include "Mat.h"
#include "Init.h"
#include "Operate/Basic.h"
#include "Transform.h"
#include "Submatrix.h"
/*
 * 矩阵分解
 */
namespace Matrix{
void LUP(Mat<>& A, Mat<>& U, Mat<>& L, Mat<>& P);	// LUP 上下三角分解
														// LU  上下三角对角分解
														// GGT 对称三角分解
void QR (Mat<>& A, Mat<>& Q, Mat<>& R);					// QR  正交三角分解
void SVD(Mat<>& A, Mat<>& U, Mat<>& S, Mat<>& V);		// SVD 奇异值分解
														// FG  满秩分解

/******************************************************************************
* LUP 上下三角分解
******************************************************************************/
inline void LUP(Mat<>& a, Mat<>& U, Mat<>& L, Mat<>& P) {
    if (a.rows != a.cols)
        exit(-1);

    int n = a.rows;
    Mat<> A(a);
    P.zero(n);
    U.zero(n, n);
    L.zero(n, n);

    for (int i = 0; i < n; i++) 
        P[i] = i;

    //[1]
    for (int k = 0; k < n; k++) {
        //[2] 选主元
        double maxvalue = 0;
        int kt;
        for (int i = k; i < n; i++) {
            if (fabs(A(i, k)) > maxvalue) { 
                maxvalue = fabs(A(i, k));
                kt = i;
            }
        }
        if (maxvalue == 0) exit(-1);				// singular matrix，秩 rank<n

        //[3] 置换行
        for (int i = 0; i < n; i++) {
            double t = A(k, i); A(k, i) = A(kt, i); A(kt, i) = t;
        }
        int t = P[k]; P[k] = P[kt]; P[kt] = t;

        //[4] LU分解: 高斯消元法
        for (int i = k + 1; i < n; i++) {
            A(i, k) /= A(k, k);					//aik存储消去该行第k位所需的乘数,即L
            for (int j = k + 1; j < n; j++)
                A(i, j) -= A(i, k) * A(k, j);	//初等行变换，消去该行第k位
        }
    }
    //[5] A中包含U,L，分离出来即可
    E(L);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (i > j) L(i, j) = A(i, j);
            else	   U(i, j) = A(i, j);
}
/******************************************************************************
* QR  正交三角分解
******************************************************************************/
inline void QR(Mat<>& A, Mat<>& Q, Mat<>& R) {
    R = A;
    Q.zero(A.rows);
    E(Q);

    Mat<> v, e, Ti, T(A.rows, A.cols);
    for (int i = 0; i < A.rows; i++) {
        block(R, v, i, R.rows - 1, i, i);
        e.zero(v.rows);
        e(0) = 1;

        mul(e, norm(v), e);
        mul(v, v, e);
        normalize(v);

        reflect(v, Ti);
        E(T);
        setBlock(T, Ti, i, i);

        mul(R, T, R);
        mul(Q, T, Q);
    }

    transpose(Q, Q);
}

}

#endif