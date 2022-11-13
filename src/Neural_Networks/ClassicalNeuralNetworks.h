#ifndef NEURAL_NETWORKS_CLASSICAL_NEURAL_NETWORKS_H
#define NEURAL_NETWORKS_CLASSICAL_NEURAL_NETWORKS_H

#include "NeuralLayer.h"
#include "ConvLayer.h"
#include "PoolLayer.h"
#include "BatchNorm.h

#include "LSTMLayer.h"

#include "BasicModule.h"

namespace NeuralNetworks {

/*################################################################################################

*						Some Classical NeuralNetworks  经典神经网络结构

################################################################################################*/

/*************************************************************************************************
*							Back Propagation NeuralNetworks  反向传播神经网络
*	[Author]: 1986.Rumelhart,McClelland
*************************************************************************************************/
#include <vector>
class BackPropagationNeuralNetworks {
public:
	std::vector<NeuralLayer*> layer;
	Mat<> preIn;
	Mat<>&(*lossFunc)(Mat<>& y, Mat<>& target, Mat<>& error) = QuadraticLossFuncD;

	/*----------------[ set Layer ]----------------*/
	void addLayer(int inSize, int outSize) { 
		layer.push_back(new NeuralLayer(inSize, outSize, sigmoid, sigmoidD)); 
	}

	void setLayer(int index, int inSize, int outSize) {
		if (index >= layer.size()) exit(-1);
		delete layer[index];
		layer[index] = new NeuralLayer(inSize, outSize, sigmoid, sigmoidD);
	}

	/*----------------[ forward ]----------------*/
	Mat<>& operator()(Mat<>& in, Mat<>& out) { return forward(in, out); }
	Mat<>& forward   (Mat<>& in, Mat<>& out) {
		Mat<>* y = (*layer[0])(in);
		for (int i = 1; i < layer.size(); i++)
			y = (*layer[i])(*y);
		preIn = in;
		return out = *y;
	}

	/*----------------[ backward ]----------------*/
	void backward(Mat<>& target, double learnRate = -0.01) {
		Mat<> error; lossFunc(layer.back()->out, target, error);
		for (int i = layer.size() - 1; i >= 0; i--)
			layer[i]->backward(
				i == 0 ? preIn : layer[i - 1]->out, 
				error,
				learnRate
			);
	}

	/*----------------[ save/load ]----------------*/
	void save(const char* saveFile) {
		FILE* file = fopen(saveFile, "w");
		for (int i = 0; i < layer.size(); i++) layer[i]->save(file);
		fclose(file);
	}

	void load(const char* loadFile) {
		FILE* file = fopen(loadFile, "r");
		for (int i = 0; i < layer.size(); i++) layer[i]->load(file);
		fclose(file);
	}
};
/*************************************************************************************************
*							LeNet NeuralNetworks  LeNet卷积神经网络
*	[Author]: 1998.Yann LeCun
*************************************************************************************************/
class LeNet {
public:
	ConvLayer 
		Conv_1{  1, 16, 5, 2, 1, sigmoid, sigmoidD },
		Conv_2{ 16, 32, 5, 2, 1, sigmoid, sigmoidD };
	PoolLayer 
		MaxPool_1{ 2, 0, 2, MaxPool_1.M }, 
		MaxPool_2{ 2, 0, 2, MaxPool_2.M };
	NeuralLayer 
		FullConnect_1{ 32 * 7 * 7, 128 , sigmoid, sigmoidD }, 
		FullConnect_2{ 128, 64 ,         sigmoid, sigmoidD },
		FullConnect_3{  64, 10 ,         sigmoid, sigmoidD };
	Mat<>& (*lossFunc)(Mat<>& y, Mat<>& target, Mat<>& error) = QuadraticLossFuncD;
	Tensor<> preIn;
	LeNet() { ; }

	/*----------------[ forward ]----------------*/
	Mat<>& operator()(Tensor<>& in, Mat<>& out) { return forward(in, out); }
	Mat<>& forward   (Tensor<>& in, Mat<>& out) { 
		Tensor<>* y; preIn = in;
		y = MaxPool_1(*Conv_1(in)); 
		y = MaxPool_2(*Conv_2(*y));

		static Mat<> y2; 
		y2.rows = y->size(); 
		y2.cols = 1; 
		y2.data = y->data; 

		return out = *FullConnect_3(*FullConnect_2(*FullConnect_1(y2)));
	}

	/*----------------[ backward ]----------------*/
	void backward(Mat<>& target, double learnRate = -0.01) { 
		static Mat<> error, tmp; 
		tmp.get_(MaxPool_2.out.size(), 1, MaxPool_2.out.data);

		lossFunc(FullConnect_3.out, target, error);
		FullConnect_3.backward(FullConnect_2.out, error, learnRate);
		FullConnect_2.backward(FullConnect_1.out, error, learnRate);
		FullConnect_1.backward(tmp              , error, learnRate);

		static Tensor<> error2; 
		error2.dim.alloc(3).get(7, 7, 32); 

		if (error2.data != NULL) 
			delete error2.data; 
		error2.data = error.data; 
		error.data = NULL;

		Conv_2.backward(MaxPool_1.out, MaxPool_2.backward(Conv_2.out, error2), learnRate);
		Conv_1.backward(preIn,         MaxPool_1.backward(Conv_1.out, error2), learnRate);
	}

