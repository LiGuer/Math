#ifndef COMPUTATIONAL_GEOMETRY_H
#define COMPUTATIONAL_GEOMETRY_H

#include <stdlib.h>
#include <float.h>
#include <algorithm>
#include <vector>
#include"../../LiGu_AlgorithmLib/Mat.h"

#define PI 3.141592653589

namespace Geometry {

/*************************************************************************************************
						inTriangle 是否在三角内
*************************************************************************************************/
bool Geometry::inTriangle(Mat<>& p0, Mat<>& TriP1, Mat<>& TriP2, Mat<>& TriP3) {
	Mat<> tmp, edge[2];
	edge[0].sub(TriP2, TriP1);
	edge[1].sub(TriP3, TriP1);
	tmp.    sub(p0,    TriP1);
	double Dot00 = edge[0].dot(edge[0]),
		   Dot01 = edge[0].dot(edge[1]),
		   Dot11 = edge[1].dot(edge[1]),
		   Dot02 = edge[0].dot(tmp),
		   Dot12 = edge[1].dot(tmp);
	double t = Dot00 * Dot11 - Dot01 * Dot01,
		   u =(Dot11 * Dot02 - Dot01 * Dot12) / t,
	       v =(Dot00 * Dot12 - Dot01 * Dot02) / t;
	return (u < 0 || u > 1 || v < 0 || v > 1 || u + v > 1) ? false : true;
}

/*************************************************************************************************
						isInCircle 判断四点共圆
*	[输出]: 圆外-1，圆上0，圆内1
*	三点确定圆方程: 即 解行列式:
		| x1²+y1²  x1  y1  1 | ?= 0
		| x2²+y2²  x2  y2  1 |
		| x3²+y3²  x3  y3  1 |
		| x4²+y4²  x4  y4  1 |
*	[几何解释]: 通过把平面点提升到三维的抛物面中，由于抛物面被平面所截的截面为圆形，四点共面即使共圆，也可以用四面体的体积判断是否共圆。
*************************************************************************************************/
bool Geometry::onCircle(Mat<> Points[]) {
	Mat<> mat(4, 4);
	for (int i = 0; i < 4; i++) {
		mat(i, 0) = Points[i].dot(Points[i]);
		mat(i, 1) = Points[i][0];
		mat(i, 2) = Points[i][1];
		mat(i, 4) = 1;
	}return mat.abs() == 0 ? true : false;
}
/*************************************************************************************************
						ThreePointsToCircle 平面三点确定圆方程
*	[公式]: 圆方程: (x - cx)² + (y - cy)² = R²
*	[算法]: 三点确定圆方程: 即 解行列式:
			| x²+y²    x   y   1 |  =  0
			| x1²+y1²  x1  y1  1 |
			| x2²+y2²  x2  y2  1 |
			| x3²+y3²  x3  y3  1 |
		即.目标三点和圆上(x,y)应该满足方程组:
			(x²+y²)·a + x·b + y·c + 1·d = 0
*	[推导]:
			M11(x²+y²) - M12 x + M13 y - M14 = (x²+y²)·a + x·b + y·c + 1·d = 0
			=> (x² + b/a x) + (y² + c/a y) = - d/a
			=> (x + b/2a)² + (y + c/2a)² = -d/a + b²/4a² + c²/4a²
							CircumCircle 三角形外接圆
*	外接圆圆心: 即. 三点确定圆方程问题， 也是任意两边的垂直平分线的交点.直接用 ThreePointsToCircle()方法
*************************************************************************************************/
Mat<>& Geometry::ThreePoints2Circle(Mat<> Points[], Mat<>& center, double& R) {
	Mat<> mat(4, 4);
	for (int i = 0; i < 3; i++) {
		mat(i + 1, 0) = Points[i].dot(Points[i]);
		mat(i + 1, 1) = Points[i][0];
		mat(i + 1, 2) = Points[i][1];
		mat(i + 1, 3) = 1;
	}
	double 
		a =  mat.comi(0, 0), 
		b = -mat.comi(0, 1), 
		c =  mat.comi(0, 2), 
		d = -mat.comi(0, 3);
	R = sqrt(-d / a + b * b / (4 * a * a) + c * c / (4 * a * a));
	return center.zero(2, 1).set(-b / (2 * a), -c / (2 * a));
}

/*************************************************************************************************
						Delaunay 三角剖分
*	[定义]:
		[1] Delaunay三角剖分: 每个三角形的外接圆内不包含V中任何点
	[流程]:
		[1] 将点按坐标x从小到大排序
		[2] 确定超级三角形
			将超级三角形保存至未确定三角形列表 trianglesTemp
		[3] 遍历每一个点
			[3.1] 初始化边缓存数组 edgeBuffer
			[3.2] 遍历 trianglesTemp 中的每一个三角形
				[3.2.1] 计算该三角形的圆心和半径
				[3.2.2] 如果该点在外接圆的右侧
					则该三角形为Delaunay三角形，保存到triangles,并在temp里去除掉,跳过
				[3.2.3] 如果该点在外接圆外（即也不是外接圆右侧）
					则该三角形为不确定,跳过
				[3.2.4] 如果该点在外接圆内
					则该三角形不为Delaunay三角形,将三边保存至edgeBuffer,在temp中去除掉该三角形
			[3.3] 对edgeBuffer进行去重
			[3.4] 将edgeBuffer中的边与当前的点进行组合成若干三角形并保存至temp triangles中
		[4] 将triangles与trianglesTemp进行合并, 并除去与超级三角形有关的三角形
*	[Referance]:
		[1] http://paulbourke.net/papers/triangulate/
*************************************************************************************************/
Mat<>* Geometry::Delaunay(Mat<> point[], int n, int& TrianglesNum) {
	std::vector<Mat<>> triAns, triTemp, edgeBuffer;
	std::sort(point, point + n, [](Mat<>& a, Mat<>& b) {				// 将点按坐标x从小到大排序
		return a[0] != b[0] ? a[0] < b[0] : a[1] < b[1];
	});
	//[2]
	Mat<> maxPoint(point[0]), 
				minPoint(point[0]);
	for (int i = 1; i < n; i++) {
		maxPoint = (point[i][0] > maxPoint[0] || (point[i][0] == maxPoint[0] && point[i][1] > maxPoint[1])) ? point[i] : maxPoint;
		minPoint = (point[i][0] < minPoint[0] || (point[i][0] == minPoint[0] && point[i][1] < minPoint[1])) ? point[i] : minPoint;
	}
	Mat<> supertriangle(2, 3), length;
	length.sub(maxPoint, minPoint);
	supertriangle(0, 0) = minPoint[0] - length[0] - 2;    supertriangle(1, 0) = minPoint[1] - 2;
	supertriangle(0, 1) = maxPoint[0] + length[0] + 2;    supertriangle(1, 1) = minPoint[1] - 2;
	supertriangle(0, 2) =(maxPoint[0] + minPoint[0]) / 2; supertriangle(1, 2) = maxPoint[1] + length[1] + 2;
	triTemp.push_back(supertriangle);
	//[3]
	for (int i = 0; i < n; i++) {
		edgeBuffer.clear();
		//[3.2]
		for (int j = 0; j < triTemp.size(); j++) {
			//[3.2.1] 
			Mat<> center, triEdge[3], temp;
			for (int k = 0; k < 3; k++)
				triTemp[j].getCol(k, triEdge[k]);
			double R;
			ThreePoints2Circle(triEdge, center, R);
			double distance = (temp.sub(point[i], center)).norm();
			//[3.2.2]
			if (point[i][0] > center[0] + R) {
				triAns.push_back(triTemp[j]);
				triTemp.erase(triTemp.begin() + j--);
			}
			//[3.2.4]
			else if (distance < R) {
				Mat<> edge(2, 2), p1, p2;
				for (int k = 0; k < 3; k++) {
					triTemp[j].getCol(k, p1); 
					triTemp[j].getCol((k + 1) % 3, p2);
					if (p1[0] < p2[0] || (p1[0] == p2[0] && p1[1] < p2[1])) { 
						edge.setCol(0, p1); 
						edge.setCol(1, p2); 
					}
					else { 
						edge.setCol(0, p2);
						edge.setCol(1, p1);
					}
					edgeBuffer.push_back(edge);
				}
				triTemp.erase(triTemp.begin() + j--);
			}
		}
		//[3.3] 
		std::sort(edgeBuffer.begin(), edgeBuffer.end(), [](Mat<> a, Mat<> b) {
			if (a(0, 0) < b(0, 0) || (a(0, 0) == b(0, 0) && a(1, 0) < b(1, 0)))return true;
			if (a(0, 1) < b(0, 1) || (a(0, 1) == b(0, 1) && a(1, 1) < b(1, 1)))return true;
			return false;
		});
		for (int j = 0; j < edgeBuffer.size() - 1; j++) {
			bool flag = 0;
			while (j + 1 < edgeBuffer.size() && edgeBuffer[j] == edgeBuffer[j + 1]) {
				edgeBuffer.erase(edgeBuffer.begin() + j + 1); flag = 1;
			}
			if(flag) { edgeBuffer.erase(edgeBuffer.begin() + j); j--; }
		}
		//[3.4] 
		for (int j = 0; j < edgeBuffer.size(); j++) {
			Mat<> t(2, 3), temp;
			t.setCol(0, edgeBuffer[j].getCol(0, temp)); 
			t.setCol(1, edgeBuffer[j].getCol(1, temp)); 
			t.setCol(2, point[i]);
			triTemp.push_back(t);
		}
	}
	//[4]
	for (int i = 0; i < triTemp.size(); i++) triAns.push_back(triTemp[i]);
	for (int i = 0; i < triAns. size(); i++) {
		Mat<> t;
		for (int j = 0; j < 3; j++) {
			triAns[i].getCol(j, t);
			if (t[0]< minPoint[0] || t[1] < minPoint[1] || t[0] > maxPoint[0] || t[1] > maxPoint[1]) {
				triAns.erase(triAns.begin() + i--); break;
			}
		}
	}
	// [Output]
	TrianglesNum = triAns.size();
	Mat<>* Triangles = (Mat<>*)calloc(TrianglesNum, sizeof(Mat<>));
	for (int i = 0; i < TrianglesNum; i++) Triangles[i] = triAns[i];
	return Triangles;
}

/*************************************************************************************************
						getSphereFibonacciPoint 球面均匀点分布
*	[Referance]:
		[1] Thanks and copyright for https://github.com/SebLague/Boids
*************************************************************************************************/
Mat<>* Geometry::getSphereFibonacciPoint(int n) {
	Mat<>* point = (Mat<>*)malloc(n * sizeof(Mat<>));
	memset(point, 0, n * sizeof(Mat<>));
	double goldenRatio = (1 + sqrt(5)) / 2, angleIncrement = PI * 2 * goldenRatio;	// 黄金分割点
	for (int i = 0; i < n; i++) {
		double t = (double)i / n, inclination = acos(1 - 2 * t), azimuth = angleIncrement * i;
		point[i].zero(3, 1);
		point[i][0] = sin(inclination) * cos(azimuth);
		point[i][1] = sin(inclination) * sin(azimuth);
		point[i][2] = cos(inclination);
	} return point;
}

}
#endif