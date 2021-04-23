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
[Reference]:
[1] Thanks for nicodjimenez at https://github.com/nicodjimenez/lstm
==============================================================================*/
#ifndef LSTM_H
#define LSTM_H
/*********************************************************************************
						LSTM 长短期记忆网络
*********************************************************************************/
#include "Mat.h"
#include <math.h>
#include <vector>
/*********************************************************************************
						LSTM 节点
*	结构: Gate[4]: G, F, I, O
----------------------------------------------------------------------------------
*	正向传播
		gt = tanh(Wg×[h_(t-1), xt] + bg)
		ft = σ(Wf×[h_(t-1), xt] + bf)
		it = σ(Wi×[h_(t-1), xt] + bi)
		ot = σ(Wo×[h_(t-1), xt] + bo)
		------
		st = ft·s_(t-1) + it·gt
		ht = ot·tanh(Ct)
		------
		W_gfio			(cellNum, cellNum + xDim)
		[h_(t-1), xt]	(cellNum + xDim)
		gfiosh_t		(cellNum)
----------------------------------------------------------------------------------
*	反向传播
*********************************************************************************/
class LstmNodeParam {
public:
	int memCellCount, xDim;
	Mat<double> weights[4], bias[4], diffWeights[4], diffBias[4];
	LstmNodeParam(int _memCellCount, int _x_dim) {
		memCellCount = _memCellCount; xDim = _x_dim;
		for (int i = 0; i < 4; i++) {
			weights[i].rands(memCellCount, memCellCount + xDim, -0.1, 0.1);
			bias[i].rands(memCellCount, 1, -0.1, 0.1);
			diffWeights[i].zero(memCellCount, memCellCount + xDim);
			diffBias[i].zero(memCellCount);
		}
	}
	/*-------------------------------- applyDiff --------------------------------*/
	void applyDiff(double lr) {
		for (int i = 0; i < 4; i++) {
			weights[i] += diffWeights[i] *= -lr;
			bias[i] += diffBias[i] *= -lr;
			diffWeights[i].zero();
			diffBias[i].zero();
		}
	}
};
class LstmNode
{
public:
	// Gate[4]: G, F, I, O
	LstmNodeParam* param;
	Mat<double> gate[4];
	Mat<double> s, h, s_prev, h_prev, diff_s, diff_h, xc;
	LstmNode(LstmNodeParam* _param) {
		param = _param;
	}
	/*-------------------------------- 正向传播 --------------------------------*/
	void operator() (Mat<double>& x, Mat<double>& _s_prev, Mat<double>& _h_prev) { return forward(x, _s_prev, _h_prev); }
	void operator() (Mat<double>& x) { Mat<double> zero(param->memCellCount); return forward(x, zero, zero); }
	void forward(Mat<double>& x, Mat<double>& _s_prev, Mat<double>& _h_prev) {
		s_prev = _s_prev, h_prev = _h_prev;
		xc.rowsStack(x, h_prev);							//[h_(t-1), xt]
		// G F I O
		Mat<double> tmp;
		gate[0].function(									//gt = tanh(Wg×[h_(t-1), xt] + bg)
			tmp.add(tmp.mult(param->weights[0], xc), param->bias[0]),
			[](double x) { return tanh(x); }
		);
		for (int i = 1; i < 4; i++) 						//Ft, It, Ot = σ( W_ifo×[h_(t-1), xt] + b_ifo )	
			gate[i].function(
				tmp.add(tmp.mult(param->weights[i], xc), param->bias[i]),
				[](double x) { return 1 / (1 + exp(-x)); }
			);
		// S H
		s.add(tmp.elementMult(gate[0], gate[2]), s.elementMult(s_prev, gate[1]));//st = gt * it + s_(t-1) * ft
		h.elementMult(gate[3], s);
		//h.elementMult(gate[3], tmp.function(s, [](double x) { return tanh(x); }));	//ht = ot·tanh(Ct)
	}
	/*-------------------------------- 反向传播 --------------------------------*/
	void backward(Mat<double>& top_diff_h, Mat<double>& top_diff_s) {
		//notice that top_diff_s is carried along the constant error carousel
		Mat<double> diff[4], tmp;
		diff_s.add(diff_s.elementMult(gate[3], top_diff_h), top_diff_s);
		diff[0].elementMult(gate[2], diff_s);
		diff[1].elementMult(s_prev, diff_s);
		diff[2].elementMult(gate[0], diff_s);
		diff[3].elementMult(s, top_diff_h);
		//diffs w.r.t. vector inside sigma / tanh function
		diff[0].elementMult(tmp.function(gate[0], [](double x) { return 1 - x * x; }));
		for (int i = 1; i < 4; i++) 
			diff[i].elementMult(tmp.function(gate[i], [](double x) { return x * (1 - x); }));
		//diffs w.r.t. inputs & compute bottom diff
		Mat<double> diff_xc(xc.rows, xc.cols);
		for (int i = 0; i < 4; i++) {
			param->diffWeights[i] += tmp.mult(diff[i], xc.transpose(tmp));
			param->diffBias[i] += diff[i];
			diff_xc += tmp.mult(param->weights->transpose(tmp), diff[i]);
		}
		//save bottom diffs
		diff_s.elementMult(gate[1]);
		diff_h = diff_xc.block(param->xDim, diff_xc.rows - 1, 0, 0, tmp);
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
	LstmNodeParam* param;
	std::vector<LstmNode> nodeList;
	std::vector<Mat<double>> xList;		//input sequence
	LstmNetwork(LstmNodeParam* _param) {
		param = _param;
	}
	/*-------------------------------- 正向传播 --------------------------------*/
	void operator() (Mat<double>& x) { return forward(x); }
	void forward(Mat<double>& x) {
		xList.push_back(x);
		LstmNode node(param); nodeList.push_back(node);
		int index = xList.size() - 1; 
		if (index == 0) nodeList[index](x);
		else nodeList[index](x, nodeList[index - 1].s, nodeList[index - 1].h);
	}
	/*-------------------------------- 反向传播 --------------------------------*/
	double backward(Mat<double>& y, double(*lossLayer)(Mat<double>&, double), Mat<double>& (*lossLayer_bottomDiff)(Mat<double>&, double, Mat<double>&)) {
		double loss = 0;
		Mat<double> tmp, zero(param->memCellCount);
		for (int i = xList.size() - 1; i >= 0; i--) {
			loss += lossLayer(nodeList[i].h, y[i]);
			if (i == xList.size() - 1)
				nodeList[i].backward(
					lossLayer_bottomDiff(nodeList[i].h, y[i], tmp),
					zero
				);
			else 
				nodeList[i].backward(
					lossLayer_bottomDiff(nodeList[i].h, y[i], tmp) += nodeList[i + 1].diff_h,
					nodeList[i + 1].diff_s
				);
		}
		return loss;
	}
};
#endif