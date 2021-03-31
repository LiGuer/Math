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
==============================================================================*/
#ifndef BASIC_MACHINE_LEARNING_H
#define BASIC_MACHINE_LEARNING_H

#include "Mat.h"
#include <float.h>
#include <algorithm>
#include <vector>
/*--------------------------------[ K Mean 聚类 ]--------------------------------
*	* 对N维分布的数据点，可以将其聚类在 K 个关键簇内
*	*[流程]:
		[1] 随机选择 K 个簇心点 Center
		[2] 迭代开始
			[3] 归零 Cluster , Cluster: 簇,记录ith簇内的数据指针。
			[4] 计算每个xi到簇心μj的距离
				[5] 选择距离最小的簇心, 将该点加入其簇内
			[6] 对每个簇,计算其质心 Center'
			[7] Center≠Center' , 则更正Center为 Center'
			[8] 迭代重新开始
		[9] 一轮无更正时，迭代结束
**------------------------------------------------------------------------*/
void K_Mean(Mat<double>& x, int K, Mat<double>& Center, Mat<int>& Cluster, Mat<int>& ClusterKthNum, int TimeMax = 0x7FFFFFFF) {
	int Dimension = x.rows, N = x.cols;
	Center.zero(Dimension, K);
	Cluster.zero(K, N); ClusterKthNum.zero(K, 1);
	//[1] 随机选择 K 个簇心点 
	for (int i = 0; i < K; i++) {
		int index = rand() % N;
		for (int dim = 0; dim < Dimension; dim++)Center(dim, i) = x(dim, index);
	}
	//[2]
	int times = 0;
	while (times++ < TimeMax) {
		//[3]
		Cluster.clean(); ClusterKthNum.clean();
		//[4] 计算每个xi到Center_j的距离
		for (int i = 0; i < N; i++) {
			Mat<double> d(1, K);
			for (int j = 0; j < K; j++)
				for (int dim = 0; dim < Dimension; dim++)
					d[j] += (x(dim, i) - Center(dim, j)) * (x(dim, i) - Center(dim, j));
			//[5]
			int index; d.min(index);
			Cluster(index, ClusterKthNum[index]++) = i;
		}
		//[6] 对每个簇,计算其质心 Center'
		Mat<double> CenterTemp(Dimension, K);
		for (int i = 0; i < K; i++) {
			for (int dim = 0; dim < Dimension; dim++) {
				for (int j = 0; j < ClusterKthNum[i]; j++)
					CenterTemp(dim, i) += x(dim, Cluster(i, j));
				CenterTemp(dim, i) /= ClusterKthNum[i];
			}
		}
		//[7] 更正簇心
		bool flag = 1;
		for (int i = 0; i < Dimension * K; i++) {
			if (CenterTemp[i] != Center[i]) { flag = 0; break; }
		}
		if (flag)return;								//[9]
		else {
			free(Center.data); Center.data = CenterTemp.data; CenterTemp.data = NULL;
		}
	}
}
/*--------------------[ Principal Components Analysis ]--------------------
*	Principal Components Analysis 主成分分析
*	输入: [1] x: 待处理矩阵
*	输出: [1] y: 降维后矩阵
*	原理:
		[1] 最近重构性: 样本点到这个超平面的距离都足够近。
		[2] 最大可分性: 样本点在这个超平面的投影能尽可能分开。
*	流程:                  ->   ->
		[1] 数据中心化, Σ xi = 0
		[2] 计算协方差矩阵, X XT
		[3] 对协方差特征值分解
		[4] 取最大的d'个特征值所对应的特征向量{w1,w2,...,wd'},投影矩阵 W = [w1,w2,...,wd']
		[5] 样本点在超平面投影: yi = WT xi
*	推导:
		分别从原理[1, 2]可以推得同样的结果。
		样本点到超空间投影: y = WT x, 尽可能分开即.方差最大:max Σ WT x xT W
		可得:    maxW  tr( WT x xT W )
				 st. WT W = I
		拉格朗日乘子法,得:	X XT ωi = λi ωi,		即.对协方差X XT, 特征值求解
		取特征值最大的yDim个特征向量, 即目标投影矩阵W
**-------------------------------------------------------------------------*/
void PrincipalComponentsAnalysis(Mat<double>& x, Mat<double>& y, int yDim) {
	//[1] 数据中心化
	Mat<double> mean;
	mean.mult(1.0 / x.cols, x.sumCol(mean));		//得到均值
	Mat<double> x2(x);
	for (int i = 0; i < x.rows; i++)
		for (int j = 0; j < x.cols; j++)
			x2(i, j) -= mean[i];
	//[2] 计算协方差矩阵
	Mat<double> Cov;
	Cov.mult(x2, x2.transposi(Cov));		//X*XT
	//[3] 对协方差特征值分解
	Mat<double> eigVec, eigValue;
	Cov.eig(1e-5, eigVec, eigValue);
	//[4] 取最大的d'个特征值所对应的特征向量{w1,w2,...,wd'},投影矩阵 W = [w1,w2,...,wd']T
	// sort
	Mat<double> eigValueTemp(1, eigValue.cols), W(eigVec.rows, yDim);
	for (int i = 0; i < eigValue.cols; i++)eigValueTemp[i] = eigValue(i, i);
	std::sort(eigValueTemp.data, eigValueTemp.data + eigValueTemp.cols);
	// W
	for (int i = 0; i < yDim; i++) {
		double value = eigValueTemp[eigValueTemp.cols - 1 - i];
		for (int j = 0; j < eigValue.cols; j++) {
			if (eigValue(j, j) == value) {
				for (int k = 0; k < eigVec.rows; k++) W(k, i) = eigVec(k, j);
				eigValue(j, j) = -DBL_MAX;
				break;
			}
		}
	}
	//[5]
	y.mult(W.transposi(y), x);
}
/*--------------------------------[ QLearning ]--------------------------------
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
-----------------------------------------------------------------------------*/
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
*                    Support Vector Machines 支持向量机
*	[定义]: 特征空间上, [间隔最大]的线性分类器.
		[超平面]: y = w x + b
		[间隔]: 样本点, 到超平面的距离.
				γi = 1/||w||·|w x + b|	(点到面距离公式)
		[样本点]: {(xi,yi)} i=1toN    xi∈R_n 实向量    yi∈{-1,1}
