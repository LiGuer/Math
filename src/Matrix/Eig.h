#ifndef MATRIX_EIG_H
#define MATRIX_EIG_H
#include "Mat.h"
#include "Init.h"
#include "BasicOperate.h"

namespace Matrix {
	/******************************************************************************
	*
	*                    特征值/特征向量
	*
	******************************************************************************/
	inline void eig(Mat<>& a, Mat<>& eigvec, Mat<>& eigvalue, double esp = 1E-4) {
		if (a.rows != a.cols)
			return;

		//[1] init
		int n = a.rows;
		eigvalue = a;
		E(eigvec.zero(n, n));

		Mat<> R, Rt;

		//[2] begin iteration
		while (true) {
			//[3] Calculate row p and col q
			int p, q;
			double maxelement = eigvalue[1];

			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++) {
					if (i != j && fabs(eigvalue(i, j)) >= maxelement) {
						maxelement = fabs(eigvalue(i, j));
						p = i;
						q = j;
					}
				}
			}
			if (maxelement < esp)
				return;			// [2]

			//[4] eigvalue eigvec
			double theta = 0.5 * atan2(
				2 * eigvalue(p, q),
				eigvalue(q, q) - eigvalue(p, p)
			);

			double c = cos(theta),
				   s = sin(theta);					// c,s
			E(R.zero(n, n));
			R(p, p) = c;
			R(p, q) = s;		// R
			R(q, p) = -s;
			R(q, q) = c;

			transpose(Rt, R);
			mul(eigvalue, Rt, eigvalue);			// Dj = Rjdouble Dj-1 Rj
			mul(eigvalue, eigvalue, R);
			mul(eigvec, eigvec, R);					// X = R Y
		}
	}
}

#endif