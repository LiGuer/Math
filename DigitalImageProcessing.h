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
#ifndef DIGITAL_IMAGE_PROCESSING_H
#define DIGITAL_IMAGE_PROCESSING_H
#include <stdlib.h>
#include <time.h>
#include "Mat.h"
#include "BasicMachineLearning.h"
#include "../opencv2-include/opencv2/opencv.hpp"
#pragma comment(lib,"../opencv2-include/opencv_world430.lib")

namespace DigitalImageProcessing {
	/*--------------------------------[ 图像输入/输出 ]--------------------------------*/
	Mat<double>* Input(const char* inputImgUrl, Mat<double>* data) {
		cv::Mat input = cv::imread(inputImgUrl, cv::IMREAD_COLOR);
		for (int k = 0; k < 3; k++) data[k].zero(input.rows, input.cols);

		for (int i = 0; i < input.rows * input.cols; i++) {
			cv::MatIterator_<cv::Vec3b> it = input.begin<cv::Vec3b>() + i;
			for (int k = 0; k < 3; k++)  data[2 - k][i] = (double)(*it)[k] / 255;
		}
		return data;
	}
	void Output(const char* outputImgUrl, Mat<double>* data) {
		unsigned char* output = (unsigned char*)calloc(data[0].cols * data[0].rows * 3, sizeof(unsigned char));
		for (int i = 0; i < data[0].rows * data[0].cols; i++)
			for (int k = 0; k < 3; k++)
				output[i * 3 + k] = (data[k])[i] * 255;
		FILE* fp = fopen(outputImgUrl, "wb");
		fprintf(fp, "P6\n%d %d\n255\n", data[0].cols, data[0].rows);	// 写图片格式、宽高、最大像素值
		fwrite(output, 1, data[0].cols * data[0].rows * 3, fp);	// 写RGB数据
		fclose(fp);
		free(output);
	}
	void Output(const char* outputImgUrl, Mat<double>& data) {
		unsigned char* output = (unsigned char*)calloc(data.cols * data.rows, sizeof(unsigned char));
		for (int i = 0; i < data.rows * data.cols; i++)
				for (int k = 0; k < 3; k++)
					output[i] = data[i] * 255;
		FILE* fp = fopen(outputImgUrl, "wb");
		fprintf(fp, "P5\n%d %d\n255\n", data.cols, data.rows);	// 写图片格式、宽高、最大像素值
		fwrite(output, 1, data.cols * data.rows, fp);	// 写RGB数据
		fclose(fp);
		free(output);
	}
	/*--------------------------------[ 转灰度图 ]--------------------------------*/
	Mat<double>& Gray(Mat<double>* input, Mat<double>& output, double Rk = 0.3, double Gk = 0.59, double Bk = 0.11) {
		output.zero(input[0].rows, input[0].cols);
		Mat<double> tmp;
		output.add(output, tmp.mult(Rk / (Rk + Gk + Bk), input[0]));
		output.add(output, tmp.mult(Gk / (Rk + Gk + Bk), input[1]));
		output.add(output, tmp.mult(Bk / (Rk + Gk + Bk), input[2]));
		return output;
	}
	/*--------------------------------[ 边缘检测 ]--------------------------------*/
	Mat<double>& EdgeDetection(Mat<double>& input, Mat<double>& output) {
		output.zero(input.rows, input.cols);
		Mat<double> SobelKernel(3, 3);
		{
			double t[] = {
				-1,0,1,
				-2,0,2,
				-1,0,1
			}; SobelKernel.getData(t);
		}
		Mat<double> output_x, output_y;
		output_x.conv(input, SobelKernel, 1);
		output_y.conv(input, SobelKernel.transposi(output_y), 1);
		for (int i = 0; i < input.rows * input.cols; i++)
				output[i] = sqrt(output_x[i] * output_x[i] + output_y[i] * output_y[i]);
		return output;
	}
	/*--------------------------------[ 颜色聚类 ]--------------------------------*/
	Mat<double>* ColorCluster(Mat<double>* input, int K = 3, int TimesMax = 100) {
		// Process input & output
		Mat<double> data(3, input[0].rows * input[0].cols), output[3];
		for (int k = 0; k < 3; k++)
			for (int i = 0; i < input[0].rows; i++)
				for (int j = 0; j < input[0].cols; j++)
					data(k, i * input[0].cols + j) = (input[k])(i, j);
		for (int k = 0; k < 3; k++) output[k].zero(input[0].rows, input[0].cols);
		// Color Cluster
		time_t now; srand((unsigned)time(&now));
		Mat<double> Center;
		Mat<int> Cluster, Cluster_Cur;
		K_Mean(data, K, Center, Cluster, Cluster_Cur, TimesMax);
		for (int i = 0; i < K; i++)
			for (int j = 0; j < Cluster_Cur[i]; j++)
				for (int dim = 0; dim < 3; dim++)
					(output[dim])(Cluster(i, j) / output[0].cols, Cluster(i, j) % output[0].cols) = Center(dim, i);
		return output;
	}
}
#endif