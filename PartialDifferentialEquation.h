#include "Mat.h"
/*----------------[ 波动方程 ]----------------
*	[方程]: a ▽²u = ∂²a/∂t²
			其中 ▽² ≡ ∂²/∂x² + ∂²/∂y² + ∂²/∂z² + ...
		边界条件:
			[1] u(r,t = 0) = f(r)		//初位置
			[2] ∂u(r,t=0)/∂t = g(r)		//初速度
			[3]	u(a,t) = b				//边界值
	[算法]: 有限差分法
		u(t+1,...) = 2·u(t,...) - u(t-1,...)
				+ Δt²·a{[u(x+1,...) - 2·u(x,...) + u(x-1,...)]/Δx² + ...}
	[推导]:
		∂²a/∂t² = { [u(t+1,rt) - u(t,rt)]/Δt - [u(t,rt) - u(t-1,rt)]/Δt } / Δt
				= [u(t+1,rt) - 2·u(t,rt) + u(t-1,rt)] / Δt²
		其他同理, 代入波动方程,
		a { [u(x+1,...) - 2·u(x,...) + u(x-1,...)] / Δx² + ...}
		= [u(t+1,...) - 2·u(t,...) + u(t-1,...)] / Δt²
		所以, 下一时刻有
		u(t+1,...) = 2·u(t,...) - u(t-1,...)
						+ Δt²·a{[u(x+1,...) - 2·u(x,...) + u(x-1,...)]/Δx² + ...}
		对于t = 1 * Δt时刻, 有
		u(r,t=Δt) = u(r,t=0) + ∂u(r,t=0)/∂t·Δt + ▽²u·Δt²
	[***] 该程序无法被使用，仅作为演示参考,只二维
**--------------------------------------*/
void WaveEquation(Mat<double>& Map, Mat<double>& veloc, void (*setBoundaryEquations) (Mat<double>& x),
	double alpha, double deltaTime, double deltaX, double deltaY, int EndTimes) {
	Mat<double> MapNow(Map), MapPrev(Map);
	//对于t = 1 * Δt时刻, 有 u(r,t=Δt) = u(r,t=0) + ∂u(r,t=0)/∂t·Δt + ▽²u·Δt²
	for (int x = 0; x < Map.rows; x++) {
		for (int y = 0; y < Map.cols; y++) {
			MapNow(x, y) += veloc(x, y) * deltaTime + alpha * deltaTime * deltaTime * (
				(MapNow(x + 1, y) - 2 * MapNow(x, y) + MapNow(x - 1, y)) / (deltaX * deltaX)
				+ (MapNow(x, y + 1) - 2 * MapNow(x, y) + MapNow(x, y - 1)) / (deltaY * deltaY)
				);
		}
	}
	while (--EndTimes) {//u(t+1,...) = 2·u(t,...) - u(t-1,...)+Δt²·a{ [u(x + 1,...) - 2·u(x,...) + u(x - 1,...)] / Δx² + ... }
		for (int x = 0; x < Map.rows; x++) {
			for (int y = 0; y < Map.cols; y++) {
				MapPrev(x, y) =
					2 * MapNow(x, y) - MapPrev(x, y) + alpha * deltaTime * deltaTime * (
					(MapNow(x + 1, y) - 2 * MapNow(x, y) + MapNow(x - 1, y)) / (deltaX * deltaX)
						+ (MapNow(x, y + 1) - 2 * MapNow(x, y) + MapNow(x, y - 1)) / (deltaY * deltaY)
						);
			}
		}
		MapPrev.swap(MapNow);
		setBoundaryEquations(MapNow);
	}
}