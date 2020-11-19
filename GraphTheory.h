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
/*--------------------------------[ 基础数据结构 ]--------------------------------*/
struct GraphTheory_Edge			//图的边Edge
{
	int u, v; double w;
	bool friend operator<(GraphTheory_Edge a, GraphTheory_Edge b) {
		if (a.w != b.w)return a.w < b.w;
		else if (a.u != b.u) return a.u < b.u;
		return a.v < b.v;
	}
};
struct GraphTheory_ListNode { int v; double w; GraphTheory_ListNode* next = NULL; };	//邻接链表 节点
/*--------------------------------[ 构建邻接链表 ]--------------------------------
**----------------------------------------------------------------------------------*/
GraphTheory_ListNode** bulidGraphList(int u[], int v[], double w[], int n)
{
	GraphTheory_ListNode** Graph = (GraphTheory_ListNode**)malloc(sizeof(GraphTheory_ListNode*) * n);
	memset(Graph, 0, sizeof(GraphTheory_ListNode*) * n);
	for (int i = 0; i < n; i++) {
		GraphTheory_ListNode* ptr = new GraphTheory_ListNode;
		ptr->v = v[i]; ptr->w = w[i]; ptr->next = Graph[u[i]];		//加入临接链表
		Graph[u[i]] = ptr;
	}
	return Graph;
}
/*--------------------------------[ 最小生成树 Prim ]--------------------------------
*	[输入]: 图的邻接链表[Graph], 节点数量[n]
*	[输出]：最小生成树,每一条有向边的 起点[TreeU],终点[TreeV],总边数[TreeCur]
*	[原理]: 按点贪心, 每次加入已搜索点集u的最短边(u,v)，其中v不属于已搜索点集的点v
*	[流程]:
		[1] 初始化[VertexNew]: 已搜索点集的存储数组
		[2] 将第一个图的节点, 加入VertexNew
		[3] 开始迭代, 直至所有节点均已搜索完成, 即VertexNew已满
			[4] 在已搜索点集, 寻找最短边(u,v), 其中u∈VertexNew, v不属于VertexNew
			[5] 将边(u,v)加入最小生成树， 将v加入已搜索点集VertexNew
**----------------------------------------------------------------------------------*/
void Prim(GraphTheory_ListNode* Graph[], int n, int TreeU[], int TreeV[], int& TreeCur)
{
	TreeCur = 0;
	bool* flag = (bool*)malloc(sizeof(bool) * n);
	int* VertexNew = (int*)malloc(sizeof(int) * n);
	VertexNew[0] = 0;
	//[3]
	while (true) {
		//[4]
		GraphTheory_Edge minEdge = { -1,-1,DBL_MAX };
		for (int i = 0; i < TreeCur + 1; i++) {
			int u = VertexNew[i];
			GraphTheory_ListNode* ptr = Graph[u];
			while (ptr != NULL) {
				if (ptr->w < minEdge.w && flag[ptr->v] == 0) {
					minEdge.w = ptr->w;
					minEdge.u = u;
					minEdge.v = ptr->v;
				}ptr = ptr->next;
			}
		}
		if (minEdge.u == -1)break;			//[3]
		//[5]
		TreeU[TreeCur] = minEdge.u; TreeV[TreeCur] = minEdge.v;	//加入最小生成树
		VertexNew[TreeCur + 1] = minEdge.v;						//v加入已搜索点集VertexNew
		TreeCur++;
		flag[minEdge.v] = 1;
	}
	free(VertexNew); free(flag);
}
/*--------------------------------[ 最小生成树 Kruskal ]--------------------------------
*	[输入]: 图的边数据 起点[u] 终点[v] 权值[w] 边数[n]
*	[输出]: 最小生成树,每一条有向边的 起点[TreeU],终点[TreeV],总边数[TreeCur]
*	[时间复杂度] O(E logE)
*	[流程]:
		[1] 初始化未搜索边集EdgeNew = E0图边集
		[2] 开始迭代,直至未搜索边集为空集
			[3] 在边集合,选择最短边(u,v)
			[4] 若(u,v)不在同一颗树, u,v所在两棵树合并,(u,v)加入该树
			[5] 点集中删(u,v)
		[6] 最后剩下的那棵树,就是最小生成树
*		Kruskal	是按边贪心，适合稀疏图。
		Prim	是按点贪心，适合稠密图。
**----------------------------------------------------------------------------------*/
void Kruskal(int u[], int v[], double w[], int n, int TreeU[], int TreeV[], int& TreeCur)
{
	GraphTheory_Edge* EdgeNew = (GraphTheory_Edge*)malloc(sizeof(GraphTheory_Edge) * n);
	for (int i = 0; i < n; i++) {
		EdgeNew[i].u = u[i]; EdgeNew[i].v = v[i]; EdgeNew[i].w = w[i];
	}
	std::sort(EdgeNew, EdgeNew + n);				//边集排序, 让其顺序自然从小到大, 不需要再一个个寻找
	int* root = (int*)malloc(sizeof(int) * n);		//根节点: 标记每一个点所在的树 (并查集思想)
	memset(root, 0, sizeof(int) * n);
	for (int i = 0; i < n; i++) {
		//[3]
		GraphTheory_Edge edge = EdgeNew[i];
		int uRoot = edge.u, vRoot = edge.v;
		while (root[uRoot] != 0)uRoot = root[uRoot];	//并查集思想, 找该点根节点
		while (root[vRoot] != 0)vRoot = root[vRoot];
		if (uRoot == 0 || vRoot == 0 || uRoot != vRoot) {
			//[4]
			TreeU[TreeCur] = edge.u;
			TreeV[TreeCur] = edge.v;
			TreeCur++;
			if (uRoot == 0 && vRoot == 0) root[edge.u] = root[edge.v] = edge.u;	//更新根节点,并查集思想
			else if (uRoot == 0) root[edge.u] = vRoot;
			else if (vRoot == 0) root[edge.v] = uRoot;
			else root[vRoot] = uRoot;
		}
	}
}
/*--------------------------------[ 最短路径 Dijkstra ]--------------------------------
**----------------------------------------------------------------------------------*/


