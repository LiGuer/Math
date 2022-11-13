#ifndef NEURAL_NETWORKS_LSTMLAYER_H
#define NEURAL_NETWORKS_LSTMLAYER_H

#include "include.h"

namespace NeuralNetworks {

/*********************************************************************************
						LSTM Layer 长短期记忆层
*	结构: Gate[4]: G, F, I, O 四个并行神经网络层
----------------------------------------------------------------------------------
*	正向传播
		gt = tanh(Wg×[h_(t-1), xt] + bg)
		ft = Sigmoid(Wf×[h_(t-1), xt] + bf)
		it = Sigmoid(Wi×[h_(t-1), xt] + bi)
		ot = Sigmoid(Wo×[h_(t-1), xt] + bo)
		------
		st = ft·s_(t-1) + it·gt
		ht = ot·tanh(Ct)
		------
		W_gfio			(cellNum, cellNum + xDim)
		[h_(t-1), xt]	(cellNum + xDim)
		gfiosh_t		(cellNum)
----------------------------------------------------------------------------------
*	反向传播
		Δg = it	 ·Δs		·tanh'(gt)
		Δf = s_(t-1)·Δs		·Sigmoid'(ft)
		Δi = gt	 ·Δs		·Sigmoid'(it)
		Δo = st	 ·Δh_(t+1)·Sigmoid'(ot)
		------
		Δgfio_W += Δgfio×xc^T
		Δgfio_b += Δgfio
		------
		Δs = ot·Δh_(t+1) + Δs_(t+1)
		Δh = Δxc[Δx, Δh]
		Δxc = ΣW^T×Δ_gfio
*********************************************************************************/
class LstmLayer
{
public:
	Mat<> gate[4], weights[4], bias[4], diffWeights[4], diffBias[4];	// Gate[4]: G, F, I, O
	Mat<> s, h, xc;													// State,out
	std::vector<Mat<>> gateSet[4], prevSSet, prevHSet, xcSet;
	LstmLayer() { ; }
	LstmLayer(int inSize, int outSize) { 
		init(inSize, outSize); 
	}

	void init(int inSize, int outSize) {
		for (int i = 0; i < 4; i++) {
			gate		[i].zero (outSize);
			weights		[i].rands(outSize, outSize + inSize, -0.1, 0.1);
			bias		[i].rands(outSize, 1, -0.1, 0.1);
			diffWeights	[i].zero (weights[i]);
			diffBias	[i].zero (bias	 [i]);
		}
	}

	/*-------------------------------- 正向传播 --------------------------------*/
	std::vector<Mat<>>* operator()(std::vector<Mat<>>& in, Mat<>& s_prev, Mat<>& h_prev) { 
		return forward(in, s_prev, h_prev);
	}

	std::vector<Mat<>>* forward   (std::vector<Mat<>>& in, Mat<>& s_prev, Mat<>& h_prev) {
		for (int i = 0; i < 4; i++)
			gateSet[i].clear();

		prevSSet.clear(); 
		prevSSet.push_back(s_prev);

		prevHSet.clear();
		prevHSet.push_back(h_prev);

		xcSet.clear();

		for (int timeStep = 0; timeStep < in.size(); timeStep++) {
			forward(
				in   [timeStep], 
				prevSSet[timeStep], 
				prevHSet[timeStep]
			);
			for (int i = 0; i < 4; i++) 
				gateSet[i].push_back(gate[i]);
			prevSSet.push_back(s);
			prevHSet.push_back(h);
			xcSet.   push_back(xc);
		}
		s_prev = s;
		h_prev = h;
		return &prevHSet;
	}

