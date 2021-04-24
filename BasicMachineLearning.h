/*
Copyright 2020,2021 LiGuer. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
	http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#ifndef BASIC_MACHINE_LEARNING_H
#define BASIC_MACHINE_LEARNING_H

#include "Mat.h"
#include <float.h>
#include <algorithm>
#include <vector>
namespace BasicMachineLearning {
	void Apriori(std::vector<Mat<int>>& dataSet, double minSupport, double minConfidence, std::vector<Mat<int>>& RuleSet_A, std::vector<Mat<int>>& RuleSet_B, std::vector<double>& RuleSet_confidence);
	void K_Mean(Mat<double>& x, int K, Mat<double>& Center, Mat<int>& Cluster, Mat<int>& ClusterKthNum, int TimeMax = 0x7FFFFFFF);
	void MahalanobisDist(Mat<double>& x, Mat<double>& mahalanobisDistance);
	void PrincipalComponentsAnalysis(Mat<double>& x, Mat<double>& y, int yDim);
	void SupportVectorMachines(Mat<double> X, Mat<int> Y);
}

/******************************************************************************
*								QLearning
*	[定义]:Q(s,a) = (1 + lr)·Q(s,a) + lr·( R + g·max Q(s',:) )
			s: state	a: action	R: reward	lr: learning rate	g: forget factor
*	[原理]:
		选择动作: ε-greedy方法:
			每个状态以ε概率随机选取动作，1-ε概率选择当前最优解
		眼前利益R: 反馈值.
		记忆中的利益 max Q(s',:): 记忆里，新位置s'能给出的最大效用值.
		forget factor越大，越重视以往经验，越小，则只重视眼前利益R.
*	[流程]:
		[1] Init Q table arbitrarily
		[2] Repeat (for each episode), until s is terminal
			[3] Choose a from s using policy derived from Q (eg. ε-greedy)
			[4] Take action a, observe r s'
			[5] Q(s,a) = (1 + lr)·Q(s,a) + lr·( R + g·max Q(s',:) )
				s = s'
*	[Ps]:
		可以逐渐降低随机选取动作的概率ε，一开始随机率可达100%
			然后随训练次数的深入，应当逐渐降低随机概率。
*******************************************************************************/
class QLearning {
public:
	double learnRate = 0.6, Gamma = 0.8, greedy = 0.9; //奖励递减值# 贪婪度
	int actionNum = 0, stateNum = 0;
	Mat<double> QTable;
	double preState = 0;
	/*---------------- 初始化 ----------------*/
	QLearning(int _stateNum, int _actionNum) { init(_stateNum, _actionNum); }
	void init(int _stateNum, int _actionNum) {
		actionNum = _actionNum;
		stateNum = _stateNum;
		QTable.zero(_stateNum, _actionNum);
	}
	/*---------------- 选择行为 ----------------*/
	int chooseAction(int state) {
		int action = 0;
		bool flag = 1;
		for (int i = 0; i < actionNum; i++)
			if (QTable(state, i) != 0) { flag = 0; break; }
		if (rand() / double(RAND_MAX) < greedy || flag) return rand() % actionNum;
		double t = -DBL_MAX;
		for (int i = 0; i < actionNum; i++)
			if (QTable(state, i) > t) { t = QTable(state, i); action = i; }
		return action;
	}
	/*---------------- 反馈学习 ----------------*/
	void feedback(int state, int action, double R) {
		double t = -DBL_MAX;
		for (int i = 0; i < actionNum; i++)
			t = QTable(state, i) > t ? QTable(state, i) : t;
		QTable(preState, action) = (1 - learnRate) * QTable(preState, action) + learnRate * (R + Gamma * t);
		preState = state;
	}
};
/******************************************************************************
*                    Deep Q Network
*	[思想]: Q-Learning + Neural Network
		[Loss Function]: 近似值和真实值的均方差
*	[流程]:
	[1] Initialize replay memory D to capacity N
		Initialize action-value function Q with random weights θ
		Initialize target action-value function Q with weights θ- = θ
	[2] for episode 1,M do
		[] for t= 1,T do
			[] Initialize sequence 81 = {x1 } and preprocessed sequenceΦ1 =Φ(81)
			[] With probability ε select a random action at
			[] otherwise select at = arg maxa Q(Φ(8t),a;0)
			[] Execute action at in the emulator and observe reward rt and image rt+1
			[] Setst+1 = 8t,at,2t+1 and preprocess中+1 =Φ(8+1)
			[] Store experience (φ,at,rt,φ++1)in D
			[] Sample random minibatch of experiences (Φ, aj,rj,Φj+1) from D
			[] Setyj=	{ rj				if episode terminates at stepj+1
					(rj + r maxa' Q(φj+1,a';0-) otherwise
			[] Perform a gradient descent step on (9j - Q(Φj, aj; 0))2 with respect to the weights θ
			[] Every C steps reset Q=Q
******************************************************************************/
class DeepQNetwork {
public:
	int stateNum, actionNum;
	double greedy = 1;
	int chooseAction(int state) {
		int action = 0;
		if (rand() / double(RAND_MAX) < greedy) return rand() % actionNum;
		Mat<float> ans;// = evalNet(state);
		ans.max(action);
		return action;
	}
};
#endif