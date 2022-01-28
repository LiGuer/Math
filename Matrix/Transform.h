#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "Matrix.h"
/*
 * 线性变换
 */
namespace Matrix{
Mat<>& rotate	(int i, int j, double theta, 		Mat<>& T);	// 旋转
Mat<>& rotate	(Mat<>& theta, 						Mat<>& T);	// 旋转
Mat<>& rotate	(Mat<>& rotateAxis, double theta, 	Mat<>& T);	// 旋转 (3D)
Mat<>& shear	(int i, int j, double a, 			Mat<>& T);	// 斜切
Mat<>& scale	(Mat<>& ratio, 						Mat<>& T);	// 缩放
Mat<>& reflect	(Mat<>& e, 							Mat<>& T);	// 镜像
Mat<>& project 	(Mat<>& X, 							Mat<>& T);	// 正交投影
																// 正交化
}

#endif