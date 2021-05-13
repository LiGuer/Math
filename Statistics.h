/*
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

namespace Statistics {
	/*--------------------------------[ 基础参数 ]--------------------------------*/
	double Mean(Mat<>& x);											//平均值
	Mat<>& Mean(Mat<>& x, Mat<>& ans);								//平均值
	double Variance(Mat<>& x);										//方差
	Mat<>& Variance(Mat<>& x, Mat<>& ans);							//方差
	/*--------------------------------[ 概率分布、检验 ]--------------------------------*/
	bool SkewnessKurtosisTest(Mat<>& x, double SignificanceLevel);
	/*--------------------------------[ 统计图表 ]--------------------------------*/
	Mat<int>& Histogram(Mat<>& x, int N, Mat<int>& frequency, double overFlow = NULL, double underFlow = NULL);			//直方图
	void BoxPlot(Mat<>& input, Mat<>& MediQuartThreshold, std::vector<int>* OutlierIndex);						//箱形图
/***************************************************************************
*								常见分布  密度函数
*	[公式]: 
		正态分布: f(x) = 1 / sqrt(2πσ²)·exp(-(x-μ)² / (2σ²))
		指数分布: f(x) = 1 / μ· exp(-x/μ)
/***************************************************************************/
inline double NormalDistribFunc	(double x, double mean = 0, double var = 1) {
	return 1 / sqrt(2 * PI * var) * exp(-pow(x - mean, 2) / (2 * var));
}
inline double ExpDensityFunc	(double x, double mean) {
	return x <= 0 ? 0.0 : 1.0 / mean * exp(-x / mean);
}
inline double ExpDistribFunc	(double x, double mean) {
	return x < 0 ? 0.0 : 1 - exp(-x / mean);
}
/***************************************************************************
*								X²拟合检验
*	[公式]: X² = Σ_(i=0)^N  fi² / (n pi) - n
*	[流程]:
		[1] 频数统计
		[2] X²拟合检验
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
		printf("[%f,%f] %d %f %f\n", St + (i - 1) * delta, St + i * delta, freq[i], n * p, p);
		if (p == 0) continue;
		X2 += pow(freq[i], 2) / (n * p);
	} return X2 -= n;
}
}
#endif