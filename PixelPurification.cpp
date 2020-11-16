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
/*
【K聚类 · 像素纯化】
*[Idea]
[1] 对于色彩复杂的照片，每个RGB像素就是[R,G,B]三维分布的数据点，可以将其聚类为K个关键簇，簇内像素均用其簇心像素替换，从而实现任意照片的像素纯化。
[2] 本质上是[K-mean]聚类算法，在图像处理中的运用。
*/

#include <stdlib.h>
#include <string.h>
#include "LiGu_GRAPHICS/Mat.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include "LiGu_GRAPHICS/Graphics.h"
#pragma comment(lib,"opencv2-include/opencv_world430.lib")
/*--------------------------------[ K聚类 ]--------------------------------
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
void K_Mean(Mat<double>& x, int K, int TimesMax, Mat<double>& Center, Mat<int>& Cluster, Mat<int>& Cluster_Cur) {
	int Dimension = x.rows, N = x.cols;
	Center.zero(Dimension, K);
	Cluster.zero(K, N); Cluster_Cur.zero(K, 1);
	//[1] 随机选择 K 个簇心点 
	for (int i = 0; i < K; i++) {
		int index = rand() % N;
		for (int dim = 0; dim < Dimension; dim++)Center(dim, i) = x(dim, index);
	}
	//[2]
	int Times = 0;
	while (true) {
		printf("%d\n", Times);
		if (Times++ > TimesMax)return;
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
int ArgPos(char* str, int argc, char** argv) {
	for (int a = 1; a < argc; a++) if (!strcmp(str, argv[a])) {
		if (a == argc - 1) {printf("Argument missing for %s\n", str); exit(-1);}
		return a;
	}
	return -1;
}
#include <time.h>
int main(int argc, char** argv){
	char InputImgUrl[1000], OutputUrl[1000];
	int K = 5, TimesMax = 100;
	if (argc == 1) {
		printf("K-Mean__Pixel-Purification__@LiGu\n\n");
		printf("Options:\n");
		printf("\t-InputImgUrl <file>\n");
		printf("\t-OutputUrl <file>\n");
		printf("\t-TimesMax <int>\n");
		printf("\t-K <int>\n");
		return 0;
	}
	int i = 0;
	if ((i = ArgPos((char*)"-InputImgUrl", argc, argv)) > 0) strcpy(InputImgUrl, argv[i + 1]);
	if ((i = ArgPos((char*)"-OutputUrl", argc, argv)) > 0) strcpy(OutputUrl, argv[i + 1]);
	if ((i = ArgPos((char*)"-K", argc, argv)) > 0) K = atoi(argv[i + 1]);
	if ((i = ArgPos((char*)"-TimesMax", argc, argv)) > 0) TimesMax = atoi(argv[i + 1]);

	time_t t;
	srand((unsigned)time(&t));

	cv::Mat src;
	src = cv::imread(InputImgUrl, cv::IMREAD_COLOR);
	int cols = src.cols, rows = src.rows;
	printf("%d %d %d\n", cols, rows, src.channels());
	Mat<double> data(3, rows * cols), Center;
	Mat<int> Cluster, Cluster_Cur;


	int cur = 0;
	cv::MatIterator_<cv::Vec3b> it, end;
	for (it = src.begin<cv::Vec3b>(), end = src.end<cv::Vec3b>(); it != end; it++) {
		data(0, cur) = (double)(*it)[0];
		data(1, cur) = (double)(*it)[1];
		data(2, cur) = (double)(*it)[2];
		cur++;
	}
	K_Mean(data, K, TimesMax, Center, Cluster, Cluster_Cur);
	for (int i = 0; i < K; i++) {
		printf("%f %f %f\n", Center(0, i), Center(1, i), Center(2, i));
	}
	for (int i = 0; i < K; i++) {
		for (int j = 0; j < Cluster_Cur[i]; j++) {
			for (int dim = 0; dim < 3; dim++) {
				data(dim, Cluster(i, j)) = Center(dim, i);
			}
		}
	}

	Graphics g(cols, rows);
	for (int i = 0; i < rows * cols; i++) {
		g.PaintColor = (RGB)data(2, i) * 0x10000 + (RGB)data(1, i) * 0x100 + (RGB)data(0, i);
		g.drawPoint(i % cols, i / cols);
	}
	g.PicWrite(OutputUrl);
}