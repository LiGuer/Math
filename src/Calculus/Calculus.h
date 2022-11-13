#ifndef CALCULUS_H
#define CALCULUS_H
#include "../Matrix/Matrix.h"
#include <functional>
#include <complex>

#define PI 3.141592653589

/******************************************************************************
* 
*                    微积分

******************************************************************************/
namespace Calculus {

/*#############################################################################

								微分

#############################################################################*/

/******************************************************************************
* 
*                    导数  N阶
* 
******************************************************************************/
template<typename F>
double diff(double x0, F&& f, int N = 1, double dx = 1E-3) {
	return N == 0 ? f(x0) :
		(diff(x0 + dx, f, N - 1) - diff(x0 - dx, f, N - 1)) / (2 * dx);
}

template<typename F>
double diff_(double x0, F&& f, int N = 1, double dx = 1E-3) {
	return N == 0 ? f(x0) : 
		(
		- diff_(x0 + 2 * dx, f, N - 1) 
		+ diff_(x0 +	 dx, f, N - 1) * 8
		- diff_(x0 -	 dx, f, N - 1) * 8
		+ diff_(x0 - 2 * dx, f, N - 1)
		) / (12 * dx);
}

/******************************************************************************
* 
*                    曲率 : |y''| / (1 + y'²)^{3/2}
* 
******************************************************************************/
template<typename F>
double Curvature(double x0, F&& y, double dx = 1E-3) {
	return fabs(diff(x0, y, 2)) / pow(1 + pow(diff(x0, y), 2), 1.5);
}

/******************************************************************************
* 
*                    偏导数
* 
******************************************************************************/
template<typename F>
double PartiDeriv(Mat<>& x, int dim, double dx, F&& f) {
	x[dim] += dx;	double t1 = f(x);	x[dim] -= dx;
	x[dim] -= dx;	double t2 = f(x);	x[dim] += dx;
	return (t1 - t2) / (2 * dx);
}

template<typename F>
double PartiDeriv2(Mat<>& x, int dim, double dx, F&& f) {
	double ans = 0;
	x[dim] += dx; ans += f(x); x[dim] -= dx;
	x[dim] -= dx; ans += f(x); x[dim] += dx;
				  ans -= f(x) * 2;
	return ans / (dx * dx);
}

/******************************************************************************
*                    Hamilton 算子、Laplace 算子
*		Hamilton: ▽  ≡ ∂/∂x \vec x + ∂/∂y \vec y + ∂/∂z \vec z + ...
*		Laplace:  ▽² ≡ ∂²/∂x² + ∂²/∂y² + ∂²/∂z² + ...
******************************************************************************/
template<typename F>
inline double LaplaceOperator(Mat<>& x, Mat<>& dx, F&& f) {
	double ans = 0;
	for (int dim = 0; dim < dx.size(); dim++) 
		ans += PartiDeriv2(x, dim, dx[dim], f);
	return ans;
}

/******************************************************************************
*                    梯度、散度、旋度
*	[定义]:
		梯度: ▽f		: 矢量, 函数在该点处变化率最大的方向.
		散度: ▽·\vec f: 标量, 矢量场在该点发散的程度, 表征场的有源性(>0源,<0汇,=0无源)
		旋度: ▽×\vec f: 矢量, 矢量场在该点旋转的程度, 
							方向是旋转度最大的环量的旋转轴, 旋转的方向满足右手定则,
							大小是绕该旋转轴旋转的环量与旋转路径围成的面元面积之比.
*	[公式]: (直角坐标系)
		▽f        = ∂f/∂x \vec x + ∂f/∂y \vec y + ∂f/∂z \vec z + ...
		▽·\vec f =  ∂fx/∂x + ∂fy/∂y + ∂fz/∂z + ...
		▽×\vec f = (∂fz/∂y - ∂fy/∂z) \vec x
				   + (∂fx/∂z - ∂fz/∂x) \vec y
				   + (∂fy/∂x - ∂fx/∂y) \vec z
******************************************************************************/
template<typename F>
Mat<>& Grad(Mat<>& x, Mat<>& dx, F&& f, Mat<>& ans) {
	ans.alloc(x.rows);
	for (int dim = 0; dim < x.size(); dim++) 
		ans[dim] = PartiDeriv(x, dim, dx[dim], f);
	return ans;
}

template<typename FX, typename FY, typename FZ>
double Div(Mat<>& x, Mat<>& dx, FX&& fx, FY&& fy, FZ&& fz) {
	double ans = 0;
	ans += PartiDeriv(x, 0, dx[0], fx);
	ans += PartiDeriv(x, 1, dx[1], fy);
	ans += PartiDeriv(x, 2, dx[2], fz);
	return ans;
}

template<typename FX, typename FY, typename FZ>
Mat<>& Curl(Mat<>& x, Mat<>& dx, FX&& f0, FY&& f1, FZ&& f2, Mat<>& ans) {
	ans.alloc(x.rows);
	ans[0] = PartiDeriv(x, 1, dx[1], f2) - PartiDeriv(x, 2, dx[2], f1);
	ans[1] = PartiDeriv(x, 2, dx[2], f0) - PartiDeriv(x, 0, dx[0], f2);
	ans[2] = PartiDeriv(x, 0, dx[0], f1) - PartiDeriv(x, 1, dx[1], f0);
	return ans;
}

/*#############################################################################

								级数展开

#############################################################################*/
/******************************************************************************
*                    Taylor 展开
*	[定义]: 
		f(x) = f(x0)/0! + f'(x0)/1!·(x-x0) + ... + f^(n)(x0)/n!·(x-x0)
*	[Example]:
		Calculus::TaylorFormula(0, [](double x) { return sin(x); }, Coeff, 10);
******************************************************************************/
template<typename F>
Mat<>& TaylorFormula(double x0, F&& f, Mat<>& Coeff, int N = 3) {
	Coeff.alloc(N + 1);
	for (int i = 0; i <= N; i++) 
		Coeff[i] = diff(x0, f, i) / NumberTheory::Factorial(i);
	return Coeff;
}

/******************************************************************************
*                    常用函数
*	[公式]: (Taylor展开)
		exp(x) = 1 + x + x^2/2! + x^3/3! + ...
		sin(x) = x - x^3/3! + x^5/5! - x^7/7! + ...
		cos(x) = 1 - x^2/2! + x^4/4! - x^6/6! + ...
		ln(1+x)= x - x^2/2  + x^3/3  - x^4/4  + ...	x∈[-1,1]
	  arctan(x)= x - x^3/3  + x^5/5  - x^7/7  + ...	x∈[-1,1]
		(1+x)^p= 1 + px + p(p-1)/2!·x^2 + p(p-1)(p-2)/3!·x^2 + ...	|x| < 1
******************************************************************************/
double Exp(double x, int N = 18) {
	double ans = 0;
	for (int i = 0; i <= N; i++) 
		ans += pow(x, i) / NumberTheory::Factorial(i);
	return ans;
}

double Sin(double x, int N = 18) {
	double ans = 0;
	for (int i = 1; i <= N; i += 2) 
		ans += (i % 4 == 1 ? 1 : -1) * pow(x, i) / NumberTheory::Factorial(i);
	return ans;
}

double Cos(double x, int N = 18) {
	double ans = 0;
	for (int i = 0; i <= N; i += 2) 
		ans += (i % 4 == 0 ? 1 : -1) * pow(x, i) / NumberTheory::Factorial(i);
	return ans;
}

double lnOneAdd(double x, int N = 18) {
	double ans = 0;
	for (int i = 1; i <= N; i++) 
		ans += (i % 2 == 1 ? 1 : -1) * pow(x, i) / i;
	return ans;
}

double Arctan(double x, int N = 18) {
	double ans = 0;
	for (int i = 1; i <= N; i += 2) 
		ans += (i % 4 == 1 ? 1 : -1) * pow(x, i) / i;
	return ans;
}

double PowOneAdd(double x, double p, int N = 18) {
	double ans = 0, pTmp = 1;
	for (int i = 0; i <= N; i++) { 
		ans += pTmp * pow(x, i) / NumberTheory::Factorial(i);
		pTmp *= (p - i); 
	}
	return ans;
}

/******************************************************************************
*					Fast Fourier Transform 快速Fourier变换
*	[定义]: 离散Fourier变换的高效算法
*	[公式]:
		离散Fourier变换: X[k] = Σ_(n=0)^(N-1)  e^(-j2πnk/N)·x[n]
*	[时间复杂度]: O(N·logN)
*	[算法]:
		N-Point Model:
				___________
		x[0] —| N/2-Point |—> E[0] —×××> X[0]  (E[0],+O[0])
		x[2] —|   DFT	   |—> E[1] —×××> X[1]  (E[1],+O[1])
		x[4] —|		   |—> E[2] —×××> X[2]  (E[2],+O[2])
		x[6] —|		   |—> E[3] —×××> X[3]  (E[3],+O[3])
                ___________
		x[1] —| N/2-Point |—> O[0] —×××> X[4]  (E[0],-O[0])
		x[3] —|   DFT	   |—> O[1] —×××> X[5]  (E[1],-O[1])
		x[5] —|		   |—> O[2] —×××> X[6]  (E[2],-O[2])
		x[7] —|		   |—> O[3] —×××> X[7]  (E[3],-O[3])

		(E[k],±O[k]) = E[k] ± W_N^K·O[k]
*	[Reference]:
		Thanks for https://www.math.wustl.edu/~victor/mfmm/fourier/fft.c
-------------------------------------------------------------------------------
*	[Example]:
		#define N 256
		std::complex<double> x[N], X[N], tmp[N];
		double f1 = 2, f2 = 10, Ts = 1.0/100;			//信号频率、采样周期
		for (int i = 0; i < N; i++)
			x[i] = sin((double)i * Ts * 2 * PI * f1)
				 + cos((double)i * Ts * 2 * PI * f2);
		memcpy(X, x, sizeof(complex<double>) * N);
		Calculus::FFT(X, N, tmp);
****************************************************************************** /
void FFT(std::complex<double>* x, int n) {
	if (n <= 1) return;
	std::complex<double> z, w, 
		*xe = tmp,								//FFT 偶数序列 E[] 
		*xo = tmp + n / 2; 						//FFT 奇数序列 O[] 		
	for (int k = 0; k < n / 2; k++) {
		xe[k] = x[2 * k];
		xo[k] = x[2 * k + 1];
	}
	FFT(xe, n / 2, x);							//N/2 DFT
	FFT(xo, n / 2, x);
	for (int m = 0; m < n / 2; m++) {
		w =     cos(2 * PI * m / (double)n)		//W_N^K
		+ 1i * -sin(2 * PI * m / (double)n);
		z			 = w * xo[m];				//W_N^K·O[k]
		x[m]		 = xe[m] + z;
		x[m + n / 2] = xe[m] - z;
	}
}
//逆Fourier变换
void iFFT(std::complex<double>* X, int n){
	if (n <= 1) return;
	std::complex<double> z, w,
		*Xe = tmp,
		*Xo = tmp + n / 2;
	for (int k = 0; k < n / 2; k++) {
		Xe[k] = X[2 * k];
		Xo[k] = X[2 * k + 1];
	}
	iFFT(Xe, n / 2, X);							//FFT 偶数序列 X[]
	iFFT(Xo, n / 2, X);							//FFT 奇数序列 X[]
	for (int m = 0; m < n / 2; m++) {
		w =     cos(2 * PI * m / (double)n)		//W_N^K
		+ 1i * -sin(2 * PI * m / (double)n);
		z			 = w * Xo[m];
		X[m]		 = Xe[m] + z;
		X[m + n / 2] = Xe[m] - z;
	}
}*/
/*#############################################################################

								积分

#############################################################################*/
/******************************************************************************
*                    积分
*	[定义]:
*	[算法]: NewtonCotes 公式
		∫_a^b f(x) = (b - a) Σ_(k=0)^n  C_k^(n) f(xi)
		C_k^(n) = (-1)^(n-k) / (n·k!(n-k)!) ∫_0^n Π_(k≠j) (t-j)dt 
		n = 1: C = {1/2, 1/2}
		n = 2: C = {1/6, 4/6, 1/6}
		n = 4: C = {7/90, 32/90, 12/90, 32/90, 7/90}
		* NewtonCotes 公式在 n > 8 时不具有稳定性
		复合求积法: 将积分区间分成若干个子区间, 再在每个子区间使用低阶求积公式.
******************************************************************************/
template<typename F>
double integral_NewtonCotes(double xSt, double xEd, F&& f, int n = 4) {
	double ans = 0, dx = (xSt - xEd) / n, xi = xSt,
		C[] = { 7 / 90.0, 32 / 90.0, 12 / 90.0, 32 / 90.0, 7 / 90.0 };
	for (int i = 0; i <= n; i++, xi += dx) ans += C[i] * f(xi);
	return ans *= (xEd - xSt);
}
template<typename F>
double integral(double xSt, double xEd, F&& f, int n) {
	double ans = 0, dx = (xSt - xEd) / n, xi = xSt;
	for (int i = 0; i < n; i++, xi += dx) ans += integral_NewtonCotes(xi, xi + dx, f);
	return ans;
}
/******************************************************************************
*                    重积分
*	[定义]: ∫∫∫ f(r) dr³
******************************************************************************/
template<typename F>
double multIntegral(Mat<>& dx, Mat<>&St, Mat<>& Ed, F&& f) {
	Mat<> x = St;
	double ans = 0, dx_n = dx.product();
	while (true) {
		int dim = 0; x[dim] += dx[dim];
		while (dim <  dx.size() - 1 && x[dim] > Ed[dim]) { x[dim] = St[dim]; dim++; x[dim] += dx[dim]; }
		if    (x[dx.size() - 1] > Ed[dx.size() - 1]) break;
		ans += f(x) * dx_n;
	}
	return ans;
}

/*#############################################################################

								插值拟合

#############################################################################*/
/******************************************************************************
*					Lagrange插值
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
*					样条插值
*	[算法]: 通过求解三弯矩方程组得出曲线函数组的过程
******************************************************************************/
void CubicSpline(double x, double y) {

}
/******************************************************************************
*					二分法
******************************************************************************/
template<typename F>
double BisectionMethod(double st, double ed, F&& f) {
	double mid = (st + ed) / 2;
	if (f(st) * f(ed) > 0 || f(st) * f(mid) == 0 || ed - st < 1E-9) return mid;
	return f(mid) * f(ed) > 0 ? BisectionMethod(st, mid, f) : BisectionMethod(mid, ed, f);
}
}
#endif