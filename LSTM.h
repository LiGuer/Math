#include "Mat.h"
#include <math.h>
Mat<float>& sigmoid(Mat<float>& x)
{
	for (int i = 0; i < x.rows * x.cols; i++)
		x[i] = 1 / (1 + exp(-x[i]));
	return x;
}
Mat<float>& tanh(Mat<float>& x)
{
	for (int i = 0; i < x.rows * x.cols; i++)
		x[i] = tanh(x[i]);
	return x;
}

class LstmParam
{
public:
	int mem_cell_ct, x_dim, concat_len;
	// G, I, F, O
	Mat<float>weights[4], bias[4], differWeights[4], differBias[4];

	LstmParam(int _mem_cell_ct, int _x_dim) {
		mem_cell_ct = _mem_cell_ct;
		x_dim = _x_dim;
		concat_len = mem_cell_ct + x_dim;
		for (int i = 0; i < 4; i++) {
			weights[i].rands(mem_cell_ct, concat_len, -0.1, 0.1);
			bias[i].rands(mem_cell_ct, 1, -0.1, 0.1);
			differWeights[i].zero(mem_cell_ct, concat_len);
			differBias[i].zero(mem_cell_ct);
		}
	}
	~LstmParam() {};
	void applyDiff(float lr) {
		for (int i = 0; i < 4; i++) {
			differWeights[i].mult(-lr, differWeights[i]);
			differBias[i].mult(-lr, differBias[i]);
			weights[i].add(weights[i], differWeights[i]);
			bias[i].add(bias[i], differBias[i]);
			// reset diffs to zero
			differWeights[i].zero();
			differBias[i].zero();
		}
	}
};

class LstmState
{
public:
	// Gate[4]: G, I, F, O
	Mat<float> gate[4], s, h;
	Mat<float> bottomDiffer_H, bottomDiffer_S;
	LstmState(int mem_cell_ct, int x_dim) {
		for (int i = 0; i < 4; i++)gate[i].zero(mem_cell_ct);
		s.zero(mem_cell_ct);
		h.zero(mem_cell_ct);
		bottomDiffer_H.zero(mem_cell_ct);
		bottomDiffer_S.zero(mem_cell_ct);
	}
};

class LstmNode
{
public:
	LstmState* state;
	LstmParam* param;
	Mat<float> xc, s_prev, h_prev;

	LstmNode(LstmState* _state, LstmParam* _param) {
		state = _state;
		param = _param;
	}
	void Forward(Mat<float>* x, Mat<float>* _s_prev, Mat<float>* _h_prev) {
		//if this is the first lstm node in the network, else save data for use in backprop
		if (_s_prev == NULL)s_prev.zero(state->s.rows, state->s.cols);
		else s_prev = *_s_prev;
		if (_h_prev == NULL)h_prev.zero(state->s.rows, state->s.cols);
		else h_prev = *_h_prev;
		xc.horizStack(*x, h_prev);							//concatenate x(t) and h(t-1) into xc
		Mat<float> temp;
		temp.add(temp.dot(param->weights[0], xc), param->bias[0]);	//G(t) = tanh(Wg·xc + ba)
		state->gate[0] = tanh(temp);
		for (int i = 1; i < 4; i++) {						//It, Ft, Ot = σ( Wifo·xc + bifo )	
			temp.add(temp.dot(param->weights[i], xc), param->bias[i]);
			state->gate[i] = sigmoid(temp);
		}
		state->s.add(temp.mult(state->gate[0], state->gate[1]), state->s.mult(s_prev, state->gate[2]));//st = gt * it + s(t-1) * ft
		state->h.mult(state->s, state->gate[3]);			//ht = st * ot
	}

	void Backward(Mat<float>& top_diff_h, Mat<float>& top_diff_s) {
		//notice that top_diff_s is carried along the constant error carousel
		Mat<float> delta[4], delta_s;
		delta_s.add(delta_s.dot(state->gate[3], top_diff_h), top_diff_s);
		delta[3].dot(state->s, top_diff_h);
		delta[1].dot(state->gate[0], delta_s);
		delta[0].dot(state->gate[1], delta_s);
		delta[2].dot(s_prev, delta_s);
		//diffs w.r.t. vector inside sigma / tanh function
		delta[0].dot(tanh_derivative(state->gate[0]), delta[0]);
		for (int i = 1; i < 4; i++) {
			delta[i].dot(sigmoid_derivative(state->gate[i]), delta[i]);
		}
		//diffs w.r.t. inputs
		Mat<float> temp;
		for (int i = 0; i < 4; i++) {
			temp.outer(delta[i], xc);
			param->differWeights[i].add(param->differWeights[i], temp);
			param->differBias[i].add(param->differBias[i], delta[i]);
		}
		//compute bottom diff
		Mat<float> delta_xc(xc.rows, xc.cols);
		for (int i = 0; i < 4; i++) {
			param->weights->transposi(temp);
			delta_xc.add(delta_xc, temp.dot(temp, delta[i]));
		}
		//save bottom diffs
		state->bottomDiffer_S.dot(delta_s, state->gate[2]);
		state->bottomDiffer_H = dxc[self.param.x_dim:];
	}
};

class LstmNetwork
{
	LstmParam* param;
	LstmNode* node_list;
	Mat<float> x_list;		//input sequence
	LstmNetwork(LstmParam* _param) {
		param = _param;
	}
	void x_list_clear() {
		x_list.zero();
	}
	void x_list_add() {
		x_list.append(x);
		if (len(x_list) > len(node_list)) {
			lstm_state = LstmState(param->mem_cell_ct, param->x_dim);
			node_list.append(LstmNode(param, lstm_state));
		}
		int idx = len(x_list) - 1;
		if (idx == 0)node_list[idx].Forward(x);
		else {
			Mat<float>s_prev = node_list[idx - 1].state.s;
			Mat<float>h_prev = node_list[idx - 1].state.h;
			node_list[idx].Forward(x, s_prev, h_prev);
		}
	}
	float y_list_is(Mat<float> y_list, loss_layer) {
		assert len(y_list) == len(self.x_list);
		int idx = len(x_list) - 1;
		float loss = loss_layer.loss(node_list[idx].state.h, y_list[idx]);
		Mat<float>diff_h = loss_layer.bottom_diff(node_list[idx].state.h, y_list[idx]);
		Mat<float>diff_s(param->mem_cell_ct);
		node_list[idx].Backward(diff_h, diff_s);
		while (--idx >= 0) {
			loss += loss_layer.loss(node_list[idx].state.h, y_list[idx]);
			diff_h = loss_layer.bottom_diff(node_list[idx].state.h, y_list[idx]);
			diff_h += node_list[idx + 1].state.bottom_diff_h;
			diff_s = node_list[idx + 1].state.bottom_diff_s;
			node_list[idx].Backward(diff_h, diff_s);
		}
		return loss;
	}
};

int main()
{

}