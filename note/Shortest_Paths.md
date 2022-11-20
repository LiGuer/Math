* Shortest Paths
  - Problem  
    For a graph $G = \{V, E\}$ and weight $w : E \to \mathbb R$, 
    $$
    \begin{align*}
    d(v_s, v_e) = \min_{P} \quad& \sum_{e_i \in P} w(e_i) \\
    s.t. \quad& P[1] = v_{s}\\
      & P[end] = v_{e}
    \end{align*}
    $$

  - Property
    - If $P_{0,k} = (v_0, ..., v_i, ..., v_j, ..., v_k)$ is the shortest paths between the node $v_0$ and $v_k$, and the node $v_i$ and $v_j$ is the relay node of this path, then the subpath $(v_i, ..., v_j) \subset P_{0,k}$ is the shortest paths between the node $v_i$ and $v_j$. 

    - For any edges $(v_i, v_j) \in E$,
      $$d(v_s, v_j) \le d(v_s, v_i) + w((v_i, v_j))$$

    - estimation of the shorteset paths and distances

      - Initial estimation  
        $$
        \begin{align*}
        \tilde d(v_s, v_s) &\gets 0  \tag{estimation of the shorteset distance}\\
        \tilde d(v_s, v_i) &\gets \inf, \quad v_i \in V - \{v_s\}\\
        \tilde P(v_s, v_s) &\gets (v_s)  \tag{estimation of the shorteset path}\\
        \tilde P(v_s, v_i) &\gets \emptyset, \quad v_i \in V - \{v_s\}
        \end{align*}
        $$ 

        where, $\tilde d(v_s, v_i)$ is the estimation and upper-bound of $d(v_s, v_i)$, while $\tilde P(v_s, v_i) \Leftrightarrow \tilde d(v_s, v_i) = \inf$.

      - Update estimation  
        if there is an edge $(v_i, v_j)$, and $\tilde d(v_s, v_j) > \tilde d(v_s, v_i) + w((v_i, v_j))$, then,
        $$
        \begin{align*}
        \tilde d(v_s, v_j) &\gets \tilde d(v_s, v_i) + w((v_i, v_j))  \\
        \tilde P(v_s, v_j) &\gets \tilde P(v_s, v_i) + (v_j)
        \end{align*}
        $$ 

  - Algorithm
    * Floyd's Algorithm

    * Dijkstra

    * Bellman Ford
