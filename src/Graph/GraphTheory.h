#ifndef GRAPH_THEORY_H
#define GRAPH_THEORY_H

#include "../Matrix/Mat.h"
#include <float.h>
#include <algorithm>
#include <vector>
#include <array>
#include <queue>

using namespace std;

namespace GraphTheory {

/* --------------------------------
 * 
 *		构建  邻接图 / 邻接链表 
 * 
 * -------------------------------- */
	struct Edge {
		int u, v;
		double w = 1;

		void set(int _u, int _v, double _w) {
			u = _u;
			v = _v;
			w = _w;
		}
	};

	Mat<>& bulidGraphMat(int* u, int* v, double* w, int N, Mat<>& GraphMat) {
		GraphMat.alloc(N, N).fill(DBL_MAX);

		for (int i = 0; i < N; i++) 
			GraphMat(u[i], v[i]) = w[i];

		return GraphMat;
	}

	vector<vector<pair<int, double>>>* bulidGraphList(int* u, int* v, double* w, int N) {
		vector<vector<pair<int, double>>>* GraphList = new vector<vector<pair<int, double>>>(N);

		for (int i = 0; i < N; i++) {
			GraphList[u[i]].push_back(pair<int, double> {v[i], w[i]});
		} 
		return GraphList;
	}

/* --------------------------------
 * 
 *		最小生成树 
 * 
 * -------------------------------- */

	/*
	 *  Prim
	 */
	void Prim(vector<vector<pair<int, double>>>& GraphList, vector<int> TreeU[], vector<int> TreeV[])	{

		bool* flag = (bool*)calloc(GraphList.size(), sizeof(bool));
		int* VertexSet = (int*)calloc(GraphList.size(), sizeof(int));
		int VertexNum = 0;

		VertexSet[VertexNum++] = 0;
		flag[0] = 1;

		//[3]
		while (true) {
			//[4]
			Edge minEdge = { -1, -1, DBL_MAX };

			for (int i = 0; i < VertexNum; i++) {
				int u = VertexSet[i], 
					m = GraphList[u].size();

				for (int j = 0; j < m; j++) {
					if (GraphList[u][j].second < minEdge.w && !flag[GraphList[u][j].first]) {
						minEdge.set(u, ptr->v, ptr->w);
					}
				}

			}
			if (minEdge.u == -1)		//[3]
				break;

			//[5]
			TreeU->push_back(minEdge.u);
			TreeV->push_back(minEdge.v);
			VertexSet[VertexNum++] = minEdge.v;
			flag[minEdge.v] = 1;
		}
		free(flag);
		free(VertexSet);
	}

	void Prim(Mat<>& graph, vector<int>& TreeU, vector<int>& TreeV) {

		int N = graph.rows;
		bool* flag = (bool*)calloc(N, sizeof(bool));
		int* visitSet = (int*)calloc(N, sizeof(int));
		int visitNum = 0;

		visitSet[visitNum++] = 0;
		flag[0] = 1;

		//[3]
		while (true) {
			//[4]
			Edge minEdge = { -1, -1, DBL_MAX };

			for (int i = 0; i < visitNum; i++) {
				int u = visitSet[i];

				for (int v = 0; v < N; v++)
					if (!flag[v] && graph(u, v) < minEdge.w)
						minEdge.set(u, v, graph(u, v));
			}

			if (minEdge.u == -1)
				break;//[3]

			//[5]
			TreeU.push_back(minEdge.u);
			TreeV.push_back(minEdge.v);
			visitSet[visitNum++] = minEdge.v;
			flag[minEdge.v] = 1;
		}
		free(flag);
		free(visitSet);
	}

	/*
	 *  Kruskal
	 */
	void Kruskal(Edge* edge, int N, vector<int>& TreeU, vector<int>& TreeV) {

		sort(edge, edge + N, [](Edge a, Edge b) {				//边集排序, 让其顺序自然从小到大, 不需要再一个个寻找
			if (a.w != b.w) return a.w < b.w;
			if (a.u != b.u) return a.u < b.u;
			return a.v < b.v;
		});

		Mat<int> root;
		for (int i = 0; i < N; i++) {
			//[3]
			int uRoot = edge[i].u,
				vRoot = edge[i].v;
			while (root[uRoot] != 0) uRoot = root[uRoot];			//并查集思想, 找该点根节点
			while (root[vRoot] != 0) vRoot = root[vRoot];

			if (uRoot == 0 || vRoot == 0 || uRoot != vRoot) {
				//[4]
				TreeU.push_back(edge[i].u);
				TreeV.push_back(edge[i].v);

				if (uRoot == 0 && vRoot == 0)
					root[edge[i].u] = root[edge[i].v] = edge[i].u;	//更新根节点,并查集思想
				else if (uRoot == 0) root[edge[i].u] = vRoot;
				else if (vRoot == 0) root[edge[i].v] = uRoot;
				else root[vRoot] = uRoot;
			}
		}
	}

/* --------------------------------
 * 
 *		最短路径
 * 
 * -------------------------------- */

	/*
	 *  Dijkstra
	 */
	double* Dijkstra(Mat<>& G, int st) {
		int N = G.rows;

		double* dis = (int*)malloc(N * sizeof(double));
		bool* used = (bool*)calloc(N, sizeof(bool));
		
		for (int i = 0; i < N; i++) {
			dis[i] = DBL_MAX;
		}
		dis[st] = 0;

		for (int i = 0; i < N; i++) {
			int x = -1;

			for (int j = 0; j < N; j++) {
				if (!used[x] && (x != -1 || dis[j] < dis[x])) {
					x = j;
				}
			}

			used[x] = true;

			for (int j = 0; j < N; j++) {
				dis[j] = min(dis[j], dis[x] + G(x, j));
			}
		}

		free(uesd);
		return dis;
	}

	double* Dijkstra(vector<vector<pair<int, double>>>& GraphList, int st) {
		int N = G.rows;

		double* dis = (int*)malloc(N * sizeof(double));
		bool* used = (bool*)calloc(N, sizeof(bool));

		for (int i = 0; i < N; i++) {
			dis[i] = DBL_MAX;
		}
		dis[st] = 0;

		for (int i = 0; i < N; i++) {
			int x = -1;

			for (int j = 0; j < N; j++) {
				if (!used[x] && (x != -1 || dis[j] < dis[x])) {
					x = j;
				}
			}

			used[x] = true;

			int m = GraphList[x].size();

			for (auto t : GraphList[x]) {
				dis[t.first] = min(dis[t.first], dis[x] + t.second);
			}
		}

		free(uesd);
		return dis;
	}

	/*
	 *  Floyd
	 */
	void Floyd(Mat<>& GraphMat, Mat<>& dis, Mat<int>& Path) {
		//[1]
		int N = GraphMat.rows;
		dis = GraphMat;
		Path.zero(GraphMat.rows, GraphMat.cols);

		for (int i = 0; i < Path.size(); i++)
			Path[i] = i % N;

		//[2]
		for (int k = 0; k < N; k++)
			for (int i = 0; i < N; i++)
				for (int j = 0; j < N; j++)
					dis(i, j) = dis(i, j) > dis(i, k) + dis(k, j) ?
						Path(i, j) = k, dis(i, k) + dis(k, j) :
						dis(i, j);
	}


/* --------------------------------
 * 
 *		网络最大流
 * 
 * -------------------------------- */

	/*
	 *  Dinic
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


/* --------------------------------
 * 
 *		二分图最大匹配
 * 
 * -------------------------------- */

	/*
	 *  Hungarian
	 */
	void Hungarian() {

	}


}
#endif