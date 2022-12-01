* Optimization Problem
  - Define
    $$
    \begin{align*}
      \min \quad & f_0(x)  \tag{objective Function}\\
      s.t. \quad & f_i(x) ≤ 0  \tag{Inequality Constraint}\\
            & h_i(x) = 0  \tag{Equality Constraint}
    \end{align*}
    $$

    Optimal solution of optimization problem: 
    $$p^*= \inf \{f_{0}(x) | f_i(x) ≤ 0, h_i(x) = 0 \}$$

  - Include
    * Feasibility Problem
      - Define
        $$
        \begin{align*}
          \min \quad & x  \\
          s.t. \quad & f_i(x) ≤ 0  \\
                & h_i(x) = 0
        \end{align*}
        $$

        If the objective function is equal to 0, the optimal solution is 0 (Feasible set is not empty) or $\infty$ (Feasible set is empty).

    * Convex Optimization Problem
      - Define
        $$
        \begin{align*}
          \min \quad & f_0(x) \tag{$f_0$is Convex}\\ 
          s.t. \quad & f_i(x) ≤ 0 \tag{$f_i$is Convex}\\ 
            & a_i^T x = b_i  \tag{Affine function} 
        \end{align*}
        $$

      - Problem -- Solving Convex Optimization Problems

        - Problem -- Unconstrained Convex Optimization Problems
          $\min_{\boldsymbol x}\ f(\boldsymbol x)$
          其中$f(\boldsymbol x)$是可微凸函数

          最优性条件: $∇ f(\boldsymbol x^*) = 0$

          - Algorithm
            - Gradient Descent Method
              $$x_{k+1} = x_k - λ ∇ f(x_k)$$

            - Steepest Descent Method
              $$x_{k+1} = x_k + \arg\min \{ (∇ f(x_k))^T v\ |\ ||v|| = 1 \}$$

            - Newton Iteration Method
              $$x_{k+1} = x_k - \frac{1}{∇^2 f(x)} ∇ f(x_k)$$

            - Quasi-Newton Method

            - Block Coordinate Descent Method

        - Problem -- Solving Convex Optimization Problems with Equality Constraints

        - Property
          - KKT Conditions
            $$\begin{align*}\left\{\begin{matrix}
              f_i(\boldsymbol x^*) ≤ 0  \\
              \boldsymbol A \boldsymbol x = \boldsymbol b  \\
              \boldsymbol λ^* ⪰ 0  \\
              λ_i^* f_i(\boldsymbol x^*) = 0  \\
              ∇ L = ∇ f_0(\boldsymbol x^*) + \sum_{i=1}^m λ_i^* ∇ f_i(\boldsymbol x^*) + \boldsymbol A^T \boldsymbol v^* = 0
            \end{matrix}\right.\end{align*}$$

        - Algorithm
          * Interior Point Method

      - Include
        * Linear Programming
          - Define
            $$
            \begin{align*}
              \min \quad & c^T x + d  \\
              s.t. \quad & G x ⪯ h  \\
                & A x = b
            \end{align*}
            $$
            A optimization problem, which both objective function and constraint function are affine function.
            
            - Standard form
              $$
              \begin{align*}
                \min \quad & c^T x  \\
                s.t. \quad & A x = b  \\
                  & x ⪰ 0
              \end{align*}
              $$
            - Inequality form
              $$
              \begin{align*}
                \min \quad & c^T x  \\
                s.t. \quad & A x ⪯ b  \\
              \end{align*}
              $$

          - Note
            - 可行域是多面体, 等位曲线是与向量$c^T$正交的超平面, 最优解是多面体中在$-c^T$方向最远的顶点.
            - 若线性规划问题存在两个最优解, 则其必然存在无穷多个最优解. 
          
          - Algorithm -- 线性规划转换为标准形式
            - Procedure
              - Introducing Relaxation Variables into Inequalities
                $$
                \begin{align*}
                  \min \quad & c^T x + d  \\
                  s.t. \quad & G x + s = h  \\
                    & A x = b  \\
                    & s ⪰ 0
                \end{align*}
                $$

              - Express $x$ as the difference between two non negative variables $x = x^+ - x^-, x^+ ⪰ 0, x^- ⪰ 0$, just substitute.
                $$
                \begin{align*}
                  \min \quad & c^T x^+ - c^T x^- + d  \\
                  s.t. \quad & G x^+ - G x^- + s = h  \\
                    & A x^+ - A x^- = b  \\
                    & s ⪰ 0, x^+ ⪰ 0, x^- ⪰ 0
                \end{align*}
                $$

          - Algorithm -- Solving Linear Programming
            * Simplex Method

              Due to the feasible solution set of linear programming is a high-dimensional simplex (the simplest polyhedron), and the optimal solution is on the vertex or boundary of the polyhedron, not inside the polyhedron. Therefore, we can find the optimal solution by walking along each vertex of the simplex.

        * Linear Fractional Programming
          - Define
            $$
            \begin{align*}
              \min \quad & \frac{a^T x+ b}{c^T x + d}  \\
              s.t. \quad & Gx ⪯ 0  \\
                & Ax = b
            \end{align*}
            $$
            The problem can be equivalent to Linear Programming.

        * Quadratic Programming
          - Define
            $$
            \begin{align*}
              \min \quad & \frac{1}{2} x^T P x + q^T x + r  \\
              s.t. \quad & Gx ⪯ 0  \\
                & Ax = b
            \end{align*}
            $$

          - Example
            - Least Square Method
              $$\min ||Ax + b||_2^2$$

        * Quadratically Constrained Quadratic Programming ; QCQP
          - Define
            $$
            \begin{align*}
              \min \quad & \frac{1}{2} x^T P_0 x + q_0^T x + r_0  \\
              s.t. \quad & \frac{1}{2} x^T P_i x + q_i^T x + r_i ⪯ 0  \\
                & Ax = b
            \end{align*}
            $$

        * Quadratic Cone Programming
          - Define
            $$
            \begin{align*}
              \min \quad & f^T x  \\
              s.t. \quad & ||A_i x + b_i|| ≤ c_i^T + d_i  \\
                & Fx = g
            \end{align*}
            $$

        * Geometric Programming
          - Define
            $$
            \begin{align*}
              \min \quad & f_0(x)  \\
              s.t. \quad & f_i(x) ≤ 1  \\
                & h_i(x) = 1
            \end{align*}
            $$
            The natural form is not Convex, but can be transformed into a Convex Optimization Problem.
            
        * Semi-Definite Programming

        - Note
          $$Quadratic Cone Programming \supset \{Quadratic Programming \supset \{ Linear Programming \} , Quadratically Constrained Quadratic Programming\}$$
            
    * Integer Programming & Mixed Integer Programming
      - Define  
        Integer Programming: A class of problems in which variables can only be integer.

        Mixed Integer Programming: A class of problems in which variables can be integer.
            
      - Include
        * 0-1 Programming
          - Define
            A class of problems in which variables can only be 0 or 1.

      - Algorithm -- Solving Integer Programming & Mixed Integer Programming
        - Branch and Bound Method

    * Nonconvex Optimization Problem

  - Property
    * Lagrange Function
    * Lagrange Dual function, Dual Problem

  
    