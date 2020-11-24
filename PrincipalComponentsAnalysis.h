/*
Copyright 2020 LiGuer. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
	http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#include "Mat.h"
/*--------------------[ Principal Components Analysis ]--------------------
*	Principal Components Analysis 主成分分析
*	输入: [1] x: 待处理矩阵
*	输出: [1] y: 降维后矩阵
*	原理:
		[1] 最近重构性: 样本点到这个超平面的距离都足够近。
		[2] 最大可分性: 样本点在这个超平面的投影能尽可能分开。
*	流程:                  ->   ->
		[1] 数据中心化, Σ xi = 0
		[2] 计算协方差矩阵, X XT
		[3] 对协方差特征值分解
		[4] 取最大的d'个特征值所对应的特征向量{w1,w2,...,wd'},投影矩阵 W = [w1,w2,...,wd']
		[5] 样本点在超平面投影: yi = WT xi
*	推导:
		分别从原理[1, 2]可以推得同样的结果。
		样本点到超空间投影: y = WT x, 尽可能分开即.方差最大:max Σ WT x xT W
		可得:    maxW  tr( WT x xT W )
				 st. WT W = I
		拉格朗日乘子法,得:	X XT ωi = λi ωi,		即.对协方差X XT, 特征值求解
		取特征值最大的yDim个特征向量, 即目标投影矩阵W
**-------------------------------------------------------------------------*/
void PrincipalComponentsAnalysis(Mat<double>& x, Mat<double>& y, int yDim) {
	//[1] 数据中心化
	Mat<double> mean;
	x.sum(0, mean);
	mean.mult(1.0 / x.cols, mean);		//得到均值
	Mat<double> x2(x.rows, x.cols);
	for (int i = 0; i < x.rows; i++) {
		for (int j = 0; j < x.cols; j++) {
			x2[i * x.cols + j] = -mean[i];
		}
	}
	x2.add(x, x2);
	//[2] 计算协方差矩阵
	Mat<double> Cov;
	x2.transposi(Cov);			//XT
	Cov.mult(x2, Cov);		//X*XT
	//[3] 对协方差特征值分解
	Mat<double> eigVec, eigValue;
	Cov.eig(1e-5, eigVec, eigValue);
	//[4] 取最大的d'个特征值所对应的特征向量{w1,w2,...,wd'},投影矩阵 W = [w1,w2,...,wd']T
	// sort
	Mat<double> eigValueTemp(1, eigValue.cols), W(eigVec.rows, yDim);
	for (int i = 0; i < eigValue.cols; i++)eigValueTemp[i] = eigValue[i * i];
	std::sort(eigValueTemp.data, eigValueTemp.data + eigValueTemp.cols);
	// W
	for (int i = 0; i < yDim; i++) {
		double value = eigValueTemp[eigValueTemp.cols - 1 - i];
		for (int j = 0; j < eigValue.cols; j++) {
			if (eigValue[j * eigValue.cols + j] == value) {
				for (int k = 0; k < eigVec.rows; k++) {
					W[k * W.cols + i] = eigVec[k * eigVec.cols + j];
				}
				eigValue[j * eigVec.cols + j] = -DBL_MAX;
				break;
			}
		}
	}
	W.transposi(W);
	//[5]
	y.mult(W, x);
}