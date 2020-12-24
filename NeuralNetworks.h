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
#include "Tensor.h"
/*************************************************************************************************
*							NeuralLayer	神经网络层
*	[公式]:
		[正向]: y = σ( Σwi·xi + b )
				(矩阵式): y = σ(W x + b)
				xi: 输入  y: 输出    Σwi·xi + b: 线性拟合
				σ(): 激活函数, 使线性拟合非线性化, eg. relu(x), Sigmoid(x)
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
	int ActivFuncType = 1;
	/*----------------[ init ]----------------*/
	NeuralLayer(int inputSize, int outputSize) {
		weight.rands(outputSize, inputSize, -1, 1);
		bias.rands(outputSize, 1, -1, 1);
	}
	/*----------------[ forward ]----------------
	*	y = σ(W x + b)
	*-------------------------------------------*/
	Mat<float>* operator()(Mat<float>& input) { return forward(input); }
	Mat<float>* forward(Mat<float>& input) {
		linearOut.mult(weight, input);
		switch (ActivFuncType) {
		case 0:output = linearOut; break;
		case 1:sigmoid(linearOut, output); break;
		case 2:relu(linearOut, output); break;
		}return &output;
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
	/*----------------[ save / load ]----------------*/
	void save(FILE* file) {
		for (int i = 0; i < weight.rows * weight.cols; i++) fprintf(file, "%f ", weight[i]);
		for (int i = 0; i < bias.rows; i++) fprintf(file, "%f ", bias[i]);
		fprintf(file, "\n");
	}
	void load(FILE* file) {
		for (int i= 0; i < weight.rows * weight.cols; i++) fscanf(file, "%f", &weight[i]);
		for (int i = 0; i < bias.rows; i++) fscanf(file, "%f", &bias[i]);
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
*	[]: kernel: 卷积核		padding: 加边框宽度		in/outChannelNum: 输入/输出通道数
*	[]:
		Height_out = (Height_in - Height_kernel + 2 * padding) / (stride + 1)
		Width_out = (Width_in - Width_kernel + 2 * padding) / (stride + 1)
*	[正向]: 卷积操作
*************************************************************************************************/
class ConvLayer {
public:
	Tensor<float> kernel, output;
	int inChannelNum, outChannelNum, padding, stride;
	/*----------------[ init ]----------------*/
	ConvLayer(int _inChannelNum, int _outChannelNum,int kernelSize,int _padding,int _stride)
		: inChannelNum(_inChannelNum), outChannelNum(_outChannelNum), padding(_padding), stride(_stride)
	{
		kernel.rands(kernelSize, kernelSize, inChannelNum * outChannelNum, -1, 1);
	}
	/*----------------[ forward ]----------------*/
	Tensor<float>* operator()(Tensor<float>& input) { return forward(input); }
	Tensor<float>* forward(Tensor<float>& input) {
		int rows_out = (input.dim[0] - kernel.dim[0] + 2 * padding) / stride + 1;
		int cols_out = (input.dim[1] - kernel.dim[1] + 2 * padding) / stride + 1;
		output.zero(rows_out, cols_out, outChannelNum);
		// for each element of output
		for (int z = 0; z < output.dim[2]; z++) {
			for (int y = 0; y < output.dim[1]; y++) {
				for (int x = 0; x < output.dim[0]; x++) {
					// for each element of kernel
					for (int kz = z * inChannelNum; kz < (z + 1) * inChannelNum; kz++) {
						for (int ky = 0; ky < kernel.dim[1]; ky++) {
							for (int kx = 0; kx < kernel.dim[0]; kx++) {
								float t;
								// get the corresponding element of input
								int xt = -padding + x * stride + kx, yt = -padding + y * stride + ky;
								if (xt < 0 || xt >= input.dim[0] || yt < 0 || yt >= input.dim[1])t = 0;
								else t = input(xt, yt, kz % inChannelNum) * kernel(kx, ky, kz);
								output(x, y, z) += t;
							}
						}
					}
					output(x, y, z) = output(x, y, z) > 0 ? output(x, y, z) : 0;	// 激活函数
				}
			}
		}
		return &output;
	}
	/*----------------[ backward ]----------------*/
	/*----------------[ save / load ]----------------*/
	void save(FILE* file) {
		for (int i = 0; i < kernel.dim.product(); i++) fprintf(file, "%f ", kernel[i]);
		fprintf(file, "\n");
	}
	void load(FILE* file) {
		for (int i = 0; i < kernel.dim.product(); i++) fscanf(file, "%f", &kernel[i]);
	}
};
/*************************************************************************************************
*							Pool Layer	采样层
*	[分类]:	[1] AvePool 平均采样层    [2] MaxPool 最大采样层
*************************************************************************************************/
class PoolLayer {
public:
	Tensor<float> output;
	int kernelSize, padding, stride, poolType = 0;
	enum { A, M };
	/*----------------[ init ]----------------*/
	PoolLayer(int _kernelSize, int _padding, int _stride, int _poolType)
		:kernelSize(_kernelSize), padding(_padding), stride(_stride), poolType(_poolType)
	{}
	/*----------------[ forward ]----------------*/
	Tensor<float>* operator()(Tensor<float>& input) { return forward(input); }
	Tensor<float>* forward(Tensor<float>& input) {
		int rows_out = (input.dim[0] - kernelSize + 2 * padding) / stride + 1;
		int cols_out = (input.dim[1] - kernelSize + 2 * padding) / stride + 1;
		output.zero(rows_out, cols_out, input.dim[2]);
		// for each element of output
		for (int z = 0; z < output.dim[2]; z++) {
			for (int y = 0; y < output.dim[1]; y++) {
				for (int x = 0; x < output.dim[0]; x++) {
					// for each element of kernel
					for (int ky = 0; ky < kernelSize; ky++) {
						for (int kx = 0; kx < kernelSize; kx++) {
							float t;
							// get the corresponding element of input
							int xt = -padding + x * stride + kx, yt = -padding + y * stride + ky;
							if (xt < 0 || xt >= input.dim[0] || yt < 0 || yt >= input.dim[1])t = 0;
							else t = input(xt, yt, z);
							switch (poolType) {
							case A: output(x, y, z) += t; break;
							case M: output(x, y, z) = t > output(x, y, z) ? t : output(x, y, z); break;
							}
						}
					}
				}
			}
		}
		if (poolType == A)output.mult(1.0 / (kernelSize * kernelSize), output);
		return &output;
	}
	/*----------------[ backward ]----------------*/
};
/*************************************************************************************************
*							Some Classical NeuralNetworks  经典神经网络
*	[1] BackPropagation NeuralNetworks
*	[2] LeNet NeuralNetworks
*************************************************************************************************/
#include <vector>
class BackPropagation_NeuralNetworks {
public:
	std::vector<NeuralLayer*> layer;
	Mat<float> preIntput;
	float learnRate = 0.01;
	/*----------------[ set Layer ]----------------*/
	void addLayer(int inputSize, int outputSize) { layer.push_back(new NeuralLayer(inputSize, outputSize)); }
	void setLayer(int index, int inputSize, int outputSize) {
		if (index >= layer.size())exit(1);
		delete layer[index];
		layer[index] = new NeuralLayer(inputSize, outputSize);
	}
	/*----------------[ forward ]----------------*/
	void forward(Mat<float>& input, Mat<float>& output) {
		Mat<float>* y = layer[0]->forward(input);
		for (int i = 1; i < layer.size(); i++) y = layer[i]->forward(*y);
		output = *y; preIntput = input;
	}
	/*----------------[ backward ]----------------*/
	void backward(Mat<float>& target) {
		Mat<float> error;
		error.add(target, layer.back()->output.negative(error));
		for (int i = layer.size() - 1; i >= 1; i--)
			layer[i]->backward(layer[i - 1]->output, error, learnRate);
		layer[0]->backward(preIntput, error, learnRate);
	}
	/*----------------[ save/load ]----------------*/
	void save(const char* saveFile) {
		FILE* file = fopen(saveFile, "w+");
		for (int i = 0; i < layer.size(); i++) layer[i]->save(file);
		fclose(file);
	}
	void load(const char* loadFile) {
		FILE* file = fopen(loadFile, "r+");
		for (int i = 0; i < layer.size(); i++) layer[i]->load(file);
		fclose(file);
	}
};

class LeNet_NeuralNetworks {
public:
	ConvLayer Conv_1{ 1,16,5,2,1 }, Conv_2{ 16,32,5,2,1 };
	PoolLayer MaxPool_1{ 2,0,2,MaxPool_1.M }, MaxPool_2{ 2,0,2,MaxPool_2.M };
	NeuralLayer FullConnect_1{ 32 * 7 * 7,128 }, FullConnect_2{ 128,64 }, FullConnect_3{ 64,10 };
	/*----------------[ forward ]----------------*/
	LeNet_NeuralNetworks() {
		FullConnect_1.ActivFuncType = FullConnect_2.ActivFuncType = FullConnect_3.ActivFuncType = 0;
	}
	void forward(Tensor<float>& input, Mat<float>& output) {
		Tensor<float>* y;
		y = Conv_1(input);
		y = MaxPool_1(*y);
		y = Conv_2(*y);
		y = MaxPool_2(*y);
		Tensor<float> t = *y;
		Mat<float> t2(t.dim.product(), 1); t2.data = t.data; t.data = NULL;
		Mat<float>* maty = &t2;
		maty = FullConnect_1(*maty);
		maty = FullConnect_2(*maty);
		maty = FullConnect_3(*maty);
		output = *maty;
	}
	/*----------------[ backward ]----------------*/
	/*----------------[ save/load ]----------------*/
	void save(const char* saveFile) {
		FILE* file = fopen(saveFile, "w+");
		Conv_1.save(file);
		Conv_2.save(file);
		FullConnect_1.save(file);
		FullConnect_2.save(file);
		FullConnect_3.save(file);
		fclose(file);
	}
	void load(const char* loadFile) {
		FILE* file = fopen(loadFile, "r+");
		Conv_1.load(file);
		Conv_2.load(file);
		FullConnect_1.load(file);
		FullConnect_2.load(file);
		FullConnect_3.load(file);
		fclose(file);
	}
};
#endif