#ifndef NEURAL_NETWORKS_INCLUDE_H
#define NEURAL_NETWORKS_INCLUDE_H

#include "../../../LiGu_Math/src/Math/Matrix/Matrix.h"
#include "../../../LiGu_Math/src/Math/Tensor.h"

using namespace Matrix;

namespace NeuralNetworks {
	/****************************************************************
	*							»ù´¡º¯Êý
	****************************************************************/
	/*----------------[ ReLU ]----------------*/
	double relu (double x) { return x > 0 ? x : 0; }
	double reluD(double x) { return x > 0 ? 1.0 : 0; }

	/*----------------[ sigmoid ]----------------*/
	double sigmoid (double x) { return 1 / (1 + exp(-x)); }
	double sigmoidD(double x) { return sigmoid(x) * (1 - sigmoid(x)); }

	/****************************************************************
	*							ËðÊ§º¯Êý
	****************************************************************/
	double QuadraticLossFunc(Mat<>& y, Mat<>& target) {
		Mat<> t;
		return t.sub(y, target).dot(t);
	}

	Mat<>& QuadraticLossFuncD(Mat<>& y, Mat<>& target, Mat<>& error) {
		return error.mul(-2, error.sub(target, y));
	}
}

#endif