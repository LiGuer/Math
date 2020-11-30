#include <math.h>
#include <complex.h>
#include "Mat.h"
/*--------------------------------[ Mandelbrot 曼德布洛特集 ]--------------------------------
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
/*--------------------------------[ Julia 茱莉亚集 ]--------------------------------
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