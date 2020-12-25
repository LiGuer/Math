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

Reference.
[1]Introduction Algorithms.THOMAS H.CORMEN,CHARLES E.LEISERSON,RONALD L.RIVEST,CLIFFORD STEIN
==============================================================================*/
#include "Mat.h"
template<class T>
class Tensor
{
public:
/******************************************************************************
*                    核心数据
******************************************************************************/
	T* data = NULL;	
	Mat<int> dim;										//{x, y, z, ...}
/******************************************************************************
*                    函数
******************************************************************************/
	/*---------------- 构造析构函数 ----------------*/
	Tensor() { ; }
	Tensor(int dimNum, int* dimLength) { zero(dimNum, dimLength); }
	Tensor(int x0, int y0, int z0) { zero(x0, y0, z0); }
	Tensor(Tensor& a) { *this = a; }
	~Tensor() { free(data); }
	/*---------------- 基础函数 ----------------*/
	void clean() { memset(data, 0, sizeof(T) * dim.product()); }											//清零 
	void error() { exit(-1); }
	void eatMat(Tensor& a) {									//吃掉另一个矩阵的数据 (指针操作)
		if (data != NULL)free(data);
		data = a.data; a.data = NULL;
		dim = a.dim; a.dim.zero(1, 1);
	}
	/*---------------- 零元 ----------------*/
	Tensor& zero(int dimNum, int* dimLength) {
		dim.zero(dimNum, 1);
		memcpy(dim.data, dimLength, dimNum * sizeof(int));
		data = (T*)calloc(dim.product(), sizeof(T));
		return *this;
	}
	Tensor& zero(int x0, int y0, int z0) {
		dim.zero(3, 1);
		dim[0] = x0; dim[1] = y0; dim[2] = z0;
		data = (T*)calloc(dim.product(), sizeof(T));
		return *this;
	}
	/*---------------- 随机元 ----------------*/
	Tensor& rands(int dimNum, int* dimLength, T st, T ed) {
		zero(dimNum, dimLength);
		for (int i = 0; i < dim.product(); i++)
			data[i] = rand() / double(RAND_MAX) * (ed - st) + st;	//[st,ed)
		return *this;
	}
	Tensor& rands(int x0, int y0, int z0, T st, T ed) {
		zero(x0, y0, z0);
		for (int i = 0; i < dim.product(); i++)
			data[i] = rand() / double(RAND_MAX) * (ed - st) + st;	//[st,ed)
		return *this;
	}
	/*---------------- "[]"取元素 ----------------
	*	[坐标]: { x, y, z, ...} = data[ x + y·X0 + z·X0·Y0 + ... ]
	*	[Data堆叠方向]: 满x,一列 => 满xy,一矩阵 => 满xyz,一方块 => ....
	**-------------------------------------------*/
	T& operator[](int i) { return data[i]; }
	T& operator()(int i) { return data[i]; }
	T& operator()(int x, int y) { return data[y * dim[0] + x]; }
	T& operator()(int x, int y, int z) {
		return data[z * dim[1] * dim[0] + y * dim[0] + x];
	}
	T& operator()(int* dimIndex) {
		int index = 0, step = 1;
		for (int i = 0; i < dim.rows; i++) {
			index += step * dimIndex[i];
			step *= dim[i];
		}
		return data[index];
	}
	/*----------------赋值 [ = ]----------------*/ //不能赋值自己
	Tensor& operator=(const Tensor& a) {
		if (a.data == NULL)error();
		zero(a.dim.rows, a.dim.data);
		int N = 1;
		for (int i = 0; i < dim.rows; i++)N *= a.dim.data[i];
		memcpy(data, a.data, sizeof(T) * N);
		return *this;
	}
	/*----------------数乘 [ mult × ]----------------*/
	Tensor& mult(const double a, Tensor& b) {
		Tensor ansTemp(b.dim.rows, b.dim.data);
		for (int i = 0; i < b.dim.product(); i++)
			ansTemp.data[i] = a * b.data[i];
		eatMat(ansTemp);
		return *this;
	}
	/*----------------何并 [ merge ]----------------
	*	比 dimIndex 阶数低级的，作为元素块，整体进行内存复制
		比 dimIndex 阶数高级的，作为复制次数
	-----------------------------------------------*/
	Tensor& merge(Tensor* a[], const int N, int dimIndex) {
		// new memory
		int length = 0;
		for (int i = 0; i < N; i++)length += a[i]->dim[dimIndex];
		Mat<int> dimSize = a[0]->dim;
		dimSize[dimIndex] = length;
		Tensor ansTemp(dimSize.rows, dimSize.data);
		// merge
		int elementBlockSize = 1, copyTimes = 1;
		for (int i = 0; i < dimIndex; i++)elementBlockSize *= dim[i];
		for (int i = dimIndex + 1; i < dim.rows; i++)copyTimes *= dim[i];
		int pos = 0;
		Mat<int> inputPosMem(N, 1);
		while (copyTimes--) {
			for (int i = 0; i < N; i++) {
				int t = elementBlockSize * a[i]->dim[dimIndex];
				memcpy((&data + pos), &(a[i]->data) + inputPosMem[i], t * sizeof(T));
				pos += t; inputPosMem[i] += t;
			}
		}
		eatMat(ansTemp);
		return *this;
	}
};