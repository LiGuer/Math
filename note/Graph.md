* Graph 
  - Define  
    $$G = (V, E)  \tag{Graph}$$  

    Graph is a group consist of vertex set $V$ and edge set $E$ with weights of edges $w: E \to \mathbb R$.

    |Symbol|Means|
    |---|---|
    |$V$|Vertex set |
    |$E = \{(v_i, v_j)\ \|\ v_i, v_j \in V\}$|Edge set, a set of paired vertices|
    |$w: E \to \mathbb R$|weight of edge|  
    |||

    Undirected Garph, is a type of graph that does not distingush the direction of edges.
    $$E = \{\{v_i, v_j\}\ |\ v_i, v_j \in V\}$$

    Directed Graph, is a type of graph that distingush the direction of edges and its edge set is a set of ordered pairs.
    $$E = \{(v_i, v_j)\ |\ v_i, v_j \in V\}$$

  - Perperty  
    - Degree  
      Degree of a node refers to the number of edges connecting this node. 

    - Representation by Adjasency Matrix  
      Due to the discreteness of vertices, we can represente the weight of edge $w: E \to \mathbb R$ by Matrix $\boldsymbol G \in \mathbb R^{n \times n}$.
      
      $$w(v_i, v_j) \Rightarrow G_{ij}$$

      $$\boldsymbol G = \left(\begin{matrix} w(v_1, v_1) & \cdots & w(v_1, v_n) \\ \vdots&\ddots &\vdots \\ w(v_n, v_1) & \cdots & w(v_n, v_n) \end{matrix}\right)$$

    - Connectivity
      - Problem: Determine connectivity
        - Algoritm
          * Union-Find Sets
            - Define
              Union-Find Sets is a data structure that maintains a collection of disjoint sets, each containing a number of elements. It provides two operations: Union and Find.

              The Union operation takes two elements from different sets and merges them into a single set.  
              The Find operation is used to determine which set an element belongs to. It takes an element as input and returns a representative element of the set containing that element.  

    - Directivity
    - Acyclicity
    * Euler Path & Euler Graph
      - Define  
        Euler path is a path in a graph that passes through every edge exactly once. If the path starts and ends at the same vertex, it is called an Euler circuit. A graph that has an Euler circuit is called an Eulerian graph, while a graph that has an Euler path but not an Euler circuit is called a semi-Eulerian graph. 

      - Property
        - The existence of an Euler path or circuit in a connected undirected graph depends on the degree of the vertices. For a graph to have an Euler circuit, every vertex must have an even degree. For a connected undirected graph to have an Euler path, exactly two vertices must have an odd degree (all other vertices must have even degree).

      - Problem: search Euler Path
        - Hierholzer's algorithm 

  - Include
    * Complete Graph
      - Define  
        Complete Graph is a undirected graph in which every pair of vertices is adjacent.  
    * Tree
    * Directed Acyclic Graph
    * Bipartite Graph
  - Problem
    - Traversal
      - Depth-First Search  
        
      - Breadth-First Search

    * Shortest Paths

    * Minimum Spanning Tree
      - Purpose  
        Find a Tree (acyclic subgraph) on the undirected Graph $T \subseteq G, V^{(T)} = V^{(G)}$ with the smallest sum of edge weights connected all nodes.

        $$\begin{align*}
          T_{\min} =  \arg\min_{T \subseteq G} \quad & \sum_{e \in E^{(T)}} w(e)  \\
          s.t. \quad & V^{(T)} = V^{(G)} \\
          & T \text{ is acyclic}
        \end{align*}$$

      - Property
          $$\begin{align*}
            T_{\min}^{(G)} &\Rightarrow \left(T_{\min}^{(G)} - \{v_i\}\right) \text{ is a min spanning tree of } (G - \{v_i\})  \\
            T_{\min}^{(G - \{v_i\})} &\Rightarrow \left(T_{\min}^{(G - \{v_i\})} + \arg\min_{e \in (v_i,\cdot)} w(e) \right)  \text{ is a min spanning tree of } G
          \end{align*}$$

      - Algorithm  
        * Prim's Algorithm  
          Greed by vertices, put the shortest edge $(u,v)$ of $u$ from the searched vertices into the result edge sequence every time, and $v$ does not belong to the searched vertices. $T_{\min, k}$ refers to a sub-tree of $T_{\min}$ with $k+1$ vertices and $k$ edges.

          $$\begin{align*} 
            T_{\min, 0} &= (\{v_1\}, \empty)  \tag{initial}\\
            T_{\min, n} &= T_{\min}  \tag{answer}
          \end{align*}$$

          $$\begin{align*}
            (v_k, e_k) = \arg\min_{e_k = (v_k, v')}\quad & w(e_k)  \\
            s.t. \quad
            & e_k \in E^{(G)}  \\
            & v_k \in V^{(G)}  \\
            & v_k \notin V^{(T_{\min, k-1})}  \\
            & v' \in V^{(T_{\min, k-1})}
          \end{align*}$$
          $$T_{\min, k} = T_{\min, k-1} + (\{v_k\}, \{e_k\})$$

          - Property: Time complexity $O(E·logV)$

        * Kruskal's Algorithm  
          Greed by edges. $T_{\min, k}$ refers to a sub-tree of $T_{\min}$ with $k$ edges.

          $$\begin{align*} 
            T_{\min, 0} &= (V^{(G)}, \empty)  \tag{initial}\\
            T_{\min, n} &= T_{\min}  \tag{answer}
          \end{align*}$$

          $$\begin{align*}
            e_k =& \arg\min_{e_k \in E^{(G)}}\quad w(e_k)  \\
            &s.t. \quad \nexists \text{ path } \in T_{\min, k-1} \text{ , lets } e_k[1] \to e_k[2]
          \end{align*}$$

          $$T_{\min, k} = T_{\min, k-1} + (\{e_k\})$$

          - Property: Time complexity $O(E·logV)$

    * Network Flow Problem
      - Purpose
        We set $G'$ is a sub-graph of $G$, 
        $$\begin{align*}
          w'(u, v) \le w(u, v)  \tag{Flow constraint}\\
          \sum_i w'(u, v_i) = \sum_i w'(v_i, u)  \quad; u \neq s, u \neq e  \tag{非源、汇点进出流量相等}
        \end{align*}$$

      - Include
        - Maximum Flow Problem
          - Purpose
            $$\begin{align*}
              \max \quad & \sum_i w'(v_i, e) = w'(s, v_i)  \tag{源、汇点最大流量}\\
              s.t. \quad & w'(u, v) ≤ w(u, v)  \tag{Flow constraint}\\
                &\sum_i w'(u, v_i) = \sum_i w'(v_i, u)  \quad ; u \neq s, u \neq e  \tag{非源、汇点进出流量相等}
            \end{align*}$$

          - Algorithm
            - Dinic Algorithm

        - Minimum Cost Problem

    * Travelling Salesman Problem  
      - Purpose  
        Find the shortest closed path traversing all given points.