/*--------------------------------[ 最短路径 Floyd ]--------------------------------
*	[输入]:	[1] 图的邻接矩阵Graph
*	[输出]: [1] 距离矩阵Distance	[2] 后继节点矩阵Path
*	[数据结构]:
		* 邻接矩阵Graph, 即.图的权值矩阵
		* 距离矩阵Distance: i到j最短路径长度
		* 后继节点矩阵Path: 记录两点间的最短路径, 表示从Vi到Vj需要经过的点
	[原理]:
		* Floyd的本质的动态规划, Dijkstra的本质是贪心
		* 状态转移方程:
			Distance[i,j] = min{ Distance[i,k] + Distance[k,j] , Distance[i,j] }
*	[流程]:
		[1] 初始化距离矩阵Distance = 权值矩阵Graph
			初始化后继矩阵Path(i,j) = j
		[2] 对于每一对顶点 i 和 j, 看是否存在点 k 使得u->k->v比已知路径更短
				即. Distance[i,j] = min{ Distance[i,k] + Distance[k,j] , Distance[i,j] }
			若是,则更新Distance, Path(i,j) = k
*	[时间复杂度]:
		Floyd	时间复杂度O(V³)【3个for()循环嵌套】,空间复杂度O(V²)【2个矩阵】
		Dijkstra时间复杂度O(V²)
*	[对比Dijkstra]:
		Dijkstra一次只能算出给定两点间的最短路径。
		Floyd   一次可以算出任意两点间的最短路径。
**----------------------------------------------------------------------------------*/
void Floyd(Mat<double>& Graph, Mat<double>& Distance, Mat<double>& Path)
{
	int N = Graph.cols;
	//[1]
	Distance.assign(Graph);
	Path.zero(N, N);
	for (int i = 0; i < N * N; i++)Path[i] = i % N;
	//[2]
	for (int k = 1; k <= N; k++) {
		for (int i = 1; i <= N; i++) {
			for (int j = 1; j <= N; j++) {
				if (Distance(i, j) > Distance(i, k) + Distance(k, j)) {
					Distance(i, j) = Distance(i, k) + Distance(k, j);
					Path(i, j) = k;
				}
			}
		}
	}
}
/*--------------------------------[ 网络最大流 Dinic ]--------------------------------
*	[原理]: 贪心 + "反悔"机制
*	增广路: 就是源->汇的一条路径。
		利用深搜DFS，找增广路。
		利用广搜BFS，确定此时各顶点的层次。
		利用添加反向边，协助"反悔"。
		广搜BFS是Dinic对于EK的优化。
		广搜BFS，用队列queue。
		深搜BFS，用递归or栈stack。
**----------------------------------------------------------------------------------*/
void Dinic_bfs(int s, int N)
{
	memset(level, -1, sizeof(level));
	queue<int> Q;
	Q.push(s); level[s] = 0;
	while (!Q.empty()) {
		int temp = Q.front();
		Q.pop();
		for (int i = 1; i <= N; i++) {
			if (level[i] == -1 && Graph[temp][i] > 0) {
				Q.push(i);
				level[i] = level[temp] + 1;
			}
		}
	}
}
void Dinic_dfs(int s, int t, int N) {
	if (s == t) {
		Path.push_back(s);
		return;
	}
	for (int i = 1; i <= N; i++) {
		if (level[i] == level[s] + 1) {
			dfs(i, t, N);
			if (!Path.empty()) {
				Path.push_back(s);
				minwight = minwight < Graph[s][i] ? minwight : Graph[s][i];
				return;
			}
		}
	}
}
int Dinic(int s, int t, int N) {
	double Ans = 0;
	bfs(s, N);
	while (level[t] != -1) {
		int minwight = 0x7FFFFFFF;
		Path.clear();
		dfs(s, t, N);
		for (int i = Path.size() - 1; i > 0; i--) {
			int u = Path[i], v = Path[i - 1];
			Graph[u][v] -= minwight;
			Graph[v][u] += minwight;
		}
		Ans += minwight;
		bfs(s, N);
	}
	return Ans;
}
#endif