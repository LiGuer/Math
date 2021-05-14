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
#include "BasicMachineLearning.h"
/******************************************************************************
*								Apriori
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
*******************************************************************************/
//[2] 生成K项所有可以组合的集合.
void Apriori_GenCandidate(std::vector<Mat<int>>& frozenSet, int K, std::vector<Mat<int>>& newfrozenSet) {
	std::vector<Mat<int>> newfrozenSet_Tmp;
	Mat<int> tmp(1, K);
	for (int i = 0; i < frozenSet.size(); i++) {
		for (int j = i + 1; j < frozenSet.size(); j++) {
			bool flag = true;
			for (int k = 0; k < K - 2; k++) {
				tmp[k] = frozenSet[i][k];
				if (frozenSet[i][k] != frozenSet[j][k]) { flag = false; break; }
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
				if (flag2) newfrozenSet_Tmp.push_back(tmp);
			}
		}
	}
	newfrozenSet = newfrozenSet_Tmp;
}
//[3] 过滤小于支持度的集合.
void Apriori_Filter(std::vector<Mat<int>>& dataSet, std::vector<Mat<int>>& frozenSet, double minSupport, std::vector<double>& frozenSet_Support) {
	//[3.1]计算支持度P(AB) ≌ num(AB) / num(all)
	frozenSet_Support.clear();
	Mat<> frozenSet_Count(1, frozenSet.size());
	for (int i = 0; i < frozenSet.size(); i++) {
		for (int j = 0; j < dataSet.size(); j++) {
			// if frozenSet[i] is dataSet[j]'s subset
			int cur = 0;
			for (int k = 0; k < dataSet[j].cols; k++)
				if (frozenSet[i][cur] == dataSet[j][k]) cur++;
			if (cur == frozenSet[i].cols) frozenSet_Count[i]++;
		}
	}
	frozenSet_Count.mul(1.0 / dataSet.size(), frozenSet_Count);
	//[3.2]删除小于支持度的集合.
	std::vector<Mat<int>> frozenSet_Tmp;
	for (int i = 0; i < frozenSet.size(); i++) {
		if (frozenSet_Count[i] > minSupport) {
			frozenSet_Tmp.    push_back(frozenSet[i]);
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
		if (BSet.size() > 1) Apriori_RulesFromConseqence(
			frozenSet, 
			frozenSet_Support, 
			K, BSet, minConfidence, 
			RuleSet_A, 
			RuleSet_B, 
			RuleSet_confidence
		);
	}
}
//Main
void BasicMachineLearning::Apriori(std::vector<Mat<int>>& dataSet, double minSupport, double minConfidence, std::vector<Mat<int>>& RuleSet_A, std::vector<Mat<int>>& RuleSet_B, std::vector<double>& RuleSet_confidence) {
	std::vector<std::vector<Mat<int>>> frozenSet;
	std::vector<std::vector<double>>   frozenSet_Support;
	std::vector<Mat<int>> frozenSet_K;
	std::vector<double>   frozenSet_Support_K;
	frozenSet.push_back(frozenSet_K); 
	frozenSet_Support.push_back(frozenSet_Support_K);	//故意填占 frozenSet[0]位置
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
			Mat<int> tmp(1,1);
			for (int i = 0; i < frozenSet[k].size(); i++) {
				for (int j = 0; j < frozenSet[k][i].cols; j++) {
					bool flag = true;
					for (int j2 = 0; j2 < OneElementSet.size(); j2++)
						if (frozenSet[k][i][j] == OneElementSet[j2][0]) { flag = false; break; }
					if (flag) { tmp[0] = frozenSet[k][i][j]; OneElementSet.push_back(tmp); }
				}
			}
		}
		Apriori_RulesFromConseqence(
			frozenSet, 
			frozenSet_Support, 
			k, OneElementSet, minConfidence, 
			RuleSet_A, 
			RuleSet_B, 
			RuleSet_confidence
		);
	}
}
/******************************************************************************
*								K Mean 聚类
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
*******************************************************************************/
void BasicMachineLearning::K_Mean(Mat<>& x, int K, Mat<>& Center, Mat<int>& Cluster, Mat<int>& ClusterKthNum, int TimeMax) {
	int Dimension = x.rows, N = x.cols;
	Center. zero(Dimension, K);
	Cluster.zero(K, N); 
	ClusterKthNum.zero(K);
	//[1] 随机选择 K 个簇心点 
	for (int i = 0; i < K; i++) {
		int index = rand() % N;
		for (int dim = 0; dim < Dimension; dim++) Center(dim, i) = x(dim, index);
	}
	//[2]
	int times = 0;
	while (times++ < TimeMax) {
		//[3]
		Cluster.      zero(); 
		ClusterKthNum.zero();
		//[4] 计算每个xi到Center_j的距离
		for (int i = 0; i < N; i++) {
			Mat<> d(1, K);
			for (int j = 0; j < K; j++)
				for (int dim = 0; dim < Dimension; dim++)
					d[j] += pow(x(dim, i) - Center(dim, j), 2);
			//[5]
			int index; d.min(index);
			Cluster(index, ClusterKthNum[index]++) = i;
		}
		//[6] 对每个簇,计算其质心 Center'
		Mat<> CenterTemp(Dimension, K);
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
/******************************************************************************
*				Mahalanobis Distance
* Mahalanobis Distance
*******************************************************************************/
void BasicMachineLearning::MahalanobisDist(Mat<>& x, Mat<>& mahalanobisDistance) {
	mahalanobisDistance.alloc(1, x.cols);
	// mean, diff, cov
	Mat<> mean, diff, covMat, tmp;
	mean.  mul(1.0 / x.cols, x.sum(mean, 1));
	covMat.mul(x, x.transpose(covMat));
	// mahalanobis distance
	covMat.inv(covMat);
	Mat<> xt;
	for (int i = 0; i < x.cols; i++) {
		x.getCol(i, xt);
		diff.sub(xt, mean);
		tmp.mul(tmp.mul(diff.transpose(tmp), covMat), diff);
		mahalanobisDistance[i] = tmp[0];
	}
}
/******************************************************************************
*				Principal Components Analysis 主成分分析
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
*******************************************************************************/
void BasicMachineLearning::PrincipalComponentsAnalysis(Mat<>& x, Mat<>& y, int yDim) {
	//[1] 数据中心化
	Mat<> mean;
	mean.mul(1.0 / x.cols, x.sum(mean, 1));		//得到均值
	Mat<> x2(x);
	for (int i = 0; i < x.rows; i++)
		for (int j = 0; j < x.cols; j++)
			x2(i, j) -= mean[i];
	//[2] 计算协方差矩阵
	Mat<> Cov;
	Cov.mul(x2, x2.transpose(Cov));		//X*XT
	//[3] 对协方差特征值分解
	Mat<> eigVec, eigValue;
	Cov.eig(1e-5, eigVec, eigValue);
	//[4] 取最大的d'个特征值所对应的特征向量{w1,w2,...,wd'},投影矩阵 W = [w1,w2,...,wd']T
	// sort
	Mat<> eigValueTemp(1, eigValue.cols), W(eigVec.rows, yDim);
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
	y.mul(W.transpose(y), x);
}