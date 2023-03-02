#ifndef GRAPH_SPAINNINGTREE_H
#define GRAPH_SPAINNINGTREE_H


#include <algorithm>
#include <vector>

using namespace std;

/****************************
 * 
 *  Spaning Tree of graph
 * 
 ****************************/
namespace GraphTheory {

/*
 *  Prim algorithm
 */
template<class Graph>
void Prim(Graph& G, vector<pair<int, int>>& treeEdges) {
	int n = G.vertex_num;

	vector<bool> flag(n);
	vector<int> treeVertexes(n);
	int treeVertexesNum = 0;

	treeVertexes[treeVertexesNum++] = 0;
	flag[0] = 1;

	while (true) {
		pair<int, int> minEdge;
		double minValue = DBL_MAX;

		for (int i = 0; i < treeVertexesNum; i++) {
			int u = treeVertexes[i];

			for (int j = 0; j < n; j++)
				if (G(u, j) < minValue && !flag[j]) {
					minEdge.first  = u;
					minEdge.second = j;
					minValue = G(u, j);
				}
		}
		if (minEdge.u == -1)		//[3]
			break;

		//[5]
		treeEdges.push_back({ minEdge.first, minEdge.second });
		treeVertexes[treeVertexesNum++] = minEdge.second;
		flag[minEdge.second] = 1;
	}
}

/*
 *  Kruskal algorithm
 */
template<class Graph>
void Kruskal(Graph& G, vector<pair<int, int>>& treeEdges) {
	int nV = G.vertex_num;

	// First, we extract and sort the edge set of the graph
	vector<pair<int, int>> graphEdges = G.edge_set;
	
	sort(graphEdges.begin(), graphEdges.end(), [](pair<int, int>& a, pair<int, int>& b) {
		return G(a.first, a.second) <= G(b.first, b.second);
	});

	// function find root of union-find set
	std::function<int(vector<int>&, int)> find = [](vector<int>& S, int x) {
		return S[x] == x ? S[x] : (S[x] = find(S[x]));
	}

	int nE = graphEdges.size();
	vector<int> S(nV);

	for (int i = 0; i < nV; i++)
		S[i] = i;


	// run the algorithm
	for (int i = 0; i < nE; i++) {
		if (find(S, graphEdges[i].first) != 
			find(S, graphEdges[i].second)) {

			treeEdges.push_back(graphEdges[i]);
			S[find(S, graphEdges[i].first)] = find(S, graphEdges[i].second);
		}
	}
}

}

#endif