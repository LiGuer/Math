#ifndef NEURAL_NETWORKS_BATCHNORM_H
#define NEURAL_NETWORKS_BATCHNORM_H

#include "include.h"

namespace NeuralNetworks {

/*************************************************************************************************
*							BatchNorm
*************************************************************************************************/
class BatchNorm {
public:
	int outChannelNum;
	double eps;
	BatchNorm() { ; }
	BatchNorm(int _outChannelNum, double _eps) { init(_outChannelNum, _eps); }
	void init(int _outChannelNum, double _eps) {
		outChannelNum = _outChannelNum;
		eps = _eps;
	}

	/*----------------[ forward ]----------------*/
	Tensor<>* operator()(Tensor<>& in) { return forward(in); }
	Tensor<>* forward   (Tensor<>& in) {
	}

	/*----------------[ backward ]----------------*/
	void backward() { ; }

};

}

#endif