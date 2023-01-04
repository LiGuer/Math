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
        Find a Tree on the Graph with the smallest sum of edge weights connected all nodes.

      - Algorithm  
        * Prim Algorithm  
          - Greed by vertices, put the shortest edge $(u,v)$ of $u$ from the searched vertices into the result edge sequence every time, and $v$ does not belong to the searched vertices.
          - Time complexity $O(E·logV)$

        * Kruskal Algorithm
          - Greed by edges
          - Time complexity $O(E·logV)$

    * Maximum Flow