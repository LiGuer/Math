* Markov Chain
  - Define  
    A sequence of Random Variables $X_0, X_1, ..., X_n$ taking value in the state space $\{1, ..., M\}$, and the transition of state $X_{n-1} \to X_{n}$ only depend on the value of the last state $X_{n-1}$.
    $$\mathbb P(X_n = x_n | X_{n-1} = x_{n-1}, ..., X_{0} = x_{0}) = \mathbb P(X_n = x_n | X_{n-1} = x_{n-1})  \tag{Tansition Probability}$$  
    The probability of the Tansition from state $X_{n-1} \to X_{n}$ is called Tansition Probability.

  - Property
    * Tansition Matrix
      - Define
        $$T_{i,j} = \mathbb P(X_n = j | X_{n-1} = i)  \tag{$i \to j$}$$
        $$
        \begin{align*}
          \boldsymbol T_{M \times M} &= \left(\begin{matrix}
            \mathbb P(X_n = 1 | X_{n-1} = 1) & \mathbb P(X_n = 2 | X_{n-1} = 1) & ... & \mathbb P(X_n = M | X_{n-1} = 1)  \\
            \vdots & \vdots & \ddots & \vdots \\
            \mathbb P(X_n = 1 | X_{n-1} = M) & \mathbb P(X_n = 2 | X_{n-1} = M) & ... & \mathbb P(X_n = M | X_{n-1} = M)  \\
          \end{matrix}\right) \tag{Tansition Matrix} \\
          \boldsymbol p_{n} &= \boldsymbol T_{M \times M} \boldsymbol p_{n-1}  \\
          &= \left(\begin{matrix} \mathbb P(X_n = 1) \\ \mathbb P(X_n = 2) \\ \vdots \\ \mathbb P(X_n = M) \end{matrix}\right)  \\
          &= \left(\begin{matrix}
          \mathbb P(X_n = 1 | X_{n-1} = 1) & ... & \mathbb P(X_n = M | X_{n-1} = 1)  \\
          \vdots & \ddots & \vdots \\
          \mathbb P(X_n = 1 | X_{n-1} = M) & ... & \mathbb P(X_n = M | X_{n-1} = M)  \\
          \end{matrix}\right)
          \left(\begin{matrix} \mathbb P(X_{n-1} = 1) \\ \mathbb P(X_{n-1} = 2) \\ \vdots \\ \mathbb P(X_{n-1} = M) \end{matrix}\right)  \tag{Tansition of Probability Distribution}
        \end{align*}
        $$

        $k$-step Tansition Matrix
        $$
        \begin{align*}
        T^{(k)}_{ij} &= \mathbb P(X_n = j | X_{n-k} = i)  \tag{element of $k$-step Tansition Matrix}\\
          \boldsymbol p_{n} &= \boldsymbol T^{(k)} \boldsymbol p_{n-k}  \\
        \end{align*}
        $$
        $$T^{(k)} = T^k$$ 

      - Property
        - simple
          $$
          \begin{align*}
            T_{i,j} &\ge 0\\
            \sum_{j=1}^M T_{i,j} &= 1  \\
          \end{align*}
          $$

        * Stationary Distribution
          - Define 
            $$\boldsymbol p_{\pi} = \boldsymbol T \boldsymbol p_{\pi}  \tag{$p_{\pi}$: Stationary Distribution}$$

  - Example
    * Random Walk
      - Define  
        For a squence $\{Y_i\}$ of iid. random variable with value $\{-1, +1\}$ and probability $\{1-p, p\}$, then the 1-dimensional random walk is the squence $\{X_0, X_1, ...\}, X_0 = 0$ of random variable such that
        $$X_t = \sum_{i=1}^t Y_i$$

    * Branching Process 
      - Define  
        Branching Process are used to medel population growth by reproduction. At the beginning, there is only one individual $Z_0 = 1$. In the next generation $n+1$, each offspring will reproduce randomly according to iid. random variable $X_{n,i}$. In branching process, the expected number of individuals in a particular $\mathbb E(Z_n)$ and the probability of ultimate extinction $\mathbb P(Z_n = 0)$ are central questions. The recurrence equation of branching process is 
        $$Z_{n+1} = \sum_{i=1}^{Z_n} X_{n, i}$$
        $Z_{n+1}$: the size of generation $n$ with $Z_0 = 1$.  
        $X_{n, i}$: a random variable denoting the number of direct successors of member $i$ in generation $n$. And $X_{n,i}$ are iid. over all $n$ and $i$.  

      - Property

          

        
    