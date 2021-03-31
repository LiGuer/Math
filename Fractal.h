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
#ifndef FRACTAL
#define FRACTAL
#include <math.h>
#include <complex.h>
#include <vector>
#include "Mat.h"
namespace Fractal {
#define PI 3.141692653589
	/*--------------------------------[ Mandelbrot集 ]--------------------------------
	*	[定义]: Zn+1 = Zn² + C
				所有能使Zn+1不发散的复数点C, 所构成的集合,即 Mandelbrot Set
				(不发散,不一定收敛,有可能在几个不同点来回跳)
	*	[性质]: |Zn|>2不可能收敛, 即Mandelbrot Set在半径为2的圆内.
	**--------------------------------------------------------------------------------------------
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
	}*/
	/*----------------[ 是否属于MandelbrotSet/JuliaSet ]----------------
	int isMandelbrotSet(double complex C, double complex Z0, int iterateTimes) {
		double complex z = Z0;
		for (int epoch = 0; epoch < iterateTimes; epoch++) {	// 迭代
			if (cabs(z) > 2) return epoch;						// |Zn|>2不可能收敛
			z = z * z + C;										// Zn+1 = Zn² + C   
		}
		return 0;							//属于,输出0; 不属于,输出判断出的当次迭代数
	}*/
	/*--------------------------------[ Julia集 ]--------------------------------
	*	[定义]: Zn+1 = Zn² + C
				对于某复数值C,所有能使Zn+1不发散的Z0的集合,即 Julia Set
				类似于. Mandelbrot 曼德布洛特集
	**--------------------------------------------------------------------------------
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
	}*/
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
	/*--------------------------------[ Perlin Noise ]--------------------------------
	Function to linearly interpolate between a0 and a1 , Weight w should be in the range [0.0, 1.0]
	--------------------------------------------------------------------------------*/
	double PerlinNoise(double x, double y, Mat<double>& randomGridGradient) {
		// 对四个格点
		int x0[] = { x, x + 1, x, x + 1 }, y0[] = { y, y, y + 1, y + 1 };
		double n[4];
		for (int i = 0; i < 4; i++) {
			//[1] 格点随机梯度矢量
			double random = randomGridGradient(x0[i], y0[i]);
			//[2] (x,y)与格点距离,梯度点积
			double dx = x - x0[i], dy = y - y0[i];
			n[i] = dx * cos(random) + dy * sin(random);
		}
		//[3] 插值
		double sx = x - (int)x, sy = y - (int)y;
		double ix0 = (n[1] - n[0]) * (3.0 - sx * 2.0) * sx * sx + n[0];
		double ix1 = (n[3] - n[2]) * (3.0 - sx * 2.0) * sx * sx + n[2];
		return (ix1 - ix0) * (3.0 - sy * 2.0) * sy * sy + ix0;
	}
	Mat<double>& PerlinNoise(Mat<double>& output, int frequency) {
		Mat<double> randomGridGradient;
		randomGridGradient.rands(frequency + 1, frequency + 1, 0, 256);
		for (int y = 0; y < output.rows; y++)
			for (int x = 0; x < output.cols; x++)
				output(x, y) = PerlinNoise((double)x / output.cols * frequency, (double)y / output.rows * frequency, randomGridGradient);
		return output;
	}
	/*--------------------------------[ 平移 ]--------------------------------
	[x'] = [ 1  0  0  dx] [x]
	|y'|   | 0  1  0  dy| |y|
	|z'|   | 0  0  1  dz| |z|
	[1 ]   [ 0  0  0  1 ] [1]
	**-----------------------------------------------------------------------*/
	void translation(Mat<double>& delta, Mat<double>& transMat) {
		int n = delta.rows;
		Mat<double> translationMat(n + 1);
		for (int i = 0; i < n; i++)translationMat(i, n) = delta[i];
		transMat.mult(translationMat, transMat);
	}
	/*--------------------------------[ 三维旋转·四元数 ]--------------------------------
	*	[公式]: v' = q v q`¹
			q = [cos(θ/2), u s in(θ/2)]
			v=>[0,v]经旋转轴u旋转Ѳ角后得到v'
		多次旋转:
			v' = q1q2 v q2`¹q1`¹ = (q1q2) v (q1q2)`¹
		四元数化旋转矩阵:
			四元数左乘:
			q v =	[a -b -c -d] v
					|b  a -d  c|
					|c  d  a -b|
					[d -c  b  a]
			四元数右乘:
			v q =	[a -b -c -d] v
					|b  a  d -c|
					|c -d  a  b|
					[d  c -b  a]
	**--------------------------------------------------------------------------*/
	void rotate(Mat<double>& rotateAxis, double theta, Mat<double>& center, Mat<double>& transMat) {
		Mat<double> tmp;
		translation(center.negative(tmp), transMat);
		// rotate
		tmp.mult(sin(theta / 2), rotateAxis.normalization());
		Mat<double> q(4, 1), rotateMat(4);
		{ double t[] = { cos(theta / 2), tmp[0], tmp[1], tmp[2] }; q.getData(t); }
		for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) rotateMat(i, j) = q[((j % 2 == 0 ? 1 : -1) * i + j + 4) % 4];
		for (int i = 1; i < 4; i++) rotateMat(i, i % 3 + 1) = -rotateMat(i, i % 3 + 1);
		Mat<double> rotateMatR(rotateMat);
		for (int i = 1; i < 4; i++) { rotateMat(0, i) *= -1; rotateMatR(i, 0) *= -1; }
		rotateMat.mult(rotateMat, rotateMatR);
		tmp = rotateMat; rotateMat.E(4);
		for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) rotateMat(i, j) = tmp(i + 1, j + 1);
		transMat.mult(rotateMat, transMat);
		translation(center, transMat);
	}
	/*--------------------------------[ 三维分形树 Fractal Tree 3D ]--------------------------------
	----------------------------------------------------------------------------------------------*/
	void FractalTree3D(std::vector<Mat<double>>& linesSt, std::vector<Mat<double>>& linesEd, int level, double alpha, int fork = 3) {
		if (level <= 0)return;
		// 确定旋转矩阵
		Mat<double> st = linesSt.back(), ed = linesEd.back(), direction, rotateAxis, rotateMat(4), zAxis(3, 1), tmp; {double t[] = { 0, 0, 1 }; zAxis.getData(t); }
		direction.add(ed, st.negative(direction));
		if (direction[0] != 0 || direction[1] != 0) {
			rotate(
				rotateAxis.crossProduct(direction, zAxis),
				-acos(tmp.dot(direction, zAxis) / direction.norm()),
				tmp.zero(3, 1), rotateMat
			); rotateMat.cut(0, 2, 0, 2, rotateMat);
		}
		else rotateMat.E(3);
		//递归
		double Lenth = direction.norm();
		Mat<double> endPoint(3, 1);
		for (int i = 0; i < fork; i++) {
			{double t[] = { sin(alpha) * cos((double)i * 2 * PI / fork), sin(alpha) * sin((double)i * 2 * PI / fork), cos(alpha) }; endPoint.getData(t); }
			endPoint.add(ed, endPoint.mult(0.7 * Lenth, endPoint.mult(rotateMat, endPoint)));
			linesSt.push_back(ed);
			linesEd.push_back(endPoint);
			FractalTree3D(linesSt, linesEd, level - 1, alpha);
		}
	}
}
#endif