*	[目标]: 找到目标超平面, 使得[所有样本点间隔最小值γmin = min γi]最大.
			max_wb( min_i 1/||w||·|w xi + b| )
			st.  w x + b > 0 , yi = +1  and  w x + b < 0 , yi = -1  (分类)
		[推导]:
		=>	|w xi + b| = yi(w xi + b)		//去绝对值
		=>	max_wb( 1/||w||·min_i yi(w xi + b) )    st. yi(w x + b) > 0
		转化为:
			min_wb 1/2·||w||²    st. yi (W x + b) ≥ 1
			凸二次规划问题, 用拉格朗日乘子法, 得其对偶问题.
*	[结论]:
		min 1/2·Σi Σj αi αj yi yj K(xi,xj) - Σi αi	//线性时 K(xi,xj)即内积
		st. Σi αi yi = 0    0≤αi≤C
*	[Kernal Trick]: 升到高维, 实现非线性分类
		[Classical Kernal Function]:
		* 高斯核函数: K(x,z) = exp( -||x - z||² / 2σ² )
*	[二次规划优化问题]:
		[算法]: Sequential Minimal Optimization 算法
		[思路]: 若所有变量解都满足此最优化问题的KKT条件，则得到最优化问题解
		[Karush Kuhn Tucker条件]: 非线性规划最佳解的必要条件
