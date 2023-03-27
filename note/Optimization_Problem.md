* Optimization Problem
  - Purpose  
    $$
    \begin{align*}
      \min \quad & f_0(x)  \tag{objective Function}\\
      s.t. \quad & f_i(x) ≤ 0  \tag{Inequality Constraint}\\
            & h_i(x) = 0  \tag{Equality Constraint}
    \end{align*}
    $$

    $$p^*= \inf \{f_{0}(x) | f_i(x) ≤ 0, h_i(x) = 0 \}  \tag{Optimal solution}$$

    Optimization Problem aims to find the minimal values called optimal solution $p^*$ of objective function $f_0(\cdot)$, subject to the constraints $f_i(\cdot), h_i(\cdot)$.

  - Include
    * Feasibility Problem
      - Define
        $$
        \begin{align*}
          \min_x \quad & const. \\
          s.t. \quad & f_i(x) ≤ 0  \\
                & h_i(x) = 0
        \end{align*}
        $$

        If the objective function is equal to a constant, the optimal solution is 0 (Feasible set is not empty) or $\infty$ (Feasible set is empty).

    * Convex Optimization Problem
    * Integer Programming & Mixed Integer Programming
    * Nonconvex Optimization Problem

  - Property
    * Lagrange Function
    * Lagrange Dual function, Dual Problem

  
    