* Graph 
  - Define  
    $$G = (V, E)$$  

    |Symbol|Means|
    |---|---|
    |$V$|Vertex set |
    |$E = \{(v_i, v_j)\ \|\ v_i, v_j \in V\}$|Edge set, a set of paired vertices|
    |$w: E \to \mathbb R$|weight of edge|  
    |||

    Undirected Garph  
    Directed Graph

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
    - Directivity
    - Acyclicity
    * Euler Path

  - Include
    * Tree
    * Directed Acyclic Graph
    * Bipartite Graph
  
  - Problem
    - Traversal
      - Depth First Search
      - Breadth First Search
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
            T_{\min, 0} &= (v_1, \empty)  \tag{initial}\\
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
          $$T_{\min, k} = T_{\min, k-1} + (v_k, e_k)$$

          - Property: Time complexity $O(E·logV)$

        * Kruskal's Algorithm  
          Greed by edges. $T_{\min, k}$ refers to a sub-tree of $T_{\min}$ with $k$ edges.

          $$\begin{align*} 
            T_{\min, 0} &= (\empty, \empty)  \tag{initial}\\
            T_{\min, n} &= T_{\min}  \tag{answer}
          \end{align*}$$

          $$\begin{align*}
            e_k =& \arg\min_{e_k \in E^{(G)}}\quad w(e_k)  \\
            &s.t. \quad  e_k[1] \notin V^{(T_{\min, k-1})} \text{\quad or \quad} e_k[2] \notin V^{(T_{\min, k-1})}
          \end{align*}$$

          $$T_{\min, k} = T_{\min, k-1} + (\{e_k[1], e_k[2]\}, e_k)$$

          - Property: Time complexity $O(E·logV)$

    * Maximum Flow