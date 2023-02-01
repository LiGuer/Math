#ifndef NEURAL_NETWORKS_BASIC_MODULE_H
#define NEURAL_NETWORKS_BASIC_MODULE_H

#include "include.h"
#include "NeuralLayer.h"
#include "ConvLayer.h"
#include "PoolLayer.h"

namespace NeuralNetworks {

/*################################################################################################

*						经典神经网络模块

################################################################################################*/

/*************************************************************************************************
*							BasicConv 基础卷积层 模块
*	[组成]:	[1] ConvLayer	[2] BatchNorm	[3] ReLU 
*	[作用]: 每一层数据拉回均值0，方差1的标准正态分布
*************************************************************************************************/
class BasicConv {
public:
	ConvLayer conv;
	BatchNorm bn;
	Tensor<>  out;
	BasicConv() { ; }
	BasicConv(int _inChannelNum, int _outChannelNum, int kernelSize, int _padding, int _stride) {
		init(_inChannelNum, _outChannelNum, kernelSize, _padding, _stride);
	}

	void init(int _inChannelNum, int _outChannelNum, int kernelSize, int _padding, int _stride) {
		conv.init(_inChannelNum, _outChannelNum, kernelSize, _padding, _stride, relu, reluD);
		bn	.init(_outChannelNum, 0.001);
	}

	/*----------------[ forward ]----------------*/
	Tensor<>* operator()(Tensor<>& in) { return forward(in); }
	Tensor<>* forward   (Tensor<>& in) { 
		(out = *bn(*conv(in))).function(relu); 
	}

	/*----------------[ backward ]----------------*/
	void backward(Tensor<>& preIn, Tensor<>& error, double learnRate) {
		error.function(reluD);
		bn.  backward();
		conv.backward(preIn, error, learnRate);
	}

	/*----------------[ save/load ]----------------*/
	void save(FILE* file) { conv.save(file); }
	void load(FILE* file) { conv.load(file); }
};


/*************************************************************************************************
*							Inception  模块
*	[Author]: Google.Going deeper with convolutions.2014.
*************************************************************************************************/
class Inception {
public:
	BasicConv b1, b2_1x1_a, b2_3x3_b, b3_1x1_a, b3_3x3_b, b3_3x3_c, b4_1x1;
	PoolLayer b4_pool;
	Tensor<> out;
	/*----------------[ init ]----------------*/
	Inception() { ; }
	Inception(int inChannelNum, int n1x1, int n3x3red, int n3x3, int n5x5red, int n5x5, int poolChannelNum) {
		init(inChannelNum, n1x1, n3x3red, n3x3, n5x5red, n5x5, poolChannelNum);
	}
	void init(int inChannelNum, int n1x1, int n3x3red, int n3x3, int n5x5red, int n5x5, int poolChannelNum) {
		b1.		 init(inChannelNum, n1x1,			1, 0, 1);	// 1x1 conv branch
		b2_1x1_a.init(inChannelNum, n3x3red,		1, 0, 1);	// 1x1 conv -> 3x3 conv branch
		b2_3x3_b.init(n3x3red,		n3x3,			3, 1, 1);
		b3_1x1_a.init(inChannelNum, n5x5red,		1, 0, 1);	// 1x1 conv -> 3x3 conv -> 3x3 conv branch
		b3_3x3_b.init(n5x5red,		n5x5,			3, 1, 1);
		b3_3x3_c.init(n5x5,			n5x5,			3, 1, 1);
		b4_pool. init(3, 0, 3, b4_pool.M);						// x3 pool -> 1x1 conv branch
		b4_1x1.  init(inChannelNum, poolChannelNum, 1, 0, 1);
	};
	/*----------------[ forward ]----------------*/
	Tensor<>* operator()(Tensor<>& in) { return forward(in); }
	Tensor<>* forward(Tensor<>& in) {
		Tensor<>* t[]{ 
			b1							(in),
			b2_3x3_b(*b2_1x1_a			(in)),
			b3_3x3_c(*b3_3x3_b(*b3_1x1_a(in))),
			b4_1x1  (*b4_pool			(in)) 
		}; return &out.merge(t, 4, 1);
	}
	/*----------------[ backward ]----------------*/
	void backward(Tensor<>& preIn, Tensor<>& error, double learnRate) {

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
						ResBlock 残差模块
*	正向传播:
		xList, nodeList: 保持运行过程数据, 在反向传播中使用
*************************************************************************************************/
class ResBlock {
public:
	ConvLayer conv1, conv2;
	BatchNorm bn1 ,bn2;
	Tensor<>  out;
	ResBlock() { ; }
	ResBlock (int inChannelNum, int ChannelNum, int stride) { 
		init(inChannelNum, ChannelNum, stride);
	}

	void init(int inChannelNum, int ChannelNum, int stride) {
		conv1.init(inChannelNum, ChannelNum, 3, 1, stride, relu, reluD);
		conv2.init(  ChannelNum, ChannelNum, 3, 1, 1,      relu, reluD);
		bn1  .init(  ChannelNum, 0.001);
		bn2  .init(  ChannelNum, 0.001);
	}

	/*----------------[ forward ]----------------*/
	Tensor<>* operator()(Tensor<>& in) { return forward(in); }
	Tensor<>* forward   (Tensor<>& in) {
	   (out = *bn1(*conv1(in))).function(relu);
		out = *bn2(*conv2(in));
		return &((out += in).function(relu));
	}

	/*----------------[ backward ]----------------*/
	void backward(Tensor<>& preIn, Tensor<>& error, double learnRate) {

	}

	/*----------------[ save ]----------------*/
	void save(FILE* file) { conv1.save(file); conv2.save(file); }
	void load(FILE* file) { conv1.load(file); conv2.load(file); }
};

}

#endif