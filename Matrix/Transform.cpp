#include "Transform.h"

/******************************************************************************
* 平移
    $
        [1 ]   [ 1  0  0  0 ] [1]
        [x'] = [dx  1  0  0 ] [x]
        |y'|   |dy  0  1  0 | |y|
        |z'|   |dz  0  0  1 | |z|
    $
******************************************************************************/
Mat<>& translate(Mat<>& delta, Mat<>& transMat) {
	Mat<> translateMat; translateMat.E(transMat.rows);

	for (int i = 0; i < delta.rows; i++) 
        translateMat(i + 1, 0) = delta[i];

	return transMat.mul(translateMat, transMat);
}

/******************************************************************************
* 旋转变换
    * 初等旋转矩阵.
        $T_{ij} = [
            1\\
            & \ddots \\
            & & 1\\
            & & & & \cos\theta|_{(i,i)}& & & & \sin\theta|_{(i,j)} \\
            & & & & & 1 &\\
            & & & & & & \ddots\\
            & & & & & & & 1\\
            & & & &-\sin\theta|_{(j,i)}& & & & \cos\theta|_{(j,j)}\\
            & & & & & & & & & 1 \\
            & & & & & & & & & & \ddots \\
            & & & & & & & & & & & 1 \\
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
******************************************************************************/
// 初等旋转矩阵
Mat<>& rotate(int i, int j, double theta, Mat<>& transMat) {
    transMat.E();
    transMat(i, i) = transMat(j, j) = cos(theta);
    transMat(i, j) = sin(theta);
    transMat(j, i) =-sin(theta);
    return transMat;
}

Mat<>& rotate(Mat<>& theta, Mat<>& transMat) {
    transMat.E();
	Mat<> rotateMat = transMat;

	for (int i = 0; i < theta.rows; i++) {
		for (int j = i + 1; j < theta.cols; j++) {
			transMat.mul(
				rotate(i, j, theta(i, j), rotateMat), 
				transMat
			);
		}
	}
    return transMat;
}

//3D·四元数
Mat<>& rotate(Mat<>& rotateAxis, double theta, Mat<>& transMat) {
	if (transMat.rows != 3 + 1) exit(-1);
	
	rotateAxis.normalize();
	Mat<> q(4);				//四元数
	q = {
		cos(theta / 2),
		sin(theta / 2) * rotateAxis[0],
		sin(theta / 2) * rotateAxis[1],
		sin(theta / 2) * rotateAxis[2]
	};
	// rotate mat
	transMat.zero()
	for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) transMat(i, j) = q[((j % 2 == 0 ? 1 : -1) * i + j + 4) % 4];
	for (int i = 1; i < 4; i++) transMat(i, i % 3 + 1) *= -1;
	Mat<> tmp = transMat;
	for (int i = 1; i < 4; i++) 
		transMat(0, i) *= -1, 
		tmp     (i, 0) *= -1;
	transMat *= tmp;

	return transMat;
}

/******************************************************************************
* 缩放变换
    $T_{ij} = [
        dx_1 \\
        & dx_2 \\
        & & \ddots\\
        & & & dx_n
    ]$
******************************************************************************/
inline Mat<>& scale(Mat<>& ratio, Mat<>& transMat) {
    return transMat.diag(ratio);
}