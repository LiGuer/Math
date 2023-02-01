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
#include "../../LiGu_AlgorithmLib/Mat.h"
#include <float.h>
#include <algorithm>
#include <vector>
namespace BasicMachineLearning {
/*#############################################################################

						降维

##############################################################################*/
/******************************************************************************
*		主成分分析
*******************************************************************************/
void PCA(Mat<>& x, Mat<>& y, int yDim) {
	//[1]
	Mat<> mean;
	mean.mul(1.0 / x.cols, x.sum(mean, 1));		//得到均值
	Mat<> x2(x);
	for (int i = 0; i < x.rows; i++)
		for (int j = 0; j < x.cols; j++)
			x2(i, j) -= mean[i];
	//[2]
	Mat<> Cov;
	Cov.mul(x2, x2.transpose(Cov));		//X*XT
	//[3] 
	Mat<> eigVec, eigValue;
	Cov.eig(1e-5, eigVec, eigValue);
	//[4] // sort
	Mat<> eigValueTemp(1, eigValue.cols), W(eigVec.rows, yDim);

	for (int i = 0; i < eigValue.cols; i++)
		eigValueTemp[i] = eigValue(i, i);

	std::sort(eigValueTemp.data, eigValueTemp.data + eigValueTemp.cols);
	// W
	for (int i = 0; i < yDim; i++) {
		double value = eigValueTemp[eigValueTemp.cols - 1 - i];

		for (int j = 0; j < eigValue.cols; j++) {
			if (eigValue(j, j) == value) {
				for (int k = 0; k < eigVec.rows; k++) 
					W(k, i) = eigVec(k, j);
				eigValue(j, j) = -DBL_MAX;
				break;
			}
		}
	}
	//[5]
	y.mul(W.transpose(y), x);
}

/*################################################################################

						回归
	[目标]:
		* 分类，是离散的回归问题.
#################################################################################*/
/******************************************************************************

*		最小二乘法

******************************************************************************/
void OrdinaryLeastSquares(Mat<>& x, Mat<>& y, double& w, double& b) {
	//w 
	double w1 = 0, w2 = 0, xbar = x.mean();
	for (int i = 0; i < x.size(); i++) {
		w1 += y[i] * (x[i] - xbar);
		w2 += x[i] * x[i];
	}
	w = w1 / (w2 - pow(x.sum(), 2) / x.size());
	//b 
	b = 0;
	for (int i = 0; i < x.size(); i++) {
		b += y[i] - w * x[i];
	}
	b /= x.size();
}

/******************************************************************************
* 
*		支持向量机
* 
******************************************************************************/
int SupportVectorMachines_SMO(Mat<>& X, int* y, int i, double& b, Mat<>& lamb, double& C, double& toler, Mat<>& kernelMat) {
	//1.j
	int j; 
	while ((j = rand() % X.cols) == i);

	//2. error
	double err_i = b - y[i], err_j = b - y[j];
	for (int k = 0; k < lamb.size(); k++) {
		err_i += lamb[k] * y[k] * kernelMat(i, k);
		err_j += lamb[k] * y[k] * kernelMat(j, k);
	}

	//2.к_ii+к_jj-2к_ij, L, H
	double	lamb_i_old = lamb[i],
		lamb_j_old = lamb[j],
		K = kernelMat(i, i) + kernelMat(j, j) - 2 * kernelMat(i, j),
		L = y[i] != y[j] ? std::max(0.0, lamb[j] - lamb[i]) : std::max(0.0, lamb[j] + lamb[i] - C),
		H = y[i] != y[j] ? std::min(C, lamb[j] - lamb[i] + C) : std::min(C, lamb[j] + lamb[i]);
	
	if (K <= 0 || L == H) 
		return 0;

	//3.λ_j, λ_i
	lamb[j] += y[j] * (err_i - err_j) / K;
	lamb[j] = lamb[j] < H ? (lamb[j] > L ? lamb[j] : L) : H;
	lamb[i] += y[i] * y[j] * (lamb_j_old - lamb[j]);
	if (abs(lamb_j_old - lamb[j]) < 1e-5) 
		return 0;

	//4.b
	double bi = -y[i] * (lamb[i] - lamb_i_old) * kernelMat(i, i)
		- y[j] * (lamb[j] - lamb_j_old) * kernelMat(i, j) - err_i + b,
		bj = -y[i] * (lamb[i] - lamb_i_old) * kernelMat(i, j)
		- y[j] * (lamb[j] - lamb_j_old) * kernelMat(j, j) - err_j + b;
	b = (lamb[i] > 0 && lamb[i] < C) ? bi : ((lamb[j] > 0 && lamb[j] < C) ? bj : (bi + bj) / 2);
	return 1;
}

double SupportVectorMachines(Mat<>& X, int* y, Mat<>& w, Mat<>& lamb, double C, double toler, double(*kernel)(Mat<>& a, Mat<>& b), int maxIter = 100) {
	int N = X.cols;

	// kernel matrix
	Mat<> kernelMat(N, N), tmp1, tmp2;
	for (int i = 0; i < N; i++) {
		X.getCol(i, tmp1);
		for (int j = 0; j < N; j++) {
			kernelMat(i, j) = kernel(tmp1, X.getCol(j, tmp2));
		}
	}

	// λ*
	lamb.zero(N);
	double b;
	for (int iter = 0; iter < maxIter; ) {
		int pair_changed = 0;
		for (int i = 0; i < N; i++)
			pair_changed += SupportVectorMachines_SMO(X, y, i, b, lamb, C, toler, kernelMat);
		if (pair_changed == 0) 
			iter++;
		else iter = 0;
	}

	// w*,b*
	w.zero(X.rows);
	for (int i = 0; i < N; i++) {
		for (int dim = 0; dim < X.rows; dim++) {
			w[dim] += X(dim, i) * lamb[i] * y[i];
		}
	}
	return b;
};

/*################################################################################

						插值

#################################################################################*/
/******************************************************************************
[Lagrange插值]
	[原理]:
		f(x) = Σ_(i=1)^n  y_i · f_i(x)
		f_i(x) = Π_(j=1,i≠j)^n  (x - x_j) / (x_i - x_j)
		第N点y = 基函数1×第1点y + 基函数2×第2点y + 基函数3×第3点y
		基函数状态2 = (输入X-第1点x)(输入X-第3点x) / (第2点x-第1点x)(第2点x-第3点x)
******************************************************************************/
double LagrangeInterpolation(double x, double* x0, double* y0, int n) {
	double y = 0, t;
	for (int i = 0; i < n; i++) {
		t = y0[i];
		for (int j = 0; j < n; j++)
			if (i != j)
				t *= (x - x0[j]) / (x0[i] - x0[j]);
		y += t;
	}
	return y;
}

/******************************************************************************
[样条插值]
	[算法]: 过求解三弯矩方程组得出曲线函数组的过程
******************************************************************************/
void CubicSpline(double x, double y) {

}

/*********************************************************************************
[反距离加权插值]: Inverse Distance Weighted
	[目的]:
		空间插值
	[原理]:
		插值点与样本点间的距离为权重进行加权平均，离插值点越近的样本点赋予权重越大。
		f(x) = Σ w_i(x) f(x_i)
		w_i(x) = (1/d_i(x)^α) / (Σ 1/d_i(x)^α)
		d_i(x) = ||x - x_i||₂
*********************************************************************************/
double InvDisWeight(Mat<>& data, double x, double y, int alpha = 2) {
	double z = 0, d0 = 0;
	for (int i = 0; i < data.cols; i++) {
		d0 += pow((pow(x - data(0, i), 2) + pow(y - data(1, i), 2)), -alpha);
	}
	for (int i = 0; i < data.cols; i++) {
		double d = pow((pow(x - data(0, i), 2) + pow(y - data(1, i), 2)), -alpha);
		z += (d / d0) * data(2, i);
	}
	return z;
}
/*********************************************************************************
[普通Kriging插值]: Ordinary Kriging
	[目的]:
		空间插值. 满足假设:
		(1) 空间属性z是均一的. 对空间任意一点，都有相同期望、方差.
	[优化问题]:
		min		var = 2Σ w_i γ_i0 - ΣΣ w_i w_j γ_ij - γ_00
		s.t.	Σ w_i = 1
				γ_ij = σ² - C_ij = E[(Z(x_i) - Z(x_j))²]
	[步骤]:
		(1) 确定半方差函数γ(xi,xj) = E[(Z(x_i) - Z(x_j))²]
			确定半方差函数与两点间距离的函数关系.
		(2) 计算权值
			权值计算方程:
			[w_1]		[ γ(x1,x1)	...	γ(x1,xn)	 1	]^-1	[γ(x1,x*)]
			|...|  =	|	...		...		...		...	|		|	...	  |
			|w_n|		| γ(xn,x1)	...	γ(xn,xn)	 1	|		|γ(x1,x*)|
			[μ ]		[	 1		...		 1		 0	]		[	 1	  ]
		(3) 计算插值点结果
			f(x) = Σ w_i(x) f(x_i)
	[原理]:
		(1) f(x) = Σ w_i(x) f(x_i)
			z~: 估计值	z : 实际值
			求解权重系数: 使其为满足插值点处，估计值与真实值差最小的一组系数.
		(2) 对空间任意一点，都有相同期望、方差. 即:
			E	(z(x,y)) = μ
			Var	(z(x,y)) = σ²
			即. z(x,y) = μ + R(x,y)    Var(R(x,y)) = σ²
		(3) 约束方程 —— 无偏估计条件 E(z~ - z) = 0
			=> E(z~ - z) = E(Σ w_i z_i - z) = μΣ w_i - μ = 0
			=> Σ w_i = 1
		(4)	目标函数 —— 估计误差 var = Var(z~ - z)
			=> var = Var(Σ w_i z_i - z) = ΣΣ w_i w_j Cov(zi,zj) - 2Σ w_i Cov(zi,z) + cov(z,z)
			=> var = ΣΣ w_i w_j C_ij - 2Σ w_i C_i0 + cov_00		(C_ij = Cov(zi - μ,z - μ))
		(5) 定义 半方差函数γ_ij = σ² - C_ij
			=> var = 2Σ w_i γ_i0 - ΣΣ w_i w_j γ_ij - γ_00
		(6) 凸优化问题构建完成:
				min		var = 2Σ w_i γ_i0 - ΣΣ w_i w_j γ_ij - γ_00
				s.t.	Σ w_i = 1
			Lagrange函数	L(w_i, λ) = var + λ(Σ w_i - 1)
			Lagrange对偶	G(λ) = inf L(w_i, λ)
			L(W,λ)求导, 当导数为0时, 取得极值
			即. 得到权值计算方程.
		(7) 半方差函数γ_ij = σ² - C_ij = E[(Z(x_i) - Z(x_j))²]
			∵地理学第一定律: 空间上相近的属性相近.
			∴γ_ij与两点间距离，存在函数关系
			将所有d和γ绘成散点图，寻找最优曲线拟合d与γ，得到函数关系式.
*********************************************************************************/
double KrigingOrdinary(Mat<>& data, double x, double y, double(*variogram)(double x1, double y1, double x2, double y2)) {
	double z = 0;
	int N = data.cols;
	Mat<> weight(N + 1), R0(N + 1, N + 1), R(N + 1);
	//R0 R
	R0 = 1; R0(N, N) = 0;
	R(N) = 1;
	for (int i = 0; i < N; i++) {
		R(i) = variogram(data(0, i), data(1, i), x, y);
		for (int j = 0; j < N; j++) {
			R0(i, j) = variogram(data(0, i), data(1, i), data(0, j), data(1, j));
		}
	}
	//lambda z
	weight.mul(R0.inv(R0), R);
	for (int i = 0; i < data.cols; i++) {
		z += weight[i] * data(2, i);
	}
	z += weight[N];
	return z;
}

/******************************************************************************
*				Mahalanobis Distance
* Mahalanobis Distance
*******************************************************************************/
void MahalanobisDist(Mat<>& x, Mat<>& mahalanobisDistance) {
	mahalanobisDistance.alloc(x.size());
	// mean, diff, cov
	Mat<> mean, diff, covMat, tmp;
	mean.mul(1.0 / x.size(), x.sum(mean, 1));
	covMat.mul(x, x.transpose(covMat));
	// mahalanobis distance
	covMat.inv(covMat);
	Mat<> xt;
	for (int i = 0; i < x.size(); i++) {
		diff.sub(x.getCol(i, xt), mean);
		tmp.mul(tmp.mul(diff.transpose(tmp), covMat), diff);
		mahalanobisDistance[i] = tmp[0];
	}
}
double MahalanobisDist(Mat<>& x1, Mat<>& x2, Mat<>& covMat) {
	double ans;
	Mat<> diff, _covMat, tmp;
	diff.sub(x1, x2);
	covMat.inv(_covMat);
	return ((diff.transpose(tmp) *= _covMat) *= diff)[0];
}
}
#endif