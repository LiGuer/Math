﻿/*
Copyright 2020,2021 LiGuer. All Rights Reserved.
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
#ifndef STATISTICS_H
#define STATISTICS_H
#include "Mat.h"
#include <math.h>
#include <vector>
#include <algorithm>
#define PI 3.141592653589
/******************************************************************************
*                    概率统计
-------------------------------------------------------------------------------
基础统计特征:
double Mean		(Mat<>& x);					//均值
Mat<>& Mean		(Mat<>& x, Mat<>& ans);
double Variance	(Mat<>& x);					//方差
Mat<>& Variance	(Mat<>& x, Mat<>& ans);
double NormalDistrib	(double x, double mean = 0, double var = 1);	//常见分布: 分布函数、密度函数
double    ExpDensity	(double x, double mean);
double    ExpDistrib	(double x, double mean);
假设检验:
double	X2Test	(Mat<>& x, double St, double Ed, int N, F&& DistribFunc);	//X²拟合检验
bool	SkewnessKurtosisTest	(Mat<>& x, double SignificanceLevel);		//偏度-峰度
其他:
Mat<int>& Histogram(Mat<>& x, int N, Mat<int>& frequency, double overFlow, double underFlow);	//直方图
void BoxPlot(Mat<>& x, Mat<>& MediQuartThreshold, std::vector<int>* OutlierIndex)	//箱形图
******************************************************************************/
namespace Statistics {
/*#############################################################################

								基础统计特征

#############################################################################*/
/***************************************************************************
*								均值 / 方差
*	[公式]:
		Mean = Σx_i / N
		Var  = Σ(x_i - x_mean)² / (N - 1)
/***************************************************************************/
//均值
double Mean(Mat<>& x) { return x.sum() / x.size(); }
Mat<>& Mean(Mat<>& x, Mat<>& ans) { return ans.mul(1.0 / x.cols, x.sum(ans, 1)); }
//方差
double Variance(Mat<>& x) {
	double var = 0, mean = Mean(x);
	for (int i = 0; i < x.size(); i++) var += pow(x[i] - mean, 2);
	return var / (x.size() - 1);
}
Mat<>& Variance(Mat<>& x, Mat<>& ans) {
	Mat<> mean; Mean(x, mean);
	ans.alloc(x.rows);
	for (int i = 0; i < x.rows; i++) {
		for (int j = 0; j < x.cols; j++)
			ans[i] += pow(x(i, j) - mean[i], 2);
		ans[i] /= x.cols - 1;
	} return ans;
}
/***************************************************************************
*						常见分布: 分布函数、密度函数
*	[公式]: 
		正态分布: f(x) = 1 / sqrt(2πσ²)·exp(-(x-μ)² / (2σ²))
		指数分布: f(x) = 1 / μ· exp(-x/μ)	(x>0)
				  F(x) = 1 - exp(-x/μ)			(x>0)
/***************************************************************************/
inline double NormalDistrib	(double x, double mean = 0, double var = 1) {
	return 1 / sqrt(2 * PI * var) * exp(-pow(x - mean, 2) / (2 * var));
}
inline double ExpDensity	(double x, double mean) {
	return x <= 0 ? 0.0 : 1.0 / mean * exp(-x / mean);
}
inline double ExpDistrib	(double x, double mean) {
	return x <= 0 ? 0.0 : 1 - exp(-x / mean);
}
/***************************************************************************
*								偏度峰度
*	[定义]:
		偏度: 样本标准化变量Y = [X - E(X)] / sqrt D(X)的三阶矩E(Y³)=E[(X-E(X))³]/D(X)³`²
		峰度: 样本标准化变量Y = [X - E(X)] / sqrt D(X)的四阶矩E(Y⁴)=E[(X-E(X))⁴]/D(X)²
*	[过程]:
		[1] 计算样本中心矩 D(X), E[(X-E(X))³], E[(X-E(X))⁴]
		[2] 计算偏度、峰度 E[(X-E(X))³]/D(X)³`², E[(X-E(X))⁴]/D(X)²
/***************************************************************************/
double Skewness(Mat<>& x) {
	double mean = Mean(x), B[5] = { 0 };
	for (int k = 2; k <= 4; k++) {
		for (int i = 0; i < x.size(); i++) B[k] += pow(x[i] - mean, k);
		B[k] /= x.size();
	}
	return B[3] / pow(B[2], 1.5);
}
double Kurtosis(Mat<>& x) {
	double mean = Mean(x), B[5] = { 0 };
	for (int k = 2; k <= 4; k++) {
		for (int i = 0; i < x.size(); i++) B[k] += pow(x[i] - mean, k);
		B[k] /= x.size();
	}
	return B[4] / pow(B[2], 2);
}
/*#############################################################################

								假设检验

#############################################################################*/
/***************************************************************************
*								X²拟合检验
*	[公式]: X² = Σ_(i=0)^N  fi² / (n pi) - n
*	[流程]:
		[1] 频数统计
		[2] X²拟合检验
----------------------------------------------------------------------------
*	[Example]:
		double mean = Statistics::Mean(data);
		double X2   = Statistics::X2Test( data, 0, 20, 10,
			[&mean](double x) { return Statistics::ExpDistrib(x, mean); }
		);
/***************************************************************************/
template<typename F>
double X2Test(Mat<>& x, double St, double Ed, int N, F&& DistribFunc) {
	//[1]
	double delta = (Ed - St) / N, n = x.size();
	Mat<int> freq(N + 2);
	for (int i = 0; i < x.size(); i++) {
		int index = ceil((x[i] - St) / delta);
		if (x[i] < St) index = 0;
		if (x[i] > Ed) index = freq.size() - 1;
		if (x[i] ==St) index = 1;
		freq[index]++;
	}
	//[2]
	double p, X2 = 0;
	for (int i = 0; i < freq.size(); i++) {
		p = (i == 0) ? DistribFunc(St) : (
			(i == freq.size() - 1) ? (1 - DistribFunc(Ed)) :
			DistribFunc(St + i * delta) - DistribFunc(St + (i - 1) * delta)
			);
		//printf("[%.0f,%.0f] %d %.3f %.3f\n", St + (i - 1) * delta, St + i * delta, freq[i], n * p, p);
		if (p == 0) continue;
		X2 += pow(freq[i], 2) / (n * p);
	} return X2 -= n;
}
/***************************************************************************
*								正态分布 - 偏度峰度检验
*	[定义]:
		偏度: 样本标准化变量Y = [X - E(X)] / sqrt D(X)的三阶矩E(Y³)=E[(X-E(X))³]/D(X)³`²
		峰度: 样本标准化变量Y = [X - E(X)] / sqrt D(X)的四阶矩E(Y⁴)=E[(X-E(X))⁴]/D(X)²
		若样本服从正态分布, 偏度 == 0 且 峰度 == 3
*	[过程]:
		[1] 计算样本中心矩 D(X), E[(X-E(X))³], E[(X-E(X))⁴]
		[2] 计算偏度、峰度 E[(X-E(X))³]/D(X)³`², E[(X-E(X))⁴]/D(X)²
		[3] 假设检验
/***************************************************************************/
bool SkewnessKurtosisTest(Mat<>& x, double SignificanceLevel) {
	//[1]
	int N = x.size();
	double mean = Mean(x), B[5] = { 0 };
	for (int k = 2; k <= 4; k++) {
		for (int i = 0; i < N; i++) B[k] += pow(x[i] - mean, k);
		B[k] /= N;
	}
	//[2][3]
	double 
		Skewness = B[3] / pow(B[2], 1.5),
		Kurtosis = B[4] / pow(B[2], 2),
		sigma_1  = sqrt(6.0 * (N - 2) / (N + 1) / (N + 3)),
		sigma_2  = sqrt(24.0 * N * (N - 2) * (N - 3) / (N + 1) / (N + 1) / (N + 3) / (N + 5)),
		mu_2     = 3 - 6.0 / (N + 1),
		u1		 = fabs(Skewness / sigma_1),
		u2		 = fabs(Kurtosis - mu_2) / sigma_2;
	if (u1 >= SignificanceLevel || u2 >= SignificanceLevel) return false;
	return true;
}
/*#############################################################################

								其他

#############################################################################*/
/***************************************************************************
*								直方图
*	[输入]: OverFlow: 上溢出	UnderFlow: 下溢出
*	[注]: 区间规定"左开右闭"
/***************************************************************************/
Mat<int>& Histogram(Mat<>& x, int N, Mat<int>& frequency, double overFlow, double underFlow) {
	double max = overFlow == NULL ? x.max() : overFlow,
		min = underFlow == NULL ? x.min() : underFlow,
		delta = (max - min) / N;
	frequency.zero(1, N + (overFlow == NULL ? 1 : 0) + (underFlow == NULL ? 1 : 0));
	for (int i = 0; i < x.cols; i++) {
		int index = (x[i] - min) / delta + (overFlow == NULL ? 1 : 0);
		index = x[i] <= min ? 0 : index;
		index = x[i] > max ? frequency.cols - 1 : index;
		frequency[index]++;
	}
	return frequency;
}
/***************************************************************************
*								箱形图
*[输出]: MediQuartThreshold: (1) 中位数 (2/3) 小/大四分位数 (4/5) 小/大边缘
*[目的]: 数据 => 中位数、小/大四分位数  => 小/大边缘
/***************************************************************************/
void BoxPlot(Mat<>& x, Mat<>& MediQuartThreshold, std::vector<int>* OutlierIndex) {
	// 中位数、小/大四分位数、小/大边缘
	MediQuartThreshold.zero(x.rows, 5);
	Mat<> xTmp(x);
	for (int i = 0; i < x.rows; i++) {
		std::sort(
			xTmp.data + i	   * xTmp.cols,
			xTmp.data +(i + 1) * xTmp.cols
		);
		MediQuartThreshold(i, 0) = (xTmp(i, 1.0 / 2 * xTmp.cols) + xTmp(i, (int)(1.0 / 2 * xTmp.cols + 0.5))) / 2;
		MediQuartThreshold(i, 1) = (xTmp(i, 1.0 / 4 * xTmp.cols) + xTmp(i, (int)(1.0 / 4 * xTmp.cols + 0.5))) / 2;
		MediQuartThreshold(i, 2) = (xTmp(i, 3.0 / 4 * xTmp.cols) + xTmp(i, (int)(3.0 / 4 * xTmp.cols + 0.5))) / 2;
		double delta = MediQuartThreshold(i, 2) - MediQuartThreshold(i, 1);
		MediQuartThreshold(i, 3) = MediQuartThreshold(i, 1) - 1.5 * delta;
		MediQuartThreshold(i, 4) = MediQuartThreshold(i, 2) + 1.5 * delta;
	}
	for (int i = 0; i < x.rows; i++)
		for (int j = 0; j < x.cols; j++)
			if (x(i, j) < MediQuartThreshold(i, 3) || x(i, j) > MediQuartThreshold(i, 4))
				OutlierIndex[i].push_back(j);
}
}
#endif