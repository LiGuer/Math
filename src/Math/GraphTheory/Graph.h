#ifndef GRAPH_THEORY_GRAPH_H
#define GRAPH_THEORY_GRAPH_H

#include <algorithm>
#include <vector>

using namespace std;

namespace GraphTheory {

/* --------------------------------
*
*	Í¼ ½Úµã
*
* -------------------------------- */
template <class T = int>
class GraphNode {
public:
	T val;

	vector<GraphNode*> next, prev;
};

}

#endif