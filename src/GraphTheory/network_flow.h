#ifndef GRAPH_NETWORKFLOW_H
#define GRAPH_NETWORKFLOW_H

#include <algorithm>
#include <vector>
#include "../Matrix/Mat.h"

using namespace std;

/****************************
 *
 *  networks flow
 *
 ****************************/
namespace GraphTheory {
	
/*
 *  Dinic's algorithm
 */
void Dinic_BFS(Mat<>& GraphMat, int s, int e, int * level) {
	int N = GraphMat.rows;
		
	level[s] = 1;
	queue<int> Q;
	Q.push(s);

	while (!Q.empty()) {
		int tmp = Q.front();
		Q.pop();

		for (int i = 0; i < N; i++) {
			if (level[i] == 0 && GraphMat(tmp, i) > 0) {
				Q.push(i);
				level[i] = level[tmp] + 1;
			}
		}
	}

	if (level[e] == 0)
		return 0;
	return 1;
}

int Dinic_DFS(Mat<>& GraphMat, int u, int e, int dis, int* level) {
	int N = GraphMat.rows;
		
	if (u == e) 
		return dis;

	for (int i = 0; i < N; i++) {
		if (level[i] == level[u] + 1 && GraphMat(u, i) > 0) {
			int di = Dinic_DFS(GraphMat, i, e, fmin(dis, GraphMat(u, i)), level);

			if (di > 0) {
				GraphMat(u, i) -= di;
				GraphMat(i, u) += di;
				return di;
			}
		}
	}
	return 0;
}

double Dinic(Mat<>& GraphMat, Mat<>& Path, int s, int e) {
	int ans = 0,
		N = GraphMat.rows;

	int* level = (int*) calloc(sizeof(int) * N);

	while (Dinic_BFS(GraphMat, s, e, level)) {
		int d;

		while (d = Dinic_DFS(GraphMat, s, e, level)) {
			ans += d;
		}
	}

	return ans;
}


}
#endif