*	[流程]:
		[1] 选择惩罚参数 C > 0, 构造并求解凸二次规划问题, 得到最优解α
		[2] w = Σi αi yi xi    b = yj - Σαi yi (xi·xj)
		[3] 得到分离超平面 w x + b = 0
			分类决策函数: f(x) = sign( w x + b )
******************************************************************************/
void SupportVectorMachines(Mat<double> X, Mat<int> Y) {

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
/*--------------------------------[ Mahalanobis Distance ]--------------------------------
* Mahalanobis Distance
**------------------------------------------------------------------------*/
void MahalanobisDist(Mat<double>& x, Mat<double>& mahalanobisDistance) {
	mahalanobisDistance.zero(1, x.cols);
	// mean, diff, cov
	Mat<double> mean, diff, covMat, tmp;
	mean.mult(1.0 / x.cols, x.sumCol(mean));
	covMat.mult(x, x.transposi(covMat));
	// mahalanobis distance
	covMat.inv(covMat);
	Mat<double> xt;
	for (int i = 0; i < x.cols; i++) {
		x.getCol(i, xt);
		diff.add(xt, mean.negative(diff));
		tmp.mult(tmp.mult(diff.transposi(tmp), covMat), diff);
		mahalanobisDistance[i] = tmp[0];
	}
}
/*--------------------------------[ Apriori ]--------------------------------
*[概念]:
	* 频繁项集: 经常出现在一块的物品的集合.
	* 关联规则: 两种物品之间可能存在很强的关系.
	* 支持度P(AB): 数据集中包含该项集的记录所占的比例. P(AB) ≌ num(AB) / num(all)
	* 置信度P(A→B) = P(B|A) = P(AB) / P(A)
*[目的]: 根据频繁项集，寻找数据集中变量之间的关联规则.
*[输入/输出]:
	* 输入: [1]初始频繁项集  [2]最小支持度
	* 输出: [1]关联项集  [2]关联项集支持度
*[定理]:
	* 项集频繁，则其子集频繁. <=> 项集不频繁，则其超集不频繁.
	* 若规则X→Y−X低于置信度阈值，则对于X子集X',规则X'→Y−X'也低于置信度阈值
*[性质]:
	* 频繁项集生成的方法:
		[1] Fk = Fk-1 × F1
		[2] Fk = Fk-1 × Fk-1
	* 频繁项集每一项各不相同， 每一项内部排列有序.
*[过程]:
	[1] 频繁项集生成,对于K项的集合
		[2] 频繁项集子集生成. 生成K项所有可以组合的集合. eg.[frozenset({2, 3}), frozenset({3, 5})] -> [frozenset({2, 3, 5})]
		[3] 保存满足目标支持度P(AB)的集合.
	[4]  关联规则生成, 对不同长度(K)的频繁项集依次分析
		[4.1] 频繁项集只有两个元素{AB}，直接计算置信度P(A→B),P(B→A)
		[4.2] 频繁项集超过两个元素{ABC...}，依次计算置信度P(AC...→B)
		[5] 保存满足目标置信度的关联规则.
**------------------------------------------------------------------------*/
//[2] 生成K项所有可以组合的集合.
void Apriori_GenCandidate(std::vector<Mat<int>>& frozenSet, int K, std::vector<Mat<int>>& newfrozenSet) {
	std::vector<Mat<int>> newfrozenSet_Tmp;
	Mat<int> tmp(1, K);
	for (int i = 0; i < frozenSet.size(); i++) {
		for (int j = i + 1; j < frozenSet.size(); j++) {
			bool flag = true;
			for (int k = 0; k < K - 2; k++) {
				tmp[k] = frozenSet[i][k];
				if (frozenSet[i][k] != frozenSet[j][k]) { flag = false; break;	}
			}
			if (flag) {
				tmp[K - 2] = frozenSet[i][K - 2];
				tmp[K - 1] = frozenSet[j][K - 2];
				//频繁项集每一项内排列有序
				std::sort(tmp.data, tmp.data + tmp.cols);
				//频繁项集每一项各不相同
				bool flag2 = true;
				for (int k = 0; k < newfrozenSet_Tmp.size(); k++)
					if (tmp == newfrozenSet_Tmp[k]) { flag2 = false; break; }
				if(flag2) newfrozenSet_Tmp.push_back(tmp);
			}
		}
	}
	newfrozenSet = newfrozenSet_Tmp;
}
//[3] 过滤小于支持度的集合.
void Apriori_Filter(std::vector<Mat<int>>& dataSet, std::vector<Mat<int>>& frozenSet, double minSupport, std::vector<double>& frozenSet_Support) {
	//[3.1]计算支持度P(AB) ≌ num(AB) / num(all)
	frozenSet_Support.clear();
	Mat<double> frozenSet_Count(1, frozenSet.size());
	for (int i = 0; i < frozenSet.size(); i++){
		for (int j = 0; j < dataSet.size(); j++) {
			// if frozenSet[i] is dataSet[j]'s subset
			int cur = 0;
			for (int k = 0; k < dataSet[j].cols; k++)
				if (frozenSet[i][cur] == dataSet[j][k]) cur++;
			if (cur == frozenSet[i].cols) frozenSet_Count[i]++;
		}
	}
	frozenSet_Count.mult(1.0 / dataSet.size(), frozenSet_Count);
	//[3.2]删除小于支持度的集合.
	std::vector<Mat<int>> frozenSet_Tmp;
	for (int i = 0; i < frozenSet.size(); i++) {
		if (frozenSet_Count[i] > minSupport) {
			frozenSet_Tmp.push_back(frozenSet[i]);
			frozenSet_Support.push_back(frozenSet_Count[i]);
		}
	}frozenSet = frozenSet_Tmp;
}
// 对关系规则进行评估 获得满足最小可信度的关联规则
void Apriori_RulesFromConseqence(std::vector < std::vector<Mat<int>>>& frozenSet, std::vector < std::vector<double>>& frozenSet_Support, int K, std::vector<Mat<int>>& BSet, double minConfidence, std::vector<Mat<int>>& RuleSet_A, std::vector<Mat<int>>& RuleSet_B, std::vector<double>& RuleSet_confidence) {
	if (K == 2) {
		// 关系评估
		Mat<int> A(1, K - BSet[0].cols);
		for (int i = 0; i < frozenSet[K].size(); i++) {
			for (int j = 0; j < BSet.size(); j++) {
				// 计算可信度P(A→B) = P(B|A) = P(AB) / P(A)
				// 构建 A
				int B_cur = 0, A_cur = 0;
				for (int k = 0; k < K; k++) {
					if (frozenSet[K][i][k] == BSet[j][B_cur])B_cur++;
					else if (A_cur < K - BSet[0].cols)A[A_cur++] = frozenSet[K][i][k];
				}
				if (A_cur + B_cur != K)continue;
				// 查找 P(A)
				int A_index = -1;
				for (int k = 0; k < frozenSet[A.cols].size(); k++)if (frozenSet[A.cols][k] == A) { A_index = k; break; }
				if (A_index == -1) continue;
				double confidence = frozenSet_Support[K][i] / frozenSet_Support[A.cols][A_index];
				// 保存满足可信度的关联规则
				if (confidence >= minConfidence) {
					RuleSet_A.push_back(A);
					RuleSet_B.push_back(BSet[j]);
					RuleSet_confidence.push_back(confidence);
				}
			}
		}
		return;
	}
	if (K > BSet[0].cols) {
		// 组合得到 new B 集
		Apriori_GenCandidate(BSet, BSet[0].cols + 1, BSet);
		std::vector<Mat<int>> BSet_Tmp;
		// 关系评估
		Mat<int> A(1, K - BSet[0].cols);
		for (int i = 0; i < frozenSet[K].size(); i++) {
			for (int j = 0; j < BSet.size(); j++) {
				// 计算可信度P(A→B) = P(B|A) = P(AB) / P(A)
				// 构建 A
				int B_cur = 0, A_cur = 0;
				for (int k = 0; k < K; k++) {
					if (frozenSet[K][i][k] == BSet[j][B_cur])B_cur++;
					else if (A_cur < K - BSet[0].cols)A[A_cur++] = frozenSet[K][i][k];
				}
				if (A_cur + B_cur != K)continue;
				// 查找 P(A)
				int A_index = -1;
				for (int k = 0; k < frozenSet[A.cols].size(); k++)if (frozenSet[A.cols][k] == A) { A_index = k; break; }
				if (A_index == -1) continue;
				double confidence = frozenSet_Support[K][i] / frozenSet_Support[A.cols][A_index];
				// 保存满足可信度的关联规则
				if (confidence >= minConfidence) {
					RuleSet_A.push_back(A);
					RuleSet_B.push_back(BSet[j]);
					RuleSet_confidence.push_back(confidence);
					BSet_Tmp.push_back(BSet[j]);
				}
			}
		}
		// 可以进一步合并BSet
		BSet = BSet_Tmp;
		if (BSet.size() > 1) Apriori_RulesFromConseqence(frozenSet, frozenSet_Support, K, BSet, minConfidence, RuleSet_A, RuleSet_B, RuleSet_confidence);
	}
}
//Main
void Apriori(std::vector<Mat<int>>& dataSet, double minSupport, double minConfidence, std::vector<Mat<int>>& RuleSet_A, std::vector<Mat<int>>& RuleSet_B, std::vector<double>& RuleSet_confidence) {
	std::vector<std::vector<Mat<int>>> frozenSet;
	std::vector<std::vector<double>>frozenSet_Support;
	std::vector<Mat<int>> frozenSet_K;
	std::vector<double> frozenSet_Support_K;
	frozenSet.push_back(frozenSet_K);  frozenSet_Support.push_back(frozenSet_Support_K);	//故意填占 frozenSet[0]位置
	//[2] 初始一个元素的频繁项集，Frozen Set[{ 1 }, { 2 }, { 3 }, { 4 }, { 5 }]
	{
		Mat<int> tmp(1);
		for (int i = 0; i < dataSet.size(); i++) {
			for (int item = 0; item < dataSet[i].cols; item++) {
				bool flag = true;
				for (int j = 0; j < frozenSet_K.size(); j++)
					if (dataSet[i][item] == frozenSet_K[j][0]) { flag = false; break; }
				if (flag) { tmp[0] = dataSet[i][item]; frozenSet_K.push_back(tmp); }
			}
		}
	}
	// [1]
	for (int k = 1; frozenSet_K.size() > 0; k++) {
		//[3]
		Apriori_Filter(dataSet, frozenSet_K, minSupport, frozenSet_Support_K);
		frozenSet.push_back(frozenSet_K);
		frozenSet_Support.push_back(frozenSet_Support_K);
		//[2]
		Apriori_GenCandidate(frozenSet_K, k + 1, frozenSet_K);
	}
	//[4]
	for (int k = 2; k < frozenSet.size(); k++) {
		std::vector<Mat<int>> OneElementSet;
		{
			Mat<int> tmp(1);
			for (int i = 0; i < frozenSet[k].size(); i++) {
				for (int j = 0; j < frozenSet[k][i].cols; j++) {
					bool flag = true;
					for (int j2 = 0; j2 < OneElementSet.size(); j2++)
						if (frozenSet[k][i][j] == OneElementSet[j2][0]) { flag = false; break; }
					if (flag) { tmp[0] = frozenSet[k][i][j]; OneElementSet.push_back(tmp); }
				}
			}
		} 
		Apriori_RulesFromConseqence(frozenSet, frozenSet_Support, k, OneElementSet, minConfidence, RuleSet_A, RuleSet_B, RuleSet_confidence);
	}
}
#endif