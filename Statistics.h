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
	double		 Mean(Mat<>& x);													//平均值
	Mat<>& Mean(Mat<>& x, Mat<>& ans);								//平均值
	double		 Variance(Mat<>& x);												//方差
	Mat<>& Variance(Mat<>& x, Mat<>& ans);							//方差
	/*--------------------------------[ 概率分布、检验 ]--------------------------------*/
	Mat<>& NormalDistribution(double mean, double variance, double min, double max, double delta, Mat<>& output);	//正态分布
	bool SkewnessKurtosisTest(Mat<>& x, double SignificanceLevel);														//正态分布-偏度峰度检验
	double X2Test(Mat<>& x, double delta, int N, double (*F)(double));
	/*--------------------------------[ 统计图表 ]--------------------------------*/
	Mat<int>& Histogram(Mat<>& x, int N, Mat<int>& frequency, double overFlow = NULL, double underFlow = NULL);			//直方图
	void BoxPlot(Mat<>& input, Mat<>& MediQuartThreshold, std::vector<int>* OutlierIndex);						//箱形图
}
#endif