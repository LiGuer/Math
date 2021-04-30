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
#include "Statistics.h"
/***************************************************************************
*								平均值
*	Mean = Σx_i / N
/***************************************************************************/
double Statistics::Mean(Mat<double>& x) { return x.sum() / x.cols; }
Mat<double>& Statistics::Mean(Mat<double>& x, Mat<double>& ans) {
	return ans.mult(1.0 / x.cols, x.sum(ans, 1));
}
/***************************************************************************
*								方差
*	Var² = Σ(x_i - x_mean)² / (N - 1)
/***************************************************************************/
double Statistics::Variance(Mat<double>& x) {
	double var = 0, mean = Mean(x);
	for (int i = 0; i < x.cols * x.rows; i++) var += pow(x[i] - mean, 2);
	return var / (x.cols * x.rows - 1);
}
Mat<double>& Statistics::Variance(Mat<double>& x, Mat<double>& ans) {
	Mat<double> mean; Mean(x, mean);
	ans.zero(x.rows, 1);
	for (int i = 0; i < x.rows; i++) {
		for (int j = 0; j < x.cols; j++)
			ans[i] += pow(x(i, j) - mean[i], 2);
		ans[i] /= x.cols - 1;
	} return ans;
}
/***************************************************************************
*								正态分布
*	f(x) = 1 / sqrt(2πσ²)·exp(-(x-μ)² / (2σ²))
/***************************************************************************/
Mat<double>& Statistics::NormalDistribution(double mean, double variance, double min, double max, double delta, Mat<double>& output) {
	output.alloc(1, (max - min) / delta + 1);
	double x = min, A = 1.0 / (sqrt(2 * PI * variance));
	for (int i = 0; i < output.cols; i++) {
		output[i] = A * exp(-(x - mean) * (x - mean) / (2 * variance));
		x += delta;
	} return output;
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
bool Statistics::SkewnessKurtosisTest(Mat<double>& x, double SignificanceLevel) {
	//[1]
	int N = x.cols;
	double mean = Mean(x), B[5] = { 0 };
	for (int k = 2; k <= 4; k++) {
		for (int i = 0; i < N; i++) B[k] += pow(x[i] - mean, k);
		B[k] /= N;
	}
	//[2]
	double Skewness = B[3] / pow(B[2], 1.5),
		   Kurtosis = B[4] / pow(B[2],   2);
	//[3]
	double sigma_1 = sqrt(6.0  *     (N - 2) / (N + 1) / (N + 3)),
		   sigma_2 = sqrt(24.0 * N * (N - 2) * (N - 3) / (N + 1) / (N + 1) / (N + 3) / (N + 5)),
		   mu_2 = 3 - 6.0 / (N + 1);
	double u1 = fabs(Skewness / sigma_1),
		   u2 = fabs(Kurtosis - mu_2) / sigma_2;
	//printf("%f %f %f %f\n", Skewness, Kurtosis, u1, u2);
	if (u1 >= SignificanceLevel || u2 >= SignificanceLevel) return false;
	return true;
}
/***************************************************************************
*								X²拟合检验
*	[流程]:
		[1] 频数统计
		[2] X²拟合检验
/***************************************************************************/
double Statistics::X2Test(Mat<double>& x, double delta, int N, double (*F)(double)) {
	//[1]
	Mat<int> frequency(1, N + 1);
	double X2 = 0, mean = Mean(x);
	for (int i = 0; i < x.cols; i++) {
		int index = x[i] <= N * delta ? x[i] / delta - 1E-20 : N;
		frequency[index]++;
	}
	//[2]
	for (int i = 0; i <= N; i++) {
		double p = F((i + 1) * delta) - F(i * delta);
		if (i == N) p = 1 - F((i + 1) * delta);
		//printf("%d\t%f\t%f\t%f\n", frequency[i],p, x.cols * p, pow(frequency[i], 2) / (x.cols * p));
		X2 += pow(frequency[i], 2) / (x.cols * p);
	}
	return X2 -= x.cols;
}
/***************************************************************************
*								直方图
*	[输入]: OverFlow: 上溢出	UnderFlow: 下溢出
*	[注]: 区间规定"左开右闭"
/***************************************************************************/
Mat<int>& Statistics::Histogram(Mat<double>& x, int N, Mat<int>& frequency, double overFlow, double underFlow) {
	double max =  overFlow == NULL ? x.max() : overFlow,
		   min = underFlow == NULL ? x.min() : underFlow,
	double delta = (max - min) / N;
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
void Statistics::BoxPlot(Mat<double>& x, Mat<double>& MediQuartThreshold, std::vector<int>* OutlierIndex) {
	// 中位数、小/大四分位数、小/大边缘
	MediQuartThreshold.zero(x.rows, 5);
	Mat<double> xTmp(x);
	for (int i = 0; i < x.rows; i++) {
		std::sort(
			xTmp.data +  i      * xTmp.cols, 
			xTmp.data + (i + 1) * xTmp.cols
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