#include "Decompose.h"

/******************************************************************************
* LUP 上下三角分解
    * [定义]: P A = L U		其中 L: 单位下三角矩阵  U: 上三角矩阵  P: 置换矩阵
            *	因为置换矩阵每行只有一个1，可以变为一维数组，每行计入改行1的位置
    * [算法]: 高斯消元法
            [1] 从其他方程中减去第1方程的倍数，以把那些方程第1变量消去。
            [2] 从第3及以后方程中减去第2方程倍数，以把这些方程的第1,2变量都消去。
            [3] 重复过程，直至变为上三角矩阵U，单位下三角L是由消去变量所用行的乘数组成
            * 主元pivot: LPU分解中所除元素称为主元，它们处于矩阵U的对角线上。
            * 选主元: 采用置换避免除0，避免除数很小(数值会不稳定)的操作
            * 把第1行与第k行互换 <=> 置换矩阵Q左乘A--QA
    * [过程]:
            [1] 对于每一列
                [2] 选主元
                [3] 置换行,记录在P中
                [4] LU分解: 高斯消元法
            [5] A中包含U,L，分离出来即可
******************************************************************************/
void Matrix::LUP(Mat<>& a, Mat<>& U, Mat<>& L, Mat<int>& P) {
    if (a.rows != a.cols) exit(-1);
    int n = a.rows;
    Mat<> A(a);
    P.zero(n);
    for (int i = 0; i < n; i++) P[i] = i;
    //[1]
    for (int k = 0; k < n; k++) {
        //[2] 选主元
        double maxvalue = 0;
        int kt;
        for (int i = k; i < n; i++) {
            if (fabs(A(i, k)) > maxvalue) { maxvalue = fabs(A(i, k)); kt = i; }
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
    U.zero(n, n); 
    L.E(n);
    for (int i = 0; i < n; i++) 
        for (int j = 0; j < n; j++) 
            if (i > j) L(i, j) = A(i, j);
            else	   U(i, j) = A(i, j);
}
/******************************************************************************
* QR  正交三角分解
    \def{正交三角分解} 将非奇异矩阵A化成正交矩阵Q与非奇异上三角矩阵R的乘积.$A = Q R$
    * 初等反射变换
******************************************************************************/
void Matrix::QR(Mat<>& A, Mat<>& Q, Mat<>& R){
	R = A; 
	Q.E(A.rows);

	Mat<> v, e, Ti, T(A.rows, A.cols);
	for (int i = 0; i < A.rows; i++) {
		R.block(i, R.rows - 1, i, i, v);
		e.zero(v.rows);
		e(0) = 1;

		v -= (e *= v.norm());
		v.normalize();

		Matrix::reflect(v, Ti);
		T.E().setBlock(Ti, i, i);

		R.mul(T, R);
		Q.mul(T, Q);
	}

	Q.transpose(Q);
}