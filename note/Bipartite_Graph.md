* Bipartite Graph
  - Define
    $$(X, Y, E)  \tag{Bipartite Graph}$$
    $$X, Y \subset V, X \cup Y = V$$
    $$E = \{(x_i, y_j) \ |\ x_i \in X, y_j \in Y\}  \tag{Edge set}$$

    For a Bipartite Graph, The vertex set of a graph is divided into two disjoint subsets $X, Y$. And, edges in Bipartite Graph only exist between point sets $X, Y$, not within them.

  - Property
    - Matching  
      A subgraph of a bipartite graph, and any two edges in the edge set of the subgraph are not attached to the same vertex;  
      We set $M$ is a sub-graph of a bipartite graph $G$, 
      $\forall e_i, e_j \in E_M, e_i \neq e_j, then\ e_i(1)  \neq e_j(1), e_i(2) \neq e_j(2)$

      - Maximum Matching: The matching with maximum number of edges $\arg\max_{M \subseteq G} \quad \text{number}(E_M)$
      - Perfect Matching: All vertexs are in the edges of matching.

      - Problem: Search Maximum Matching
        - Algorithm
          - Hungarian Algorithm
          - Hopcroft-Karp Algorithm
