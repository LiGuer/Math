#ifndef GRAPH_SHORTESTPATH_H
#define GRAPH_SHORTESTPATH_H

#include <algorithm>
#include <vector>
#include "../Matrix/Mat.h"

using namespace std;

/****************************
 *
 *  Shortest path of graph
 *
 ****************************/
namespace GraphTheory {
/*
 *  Dijkstra's algorithm
 */
template <class Graph>
vector<double>& Dijkstra(Graph& G, int st, vector<double>& dis) {
	int n = G.vertex_num;
	vector<bool> used(n);

	dis.resize(n, DBL_MAX);
	dis[st] = 0;

	for (int i = 0; i < n; i++) {
		int v = -1;

		for (int j = 0; j < n; j++) 
			if (!used[v] && (v != -1 || dis[j] < dis[v])) 
				v = j;

		used[v] = true;

		for (int j = 0; j < n; j++) 
			dis[j] = min(dis[j], dis[v] + G(v, j));
	}
	
	return dis;
}

/*
 *  Floyd's algorithm
 */
void Floyd(Mat<double>& G, Mat<double>& dis, Mat<int>& path) {
	// initial
	int n = G.rows;
	dis = G;
	path.zero(n, n);

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			path(i, j) = j;

	// run
	for (int k = 0; k < n; k++)
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				dis(i, j) = dis(i, j) > dis(i, k) + dis(k, j) ?
				path(i, j) = k, dis(i, k) + dis(k, j) :
				dis(i, j);
}

vector<int>& Floyd_searchPath(int st, int ed, Mat<int>& pathMat, vector<int>& shortestPath) {
	
	std::function<void(int, int, Mat<int>&, vector<int>&)> search =
		[&](int st, int ed, Mat<int>& pathMat, vector<int>& shortestPath) {

		if (pathMat(st, ed) == st ||
			pathMat(st, ed) == ed)
			return;

		search(st, pathMat(st, ed), pathMat, shortestPath);
		shortestPath.push_back(pathMat(st, ed));
		search(pathMat(st, ed), ed, pathMat, shortestPath);
	};

	shortestPath.push_back(st);
	search(st, ed, pathMat, shortestPath);
	shortestPath.push_back(ed);

	return shortestPath;
}

}
#endif