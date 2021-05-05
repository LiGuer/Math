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
#ifndef NEURAL_NETWORKS_H
#define NEURAL_NETWORKS_H
#include "Mat.h"
#include "Tensor.h"
/*################################################################################################
核心类:
class NeuralLayer(int inputSize, int outputSize)	//神经网络层
class ConvLayer(int _inChannelNum, int _outChannelNum,int kernelSize,int _padding,int _stride)	//卷积层
class PoolLayer(int _kernelSize, int _padding, int _stride, int _poolType)						//下采样层
--------------------------------------------------------------------------------------------------
经典结构:
class BackPropagation_NeuralNetworks()				//反向传播神经网络 : 1986.Rumelhart,McClelland
class LeNet_NeuralNetworks()						//LeNet卷积神经网络 : 1998.Yann LeCun
class Inception()									//Inception模块 : 2014.Google
class GoogLeNet_NeuralNetworks()					//GoogLeNet卷积神经网络 : 2014.Google
################################################################################################*/
/*----------------[ ReLU ]----------------*/
double relu		(double x) { return x < 0 ? x : 0; }
/*----------------[ sigmoid ]----------------*/
double sigmoid	(double x) { return 1 / (1 + exp(-x)); }
/*************************************************************************************************
*							NeuralLayer	神经网络层
*	[核心数据]: 
		[1] weight, bias	[2] actFunc 激活函数
		[3] output 输出(运行过程存储, 反向传递使用)
		[4] delta 更新值
*	[注]:
		[ReLU]: ReLU(x) = x > 0 ? x : 0
		[Sigmoid]: Sigmoid(x) = 1 / (1 + e^-x)
----------------------------------------------------------------------------------
*	正向传播
		y = σ(W×x + b)
		xi: 输入  y: 输出    Σwi·xi + b: 线性拟合
		σ(): 激活函数, 使线性拟合非线性化, eg. relu(x), Sigmoid(x)
----------------------------------------------------------------------------------
*	误差: E_total = Σ(target_i - output_i)²
----------------------------------------------------------------------------------
*	反向传播
		[结论]:
			[1] δL = ((w_L+1)^T·δ_L+1)·σ'(zL)
				δL_outL = ▽aE·σ'(z_outL)
						▽aE  = -2(target - y_outL)
			[2] ∂E/∂wL = δL×y_L-1^T
				∂E/∂bL = δL
				wL = wL + lr·∂E/∂wL
				bL = bL + lr·∂E/∂bL
		[设]: linear Output: zi = Σwi·xi + b;  yi = σ(zi)
			E: is E_total	outL: output layer    ⊙: 逐元素乘法
			L: Lth layer	wL_jk: the kth weight of jth Neuron in Lth layer
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
*************************************************************************************************/
class NeuralLayer {
public:
	Mat<> weight, bias;
	Mat<> output, delta;
	double(*actFunc)     (double);
	double(*actDerivFunc)(double);
	/*----------------[ init ]----------------*/
	NeuralLayer() { ; }
	NeuralLayer(int inputSize, int outputSize, double(*_actFunc)(double)) { init(inputSize, outputSize, _actFunc); }
	NeuralLayer(int inputSize, int outputSize) { init(inputSize, outputSize, [](double x) { return x; }); }
	void init  (int inputSize, int outputSize, double(*_actFunc)(double)) {
		weight.rands(outputSize, inputSize, -1, 1);
		bias.  rands(outputSize, 1, -1, 1);
		actFunc = _actFunc;
	}
	/*----------------[ forward ]----------------
	*	y = σ(W x + b)
	*-------------------------------------------*/
	Mat<>* operator()(Mat<>& input) { return forward(input); }
	Mat<>* forward   (Mat<>& input) {
		output.add(output.mult(weight, input), bias);
		return &output.function(output, actFunc);
	}
	/*----------------[ backward ]----------------
	[1] δL = ((w_L+1)^T×δ_L+1)·σ'(zL)
		δL_outlayer = ▽aE·σ'(z_outL)
				▽aE  = -2(target - y_outL)
	[2] ∂E/∂wL = δL×y_L-1^T
		∂E/∂bL = δL
		wL = wL + lr·∂E/∂wL
		bL = bL + lr·∂E/∂bL
	*-------------------------------------------*/
	void backward(Mat<>& preInput, Mat<>& error, double learnRate) {
		//[1]
		delta = error;	
		//delta[i].elementMult(tmp.function(outputLiner[i], actDerivFunc) :	//σ'(z_outL) 激活函数导数
		Mat<> t;
		error.mult(weight.transpose(t), delta);
		//[2]
		weight += t.mult(-learnRate, t.mult(delta, preInput.transpose(t)));
		bias   += t.mult(-learnRate, delta);
	}
	/*----------------[ save / load ]----------------*/
	void save(FILE* file) {
		for (int i = 0; i < weight.size(); i++) fprintf(file, "%f ", weight[i]);
		for (int i = 0; i < bias.rows;     i++) fprintf(file, "%f ", bias  [i]);
		fprintf(file, "\n");
	}
	void load(FILE* file) {
		for (int i = 0; i < weight.size(); i++) fscanf(file, "%f", &weight[i]);
		for (int i = 0; i < bias.rows;     i++) fscanf(file, "%f", &bias  [i]);
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
	Tensor<double> kernel, output;
	Mat<> bias;
	int inChannelNum, outChannelNum, padding, stride;
	bool biasSwitch = false;
	/*----------------[ init ]----------------*/
	ConvLayer() { ; }
	ConvLayer(int _inChannelNum, int _outChannelNum, int kernelSize, int _padding, int _stride) {
		init(_inChannelNum, _outChannelNum, kernelSize, _padding, _stride);
	}
	void init(int _inChannelNum, int _outChannelNum, int kernelSize, int _padding, int _stride) {
		inChannelNum  =  _inChannelNum, 
		outChannelNum = _outChannelNum, 
		padding = _padding, 
		stride  = _stride;
		kernel.rands(kernelSize, kernelSize, inChannelNum * outChannelNum, -1, 1);
		bias.zero(outChannelNum);
	}
	/*----------------[ forward ]----------------*/
	Tensor<double>* operator()(Tensor<double>& input) { return forward(input); }
	Tensor<double>* forward   (Tensor<double>& input) {
		int rows_out = (input.dim[0] - kernel.dim[0] + 2 * padding) / stride + 1,
			cols_out = (input.dim[1] - kernel.dim[1] + 2 * padding) / stride + 1;
		output.zero(rows_out, cols_out, outChannelNum);
		// for each element of output
		for (int z = 0; z < output.dim[2]; z++) {
			for (int y = 0; y < output.dim[1]; y++) {
				for (int x = 0; x < output.dim[0]; x++) {
					// for each element of kernel
					for (int kz = z * inChannelNum; kz < (z + 1) * inChannelNum; kz++) {
						for (int ky = 0; ky < kernel.dim[1]; ky++) {
							for (int kx = 0; kx < kernel.dim[0]; kx++) {
								double t;
								// get the corresponding element of input
								int xt = -padding + x * stride + kx, 
									yt = -padding + y * stride + ky;
								if (xt < 0 || xt >= input.dim[0] || yt < 0 || yt >= input.dim[1]) t = 0;
								else t = input(xt, yt, kz % inChannelNum) * kernel(kx, ky, kz);
								output(x, y, z) += t;
							}
						}
					}
					if (biasSwitch) output(x, y, z) += bias[z];
					output(x, y, z) = output(x, y, z) > 0 ? output(x, y, z) : 0;	// 激活函数
				}
			}
		}
		return &output;
	}
	/*----------------[ backward ]----------------*/
	/*----------------[ save / load ]----------------*/
	void save(FILE* file) {
		for (int i = 0; i < kernel.dim.product();      i++) fprintf(file, "%f ", kernel[i]);
		if (biasSwitch) for (int i = 0; i < bias.rows; i++) fprintf(file, "%f ", bias  [i]);
		fprintf(file, "\n");
	}
	void load(FILE* file) {
		for (int i = 0; i < kernel.dim.product();      i++) fscanf(file, "%f", &kernel[i]);
		if (biasSwitch) for (int i = 0; i < bias.rows; i++) fscanf(file, "%f", &bias  [i]);
	}
};
/*************************************************************************************************
*							Pool Layer	采样层
*	[分类]:	[1] AvePool 平均采样层    [2] MaxPool 最大采样层
*************************************************************************************************/
class PoolLayer {
public:
	Tensor<double> output;
	int kernelSize, padding, stride, poolType = 0;
	enum { A, M };
	/*----------------[ init ]----------------*/
	PoolLayer() { ; }
	PoolLayer(int _kernelSize, int _padding, int _stride, int _poolType) {
		init(_kernelSize, _padding, _stride, _poolType);
	}
	void init(int _kernelSize, int _padding, int _stride, int _poolType) {
		kernelSize = _kernelSize, padding = _padding, stride = _stride, poolType = _poolType;
	}
	/*----------------[ forward ]----------------*/
	Tensor<double>* operator()(Tensor<double>& input) { return forward(input); }
	Tensor<double>* forward   (Tensor<double>& input) {
		int rows_out = (input.dim[0] - kernelSize + 2 * padding) / stride + 1,
			cols_out = (input.dim[1] - kernelSize + 2 * padding) / stride + 1;
		output.zero(rows_out, cols_out, input.dim[2]);
		// for each element of output
		for (int z = 0; z < output.dim[2]; z++) {
			for (int y = 0; y < output.dim[1]; y++) {
				for (int x = 0; x < output.dim[0]; x++) {
					// for each element of kernel
					for (int ky = 0; ky < kernelSize; ky++) {
						for (int kx = 0; kx < kernelSize; kx++) {
							double t;
							// get the corresponding element of input
							int xt = -padding + x * stride + kx, 
								yt = -padding + y * stride + ky;
							if (xt < 0 || xt >= input.dim[0] || yt < 0 || yt >= input.dim[1]) t = 0;
							else t = input(xt, yt, z);
							switch (poolType) {
							case A: output(x, y, z) += t; break;
							case M: output(x, y, z)  = t > output(x, y, z) ? t : output(x, y, z); break;
							}
						}
					}
				}
			}
		}
		if (poolType == A) output.mult(1.0 / (kernelSize * kernelSize), output);
		return &output;
	}
	/*----------------[ backward ]----------------*/
};
/*********************************************************************************
						LSTM Layer 长短期记忆层
*	结构: Gate[4]: G, F, I, O 四个并行神经网络层
----------------------------------------------------------------------------------
*	正向传播
		gt = tanh(Wg×[h_(t-1), xt] + bg)
		ft = Sigmoid(Wf×[h_(t-1), xt] + bf)
		it = Sigmoid(Wi×[h_(t-1), xt] + bi)
		ot = Sigmoid(Wo×[h_(t-1), xt] + bo)
		------
		st = ft·s_(t-1) + it·gt
		ht = ot·tanh(Ct)
		------
		W_gfio			(cellNum, cellNum + xDim)
		[h_(t-1), xt]	(cellNum + xDim)
		gfiosh_t		(cellNum)
----------------------------------------------------------------------------------
*	反向传播
		Δg = it	 ·Δs		·tanh'(gt)
		Δf = s_(t-1)·Δs		·Sigmoid'(ft)
		Δi = gt	 ·Δs		·Sigmoid'(it)
		Δo = st	 ·Δh_(t+1)·Sigmoid'(ot)
		------
		Δgfio_W += Δgfio×xc^T
		Δgfio_b += Δgfio
		------
		Δs = ot·Δh_(t+1) + Δs_(t+1)
		Δh = Δxc[Δx, Δh]
		Δxc = ΣW^T×Δ_gfio
*********************************************************************************/
class LstmLayer
{
public:
	Mat<> gate[4], weights[4], bias[4], diffWeights[4], diffBias[4];	// Gate[4]: G, F, I, O
	Mat<> s, h, xc;													// State,output
	std::vector<Mat<>> gateSet[4], prevSSet, prevHSet, xcSet;
	LstmLayer() { ; }
	LstmLayer(int inputSize, int outputSize) { init(inputSize, outputSize); }
	void init(int inputSize, int outputSize) {
		for (int i = 0; i < 4; i++) {
			gate[i].	zero (outputSize);
			weights[i].	rands(outputSize, outputSize + inputSize, -0.1, 0.1);
			bias[i].	rands(outputSize, 1, -0.1, 0.1);
			diffWeights[i].zero(weights[i]);
			diffBias[i].   zero(bias[i]);
		}
	}
	/*-------------------------------- 正向传播 --------------------------------*/
	std::vector<Mat<>>* operator()(std::vector<Mat<>>& input, Mat<>& s_prev, Mat<>& h_prev) { return forward(input, s_prev, h_prev); }
	std::vector<Mat<>>* forward   (std::vector<Mat<>>& input, Mat<>& s_prev, Mat<>& h_prev) {
		for (int i = 0; i < 4; i++)gateSet[i].clear();
		prevSSet.clear(); prevSSet.push_back(s_prev);
		prevHSet.clear(); prevHSet.push_back(h_prev);
		xcSet.   clear();
		for (int timeStep = 0; timeStep < input.size(); timeStep++) {
			forward(
				input   [timeStep], 
				prevSSet[timeStep], 
				prevHSet[timeStep]
			);
			for (int i = 0; i < 4; i++) 
				gateSet[i].push_back(gate[i]);
			prevSSet.push_back(s);
			prevHSet.push_back(h);
			xcSet.   push_back(xc);
		}
		return &prevHSet;
	}
	Mat<>* forward(Mat<>& input, Mat<>& prevS, Mat<>& prevH) {
		// G F I O
		xc.rowsStack(input, prevH);														//[h_(t-1), xt]
		for (int i = 0; i < 4; i++) { 													
			gate[i].add(gate[i].mult(weights[i], xc), bias[i]);
			i == 0 ?
				gate[i].function([](double x) { return tanh(x); }):						//gt = tanh(Wg×[h_(t-1), xt] + bg) 	
				gate[i].function([](double x) { return 1 / (1 + exp(-x)); });			//Ft, It, Ot = Sigmoid( W_ifo×[h_(t-1), xt] + b_ifo )
		}
		// S H
		Mat<> tmp;
		s.add(tmp.elementMult(gate[0], gate[2]), s.elementMult(prevS, gate[1]));		//st = gt·it + s_(t-1)·ft
		return &h.elementMult(gate[3], s);
		//h.elementMult(gate[3], tmp.function(s, [](double x) { return tanh(x); }));	//ht = ot·tanh(st)
	}
	/*-------------------------------- 反向传播 --------------------------------*/
	void backward(std::vector<Mat<>>& error, double learnRate) {
		Mat<> 
			tmp, 
			diffH(error[0].rows), 
			diffS(error[0].rows);
		for (int timeStep = error.size() - 1; timeStep >= 0; timeStep--) {
			for (int i = 0; i < 4; i++) 
				gate[i] = gateSet[i][timeStep];
			s  = prevSSet[timeStep + 1];
			xc = xcSet[timeStep];
			backward(
				prevSSet[timeStep],
				error   [timeStep], 
				diffH, 
				diffS
			);
		}
		//Update
		for (int i = 0; i < 4; i++) {
			weights[i] += (diffWeights[i] *= -learnRate);
			bias   [i] += (diffBias   [i] *= -learnRate);
			diffWeights[i].zero();
			diffBias   [i].zero();
		}
	}
	void backward(Mat<>& prevS, Mat<>& error, Mat<>& diffH, Mat<>& diffS) {
		diffH += error;
		//notice that top_diffS is carried along the constant error carousel
		Mat<> diffGate[4], tmp;
		diffS.add(tmp.elementMult(gate[3], diffH), diffS);							//Δs = ot·Δh_(t+1) + Δs_(t+1) 
		diffGate[0].elementMult(gate[2], diffS);									//Δg = it·Δs
		diffGate[1].elementMult(prevS,   diffS);									//Δf = s_(t-1)·Δs
		diffGate[2].elementMult(gate[0], diffS);									//Δi = gt·Δs
		diffGate[3].elementMult(s,       diffH);									//Δo = st·Δh_(t+1)
		//arctive Derivative function												//Δgfio = Δgfio·σ'(gfio)
		for (int i = 0; i < 4; i++)
			i == 0 ?
			diffGate[i].elementMult(tmp.function(gate[i], [](double x) { return x * x > 0 ? 0 : 1 - x * x; })) :
			diffGate[i].elementMult(tmp.function(gate[i], [](double x) { return x * (1 - x); }));
		//diffs w.r.t. inputs & compute bottom diff
		Mat<> diffXc(xc.rows);												//Δgfio_W += Δgfio×xc^T //Δgfio_b += Δgfio //Δxc = ΣW^T×Δ_gfio
		for (int i = 0; i < 4; i++) {
			diffWeights[i] += tmp.mult(diffGate[i], xc.transpose(tmp));
			diffBias   [i] += diffGate[i];
			diffXc += tmp.mult(weights[i].transpose(tmp), diffGate[i]);
		}
		diffS.elementMult(gate[1]);													//Δs = Δs·ft
		diffXc.block_(diffXc.rows - diffH.rows, diffXc.rows - 1, 0, 0, diffH);		//Δh
	}
	/*----------------[ save / load ]----------------*/
	void save(FILE* file) {
		for (int k = 0; k < 4; k++) {
			for (int i = 0; i < weights[k].size(); i++) fprintf(file, "%f ", weights[k][i]);
			for (int i = 0; i < bias[k].rows;      i++) fprintf(file, "%f ", bias   [k][i]);
		} fprintf(file, "\n");
	}
	void load(FILE* file) {
		for (int k = 0; k < 4; k++) {
			for (int i = 0; i < weights[k].size(); i++) fscanf(file, "%f", &weights[k][i]);
			for (int i = 0; i < bias[k].rows;      i++)	fscanf(file, "%f", &bias   [k][i]);
		}
	}
};
/*################################################################################################

*						Some Classical NeuralNetworks  经典神经网络结构

################################################################################################*/

/*************************************************************************************************
*							Back Propagation NeuralNetworks  反向传播神经网络
*	[Author]: 1986.Rumelhart,McClelland
*************************************************************************************************/
#include <vector>
class BackPropagation_NeuralNetworks {
public:
	std::vector<NeuralLayer*> layer;
	Mat<> preIntput;
	double learnRate = 0.01;
	/*----------------[ set Layer ]----------------*/
	void addLayer(int inputSize, int outputSize) { layer.push_back(new NeuralLayer(inputSize, outputSize)); }
	void setLayer(int index, int inputSize, int outputSize) {
		if (index >= layer.size()) exit(-1);
		delete layer[index];
		layer[index] = new NeuralLayer(inputSize, outputSize, sigmoid);
	}
	/*----------------[ forward ]----------------*/
	Mat<>& operator()(Mat<>& input, Mat<>& output) { return forward(input, output); }
	Mat<>& forward   (Mat<>& input, Mat<>& output) {
		Mat<>* y = (*layer[0])(input);
		for (int i = 1; i < layer.size(); i++) y = (*layer[i])(*y);
		preIntput = input;
		return output = *y;
	}
	/*----------------[ backward ]----------------*/
	void backward(Mat<>& target) {
		Mat<> error;
		error.sub(target, layer.back()->output);
		for (int i = layer.size() - 1; i >= 1; i--)
			layer[i]->backward(
				i == 0 ? preIntput : layer[i - 1]->output, 
				error, 
				learnRate
			);
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
/*************************************************************************************************
*							LeNet NeuralNetworks  LeNet卷积神经网络
*	[Author]: 1998.Yann LeCun
*************************************************************************************************/
class LeNet_NeuralNetworks {
public:
	ConvLayer 
		Conv_1{  1, 16, 5, 2, 1 }, 
		Conv_2{ 16, 32, 5, 2, 1 };
	PoolLayer 
		MaxPool_1{ 2, 0, 2, MaxPool_1.M }, 
		MaxPool_2{ 2, 0, 2, MaxPool_2.M };
	NeuralLayer 
		FullConnect_1{ 32 * 7 * 7, 128 }, 
		FullConnect_2{ 128, 64 }, 
		FullConnect_3{  64, 10 };
	LeNet_NeuralNetworks() { ; }
	/*----------------[ forward ]----------------*/
	Mat<>& operator()(Tensor<double>& input, Mat<>& output) { return forward(input, output); }
	Mat<>& forward(Tensor<double>& input, Mat<>& output) {
		Tensor<double>* y;
		y = Conv_1(input);
		y = MaxPool_1(*y);
		y = Conv_2(*y);
		y = MaxPool_2(*y);
		Tensor<double> t = *y;
		Mat<> t2(t.dim.product()); t2.data = t.data; t.data = NULL;
		Mat<>* maty = &t2;
		maty = FullConnect_1(*maty);
		maty = FullConnect_2(*maty);
		maty = FullConnect_3(*maty);
		return output = *maty;
	}
	/*----------------[ backward ]----------------*/
	/*----------------[ save/load ]----------------*/
	void save(const char* saveFile) {
		FILE* file = fopen(saveFile, "w+");
		Conv_1.		  save(file);
		Conv_2.		  save(file);
		FullConnect_1.save(file);
		FullConnect_2.save(file);
		FullConnect_3.save(file);
		fclose(file);
	}
	void load(const char* loadFile) {
		FILE* file = fopen(loadFile, "r+");
		Conv_1.		  load(file);
		Conv_2.		  load(file);
		FullConnect_1.load(file);
		FullConnect_2.load(file);
		FullConnect_3.load(file);
		fclose(file);
	}
};
/*************************************************************************************************
*							Inception  模块
*	[Author]: 2014.Google
*************************************************************************************************/
class Inception {
public:
	ConvLayer b1, b2_1x1_a, b2_3x3_b, b3_1x1_a, b3_3x3_b, b3_3x3_c, b4_1x1;
	PoolLayer b4_pool;
	Tensor<double> output;
	/*----------------[ init ]----------------*/
	Inception() { ; }
	Inception(int inChannelNum, int n1x1, int n3x3red, int n3x3, int n5x5red, int n5x5, int poolChannelNum) {
		init(inChannelNum, n1x1, n3x3red, n3x3, n5x5red, n5x5, poolChannelNum);
	}
	void init(int inChannelNum, int n1x1, int n3x3red, int n3x3, int n5x5red, int n5x5, int poolChannelNum) {
		// 1x1 conv branch
		b1.		 init(inChannelNum, n1x1,			1, 0, 1);	// 1x1 conv -> 3x3 conv branch
		b2_1x1_a.init(inChannelNum, n3x3red,		1, 0, 1);
		b2_3x3_b.init(n3x3red,		n3x3,			3, 1, 1);
		b3_1x1_a.init(inChannelNum, n5x5red,		1, 0, 1);	// 1x1 conv -> 3x3 conv -> 3x3 conv branch
		b3_3x3_b.init(n5x5red,		n5x5,			3, 1, 1);
		b3_3x3_c.init(n5x5,			n5x5,			3, 1, 1);
		b4_pool. init(3, 0, 3, b4_pool.M);						// x3 pool -> 1x1 conv branch
		b4_1x1.  init(inChannelNum, poolChannelNum, 1, 0, 1);
	};
	/*----------------[ forward ]----------------*/
	Tensor<double>* operator()(Tensor<double>& input) { return forward(input); }
	Tensor<double>* forward(Tensor<double>& input) {
		Tensor<double>* y1 = b1								(input);
		Tensor<double>* y2 = b2_3x3_b(*b2_1x1_a				(input));
		Tensor<double>* y3 = b3_3x3_c(*b3_3x3_b(*b3_1x1_a	(input)));
		Tensor<double>* y4 = b4_1x1  (*b4_pool				(input));
		Tensor<double>* t[]{ y1, y2, y3, y4 };
		return &output.merge(t, 4, 1);
	}
	/*----------------[ save/load ]----------------*/
	void save(FILE* file) {
		b1.		 save(file);
		b2_1x1_a.save(file); b2_3x3_b.save(file);
		b3_1x1_a.save(file); b3_3x3_b.save(file); b3_3x3_c.save(file);
		b4_1x1.  save(file);
	}
	void load(FILE* file) {
		b1.		 load(file);
		b2_1x1_a.load(file); b2_3x3_b.load(file);
		b3_1x1_a.load(file); b3_3x3_b.load(file); b3_3x3_c.load(file);
		b4_1x1.  load(file);
	}
};
/*************************************************************************************************
*							GoogLeNet  卷积神经网络
*	[Author]:  2014.Google
*************************************************************************************************/
class GoogLeNet_NeuralNetworks {
public:
	Inception a3, b3, a4, b4, c4, d4, e4, a5, b5;
	ConvLayer preLayers;
	PoolLayer maxpool, avgpool;
	NeuralLayer linear;
	GoogLeNet_NeuralNetworks() {
		preLayers.init(1, 64, 5, 1, 2);
		a3.init( 64,  64,  96, 128, 16,  32,  32);
		b3.init(256, 128, 128, 192, 32,  96,  64);
		maxpool.init(3, 2, 1, avgpool.M);
		a4.init(480, 192,  96, 208, 16,  48,  64);
		b4.init(512, 160, 112, 224, 24,  64,  64);
		c4.init(512, 128, 128, 256, 24,  64,  64);
		d4.init(512, 112, 144, 288, 32,  64,  64);
		e4.init(528, 256, 160, 320, 32, 128, 128);
		a5.init(832, 256, 160, 320, 32, 128, 128);
		b5.init(832, 384, 192, 384, 48, 128, 128);
		avgpool.init(8, 8, 0, avgpool.A);
		linear. init(1024 * 5 * 5, 7, relu);
	}
	/*----------------[ forward ]----------------*/
	Mat<>& operator()(Tensor<double>& input, Mat<>& output) { return forward(input, output); }
	Mat<>& forward   (Tensor<double>& input, Mat<>& output) {
		Tensor<double>* y;
		y = preLayers(input);
		y = b3(*a3 (*y));
		y = maxpool(*y);
		y = e4(*d4(*c4(*b4(*a4(*y)))));
		y = maxpool(*y);
		y = b5(*a5 (*y));
		y = avgpool(*y);
		Tensor<double> t = *y;
		Mat<> t2(t.dim.product()); t2.data = t.data; t.data = NULL;
		Mat<>* maty = &t2;
		maty = linear(*maty);
		return output = *maty;
	}
	/*----------------[ save/load ]----------------*/
	void save(const char* saveFile) {
		FILE* file = fopen(saveFile, "w+");
		preLayers.save(file);
		a3.save(file); b3.save(file);
		a4.save(file); b4.save(file); c4.save(file); d4.save(file); e4.save(file);
		a5.save(file); b5.save(file);
		linear.save(file);
		fclose(file);
	}
	void load(const char* loadFile) {
		FILE* file = fopen(loadFile, "r+");
		preLayers.load(file);
		a3.load(file); b3.load(file);
		a4.load(file); b4.load(file); c4.load(file); d4.load(file); e4.load(file);
		a5.load(file); b5.load(file);
		linear.load(file);
		fclose(file);
	}
};
/*********************************************************************************
						LSTM 长短期记忆网络
*	正向传播:
		xList, nodeList: 保持运行过程数据, 在反向传播中使用
*********************************************************************************/
class LstmNetwork
{
public:
	LstmLayer lstm;
	NeuralLayer nn;
	Mat<> prevLstmS, prevLstmH;
	std::vector<Mat<>> output;
	double learnRate = 0.005;
	LstmNetwork(int inputSize, int LstmOutputSize, int outputSize) {
		lstm.init(inputSize, LstmOutputSize);
		nn.  init(LstmOutputSize, outputSize, [](double x) { return x; });
		prevLstmS.zero(LstmOutputSize);
		prevLstmH.zero(LstmOutputSize);
	}
	/*-------------------------------- 正向传播 --------------------------------*/
	std::vector<Mat<>>& operator()(std::vector<Mat<>>& input) { return forward(input); }
	std::vector<Mat<>>& forward   (std::vector<Mat<>>& input) {
		*lstm(input, prevLstmS, prevLstmH);
		prevLstmS = lstm.s;
		prevLstmH = lstm.h;
		output.clear();
		Mat<> tmp;
		for (int i = 0; i < input.size(); i++)
			output.push_back(*nn(lstm.prevHSet[i + 1]));
		return output;
	}
	/*-------------------------------- 反向传播 --------------------------------*/
	double backward(std::vector<Mat<>>& target) {
		std::vector<Mat<>> errorSet;
		Mat<> error;
		for (int i = 0; i < target.size(); i++) {
			nn.backward(
				lstm.prevHSet[i + 1],
				error.mult(-2, error.sub(target[i], output[i])),
				learnRate
			);
			errorSet.push_back(error);
		}
		lstm.backward(errorSet, learnRate);
	}
	/*----------------[ save/load ]----------------*/
	void save(const char* saveFile) {
		FILE* file = fopen(saveFile, "w+");
		lstm.save(file);
		nn.  save(file);
		fclose(file);
	}
	void load(const char* loadFile) {
		FILE* file = fopen(loadFile, "r+");
		lstm.load(file);
		nn.  load(file);
		fclose(file);
	}
};
#endif