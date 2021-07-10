/*
Copyright 2020 LiGuer. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
	http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#ifndef GRAPH_THEORY_H
#define GRAPH_THEORY_H
#include "Mat.h"
#include <float.h>
#include <algorithm>
#include <vector>
#include <array>
#include <queue>
namespace GraphTheory {
/*--------------------------------[ 边 Edge ]--------------------------------*/
struct Edge { 
	int u, v; double w; 
	void set(int _u, int _v, double _w) { u = _u; v = _v; w = _w; }
};
/*********************************************************************************
						构建  邻接图 / 邻接链表
*********************************************************************************/
struct GraphListNode { int v; double w; GraphListNode* next = NULL; };	//邻接链表节点
Mat<>& bulidGraphMat(int u[], int v[], double w[], int N, Mat<>& GraphMat) {
	GraphMat.alloc(N, N).fill(DBL_MAX);
	for (int i = 0; i < N; i++) GraphMat(u[i], v[i]) = w[i];
	return GraphMat;
}
Mat<GraphListNode*>& bulidGraphList(int u[], int v[], double w[], int N, Mat<GraphListNode*>& GraphList) {
	GraphList.zero(N);
	for (int i = 0; i < N; i++) {
		GraphListNode* ptr = new GraphListNode;
		ptr->v = v[i]; 
		ptr->w = w[i];
		ptr->next = GraphList[u[i]]; GraphList[u[i]] = ptr; 		//加入临接链表
	} return GraphList;
}
/*********************************************************************************
						最小生成树 
*	[算法]: Prim
		[输入]: 图的邻接链表[Graph], 节点数量[n]
		[输出]: 最小生成树,每一条有向边的 起点[TreeU],终点[TreeV],总边数[TreeCur]
		[原理]: 按点贪心, 每次加入已搜索点集u的最短边(u,v)，其中v不属于已搜索点集的点v
		[时间复杂度]: O(E·logV)
		[流程]:
			[1] 初始化[已搜点集 VertexNew]
			[2] 将第一个图的节点, 加入VertexNew
			[3] 开始迭代, 直至所有节点均已搜索完成, 即VertexNew已满
				[4] 在已搜点集, 寻找最短边(u,v), 其中u∈VertexNew, v ∉ VertexNew
				[5] 将边(u,v)加入最小生成树， 将v加入VertexNew
----------------------------------------------------------------------------------
*	[算法]: Kruskal
		[输入]: 图的边数据 起点[u] 终点[v] 权值[w] 边数[n]
		[输出]: 最小生成树,每一条有向边的 起点[TreeU],终点[TreeV],总边数[TreeCur]
		[时间复杂度] O(E·logV)
		[流程]:
			[1] 初始化未搜索边集EdgeNew = E0图边集
			[2] 开始迭代,直至未搜索边集为空集
				[3] 在边集合,选择最短边(u,v)
				[4] 若(u,v)不在同一颗树, u,v所在两棵树合并,(u,v)加入该树
				[5] 点集中删(u,v)
			[6] 最后剩下的那棵树,就是最小生成树
		Kruskal	是按边贪心，适合稀疏图。
		Prim	是按点贪心，适合稠密图。
*********************************************************************************/
void Prim(Mat<GraphListNode*>& GraphList, std::vector<int> TreeU[], std::vector<int> TreeV[])
{
	bool* flag      = (bool*)calloc(GraphList.size(), sizeof(bool));
	int * VertexSet = (int *)calloc(GraphList.size(), sizeof(int ));
	int VertexNum = 0;
	VertexSet[VertexNum++] = 0; flag[0] = 1;
	//[3]
	while (true) {
		//[4]
		Edge minEdge = { -1, -1, DBL_MAX };
		for (int i = 0; i < VertexNum; i++) {
			int u = VertexSet[i];
			GraphListNode* ptr = GraphList[u];
			while (ptr != NULL) {
				if (ptr->w < minEdge.w && !flag[ptr->v])
					minEdge.set(u, ptr->v, ptr->w);
				ptr = ptr->next;
			}
		}
		if (minEdge.u == -1) break;//[3]
		//[5]
		TreeU->push_back(minEdge.u);
		TreeV->push_back(minEdge.v);
		VertexSet[VertexNum++] = minEdge.v;
		flag[minEdge.v] = 1;
	}
	free(flag);
	free(VertexSet);
}
void Prim(Mat<>& graph, std::vector<int>& TreeU, std::vector<int>& TreeV)
{
	int N = graph.rows;
	bool* flag     = (bool*)calloc(N, sizeof(bool));
	int * visitSet = (int *)calloc(N, sizeof(int ));
	int visitNum = 0;
	visitSet[visitNum++] = 0; flag[0] = 1;
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
		if (minEdge.u == -1) break;//[3]
		//[5]
		TreeU.push_back(minEdge.u);
		TreeV.push_back(minEdge.v);
		visitSet[visitNum++] = minEdge.v;
		flag[minEdge.v] = 1;
	}
	free(flag); free(visitSet);
}
void Kruskal(Edge* edge, int N, std::vector<int>& TreeU, std::vector<int>& TreeV)
{
	std::sort(edge, edge + N, [](Edge a, Edge b) {				//边集排序, 让其顺序自然从小到大, 不需要再一个个寻找
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
			if (uRoot == 0 && vRoot == 0) root[edge[i].u] = root[edge[i].v] = edge[i].u;	//更新根节点,并查集思想
			else if (uRoot == 0) root[edge[i].u] = vRoot;
			else if (vRoot == 0) root[edge[i].v] = uRoot;
			else root[vRoot] = uRoot;
		}
	}
}
/*********************************************************************************
						最短路径
*	[算法]: Dijkstra
----------------------------------------------------------------------------------
*	[算法]: Floyd
		[输入]:	[1] 图的邻接矩阵Graph
		[输出]: [1] 距离矩阵Distance	[2] 后继节点矩阵Path
		[数据结构]:
			* 邻接矩阵Graph, 即.图的权值矩阵
			* 距离矩阵Distance: i到j最短路径长度
			* 后继节点矩阵Path: 记录两点间的最短路径, 表示从Vi到Vj需要经过的点
		[原理]:
			* Floyd的本质的动态规划, Dijkstra的本质是贪心
			* 状态转移方程:
				Distance[i,j] = min{ Distance[i,k] + Distance[k,j] , Distance[i,j] }
		[流程]:
			[1] 初始化距离矩阵Distance = 权值矩阵Graph
				初始化后继矩阵Path(i,j) = j
			[2] 对于每一对顶点 i 和 j, 看是否存在点 k 使得u->k->v比已知路径更短
					即. Distance[i,j] = min{ Distance[i,k] + Distance[k,j] , Distance[i,j] }
				若是,则更新Distance, Path(i,j) = k
		[时间复杂度]:
			Floyd	时间复杂度O(V³)【3个for()循环嵌套】,空间复杂度O(V²)【2个矩阵】
			Dijkstra时间复杂度O(V²)
		[对比Dijkstra]:
			Dijkstra一次只能算出给定两点间的最短路径。
			Floyd   一次可以算出任意两点间的最短路径。
*********************************************************************************/
void Dijkstra(Mat<>& GraphMat, Mat<>& Distance, Mat<>& Path) {

}
void Floyd(Mat<>& GraphMat, Mat<>& Distance, Mat<int>& Path)
{
	//[1]
	int N = GraphMat.rows;
	Distance = GraphMat;
	Path.zero(GraphMat.rows, GraphMat.cols);
	for (int i = 0; i < Path.size(); i++) Path[i] = i % N;
	//[2]
	for (int k = 0; k < N; k++)
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				Distance(i, j) = Distance(i, j) > Distance(i, k) + Distance(k, j) ?
					Path(i, j) = k, Distance(i, k) + Distance(k, j) :
					Distance(i, j);
}
/*********************************************************************************
						网络最大流
*	[算法]: Dinic
		[原理]: 贪心 + "反悔"机制
		增广路: 就是源->汇的一条路径。
			利用深搜DFS，找增广路。
			利用广搜BFS，确定此时各顶点的层次。
			利用添加反向边，协助"反悔"。
			广搜BFS是Dinic对于EK的优化。
			广搜BFS，用队列queue。
			深搜BFS，用递归or栈stack。
*********************************************************************************/
void Dinic_BFS(int s, int N)
{/*
	level[s] = 0;
	std::queue<int> Q; Q.push(s);
	while (!Q.empty()) {
		int temp = Q.front();
		Q.pop();
		for (int i = 1; i <= N; i++) {
			if (level[i] == -1 && Graph[temp][i] > 0) {
				Q.push(i);
				level[i] = level[temp] + 1;
			}
		}
	}*/
}
void Dinic_DFS(int s, int t, int N) {/*
	if (s == t) { Path.push_back(s); return; }
	for (int i = 1; i <= N; i++) {
		if (level[i] == level[s] + 1) {
			Dinic_dfs(i, t, N);
			if (!Path.empty()) {
				Path.push_back(s);
				minwight = minwight < Graph[s][i] ? minwight : Graph[s][i];
				return;
			}
		}
	}*/
}
double Dinic(Mat<>& GraphMat, Mat<>& Path,int s, int t, int N) {
	double ans = 0;
	Mat<int> level; level.alloc(N).fill(-1);
	Dinic_BFS(s, N);
	while (level[t] != -1) {
		double minwight = DBL_MAX;
		Path.zero();
		Dinic_DFS(s, t, N);
		for (int i = Path.size() - 1; i > 0; i--) {
			int u = Path[i], v = Path[i - 1];
			GraphMat(u, v) -= minwight;
			GraphMat(v, u) += minwight;
		}
		ans += minwight;
		Dinic_DFS(s, t, N);
	}
	return ans;
}
/*********************************************************************************
*						商旅问题	Traveling Salesman Problem
*	[问题]: 遍历所有给定点的最短闭合路径.
*	[算法]: 蚁群算法
		[0] 距离矩阵 Dis, 信息素矩阵 S
		[1] 选择: P(x,y) = S(x,y)^α / Dis(x,y)^β
		[2] 信息素更新: S = S_t / DisSum_t
*********************************************************************************/
#define RAND_DBL rand() / (double)RAND_MAX
void TravelingSalesmanProblem_AntGroup(Mat<>& Dis, Mat<int>& ansY,
	int antNum, int iterNum, double lossS_p = 0.2, double alpha = 1, double beta = 2.3, double Sq = 10
) {
	int N = Dis.cols;
	double disSumMin = DBL_MAX;
	Mat<> S(N, N); S = 1; ansY.zero(N);
	for (int iter = 0; iter < iterNum; iter++) {
		static Mat<> dS(N, N), dSk(N, N); dS.zero();
		for (int ant = 0; ant < antNum; ant++) {
			static Mat<bool> isVisit(N);  isVisit.zero(); dSk.zero();
			static Mat<int>  yT(N);
			int x = 0;
			//Path
			for (int i = 1; i < N; i++) {
				isVisit[x] = 1;
				int y;
				static Mat<> P(N);
				for (int j = 0; j < P.size(); j++)
					P[j] = isVisit[j] ? 0 : pow(S(x, j), alpha) / pow(Dis(x, j), beta);
				P /= P.sum();
				double rands = RAND_DBL;
				for (int j = 0; j < P.size(); j++)
					if (isVisit[j] == 0 && (rands -= P[j]) <= 0) { y = j; break; };
				dSk(x, y) = Sq; x = y; isVisit[y] = 1; yT[i] = y;
			}
			//Distance
			double disSum = Dis(yT[0], yT[yT.size() - 1]);
			for (int i = 0; i < yT.size() - 1; i++)
				disSum += Dis(yT[i], yT[i + 1]);
			if (disSum < disSumMin) { ansY = yT; disSumMin = disSum; }
			dS += (dSk /= disSum);
		}
		(S *= (1 - lossS_p)) += dS;
		//if (iter % 1 == 0)printf("> %d %f\n", iter, disSumMin);
	}
}
}
#endif