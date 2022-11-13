#ifndef NEURAL_NETWORKS_NEURALLAYER_H
#define NEURAL_NETWORKS_NEURALLAYER_H

#include "include.h"

namespace NeuralNetworks {

/*************************************************************************************************
*							NeuralLayer	神经网络层
*	[核心数据]: 
		[1] weight, bias	[2] actFunc, actDerivFunc 激活函数、导函数
		[3] out 输出
*	[注]:
		[ReLU]: ReLU(x) = x > 0 ? x : 0
		[Sigmoid]: Sigmoid(x) = 1 / (1 + e^-x)
----------------------------------------------------------------------------------
*	正向传播
		y = σ(W×x + b)
		x: 输入  y: 输出    z = Σwi·xi + b: 线性拟合
		σ(): 激活函数, 使线性拟合非线性化, eg. relu(x), Sigmoid(x)
----------------------------------------------------------------------------------
*	误差·损失函数: E_total = Σ(target_i - out_i)²
----------------------------------------------------------------------------------
*	反向传播
	[结论]:
		[1] δl = ((w_l+1)^T·δ_l+1)·σ'(zl)
			δL = ▽aE·σ'(z_outl)		▽aE  = -2(target - y_outl)
		[2] ∂E/∂w_l = δl×x_l^T		wl = wl + lr·∂E/∂wl
			∂E/∂b_l = δl				bl = bl + lr·∂E/∂bl
	[设]: 
		l: lth layer	L:out layer 
	[目的]: 
		w = w + lr·∂E/∂b, 
		b = b + lr·∂E/∂b, 
		求∂E/∂w, ∂E/∂b
	[推导]:
		链式法则: ∂E/∂w_l = ∂z_l/∂w_l·(∂y_l/∂z_l·∂z_l/∂y_(l+1))·(∂y_l/∂z_(l+1)·∂z_l/∂y_(l+2))·...·(∂y_l/∂z_(L-1)·∂z_l/∂y_L)·∂y_l/∂z_L·∂E/∂y_L
		∂y_l/∂z_l		= σ'(z_l)
		∂z_l/∂y_(l+1)	= W_(l+1)^T
		δl = (∂y_l/∂z_l·∂z_l/∂y_(l+1))·(∂y_l/∂z_(l+1)·∂z_l/∂y_(l+2))·...·(∂y_l/∂z_(L-1)·∂z_l/∂y_L)·∂y_l/∂z_L·∂E/∂y_L
		    = (σ'(z_l)·W_(l+1)^T)·...·(σ'(z_(L-1))·W_L^T)·σ'(z_L)·∂E/∂y_L
		∴∂E/∂w_l = δl·∂z_l/∂w_l = δl×x_l^T
		  ∂E/∂b_l = δl·∂z_l/∂b_l = δl
		其中，输出层
		δL = ∂E/∂y_L·σ'(z_L)		∂E/∂y_L = ▽yE = -2(y_target - y_L) : 误差函数
		即可求出 ∂E/∂w_l
*************************************************************************************************/
class NeuralLayer {
public:
	Mat<> weight, bias;
	Mat<> out, z;
	double(*actFunc)     (double);
	double(*actDerivFunc)(double);
	/*----------------[ init ]----------------*/
	NeuralLayer() { ; }

	NeuralLayer(int inSize, int outSize, double(*_actFunc)(double), double(*_actDerivFunc)(double)) { 
		init(inSize, outSize, _actFunc, _actDerivFunc);
	}

	NeuralLayer(int inSize, int outSize) { 
		init(inSize, outSize, [](double x) { return x; },[](double x) { return 1.0; }); 
	}

	void init  (int inSize, int outSize, double(*_actFunc)(double), double(*_actDerivFunc)(double)) {
		weight.rands(outSize, inSize, -1, 1);
		bias.  rands(outSize, 1,	  -1, 1);
		actFunc		= _actFunc;
		actDerivFunc= _actDerivFunc;
	}

	/*----------------[ forward ]----------------*/
	Mat<>* operator()(Mat<>& in) { return forward(in); }
	Mat<>* forward   (Mat<>& in) { 
		return &out.function(z.add(z.mul(weight, in), bias), actFunc); 
	}

	/*----------------[ backward ]----------------*/
	void backward(Mat<>& preIn, Mat<>& error, double learnRate) {
		static Mat<> t, delta;
		error.mul(
			weight.transpose(t), 
			delta.function(z, actDerivFunc).elementMul(error)
		);
		weight += t.mul(learnRate, t.mul(delta, preIn.transpose(t)));
		bias   += t.mul(learnRate, delta);
	}

	/*----------------[ save/load ]----------------*/
	void save(FILE* file) {
		for (int i = 0; i < weight.size(); i++) fprintf(file, "%f ", weight[i]);
		for (int i = 0; i < bias  .size(); i++) fprintf(file, "%f ", bias  [i]);
		fprintf(file, "\n");
	}

	void load(FILE* file) {
		for (int i = 0; i < weight.size(); i++) fscanf(file, "%lf", &weight[i]);
		for (int i = 0; i < bias  .size(); i++) fscanf(file, "%lf", &bias  [i]);
	}

};

}

#endif