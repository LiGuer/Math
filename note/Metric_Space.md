* Metric Space
  - Define  
    A metric space is a pair $(X, d)$ where $X$ is a set and $d: X \times X \to \mathbb R$  is a metric on $X$ satisfying the following axioms $\forall x, y, z \in X$:
    - $d(x, y) \ge 0$, Non-negativity, with equality if and only if $x = y$
    - $d(x, y) = d(y, x)$, Symmetry
    - $d(x, y) \le d(x, z) + d(z, y)$, Triangle Inequality

  - Property
    - Lipschitz Equivalent  
      Metrics $d, d'$ on $X$ are said to be Lipschitz Equivalent if there are positive constants $A, B$ and $\forall x, y \in X$ such that
      $$A d(x, y) \le d'(x, y) \le B d(x, y)$$

    - Convergence of Sequence  
      A sequence $x_n \in X$ is said to be converge to $x$ if
      $$(\forall \epsilon > 0)(\exists K)(\forall k > K) d(x_k, x) < \epsilon$$

    - Complete Metric Space  
      A metric space $(X, d)$ is complete if all Cauchy sequences converge to a point in $X$. Cauchy sequence is a sequence satisfying 
      $$(\forall \epsilon > 0)(\exists N)(\forall n, m \ge N) d(x_n, x_m) < \epsilon$$

    - Continuity  
      For metric spaces $(X, d), (X', d')$, a function $f: X \to X'$ is continuous at $y \in X$ if
      $$(\forall \epsilon > 0)(\exists \delta > 0)(\forall x) d(X, Y) < \delta \Rightarrow d'(f(x),f(y)) < \epsilon$$

    - Uniform Continuity
      $$(\forall \epsilon > 0)(\exists \delta > 0)(\forall x) d(X, Y) < \delta \Rightarrow d(f(x),f(y)) < \epsilon$$

    - Lipschitz Continuity  
      $$d'(f(x), f(y)) \le K d(x, y)$$

    - Lipschitz Continuity $\Rightarrow$ Uniform Continuity $\Rightarrow$ Continuity

    - Contraction mapping  
      A mapping $f: X \to X$ is contraction if there existss some $\lambda \in [0, 1)$ such that
      $$d(f(x), f(y)) \le \lambda d(x, y)$$

    - Contraction mapping theorem  
      For a complete matric space, if $f: X \to X$ is a contraction, then $f$ has a unique fixed point $x$.

