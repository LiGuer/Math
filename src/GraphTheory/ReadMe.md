# **Graph Theory**

# Special Grpah Structure
* circular doubly linked list ```list<T>```
* red-black tree ```rb_tree<T>```

# Algorithm
* Spanning Tree
  * Prim algorithm ```Prim(Graph& G, vector<pair<int, int>>& treeEdges)```
  * Kruskal algorithm ```Kruskal(Graph& G, vector<pair<int, int>>& treeEdges)```
* Shortest Path
  * Dijkstra's algorithm ```Dijkstra(Graph& G, int st, vector<double>& dis)```
  * Floyd's algorithm ```Floyd(Mat<double>& G, Mat<double>& dis, Mat<int>& path)``` 
* Networks Flow
  * Dinic's algorithm ```Dinic(Mat<>& GraphMat, Mat<>& Path, int s, int e)```