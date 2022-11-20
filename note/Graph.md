* Graph 
  - Define  
    $G = (V, E)$  
    $V$: vertex set  
    $E$: edge set, a set of paired vertices  
    $w: E \to \mathbb R$: weight of edge

    Undirected Garph
    Directed Graph

  - Perperty  
    - Degree
    - Representation
      - Adjasency Matrix

      - Adjasency List
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