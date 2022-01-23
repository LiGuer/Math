#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "Matrix.h"
/*
 * 线性变换
 */
Mat<>& translate(Mat<>& delta, 						Mat<>& transMat);  // 平移
Mat<>& rotate	(int i, int j, double theta, 		Mat<>& transMat);  // 旋转
Mat<>& rotate	(Mat<>& theta, 						Mat<>& transMat);  // 旋转
Mat<>& rotate	(Mat<>& rotateAxis, double theta, 	Mat<>& transMat);  // 旋转 (3D)
Mat<>& scale	(Mat<>& ratio, 						Mat<>& transMat);  // 缩放
                                                                       // 镜像
                                                                       // 投影
                                                                       // 正交化

#endif