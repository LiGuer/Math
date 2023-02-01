#ifndef CALCULUS_H
#define CALCULUS_H

#include <functional>
#include <complex>
#include <vector>

using namespace std;

namespace Calculus {

/*####################################

				微分

####################################*/

/* 
 *   导数
 */
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

/*
 *  偏导数
 */
template<typename F>
double diff(vector<double>& x, int dim, double dx, F&& f) {
	x[dim] += dx;
	double t1 = f(x);	
	x[dim] -= dx;

	x[dim] -= dx;
	double t2 = f(x);
	x[dim] += dx;

	return (t1 - t2) / (2 * dx);
}

template<typename F>
double diff_2(vector<double>& x, int dim, double dx, F&& f) {
	double ans = 0;

	x[dim] += dx; 
	ans += f(x); 
	x[dim] -= dx;

	x[dim] -= dx; 
	ans += f(x); 
	x[dim] += dx;
				  
	ans -= f(x) * 2;
	return ans / (dx * dx);
}

/*
 *  曲率 : |y''| / (1 + y'²)^{3/2}
 */
template<typename F>
double Curvature(double x0, F&& y, double dx = 1E-3) {
	return fabs(diff(x0, y, 2)) / pow(1 + pow(diff(x0, y), 2), 1.5);
}

/*
*  Hamilton 算子 ▽、Laplace 算子 ▽²
*  Hamilton: ▽  ≡ ∂/∂x \vec x + ∂/∂y \vec y + ∂/∂z \vec z + ...
*  Laplace:  ▽² ≡ ∂²/∂x² + ∂²/∂y² + ∂²/∂z² + ...
*/
template<typename F>
inline double LaplaceOperator(vector<double>& x, vector<double>& dx, F&& f) {
	double ans = 0;

	for (int dim = 0; dim < dx.size(); dim++) 
		ans += PartiDeriv2(x, dim, dx[dim], f);

	return ans;
}

/*
 *  Gradient ▽f , Divergence ▽·f , Curl ▽×f
 */
template<typename F>
vector<double>& Grad(vector<double>& x, vector<double>& dx, F&& f, vector<double>& ans) {
	if(ans.size() != x.size())
		ans.zeros(x.rows);

	for (int dim = 0; dim < x.size(); dim++) 
		ans[dim] = PartiDeriv(x, dim, dx[dim], f);

	return ans;
}

template<typename F>
double Div(vector<double>& x, vector<double>& dx, vector<F>& f) {
	double ans = 0;
	ans += PartiDeriv(x, 0, dx[0], f[0]);
	ans += PartiDeriv(x, 1, dx[1], f[1]);
	ans += PartiDeriv(x, 2, dx[2], f[2]);
	return ans;
}

template<typename FX, typename FY, typename FZ>
vector<double>& Curl(vector<double>& x, vector<double>& dx, FX&& f0, FY&& f1, FZ&& f2, vector<double>& ans) {
	ans.alloc(x.rows);
	ans[0] = PartiDeriv(x, 1, dx[1], f2) - PartiDeriv(x, 2, dx[2], f1);
	ans[1] = PartiDeriv(x, 2, dx[2], f0) - PartiDeriv(x, 0, dx[0], f2);
	ans[2] = PartiDeriv(x, 0, dx[0], f1) - PartiDeriv(x, 1, dx[1], f0);
	return ans;
}

/*####################################

				积分

####################################*/

/*
 *  积分
 *  NewtonCotes 公式
 */
template<typename F>
double integral_NewtonCotes(double xSt, double xEd, F&& f, int n = 4) {
	double ans = 0, dx = (xSt - xEd) / n, xi = xSt,
		C[] = { 7 / 90.0, 32 / 90.0, 12 / 90.0, 32 / 90.0, 7 / 90.0 };
	
	for (int i = 0; i <= n; i++, xi += dx) 
		ans += C[i] * f(xi);

	return ans *= (xEd - xSt);
}

template<typename F>
double integral(double xSt, double xEd, F&& f, int n) {
	double ans = 0, dx = (xSt - xEd) / n, xi = xSt;

	for (int i = 0; i < n; i++, xi += dx) 
		ans += integral_NewtonCotes(xi, xi + dx, f);

	return ans;
}

/*
 *  重积分
 */
template<typename F>
double multIntegral(vector<double>& dx, vector<double>&St, vector<double>& Ed, F&& f) {
	vector<double> x = St;
	double ans = 0, dx_n = dx.product();

	while (true) {
		int dim = 0; 
		x[dim] += dx[dim];

		while (dim <  dx.size() - 1 && x[dim] > Ed[dim]) { 
			x[dim] = St[dim]; 
			dim++; 
			x[dim] += dx[dim]; 
		}

		if (x[dx.size() - 1] > Ed[dx.size() - 1]) 
			break;

		ans += f(x) * dx_n;
	}

	return ans;
}

}
#endif