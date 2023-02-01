
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


/*#############################################################################

								级数展开

#############################################################################*/
/*
*                    Taylor 展开
*	[定义]: 
		f(x) = f(x0)/0! + f'(x0)/1!·(x-x0) + ... + f^(n)(x0)/n!·(x-x0)
*	[Example]:
		Calculus::TaylorFormula(0, [](double x) { return sin(x); }, Coeff, 10);
*/
template<typename F>
Mat<>& TaylorFormula(double x0, F&& f, Mat<>& Coeff, int N = 3) {
	Coeff.alloc(N + 1);
	for (int i = 0; i <= N; i++) 
		Coeff[i] = diff(x0, f, i) / NumberTheory::Factorial(i);
	return Coeff;
}

/*
*                    常用函数
*	[公式]: (Taylor展开)
		exp(x) = 1 + x + x^2/2! + x^3/3! + ...
		sin(x) = x - x^3/3! + x^5/5! - x^7/7! + ...
		cos(x) = 1 - x^2/2! + x^4/4! - x^6/6! + ...
		ln(1+x)= x - x^2/2  + x^3/3  - x^4/4  + ...	x∈[-1,1]
	  arctan(x)= x - x^3/3  + x^5/5  - x^7/7  + ...	x∈[-1,1]
		(1+x)^p= 1 + px + p(p-1)/2!·x^2 + p(p-1)(p-2)/3!·x^2 + ...	|x| < 1
*/
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
