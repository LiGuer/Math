#ifndef INTERSECT_H
#define INTERSECT_H

#include <float.h>
#include <algorithm>
#include <functional>
#include <complex>
#include "../Matrix/Matrix.h"
#include "../Tensor/Tensor.h"
#include "../Function/SolvePolynomialEquation.h"

using namespace std;
using namespace Matrix;

#define PI 3.141592653589
#define EPS 10e-4

/*#############################################################################
* 
*						Intersect
* 
##############################################################################*/
namespace Intersect {

// Line
bool Segments(vector<double>& p1, vector<double>& p2, vector<double>& p3, vector<double>& p4);

// Plane
double RayPlane		(vector<double>& raySt, vector<double>& ray, vector<double>& a, double b);	
double RayPlane		(vector<double>& raySt, vector<double>& ray, double& A, double& B, double& C, double& D);
double RayCircle	(vector<double>& raySt, vector<double>& ray, vector<double>& center, double& R, vector<double>& normal);
double RayTriangle	(vector<double>& raySt, vector<double>& ray, vector<double>& p1, vector<double>& p2, vector<double>& p3);
double RayPolygon	(vector<double>& raySt, vector<double>& ray, vector<double>* p,  int n);
double RayPlaneShape(vector<double>& raySt, vector<double>& ray, vector<double>& center, vector<double>& normal, vector<double>& one, bool(*f)(double, double));

// Surface of Body
double RayQuadric   (vector<double>& raySt, vector<double>& ray, vector<double>& center, vector<double>& PInv);
double RaySphere	(vector<double>& raySt, vector<double>& ray, vector<double>& center, double& R);
double RaySphere	(vector<double>& raySt, vector<double>& ray, vector<double>& center, double& R, bool(*f)(double, double));
double RayCuboid	(vector<double>& raySt, vector<double>& ray, vector<double>& p1, vector<double>& p2, vector<double>& p3);
double RayCuboid	(vector<double>& raySt, vector<double>& ray, vector<double>& pmin, vector<double>& pmax);
double RayTorus		(vector<double>& raySt, vector<double>& ray, vector<double>& center, double R, double r);
}
#endif