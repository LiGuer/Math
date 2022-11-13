#ifndef NEURAL_NETWORKS_POOLLAYER_H
#define NEURAL_NETWORKS_POOLLAYER_H

#include "include.h"

namespace NeuralNetworks {

/*************************************************************************************************
*							Pool Layer	采样层
*	[分类]:	[1] AvePool 平均采样层    [2] MaxPool 最大采样层
-------------------------------------------------------------------------------------------------
*	[正向传播]:
		MaxPool: 最大值
		AvePool: 平均值
-------------------------------------------------------------------------------------------------
*	[反向传播]:
		MaxPool: 区域最大值对应的位置误差为δ，而其它位置误差为0
		AvePool: 区域每个位置的误差为采样后误差除以区域大小
*************************************************************************************************/
class PoolLayer {
public:
	Tensor<> out;
	Tensor<> MaxIndexMem;
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
	Tensor<>* operator()(Tensor<>& in) { return forward(in); }
	Tensor<>* forward   (Tensor<>& in) {
		out.zero(
			(in.dim[0] - kernelSize + 2 * padding) / stride + 1,
			(in.dim[1] - kernelSize + 2 * padding) / stride + 1,
			 in.dim[2]
		);

		if (poolType == M) 
			MaxIndexMem.alloc(out.dim);

		for (int i = 0; i < out.size(); i++)
			for (int k = 0; k < kernelSize * kernelSize; k++) {
				int xt = -padding + out.i2x(i) * stride + k % kernelSize,
					yt = -padding + out.i2y(i) * stride + k / kernelSize,
					zt = out.i2z(i);

				if (xt < 0 || xt >= in.dim[0] || yt < 0 || yt >= in.dim[1]) 
					continue;

				switch (poolType) {
				case A: 
					out(i) += in(xt, yt, zt); 
					break;
				case M: 
					out(i)  = in(xt, yt, zt) > out(i) ? MaxIndexMem(i) = in.xyz2i(xt, yt, zt), in(xt, yt, zt) : out(i); 
					break;
				}
			}

		if (poolType == A) 
			out *= 1.0 / (kernelSize * kernelSize);

		return &out;
	}

	/*----------------[ backward ]----------------*/
	Tensor<>& backward(Tensor<>& preIn, Tensor<>& error) {
		static Tensor<> delta; 
		delta.eat(error);
		error.alloc(preIn.dim).zero();

		if (poolType == A) {
			for (int i = 0; i < error.size(); i++)
				for (int k = 0; k < kernelSize * kernelSize; k++) {
					int xt = -padding + out.i2x(i) * stride + k % kernelSize,
						yt = -padding + out.i2y(i) * stride + k / kernelSize,
						zt = out.i2z(i);

					if (xt < 0 || xt >= delta.dim[0] 
					||  yt < 0 || yt >= delta.dim[1]
					)
						continue;

					error(i) += delta(xt, yt, zt) / (kernelSize * kernelSize);
				}
		}

		if (poolType == M) 
			for (int i = 0; i < delta.size(); i++)
				error(MaxIndexMem(i)) += delta(i);

		return error;
	}
};

}

#endif