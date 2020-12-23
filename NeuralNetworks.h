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
#ifndef NEURAL_NETWORKS_H
#define NEURAL_NETWORKS_H
#include "Mat.h"
/*************************************************************************************************
*							NeuralLayer	神经网络层
*	[公式]:
		[正向]: y = σ( Σwi·xi + b )
				(矩阵式): y = σ(W x + b)
				xi: 输入  y: 输出    Σwi·xi + b: 线性拟合
				σ(): 激活函数, 使线性拟合非线性化, eg. relu(x)
		[误差]: E_total = Σ(target_i - output_i)²
		[反向]: 
			[设]: linear Output: zi = Σwi·xi + b;  yi = σ(zi)
				E: is E_total	outL: output layer    ⊙: 逐元素乘法
				L: Lth layer	wL_jk: the kth weight of jth Neuron in Lth layer
			[结论]: (矩阵式)
				[1] ∂E/∂wL = δL·y_L-1'
					∂E/∂bL = δL
				[2] δL = ((w_L+1)'δ_L+1) ⊙ σ'(zL)
					δoutL = ▽aE ⊙ σ'(z_outL)
			[目的]: wL_new = wL + lr·∂E/∂wL, 求∂E/∂wL						(and bL)
			[链式法则]: ∂E/∂wL_jk = ∂E/∂yL_j·∂yL_j/∂zL_j·∂zL_j/∂wL_jk	(and bL)
			[推导]:
				[* 输出层]: L = outL
				[1] ∂E/∂yL_j = ∂(Σ(target_i - yL_i)²)/∂yL = -2(target_j - yL_j)
				[2] ∂yL/∂zL_j = σ'(zL_j)
				[3] ∂zL/∂wL_jk = y_L-1_j	: the amount that a small nudge to this weight 
						influences the last layer depends on how strong the previous neuron is.
				[∴] ∂E/∂wL_jk = -2(target_j - yL_j)·σ'(zL_j)·y_L-1_k
								= δL_j·y_L-1_k = δL·y_L-1'
					δL_j = σ'(zL_j)·-2(target_j - yL_j) = ∂E/∂zL_j
					for bL: ∂zL_j/∂bL_j = 1		∴∂E/∂bL_j = δL_j
				[* 隐藏层]:
				[1] ∂E/∂yL_j = Σ∂E_oi/∂yL_j
					∂E_oi/∂yL_j = ∂E_oi/∂zL_j·∂zL_j/∂yL_j = δ_L+1_j·∂(Σwi·xi + b)/∂yL_j = δo1·wL_jk
					∂E/∂yL_j = Σδoi·wL_jk
				[2][3] ∂yL/∂zL_j, ∂zL/∂wL_jk 同上
				[∴] ∂E/∂wL_jk = -2(target_j - yL_j)·σ'(zL_j)·y_L-1_j
		[ReLU]: ReLU(x) = x > 0 ? x : 0
		[Sigmoid]: Sigmoid(x) = 1 / (1 + e^-x)
*************************************************************************************************/
class NeuralLayer {
public:
	Mat<float> weight, bias;
	Mat<float> output, linearOut, delta;
	/*----------------[ init ]----------------*/
	NeuralLayer(int inputSize, int outputSize) { init(inputSize, outputSize); }
	void init(int inputSize, int outputSize) {
		weight.rands(outputSize, inputSize, -1, 1);
		bias.rands(outputSize, 1, -1, 1);
	}
	/*----------------[ forward ]----------------
	*	y = σ(W x + b)
	*-------------------------------------------*/
	void forward(Mat<float>& input) {
		linearOut.mult(weight, input);
		sigmoid(linearOut, output);
	}
	/*----------------[ backward ]----------------
	[1] ∂E/∂wL = δL·y_L-1'
		∂E/∂bL = δL
	[2] δL = ((w_L+1)'δ_L+1) ⊙ σ'(zL)
		δoutL = ▽aE ⊙ σ'(z_outL)
			▽aE  = -2(target - y_outL)
	[*] wL = wL + lr·∂E/∂wL
	*-------------------------------------------*/
	void backward(Mat<float>& preInput, Mat<float>& error, double learnRate) {
		//[2]
		delta = error;
		for (int j = 0; j < output.rows; j++)
			delta[j] *= output[j] * (1 - output[j]);		//σ'(z_outL) 激活函数导数
		Mat<float> t;
		error.mult(weight.transposi(t), delta);
		//[1][*]
		t.mult(delta, preInput.transposi(t));
		weight.add(weight, t.mult(learnRate, t));
		bias.add(bias, t.mult(learnRate, delta));
	}
	/*----------------[ ReLU ]----------------*/
	void relu(Mat<float>& x, Mat<float>& y) {
		y = x;
		for (int i = 0; i < y.rows; i++)
			y[i] = y[i] < 0 ? y[i] : 0;
	}
	/*----------------[ sigmoid ]----------------*/
	void sigmoid(Mat<float>& x, Mat<float>& y) {
		y.zero(x.rows, 1);
		for (int i = 0; i < x.rows; i++)
			y[i] = 1 / (1 + exp(-x[i]));
	}
};
/*************************************************************************************************
*							Convolution Layer	卷积层
*	[]: kernel: 卷积核		padding: 加边框宽度		in/outChannels: 输入/输出通道数
*	[正向]: 卷积操作
*************************************************************************************************/
class ConvLayer {
	Mat<float> kernel;
	int padding;
	/*----------------[ init ]----------------*/
	ConvLayer(int inChannels, int outChannels,int kernelSize,int _padding) {
		kernel.rands(kernelSize, kernelSize, -1, 1);
		padding = _padding;
	}
	/*----------------[ forward ]----------------*/
	void forward() {

	}
	/*----------------[ backward ]----------------*/
};
/*************************************************************************************************
*							Pool Layer	采样层
*	[分类]:	[1] AvePool 平均采样层    [2] MaxPool 最大采样层
*************************************************************************************************/
class PoolLayer {
	/*----------------[ init ]----------------*/
	PoolLayer(int size) {

	}
};
/*************************************************************************************************
*							NeuralNetworks  神经网络
*************************************************************************************************/
#include <vector>
class NeuralNetworks {
public:
	std::vector<NeuralLayer*> layer;
	Mat<float> preIntput;
	float learnRate = 0.1;
	/*----------------[ set Layer ]----------------*/
	void addLayer(int inputSize, int outputSize) {
		layer.push_back(new NeuralLayer(inputSize, outputSize));
	}
	void setLayer(int index, int inputSize, int outputSize) {
		if (index >= layer.size())exit(1);
		free(layer[index]);
		layer[index] = new NeuralLayer(inputSize, outputSize);
	}
	/*----------------[ forward ]----------------*/
	void forward(Mat<float>& input, Mat<float>& output) {
		layer[0]->forward(input);
		for (int i = 1; i < layer.size(); i++)
			layer[i]->forward(layer[i - 1]->output);
		output = layer.back()->output;
		preIntput = input;
	}
	/*----------------[ backward ]----------------*/
	void backward(Mat<float>& target) {
		Mat<float> error;
		error.add(target, layer.back()->output.negative(error));
		for (int i = layer.size() - 1; i >= 1; i--) {
			layer[i]->backward(layer[i - 1]->output, error, learnRate);
		}
		layer[0]->backward(preIntput, error, learnRate);
	}
	/*----------------[ save ]----------------*/
	void save(const char* saveFile) {
		FILE* file = fopen(saveFile, "w+");
		for (int i = 0; i < layer.size(); i++) {
			for (int j = 0; j < layer[i]->weight.rows * layer[i]->weight.cols; j++)
				fprintf(file, "%f ", layer[i]->weight[j]);
			for (int j = 0; j < layer[i]->bias.rows; j++)
				fprintf(file, "%f\n", layer[i]->bias[j]);
		}fclose(file);
	}
	/*----------------[ load ]----------------*/
	void load(const char* loadFile) {
		FILE* file = fopen(loadFile, "r+");
		for (int i = 0; i < layer.size(); i++) {
			for (int j = 0; j < layer[i]->weight.rows * layer[i]->weight.cols; j++)
				fscanf(file, "%f", &layer[i]->weight[j]);
			for (int j = 0; j < layer[i]->bias.rows; j++)
				fscanf(file, "%f", &layer[i]->bias[j]);
		}fclose(file);
	}
};
class Layer {
public:
	union {
		NeuralLayer n;
		ConvLayer c;
		PoolLayer p;
	}u;
	enum { Neural, Conv, Pool };
};
#endif