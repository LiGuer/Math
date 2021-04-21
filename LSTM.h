#ifndef LSTM_H
#define LSTM_H
/*********************************************************************************
						LSTM 长短期记忆网络
*********************************************************************************/
#include "Mat.h"
#include <math.h>
#include <vector>
// x[i] = 1 / (1 + exp(-x[i]));
// x[i] = tanh(x[i]);
/*********************************************************************************
						LSTM 参数
*********************************************************************************/
class LstmParam
{
public:
	// G, I, F, O
	int memCellCount, x_dim;
	Mat<double> weights[4], bias[4], diffWeights[4], diffBias[4];

	LstmParam(int _memCellCount, int _x_dim) {
		memCellCount = _memCellCount;
		x_dim = _x_dim;
		int concatLen = memCellCount + x_dim;
		for (int i = 0; i < 4; i++) {
			weights[i].rands(memCellCount, concatLen, -0.1, 0.1);
			bias[i].rands(memCellCount, 1, -0.1, 0.1);
			diffWeights[i].zero(memCellCount, concatLen);
			diffBias[i].zero(memCellCount);
		}
	}
	~LstmParam() {};
	void applyDiff(double lr) {
		for (int i = 0; i < 4; i++) {
			weights[i] += diffWeights[i] *= -lr;
			bias[i] += diffBias[i] *= -lr;
			diffWeights[i].zero();
			diffBias[i].zero();
		}
	}
};
/*********************************************************************************
						LSTM 状态
*********************************************************************************/
class LstmState
{
public:
	// Gate[4]: G, I, F, O
	Mat<double> gate[4], s, h, bottomDiff_H, bottomDiff_S;
	LstmState(int memCellCount, int x_dim) {
		for (int i = 0; i < 4; i++)gate[i].zero(memCellCount);
		s.zero(memCellCount);
		h.zero(memCellCount);
		bottomDiff_H.zero(memCellCount);
		bottomDiff_S.zero(memCellCount);
	}
};
/*********************************************************************************
						LSTM 节点
*	结构: G, I, F, O
----------------------------------------------------------------------------------
*	正向传播
		gt = tanh(Wg·[h_(t-1), xt] + bg)
		ft = σ(Wf·[h_(t-1), xt] + bf)
		it = σ(Wi·[h_(t-1), xt] + bi)
		ot = σ(Wo·[h_(t-1), xt] + bo)
		Ct^~ = tanh(Wc·[h_(t-1), xt] + bc)
		Ct = ft * C_(t-1) + it * Ct^~
		ht = ot·tanh(Ct)
----------------------------------------------------------------------------------
*	反向传播
*********************************************************************************/
class LstmNode
{
public:
	LstmState* state;
	LstmParam* param;
	Mat<double> xc, s_prev, h_prev;
	LstmNode(LstmState* _state, LstmParam* _param) {
		state = _state;
		param = _param;
	}
	/*-------------------------------- 正向传播 --------------------------------*/
	void forward(Mat<double>& x, Mat<double>* _s_prev, Mat<double>* _h_prev) {
		//if this is the first lstm node in the network, else save data for use in backprop
		if (_s_prev == NULL)s_prev.zero(state->s.rows, state->s.cols); else s_prev = *_s_prev;
		if (_h_prev == NULL)h_prev.zero(state->h.rows, state->h.cols); else h_prev = *_h_prev;
		xc.horizStack(x, h_prev);							//concatenate x(t) and h(t-1) into xc
		// G F I O
		Mat<double> tmp;
		state->gate[0].function(							//G(t) = tanh(Wg·xc + ba)
			tmp.add(tmp.elementMult(param->weights[0], xc), 
			param->bias[0]), [](double x) { return tanh(x); }
		);
		for (int i = 1; i < 4; i++) 						//It, Ft, Ot = σ( Wifo·xc + bifo )	
			state->gate[i].function(
				tmp.add(tmp.elementMult(param->weights[i], xc), param->bias[i]),
				[](double x) {return 1 / (1 + exp(-x)); }
			);
		// S H
		state->s.add(tmp.mult(state->gate[0], state->gate[1]), state->s.mult(s_prev, state->gate[2]));//st = gt * it + s(t-1) * ft
		state->h.mult(state->s, state->gate[3]);			//ht = st * ot
	}
	/*-------------------------------- 反向传播 --------------------------------*/
	void backward(Mat<double>& top_diff_h, Mat<double>& top_diff_s) {
		//notice that top_diff_s is carried along the constant error carousel
		Mat<double> delta[4], delta_s, tmp;
		delta_s.add(delta_s.elementMult(state->gate[3], top_diff_h), top_diff_s);
		delta[0].elementMult(state->gate[2], delta_s);
		delta[1].elementMult(s_prev, delta_s);
		delta[2].elementMult(state->gate[0], delta_s);
		delta[3].elementMult(state->s, top_diff_h);
		//diffs w.r.t. vector inside sigma / tanh function
		delta[0].elementMult(tmp.function(state->gate[0], [](double x) { return 1 - x * x; }));
		for (int i = 1; i < 4; i++) 
			delta[i].elementMult(tmp.function(state->gate[i], [](double x) { return x * (1 - x); }));
		//diffs w.r.t. inputs & compute bottom diff
		Mat<double> delta_xc(xc.rows, xc.cols);
		for (int i = 0; i < 4; i++) {
			param->diffWeights[i] += tmp.mult(delta[i], xc.transpose(tmp));
			param->diffBias[i] += delta[i];
			delta_xc += tmp.mult(param->weights->transpose(tmp), delta[i]);
		}
		//save bottom diffs
		state->bottomDiff_S.elementMult(delta_s, state->gate[1]);
		state->bottomDiff_H = delta_xc.block(param->x_dim, delta_xc.rows - 1, 0, 0, tmp);
	}
};
/*********************************************************************************
						LSTM 长短期记忆网络
*********************************************************************************/
class LstmNetwork
{
	LstmParam* param;
	std::vector<LstmNode> nodeList;
	std::vector<Mat<double>> x_list;		//input sequence
	LstmNetwork(LstmParam* _param) {
		param = _param;
	}
	void x_list_add(Mat<double>& x) {
		x_list.push_back(x);
		if (x_list.size() > nodeList.size()) {
			LstmState* st = new LstmState(param->memCellCount, param->x_dim);
			LstmNode nt(st, param);
			nodeList.push_back(nt);
		}
		int idx = x_list.size() - 1;
		if (idx == 0) nodeList[idx].forward(x, NULL, NULL);
		else {
			Mat<double>s_prev = nodeList[idx - 1].state->s;
			Mat<double>h_prev = nodeList[idx - 1].state->h;
			nodeList[idx].forward(x, &s_prev, &h_prev);
		}
	}
	double y_list_is(Mat<double> y_list, double(*lossLayer)(Mat<double>&, double), double(*lossLayer_bottomDiff)(Mat<double>&, double)) {
		int idx = x_list.size() - 1;
		double loss = lossLayer(nodeList[idx].state->h, y_list[idx]);
		Mat<double>diff_h = lossLayer_bottomDiff(nodeList[idx].state->h, y_list[idx]);
		Mat<double>diff_s(param->memCellCount);
		nodeList[idx].backward(diff_h, diff_s);
		while (--idx >= 0) {
			loss += lossLayer(nodeList[idx].state->h, y_list[idx]);
			diff_h = lossLayer_bottomDiff(nodeList[idx].state->h, y_list[idx]);
			diff_h += nodeList[idx + 1].state->bottomDiff_H;
			diff_s = nodeList[idx + 1].state->bottomDiff_S;
			nodeList[idx].backward(diff_h, diff_s);
		}
		return loss;
	}
};

#endif