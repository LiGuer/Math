#ifndef NEURAL_NETWORKS_CONVLAYER_H
#define NEURAL_NETWORKS_CONVLAYER_H

#include "include.h"

namespace NeuralNetworks {

/*************************************************************************************************
*							Convolution Layer	卷积层
*	[输入]: kernel: 卷积核		padding: 加边框宽度		in/outChannelNum: 输入/输出通道数
*	[参数]:
		Height_out = (Height_in - Height_kernel + 2 * padding) / (stride + 1)
		 Width_out = ( Width_in -  Width_kernel + 2 * padding) / (stride + 1)
-------------------------------------------------------------------------------------------------
*	[正向传播]: 卷积操作
		x * k = Σ_(c=1)^(xChannelNum) Σ_(p=1)^H Σ_(q=1)^W  x_{c,i+p-1,j+q-1} × k_cpq
		y_c = x * k[c] + b[c]
		y   = {y_1, y_2, ... , y_n}
-------------------------------------------------------------------------------------------------
*	[反向传播]: 卷积操作
		∂E / ∂y_l = ∂E / ∂y_L·(∂y_L . ∂y_(L-1))·...·(∂y_(l+1) / ∂y_l) 
				  = δ_l = δ_(l+1)·(∂y_(l+1) / ∂x_(l+1)) 
		          = δ_(l-1) * Rot180(w_l)
		∂E / ∂k_l = ∂E / ∂y_l·∂y_l / ∂k_l = δ_l·∂y_l / ∂k_l
		          = δ_l * x_l
		∂E / ∂b_l = δ_l·∂y_l / ∂b_l = Σ_x Σ_y δ_l
		其中:
		∂y_lcij / ∂k_lcpq = x_{l,c,i+p-1,j+q-1}
		∂y_lc   / ∂k_lcpq = Σ_(i=1)^Hy Σ_(j=1)^Wy x_{l,c,i+p-1,j+q-1}
		∂E      / ∂k_lcpq = Σ_(i=1)^Hy Σ_(j=1)^Wy δ_lcij × x_{l,c,i+p-1,j+q-1}  = δ_lc * x_lc
		∂E      / ∂b_lc   = Σ_(i=1)^Hy Σ_(j=1)^Wy δ_lcij = Σ_x Σ_y δ_l
*************************************************************************************************/
class ConvLayer {
public:
	Tensor<> kernel, out;
	Mat<> bias; bool isBias = true;
	int padding, stride;
	double(*actFunc)     (double);
	double(*actDerivFunc)(double);

	/*----------------[ init ]----------------*/
	ConvLayer() { ; }
	ConvLayer(int inChannelNum, int outChannelNum, int kernelSize, int _padding, int _stride, double(*_actFunc)(double), double(*_actDerivFunc)(double)) {
		init(inChannelNum, outChannelNum, kernelSize, _padding, _stride, _actFunc, _actDerivFunc);
	}

	void init(int inChannelNum, int outChannelNum, int kernelSize, int _padding, int _stride, double(*_actFunc)(double), double(*_actDerivFunc)(double)) {
		padding = _padding, 
		stride  = _stride; 
		kernel.alloc(kernelSize, kernelSize, inChannelNum, outChannelNum).rands(-1, 1);
		if (isBias)
			bias.rands(outChannelNum, 1, -1, 1);
		actFunc = _actFunc;
		actDerivFunc = _actDerivFunc;
	}

	/*----------------[ forward ]----------------*/
	Tensor<>* operator()(Tensor<>& in) { return forward(in); }
	Tensor<>* forward   (Tensor<>& in) {
		out.zero(
			(in.dim[0] - kernel.dim[0] + 2 * padding) / stride + 1,
			(in.dim[1] - kernel.dim[1] + 2 * padding) / stride + 1,
			kernel.dim[3]
		); 

		for (int i = 0; i < out.size(); i++) {
			for (int k = 0; k < kernel.size(3); k++) {
				int xi = -padding + out.i2x(i) * stride + kernel.i2x(k),
					yi = -padding + out.i2y(i) * stride + kernel.i2y(k),
					zi = kernel.i2z(k);

				if (xi < 0 || xi >= in.dim[0] 
				||  yi < 0 || yi >= in.dim[1]
				)
					continue;

				out(i) += in(xi, yi, zi) * kernel(k + zi * kernel.size(3));
			}
			if (isBias) 
				out[i] += bias[out.i2z(i)];
			out[i] = actFunc(out[i]);
		} 
		return &out;
	}

	/*----------------[ backward ]----------------*/
	void backward(Tensor<>& preIn, Tensor<>& error, double learnRate) {
		// σ'
		for (int i = 0; i < error.size(); i++) 
			error[i] *= actDerivFunc(error[i]);

		// δ_(l-1)
		static Tensor<> delta; 
		delta.eat(error);
		error.alloc(preIn.dim).zero();

		for (int i = 0; i < error.size(); i++)
			for (int zo = 0; zo < kernel.dim[3]; zo++)
				for (int k = kernel.size(2) - 1; k >= 0; k--) {
					int xi = -padding + error.i2x(i) * stride + kernel.i2x(kernel.size(2) - 1 - k),
						yi = -padding + error.i2y(i) * stride + kernel.i2y(kernel.size(2) - 1 - k),
						zi =            error.i2z(i);

					if (xi < 0 || xi >= delta.dim[0] 
					||  yi < 0 || yi >= delta.dim[1]
					) 
						continue;
					error(i) += delta(xi, yi, zo) * kernel(k + zi * kernel.size(2) + zo * kernel.size(3));
				} 
		// ∂E / ∂k_l
		static Tensor<> deltaKernel; 
		deltaKernel.alloc(kernel.dim).zero();

		for (int i = 0; i < deltaKernel.size(); i++)
			for (int k = 0; k < preIn.size(2); k++) {
				int xi = -padding + deltaKernel.i2x(i) * stride + preIn.i2x(k),
					yi = -padding + deltaKernel.i2y(i) * stride + preIn.i2y(k),
					zi = deltaKernel.i2z(i),
					zo = i / deltaKernel.size(3);

				if (xi < 0 || xi >= delta.dim[0] 
				||  yi < 0 || yi >= delta.dim[1]
				) 
					continue;
				deltaKernel(i) += delta(xi, yi, zo) * preIn(k + zi * preIn.size(2));
			}
		kernel += (deltaKernel *= learnRate);

		// ∂E / ∂b_l
		if (isBias) {
			static Mat<> deltaBias;  
			deltaBias.alloc(bias.size()).zero();

			for (int i = 0; i < deltaBias.size(); i++) 
				for (int j = 0; j < delta.size(2); j++) 
					deltaBias[i] += delta(j + i * delta.size(2));

			bias += (deltaBias *= learnRate); 
		}
	}

	/*----------------[ save / load ]----------------*/
	void save(FILE* file) {
		for (int i = 0; i < kernel.size(); i++)
			fprintf(file, "%f ", kernel[i]);

		if(isBias) 
			for (int i = 0; i < bias.size(); i++) 
				fprintf(file, "%f ", bias  [i]);

		fprintf(file, "\n");
	}
	void load(FILE* file) {
		for (int i = 0; i < kernel.size(); i++)
			fscanf(file, "%lf", &kernel[i]);

		if(isBias) 
			for (int i = 0; i < bias.size(); i++) 
				fscanf(file, "%lf", &bias  [i]);
	}

};

}

#endif