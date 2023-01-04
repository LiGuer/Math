* Shortest Paths
  - Problem  
    For a graph $G = (V, E)$ with edge weight $w : E \to \mathbb R$, we aim to find the minimum sum of edge weights of path $P(v_s, v_e)$ from $v_s \to v_e$, 
    $$P(v_s, v_e) = (e_{1}^{(P)}, ..., e_{m}^{(P)} \ |\ e_{i}^{(P)} \in E, e_{1}^{(P)}[1] = v_{s}, e_{m}^{(P)}[2] = v_{e})$$
    
    $$\begin{align*}
    d(v_s, v_e) = \min_{P} \quad& \sum_{e_{i}^{(P)} \in P} w(e_{i}^{(P)}) \\
    s.t. \quad& e_{1}^{(P)}[1] = v_{s}\\
      & e_{m}^{(P)}[2] = v_{e}
    \end{align*}$$

  - Property
    - The shortest path has no loop
      $$e_{i}^{(P)} \neq e_{j}^{(P)} \quad, \text{if } i \neq j$$

    - If $v_i$ and $v_j$ are relay nodes of shortest path $P_{\min}(v_1, v_k)$ between the node $v_1$ and $v_k$, then the subpath $P_{\min}(v_i, v_j) = (v_i, ..., v_j) \subset P(v_1, v_k)$ is the shortest path between the node $v_i$ and $v_j$. 
      $$P(v_i, v_j) \subset P_{\min}(v_1, v_k)  \Rightarrow  P_{\min}(v_i, v_j) = P(v_i, v_j)$$

    - $\forall (v_i, v_j) \in E$
      $$d(v_s, v_j) \le d(v_s, v_i) + w((v_i, v_j))$$  
      becase $s \to i \to j$ is also a feasible path, then $d(P_{min}) \le d(P_{s \to i \to j})$.

    - estimation of the shorteset paths and distances
      - Initial estimation  
        $$
        \begin{align*}
        \tilde d(v_s, v_s) &\gets 0  \tag{estimation of the shorteset distance}\\
        \tilde d(v_s, v_i) &\gets \inf, \quad v_i \in V - \{v_s\}\\
        \tilde P(v_s, v_i) &\gets (v_s), \quad v_i \in V  \tag{estimation of the shorteset path}\\
        \end{align*}
        $$ 

        where, $\tilde d(v_s, v_i)$ is the estimation and upper-bound of $d(v_s, v_i)$, while $\tilde P(v_s, v_i) \Leftrightarrow \tilde d(v_s, v_i) = \inf$.

      - Update estimation  
        if there is an edge $(v_i, v_j)$, make the path better $\tilde d(v_s, v_j) > \tilde d(v_s, v_i) + w((v_i, v_j))$, then,
        $$
        \begin{align*}
        \tilde d(v_s, v_j) &\gets \tilde d(v_s, v_i) + w((v_i, v_j))  \\
        \tilde P(v_s, v_j) &\gets \tilde P(v_s, v_i) + (v_j)
        \end{align*}
        $$ 

        for a subgraph $G' = G - (\{v_i\}, \{(v_i, \cdot), (\cdot, v_i)\})$, we have  

        $$\begin{align*}
          d^{(G)}(v_s, v_i) &= \min_{v_j \in V^{(G')}} \left(d^{(G')}(v_s, v_j) + w((v_j, v_i))\right)  \\
          d^{(G)}(v_s, v_j) &\le d^{(G)}(v_s, v_i) + w(v_i, v_j) \quad, \forall (v_i, v_j) \in E^{(G)}  \\
          P_{\min}^{(G)}(v_s, v_i) &= \arg\min_{P_{\min}^{(G')}(v_s, v_j)} \left(d\left(P_{\min}^{(G')}(v_s, v_j)\right)+ w((v_j, v_i))\right) + (v_i)
        \end{align*}$$

  - Algorithm
    * Floyd's Algorithm  
      Base on danamic programming, there is a Tensor $M^{(d)}, M^{(p)} \in \mathbb R^{n \times n \times (n+1)}$, where $M_{i, j, k}$ refer to the minimum distance and path from $v_i \to v_j$ with optional intermediate vertices $\{v_1, ..., v_k\}$. When $k = 0$, $M_{i, j, 0}$ refer to the distance of direct path $(v_i, v_j)$ with no intermediate vertice. When $k = n$, $M_{i, j, n}$ is the answer of the shorest distance and path of all virtice pairs.
      $$\begin{align*} 
        G &= M^{(d)}_{\cdot, \cdot, 0}  \\
        D &= M^{(d)}_{\cdot, \cdot, n}
      \end{align*}$$

      For $k$ and optional intermediate vertices $\{v_1, ..., v_k\}$, $M^{(d)}_{\cdot, \cdot, k}$ could be obtained by $M^{(d)}_{\cdot, \cdot, k-1}$ through determines whether to select $v_k$ as the intermediate node.
      $$
        M^{(d)}_{i, j, k} = \min \left(M^{(d)}_{i, j, k-1},\ M^{(d)}_{i, k, k-1} + M^{(d)}_{k, j, k-1}\right)
      $$

      ```cpp
      dis = G;
      
      for (int k = 1; k <= N; k++)
        for (int i = 1; i <= N; i++)
          for (int j = 1; j <= N; j++)
            dis(i, j) = dis(i, j) > dis(i, k) + dis(k, j) ?
                        path(i, j) = k, dis(i, k) + dis(k, j) :
                        dis(i, j);
      ```

    * Dijkstra

    * Bellman Ford
