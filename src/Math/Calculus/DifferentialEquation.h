#ifndef DIFFERENTIAL_EQUATION_H
#define DIFFERENTIAL_EQUATION_H

#include "Calculus.h"

namespace Calculus {
/******************************************************************************
* 
*                    微分方程
*
******************************************************************************/

/* 
 * Runge Kutta 法 : 解常微分方程组 
 */
template<typename F>
void RungeKutta(Mat<>& y, double dx, double x0, F&& f, int enpoch = 1) {
	static Mat<> tmp, k, k1, k2, k3, k4;
	double x = x0;
	while (enpoch--) {
		// k1, k2, k3 ,k4
		k1 = f(x, y);
		k2 = f(x + dx/2, add(tmp, y, mul(tmp, dx/2, k1)));
		k3 = f(x + dx/2, add(tmp, y, mul(tmp, dx/2, k2)));
		k4 = f(x + dx,   add(tmp, y, mul(tmp, dx,   k3)));

		// y[n+1] = y[n] + h/6·(k1 + 2·k2 + 2·k3 + k4)
		add(k,   k1, k4);
		add(tmp, k2, k3);
		add(k, mul(tmp, 2, tmp));
		add(y, y, mul(k, dx / 6, k));
	};
}

/*
 * Poisson's方程 : ▽²φ = f
 */
template<typename F>
double PoissonEquation(Mat<>& x, Mat<>& dx, Mat<>& St, Mat<>& Ed, F&& f) {
	Mat<> tmp; 
	return multIntegral(dx, St, Ed, [&x, &f, &tmp](Mat<>& xt) {
		return x == xt ? 0.0 : f(xt) / tmp.sub(x, xt).norm();
	}) / (4 * PI);
}

/*
 * 波动方程 : a▽²u = ∂²u/∂t²
 */
template<typename F>
inline double WaveEquation (Mat<>& x, Mat<>& dx, double t, double dt, double A, F&& u) {
	return 2 * u(x, t) - u(x, t - 1) + A * dt * dt * LaplaceOperator(x, dx, u);
}

/*
 * 扩散方程 : a▽²u = ∂u/∂t
 */
template<typename F>
inline double DiffusionEquation	(Mat<>& x, Mat<>& dx, double dt, double A, F&& u) {
	return u(x) + A * dt * LaplaceOperator(x, dx, u);
}

}
#endif