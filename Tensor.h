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
#include "LiGu_GRAPHICS/Mat.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
template<class T>
class Tensor
{
public:
/******************************************************************************
*                    核心数据
******************************************************************************/
	T* data = NULL;	
	Mat<int> dimension;										//{x, y, z, ...}
/******************************************************************************
*                    函数
******************************************************************************/
	/*---------------- 构造析构函数 ----------------*/
	Tensor() { ; }
	Tensor(int dimNum, int* dimLength) { zero(dimNum, dimLength); }
	Tensor(Tensor& a) { *this = a; }
	~Tensor() { free(data); }
	/*---------------- 基础函数 ----------------*/
	void clean() { 											//清零 
		int N = 1;
		for (int i = 0; i < dimension.rows; i++)N *= dimension[i];
		memset(data, 0, sizeof(T) * N);
	}
	void error() { exit(-1); }
	void eatMat(Tensor& a) {									//吃掉另一个矩阵的数据 (指针操作)
		if (data != NULL)free(data);
		data = a.data; a.data = NULL;
		dimension = a.dimension; a.dimension.zero(1, 1);
	}
	/*---------------- 零元 ----------------*/
	Tensor& zero(int dimNum, int* dimLength) {
		dimension.zero(dimNum, 1);
		int N = 1;
		for (int i = 0; i < dimNum; i++) {
			dimension[i] = dimLength[i];
			N *= dimLength[i];
		}
		data = (T*)malloc(N * sizeof(T));
		memset(data, 0, N * sizeof(T));
		return *this;
	}
	/*---------------- "[]"取元素 ----------------
	*	[坐标]: { x, y, z, ...} = data[ x + y·X0 + z·X0·Y0 + ... ]
	*	[Data堆叠方向]: 满x,一列 => 满xy,一矩阵 => 满xyz,一方块 => ....
	**-------------------------------------------*/
	T& operator[](int i) { return data[i]; }
	T& operator()(int* dimIndex) {
		int index = 0, step = 1;
		for (int i = 0; i < dimension.rows; i++) {
			index += step * dimIndex[i];
			step *= dimension[i];
		}
		return data[index];
	}
	/*----------------赋值 [ = ]----------------*/ //不能赋值自己
	Tensor& operator=(const Tensor& a) {
		if (a.data == NULL)error();
		zero(a.dimension.rows, a.dimension.data);
		int N = 1;
		for (int i = 0; i < dimension.rows; i++)N *= a.dimension.data[i];
		memcpy(data, a.data, sizeof(T) * N);
		return *this;
	}
};