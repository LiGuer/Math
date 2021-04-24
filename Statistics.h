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
#include <vector>
#include <algorithm>
#define PI 3.141592653589

namespace Statistics {
	/*--------------------------------[ 基础参数 ]--------------------------------*/
	double Mean(Mat<double>& x);														//平均值
	Mat<double>& Mean(Mat<double>& x, Mat<double>& ans);								//平均值
	double Variance(Mat<double>& x);													//方差
	Mat<double>& Variance(Mat<double>& x, Mat<double>& ans);							//方差
	/*--------------------------------[ 概率分布、检验 ]--------------------------------*/
	Mat<double>& NormalDistribution(double mean, double variance, double min, double max, double delta, Mat<double>& output);	//正态分布
	bool SkewnessKurtosisTest(Mat<double>& x, double SignificanceLevel);														//正态分布-偏度峰度检验
	double X2Test(Mat<double>& x, double delta, int N, double (*F)(double));
	/*--------------------------------[ 统计图表 ]--------------------------------*/
	Mat<int>& Histogram(Mat<double>& x, int N, Mat<int>& frequency, double overFlow = NULL, double underFlow = NULL);			//直方图
	void BoxPlot(Mat<double>& input, Mat<double>& MedianQuartileThreshold, std::vector<int>* OutlierIndex);						//箱形图
}
#endif