	Mat<>* forward(Mat<>& in, Mat<>& prevS, Mat<>& prevH) {
		// G F I O
		xc.rowsStack(in, prevH);														//[h_(t-1), xt]
		for (int i = 0; i < 4; i++) { 													
			gate[i].add(gate[i].mul(weights[i], xc), bias[i]);
			i == 0 ?
				gate[i].function([](double x) { return tanh(x); }):						//gt = tanh(Wg×[h_(t-1), xt] + bg) 	
				gate[i].function([](double x) { return 1 / (1 + exp(-x)); });			//Ft, It, Ot = Sigmoid( W_ifo×[h_(t-1), xt] + b_ifo )
		}
		// S H
		Mat<> tmp;
		s.add(tmp.elementMul(gate[0], gate[2]), s.elementMul(prevS, gate[1]));		//st = gt·it + s_(t-1)·ft
		return &h.elementMul(gate[3], s);
		//h.elementMul(gate[3], tmp.function(s, [](double x) { return tanh(x); }));	//ht = ot·tanh(st)
	}

	/*-------------------------------- 反向传播 --------------------------------*/
	void backward(std::vector<Mat<>>& error, double learnRate) {
		Mat<> tmp, 
			  diffH(error[0].rows), 
			  diffS(error[0].rows);

		for (int timeStep = error.size() - 1; timeStep >= 0; timeStep--) {
			for (int i = 0; i < 4; i++) 
				gate[i] = gateSet[i][timeStep];

			s  = prevSSet[timeStep + 1];
			xc = xcSet[timeStep];
			backward(
				prevSSet[timeStep],
				error   [timeStep], 
				diffH, 
				diffS
			);
		}

		//Update
		for (int i = 0; i < 4; i++) {
			weights[i] += (diffWeights[i] *= learnRate);
			bias   [i] += (diffBias   [i] *= learnRate);
			diffWeights[i].zero();
			diffBias   [i].zero();
		}
	}

	void backward(Mat<>& prevS, Mat<>& error, Mat<>& diffH, Mat<>& diffS) {
		diffH += error;
		//notice that top_diffS is carried along the constant error carousel
		Mat<> diffGate[4], tmp;
		diffS.add(tmp.elementMul(gate[3], diffH), diffS);							//Δs = ot·Δh_(t+1) + Δs_(t+1) 
		diffGate[0].  elementMul(gate[2], diffS);									//Δg = it·Δs
		diffGate[1].  elementMul(prevS,   diffS);									//Δf = s_(t-1)·Δs
		diffGate[2].  elementMul(gate[0], diffS);									//Δi = gt·Δs
		diffGate[3].  elementMul(s,       diffH);									//Δo = st·Δh_(t+1)
		//arctive Derivative function												//Δgfio = Δgfio·σ'(gfio)
		for (int i = 0; i < 4; i++)
			i == 0 ?
			diffGate[i].elementMul(tmp.function(gate[i], [](double x) { return x * x > 0 ? 0 : 1 - x * x; })) :
			diffGate[i].elementMul(tmp.function(gate[i], [](double x) { return x * (1 - x); }));
		//diffs w.r.t. ins & compute bottom diff
		Mat<> diffXc(xc.rows);												//Δgfio_W += Δgfio×xc^T //Δgfio_b += Δgfio //Δxc = ΣW^T×Δ_gfio
		for (int i = 0; i < 4; i++) {
			diffWeights[i] += tmp.mul(diffGate[i], xc.transpose(tmp));
			diffBias   [i] += diffGate[i];
			diffXc += tmp.mul(weights[i].transpose(tmp), diffGate[i]);
		}
		diffS.elementMul(gate[1]);													//Δs = Δs·ft
		diffXc.block_(diffXc.rows - diffH.rows, diffXc.rows - 1, 0, 0, diffH);		//Δh
	}

	/*----------------[ save / load ]----------------*/
	void save(FILE* file) {
		for (int k = 0; k < 4; k++) {
			for (int i = 0; i < weights[k].size(); i++) 
				fprintf(file, "%f ", weights[k][i]);

			for (int i = 0; i < bias   [k].rows;   i++)
				fprintf(file, "%f ", bias   [k][i]);
		}
		fprintf(file, "\n");
	}

	void load(FILE* file) {
		for (int k = 0; k < 4; k++) {
			for (int i = 0; i < weights[k].size(); i++) 
				fscanf(file, "%lf", &weights[k][i]);

			for (int i = 0; i < bias   [k].rows;   i++)
				fscanf(file, "%lf", &bias   [k][i]);
		}
	}
};

}

#endif