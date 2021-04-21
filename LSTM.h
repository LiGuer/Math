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
	int memCellCount, x_dim;
	Mat<double> weights[4], bias[4], diffWeights[4], diffBias[4];
	LstmNodeParam(int _memCellCount, int _x_dim) {
		memCellCount = _memCellCount; x_dim = _x_dim;
		for (int i = 0; i < 4; i++) {
			weights[i].rands(memCellCount, memCellCount + x_dim, -0.1, 0.1);
			bias[i].rands(memCellCount, 1, -0.1, 0.1);
			diffWeights[i].zero(memCellCount, memCellCount + x_dim);
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
	Mat<double> s, h, s_prev, h_prev,xc, diff_h, diff_s;
	LstmNode(LstmNodeParam* _param) {
		param = _param;
	}
	/*-------------------------------- 正向传播 --------------------------------*/
	void forward(Mat<double>& x, Mat<double>& _s_prev, Mat<double>& _h_prev) {
		s_prev = _s_prev, h_prev = _h_prev;
		xc.rowsStack(x, h_prev);
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
		h.elementMult(gate[3], tmp.function(s, [](double x) { return tanh(x); }));	//ht = ot·tanh(Ct)
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
		diff_h = diff_xc.block(param->x_dim, diff_xc.rows - 1, 0, 0, tmp);
	}
};
/*********************************************************************************
						LSTM 长短期记忆网络
*********************************************************************************/
class LstmNetwork
{
	LstmNodeParam* param;
	int memCellCount, x_dim;
	std::vector<LstmNode> nodeList;
	std::vector<Mat<double>> x_list;		//input sequence
	LstmNetwork(LstmNodeParam* _param) {
		param = _param;
	}
	void forward(Mat<double>& x) {
		x_list.push_back(x);
		LstmNode nt(param);
		if (x_list.size() > nodeList.size()) nodeList.push_back(nt);
		int index = x_list.size() - 1;
		if (index == 0) {
			Mat<double> zero(memCellCount); nodeList[index].forward(x, zero, zero);
		}
		else nodeList[index].forward(x, nodeList[index - 1].s, nodeList[index - 1].h);
	}
	double backward(Mat<double> y_list, double(*lossLayer)(Mat<double>&, double), Mat<double>& (*lossLayer_bottomDiff)(Mat<double>&, double)) {
		int index = x_list.size() - 1;
		double loss = lossLayer(nodeList[index].h, y_list[index]);
		Mat<double> tmp;
		nodeList[index].backward(lossLayer_bottomDiff(nodeList[index].h, y_list[index]), tmp.zero(memCellCount));
		while (--index >= 0) {
			loss += lossLayer(nodeList[index].h, y_list[index]);
			tmp = lossLayer_bottomDiff(nodeList[index].h, y_list[index]);
			nodeList[index].backward(tmp += nodeList[index + 1].diff_h, nodeList[index + 1].diff_s);
		}
		return loss;
	}
};

#endif