#include "Transform.h"
/****************************************************************
* 旋转变换
    * 初等旋转矩阵.
        $T_{ij} = [
            1
            & \ddots 
            & & 1
            & & & \cos\theta|_{(i,i)}& & & & \sin\theta|_{(i,j)} 
            & & & & 1 &
            & & & & & \ddots
            & & & & & & 1
            & & &-\sin\theta|_{(j,i)}& & & & \cos\theta|_{(j,j)}
            & & & & & & & & 1 
            & & & & & & & & & \ddots 
            & & & & & & & & & & 1 
        ]$
    * 四元数
        v' = q v q`¹
        q = [cos(θ/2), u sin(θ/2)]
        v=>[0,v]经旋转轴u旋转Ѳ角后得到v'
        多次旋转:
            v' = q1q2 v q2`¹q1`¹ = (q1q2) v (q1q2)`¹
        四元数化旋转矩阵:
            四元数左乘:
            q v =	[a -b -c -d] v
                    |b  a -d  c|
                    |c  d  a -b|
                    [d -c  b  a]
            四元数右乘:
            v q =	[a -b -c -d] v
                    |b  a  d -c|
                    |c -d  a  b|
                    [d  c -b  a]
****************************************************************/
// 初等旋转矩阵
Mat<>& Matrix::rotate(int i, int j, double theta, Mat<>& T) {
    T.E();
    T(i, i) = T(j, j) = cos(theta);
    T(i, j) = sin(theta);
    T(j, i) =-sin(theta);
    return T;
}

Mat<>& Matrix::rotate (Mat<>& theta, Mat<>& T) {
    T.E();
	Mat<> rotateMat = T;

	for (int i = 0; i < theta.rows; i++) {
		for (int j = i + 1; j < theta.cols; j++) {
			T.mul(
				rotate(i, j, theta(i, j), rotateMat), 
				T
			);
		}
	}
    return T;
}

//3D·四元数
Mat<>& Matrix::rotate (Mat<>& rotateAxis, double theta, Mat<>& T) {
	if (T.rows != 3 + 1) exit(-1);
	
	rotateAxis.normalize();
	Mat<> q(4);				//四元数
	q = {
		cos(theta / 2),
		sin(theta / 2) * rotateAxis[0],
		sin(theta / 2) * rotateAxis[1],
		sin(theta / 2) * rotateAxis[2]
	};
	// rotate mat
	T.zero();
	for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) T(i, j) = q[((j % 2 == 0 ? 1 : -1) * i + j + 4) % 4];
	for (int i = 1; i < 4; i++) T(i, i % 3 + 1) *= -1;
	Mat<> tmp = T;
	for (int i = 1; i < 4; i++) 
		T(0, i) *= -1, 
		tmp     (i, 0) *= -1;
	T *= tmp;

	return T;
}

/****************************************************************
* 反射变换
    $ y = H x = (I - 2 e_2 e_2^T) x $
****************************************************************/
Mat<>& Matrix::reflect (Mat<>& e, Mat<>& T){
    Mat<> tmp;
    T.mul(e, e.transpose(tmp));
    T.sub(tmp.E(T.rows), T *= 2);
    return T;
}

/****************************************************************
* 斜切变换
    单位矩阵的第(i,j)个元素改为斜切比率 $a_{ij}$
****************************************************************/
Mat<>& shear (int i, int j, double a, Mat<>& T){
    T.E();
    T(i, j) = a;
    return T;
}

/****************************************************************
* 缩放变换
    $T_{ij} = [
        dx_1 
        & dx_2 
        & & \ddots
        & & & dx_n
    ]$
****************************************************************/
Mat<>& Matrix::scale (Mat<>& ratio, Mat<>& T) {
    return T.diag(ratio);
}

/****************************************************************
* 正交投影
    P_L = X(X^H X)^{-1}X^H
    X = (x_1, ... , x_r)  投影后子空间的基
****************************************************************/
Mat<>& Matrix::project (Mat<>& X, Mat<>& T){
    Mat<> tmp;
    T.mul(X.transpose(tmp), X);
    T.mul(X, T.inv(T);
    T.mul(T, X.transpose(tmp));
    return T;
}

/****************************************************************
* 正交化
****************************************************************/
/*
void orthogonalize (Mat<>& A) {
	Mat<> q;
	q[0].normalize();
	q[1] -= t1.mul(q[1].dot(q[0]), q[0]);   //施密特正交化
	q[1].normalize();
	q[2] -= t1.mul(q[2].dot(q[0]), q[0]);
	q[2] -= t1.mul(q[2].dot(q[1]), q[1]);
	q[2].normalize();
	q[3] -= t1.mul(q[3].dot(q[0]), q[0]);
	q[3] -= t1.mul(q[3].dot(q[1]), q[1]);
	q[3] -= t1.mul(q[3].dot(q[2]), q[2]);
	q[3].normalize();
}*/

/****************************************************************
* Lorentz变换
    * Minkowski时空 (t, x, y, z)^T 下的一类参考系间变换
        $
        (\mb t' \\ x' \\ y' \\ z' \me) = (\mb
            \frac{1}{\sqrt{1-(\frac{V}{c})^2}} & \frac{-\frac{V}{c^2}}{\sqrt{1-( \frac{V}{c} )^2}}
            \frac{-V}{\sqrt{1-( \frac{V}{c} )^2}} & \frac{1}{\sqrt{1-( \frac{V}{c} )^2}}
            & & 1
            & & & 1
        \me) (\mb t \\ x \\ y \\ z \me)
        $
****************************************************************/
Mat<>& Matrix::Lorentz (double V, int i, Mat<>& T){
    const double c = 299792458;
    double t = sqrt(1 - pow(V / c, 2));

    T.E(4);
    T(0,0) = T(i,i) = 1 / t;
    T(0,i) = (-V / pow(c, 2)) / t;
    T(i,0) = (-V) / t;
}