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
#include <math.h>
#include <complex.h>
#include "Mat.h"
/*--------------------------------[ Mandelbrot集 ]--------------------------------
*	[定义]: Zn+1 = Zn² + C
			所有能使Zn+1不发散的复数点C, 所构成的集合,即 Mandelbrot Set
			(不发散,不一定收敛,有可能在几个不同点来回跳)
*	[性质]: |Zn|>2不可能收敛, 即Mandelbrot Set在半径为2的圆内.
**--------------------------------------------------------------------------------------------*/
void Mandelbrot(double complex min, double complex max, int resSize, int iterateTimes,
	Mat<double complex> Set) {
	Set.zero(resSize, resSize);
	double deltaReal = (creal(max) - creal(min)) / resSize;
	double deltaImag = (cimag(max) - cimag(min)) / resSize;
	for (int i = 0; i < resSize; i++) {
		for (int j = 0; j < resSize; j++) {
			double real = creal(min) + deltaReal * i, imag = cimag(min) + deltaImag * j;
			double complex C = real + imag * I;
			Set(i, j) = isMandelbrotSet(C, 0, iterateTimes);
		}
	}
}
/*----------------[ 是否属于MandelbrotSet/JuliaSet ]----------------*/
int isMandelbrotSet(double complex C, double complex Z0, int iterateTimes) {
	double complex z = Z0;
	for (int epoch = 0; epoch < iterateTimes; epoch++) {	// 迭代
		if (cabs(z) > 2) return epoch;						// |Zn|>2不可能收敛
		z = z * z + C;										// Zn+1 = Zn² + C   
	}
	return 0;							//属于,输出0; 不属于,输出判断出的当次迭代数
}
/*--------------------------------[ Julia集 ]--------------------------------
*	[定义]: Zn+1 = Zn² + C
			对于某复数值C,所有能使Zn+1不发散的Z0的集合,即 Julia Set
			类似于. Mandelbrot 曼德布洛特集
**--------------------------------------------------------------------------------*/
void Julia(double complex C, double complex min, double complex max, int resSize, int iterateTimes,
	Mat<double complex> Set) {
	Set.zero(res, res);
	double deltaReal = (creal(max) - creal(min)) / resSize;
	double deltaImag = (cimag(max) - cimag(min)) / resSize;
	for (int i = 0; i < resSize; i++) {
		for (int j = 0; j < resSize; j++) {
			double real = creal(min) + deltaReal * i, imag = cimag(min) + deltaImag * j;
			double complex Z0 = real + imag * I;
			Set(i, j) = isMandelbrotSet(C, Z0, iterateTimes);
		}
	}
}
/*--------------------------------[ Hilbert Curve 希尔伯特曲线 ]--------------------------------
*	[定义]: 一种自相似的分形曲线
*	[生成方法]: 四象限复制四分，翻转左下、右下, 使左下末同左上初、右下初同右上末,能够最短连接.
*	[性质]:
		* 边长: nth 2^n
		* 长度: nth 2^n - 1 / 2^n
		* 因为是四进制自相似, 所以曲线上位置 distance, 可以不断判断子象限，按二进制在位上叠加
*	[用途]: 1D to 2D 的映射算法, 随 n 增加, 映射的位置趋于收敛
*	[1,2阶曲线]: "┌┐"	   "┌┐┌┐"    "┌┐┌┐ "
				  ︱︱  =>	︱︱︱︱ =>   ︱└┘︱
							┌┐┌┐      └┐┌┘
							︱︱︱︱       -┘└-
*	[程序]: rotation : 翻转坐标, 使Hilbert曲线性质, 自相似地适用于左下、右下象限
		[1] xy2d(): 坐标 -> 曲线位置    [2] d2xy(): 曲线位置 -> 坐标
*	[Author]: 1891.Hilbert
*	[Reference]: [1] Wikipedia.Hilbert curve
**--------------------------------------------------------------------------------------------*/
int HilbertCurve_xy2d(int n, int x, int y) {	//convert (x,y) to d
	int distance = 0;
	for (int s = n / 2; s > 0; s /= 2) {
		int rx = (x & s) > 0, ry = (y & s) > 0;
		distance += s * s * ((0b11 * rx) ^ ry);
		// rotation
		if (ry == 0) {
			if (rx == 1) { x = n - 1 - x; y = n - 1 - y; }
			int t = x; x = y; y = t;
		}
	}return distance;
}
void HilbertCurve_d2xy(int n, int distance, int& x, int& y) {//convert d to (x,y)
	x = y = 0;
	for (int s = 1; s < n; s *= 2) {
		int rx = 1 & (distance / 2), ry = 1 & (distance ^ rx);
		// rotation
		if (ry == 0) {
			if (rx == 1) { x = n - 1 - x; y = n - 1 - y; }
			int t = x; x = y; y = t;
		}
		x += s * rx; y += s * ry;
		distance /= 4;
	}
}