	/*----------------[ save/load ]----------------*/
	void save(const char* saveFile) {
		FILE* file = fopen(saveFile, "w");
		Conv_1.		  save(file);
		Conv_2.		  save(file);
		FullConnect_1.save(file);
		FullConnect_2.save(file);
		FullConnect_3.save(file);
		fclose(file);
	}

	void load(const char* loadFile) {
		FILE* file = fopen(loadFile, "r");
		Conv_1.		  load(file);
		Conv_2.		  load(file);
		FullConnect_1.load(file);
		FullConnect_2.load(file);
		FullConnect_3.load(file);
		fclose(file);
	}

};
/*************************************************************************************************
*							GoogLeNet  卷积神经网络
*	[Author]: Google.Going deeper with convolutions.2014.
*************************************************************************************************/
class GoogLeNet {
public:
	Inception a3, b3, a4, b4, c4, d4, e4, a5, b5;
	BasicConv preLayers;
	PoolLayer maxpool, avgpool;
	NeuralLayer linear;
	Tensor<> preIn;

	GoogLeNet() {
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
		linear. init(1024 * 5 * 5, 7, relu, reluD);
	}

	/*----------------[ forward ]----------------*/
	Mat<>& operator()(Tensor<>& in, Mat<>& out) { return forward(in, out); }
	Mat<>& forward   (Tensor<>& in, Mat<>& out) {
		Tensor<>* y = preLayers(in); 
		preIn = in;

		y = maxpool(*b3(*a3(*y)));
		y = maxpool(*e4(*d4(*c4(*b4(*a4(*y))))));
		y = avgpool(*b5(*a5(*y)));

		Mat<> t2(y->size()); 
		memcpy(t2.data, y->data, y->size());

		return out = *linear(t2);
	}

	/*----------------[ backward ]----------------*/
	void backward(Mat<>& target, double learnRate = -0.01) {
	}

	/*----------------[ save/load ]----------------*/
	void save(const char* saveFile) {
		FILE* file = fopen(saveFile, "w");
		preLayers.save(file);
		a3.save(file); b3.save(file);
		a4.save(file); b4.save(file); c4.save(file); d4.save(file); e4.save(file);
		a5.save(file); b5.save(file);
		linear.save(file);
		fclose(file);
	}

	void load(const char* loadFile) {
		FILE* file = fopen(loadFile, "r");
		preLayers.load(file);
		a3.load(file); b3.load(file);
		a4.load(file); b4.load(file); c4.load(file); d4.load(file); e4.load(file);
		a5.load(file); b5.load(file);
		linear.load(file);
		fclose(file);
	}

};
/*************************************************************************************************
						LSTM 长短期记忆网络
*	正向传播:
		xList, nodeList: 保持运行过程数据, 在反向传播中使用
*************************************************************************************************/
class LstmNetwork
{
public:
	LstmLayer lstm;
	NeuralLayer nn;
	Mat<> prevLstmS, prevLstmH;
	std::vector<Mat<>> out;
	double learnRate = -0.005;
	LstmNetwork(int inSize, int LstmOutputSize, int outSize) {
		lstm.init(inSize, LstmOutputSize);
		nn.  init(LstmOutputSize, outSize, [](double x) { return x; }, [](double x) { return 1.0; });
		prevLstmS.zero(LstmOutputSize);
		prevLstmH.zero(LstmOutputSize);
	}
	/*-------------------------------- 正向传播 --------------------------------*/
	std::vector<Mat<>>& operator()(std::vector<Mat<>>& in) { return forward(in); }
	std::vector<Mat<>>& forward   (std::vector<Mat<>>& in) {
		*lstm(in, prevLstmS, prevLstmH);
		out.clear();

		for (int i = 0; i < in.size(); i++)
			out.push_back(*nn(lstm.prevHSet[i + 1]));

		return out;
	}
	/*-------------------------------- 反向传播 --------------------------------*/
	double backward(std::vector<Mat<>>& target) {
		std::vector<Mat<>> errorSet;
		Mat<> error;
		for (int i = 0; i < target.size(); i++) {
			nn.backward(
				lstm.prevHSet[i + 1],
				error.mul(-2, error.sub(target[i], out[i])),
				learnRate
			);
			errorSet.push_back(error);
		}
		lstm.backward(errorSet, learnRate);
	}
	/*----------------[ save/load ]----------------*/
	void save(const char* saveFile) {
		FILE* file = fopen(saveFile, "w");
		lstm.save(file);
		nn.  save(file);
		fclose(file);
	}
	void load(const char* loadFile) {
		FILE* file = fopen(loadFile, "r");
		lstm.load(file);
		nn.  load(file);
		fclose(file);
	}
};

}

#endif