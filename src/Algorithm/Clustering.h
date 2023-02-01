#ifndef CLUSTERING_H
#define CLUSTERING_H

#include <vector>
#include "../../../LiGu_Codes/LiGu_Math/src/Math/Matrix/Matrix.h"

using namespace Matrix;

/*#############################################################################

						聚类

##############################################################################*/

namespace Clustering {

	/******************************************************************************
	[K-Mean 聚类]
		[目标]:
			聚类. 对N维分布的数据点，可以将其聚类在 K 个关键簇内.
		[步骤]:
			(1) 随机选择 K 个簇心点 Center
			(2) 迭代开始
				(3) 归零 Cluster , Cluster: 簇,记录ith簇内的数据指针。
				(4) 计算每个xi到簇心μj的距离
					(5) 选择距离最小的簇心, 将该点加入其簇内
				(6) 对每个簇,计算其质心 Center'
				(7) Center≠Center' , 则更正Center为 Center'
				(8) 迭代重新开始
			(9) 一轮无更正时，迭代结束
	*******************************************************************************/
	/*
	 * K-Means
	 */
	Mat<>& K_Means(
		Mat<>& X, int K, 
		Mat<>& Center, std::vector<int>* Cluster, 
		int TimeMax = 0x7FFFFFFF, double eps = 1e-4
	) {
		int Dim = X.rows, N = X.cols;
		Mat<> CenterTmp(Dim, K);
		Center.zero(Dim, K);

		//[1] 随机选择 K 个簇心点 
		for (int i = 0; i < K; i++) {
			int index = rand() % N;
			for (int dim = 0; dim < Dim; dim++)
				Center(dim, i) = X(dim, index);
		}
		 
		//[2]
		for (int times = 0; times < TimeMax; times++) {
			//[3]
			for (int k = 0; k < K; k++)
				Cluster[k].clear();
			CenterTmp.zero(Dim, K);

			//[4] 计算每个xi到Center_j的距离
			for (int i = 0; i < N; i++) {
				Mat<> d(1, K);
				for (int k = 0; k < K; k++)
					for (int dim = 0; dim < Dim; dim++)
						d[k] += pow(X(dim, i) - Center(dim, k), 2);
				//[5]
				int index;
				min(d, index);
				Cluster[index].push_back(i);

				//[6] 计算簇心
				for (int dim = 0; dim < Dim; dim++)
					CenterTmp(dim, index) += X(dim, i);
			}

			//[6] 计算簇心
			for (int k = 0; k < K; k++)
				for (int dim = 0; dim < Dim; dim++)
					CenterTmp(dim, k) /= Cluster[k].size();

			//[7] 更新簇心
			bool equal = 1;
			for (int i = 0; i < Center.size(); i++) {
				if (abs(Center(i) - CenterTmp(i)) > eps) {
					equal = 0; break;
				}
			}
			if (equal)
				return Center;

			Center.swap(CenterTmp);
		}
		return Center;
	}


}

#endif