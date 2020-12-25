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
#include "Mat.h"
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
void K_Mean(Mat<double>& x, int K, Mat<double>& Center, Mat<int>& Cluster, Mat<int>& Cluster_Cur) {
	int Dimension = x.rows, N = x.cols;
	Center.zero(Dimension, K);
	Cluster.zero(K, N); Cluster_Cur.zero(K, 1);
	//[1] 随机选择 K 个簇心点 
	for (int i = 0; i < K; i++) {
		int index = rand() % N;
		for (int dim = 0; dim < Dimension; dim++)Center(dim, i) = x(dim, index);
	}
	//[2]
	while (true) {
		//[3]
		Cluster.clean(); Cluster_Cur.clean();
		//[4] 计算每个xi到Center_j的距离
		for (int i = 0; i < N; i++) {
			Mat<double> d(1, K);
			for (int j = 0; j < K; j++)
				for (int dim = 0; dim < Dimension; dim++)
					d[j] += (x(dim, i) - Center(dim, j)) * (x(dim, i) - Center(dim, j));
			//[5]
			int index; d.min(index);
			Cluster(index, Cluster_Cur[index]++) = i;
		}
		//[6] 对每个簇,计算其质心 Center'
		Mat<double> CenterTemp(Dimension, K);
		for (int i = 0; i < K; i++) {
			for (int dim = 0; dim < Dimension; dim++) {
				for (int j = 0; j < Cluster_Cur[i]; j++)
					CenterTemp(dim, i) += x(dim, Cluster(i, j));
				CenterTemp(dim, i) /= Cluster_Cur[i];
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
	x.sum(0, mean);
	mean.mult(1.0 / x.cols, mean);		//得到均值
	Mat<double> x2(x.rows, x.cols);
	for (int i = 0; i < x.rows; i++) {
		for (int j = 0; j < x.cols; j++) {
			x2[i * x.cols + j] = -mean[i];
		}
	}
	x2.add(x, x2);
	//[2] 计算协方差矩阵
	Mat<double> Cov;
	x2.transposi(Cov);			//XT
	Cov.mult(x2, Cov);		//X*XT
	//[3] 对协方差特征值分解
	Mat<double> eigVec, eigValue;
	Cov.eig(1e-5, eigVec, eigValue);
	//[4] 取最大的d'个特征值所对应的特征向量{w1,w2,...,wd'},投影矩阵 W = [w1,w2,...,wd']T
	// sort
	Mat<double> eigValueTemp(1, eigValue.cols), W(eigVec.rows, yDim);
	for (int i = 0; i < eigValue.cols; i++)eigValueTemp[i] = eigValue[i * i];
	std::sort(eigValueTemp.data, eigValueTemp.data + eigValueTemp.cols);
	// W
	for (int i = 0; i < yDim; i++) {
		double value = eigValueTemp[eigValueTemp.cols - 1 - i];
		for (int j = 0; j < eigValue.cols; j++) {
			if (eigValue[j * eigValue.cols + j] == value) {
				for (int k = 0; k < eigVec.rows; k++) {
					W[k * W.cols + i] = eigVec[k * eigVec.cols + j];
				}
				eigValue[j * eigVec.cols + j] = -DBL_MAX;
				break;
			}
		}
	}
	W.transposi(W);
	//[5]
	y.mult(W, x);
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