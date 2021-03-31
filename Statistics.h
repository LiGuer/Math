#ifndef STATISTICS_H
#define STATISTICS_H
#include "Mat.h"
#include <vector>
#include <algorithm>
namespace Statistics {
	/*--------------------------------[ 平均值 ]--------------------------------*/
	Mat<double>& Mean(Mat<double>& x, Mat<double>& ans) { ans.mult(1.0 / x.cols, x.sumCol(ans)); return ans; }
	/*--------------------------------[ 方差 ]--------------------------------*/
	Mat<double>& Variance(Mat<double>& x, Mat<double>& ans) {
		//s² = Σ(x_i - x_mean) / (n-1)
		Mat<double> x_mean;
		x_mean.repeatCol(x.cols, Mean(x, x_mean));
		ans.add(x, x_mean.negative(ans));
		ans.mult(1.0 / (x.cols - 1), ans.sumCol(ans));
		return ans;
	}
	/*--------------------------------[ 直方图 ]--------------------------------*/
	Mat<double>& Histogram(Mat<double>& x, double delta, Mat<double>& Histogram_x, Mat<double>& Histogram_Frequency) {
		double min = x.min();
		double max = x.max();
		Histogram_x.zero(x.rows, (max - min) / delta);
		Histogram_Frequency.zero(x.rows, (max - min) / delta);
		for (int i = 0; i < (max - min) / delta; i++)
			Histogram_x[i] = (max - min) / delta + i * delta;
		for (int i = 0; i < x.rows; i++)
			for (int j = 0; j < x.cols; j++)
				Histogram_Frequency(i, (x(i, j) - min) / delta)++;
		return Histogram_Frequency;
	}
	/*--------------------------------[ BoxPlot 箱形图 ]--------------------------------
	*[目的]: 数据 => 中位数、上/下四分位数  => 上边缘、下边缘
	**------------------------------------------------------------------------*/
	void BoxPlot(Mat<double>& input, Mat<double>& Median, Mat<double>& Quartile_min, Mat<double>& Quartile_max, Mat<double>& Threshold_min, Mat<double>& Threshold_max, std::vector<int>* OutlierIndex) {
		// 中位数、上/下四分位数
		Median.zero(input.rows, 1); Quartile_min.zero(input.rows, 1); Quartile_max.zero(input.rows, 1);
		Mat<double> inputTmp(input);
		for (int i = 0; i < input.rows; i++) {
			std::sort(inputTmp.data + inputTmp.cols * i, inputTmp.data + inputTmp.cols * (i + 1));
			Median[i] = (inputTmp(i, inputTmp.cols / 2) + inputTmp(i, (int)(inputTmp.cols / 2 + 0.5))) / 2;
			Quartile_min[i] = (inputTmp(i, 1.0 / 4 * inputTmp.cols) + inputTmp(i, (int)(1.0 / 4 * inputTmp.cols + 0.5))) / 2;
			Quartile_max[i] = (inputTmp(i, 3.0 / 4 * inputTmp.cols) + inputTmp(i, (int)(3.0 / 4 * inputTmp.cols + 0.5))) / 2;
		}
		// 上边缘、下边缘
		Mat<double> delta, tmp;
		delta.add(Quartile_max, Quartile_min.negative(tmp));
		Threshold_min.add(Quartile_min, tmp.mult(-1.5, delta));
		Threshold_max.add(Quartile_max, tmp.mult(+1.5, delta));
		for (int i = 0; i < input.rows; i++)
			for (int j = 0; j < input.cols; j++)
				if (input(i, j) < Threshold_min[i] || input(i, j) > Threshold_max[i])
					OutlierIndex[i].push_back(j);
	}
}
#endif