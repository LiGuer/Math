* Quadratic Function
  - Define
    $$
    \begin{align*}
      f(x) &= a x^2 + b x + c  \tag{Univariate Quadratic}  \\
      f(\boldsymbol x) &= \boldsymbol x^T \boldsymbol A \boldsymbol x + \boldsymbol b \boldsymbol x + c  \tag{Multivariate}  \\
        &= \sum_{i=1}^{\dim} \sum_{j=1}^{\dim} a_{ij} · x_i x_j + \sum_{i=1}^{\dim} b_i x_i + c
    \end{align*}
    $$

  - Property
    * 二次函数的零点集 , Quadric Surface
      - Define  
        Quadric Surface:  
        $$
        \begin{align*}
          &\{ \boldsymbol x \ |\ \boldsymbol x^T \boldsymbol A \boldsymbol x + \boldsymbol b \boldsymbol x + c = 0\} \\
        \Leftrightarrow &\left\{ \boldsymbol x' \ |\ \boldsymbol x'^T \boldsymbol A' \boldsymbol x' = 0 \ |\  \boldsymbol x' = \left(\begin{matrix} \boldsymbol x \\ 1 \end{matrix}\right)\right\}
        \end{align*}
        $$

      - Solve quadratic equation
        - Problem  
          (input) $a, b, c$  
          求输入二次函数的零点集$\{x\}$  
          $$f(x) = a x^2 + b x + c = 0$$

        - Answer  
          一元二次:
          $$
          \begin{align*}
            x &= \frac{- b \pm \sqrt{Δ}}{2 a}\\
            Δ &= b^2 - 4 a c
          \end{align*}
          $$
          解的性质
          - $Δ > 0$, 两个实数根
          - $Δ = 0$, 一个实数二重根
          - $Δ < 0$, 两个复数根

      - Include
        * Sphere & Spherical Surface
          - Define  
            For $\boldsymbol A = \boldsymbol I, \boldsymbol b = \boldsymbol 0, c = -r^2$
            $$
            \begin{align*}
              &\{ \boldsymbol x \ |\ ||\boldsymbol x - \boldsymbol x_c||_2 ≤ r < 0\}  \tag{Sphere}\\
            \Leftrightarrow\quad &\{ \boldsymbol x \ |\ (\boldsymbol x - \boldsymbol x_c)^T (\boldsymbol x - \boldsymbol x_c) ≤ r^2 < 0\}  \\
            \Leftrightarrow\quad &\{ \boldsymbol x_c + r \boldsymbol u \ |\ ||\boldsymbol u||_2 ≤ r < 0\}
            \end{align*}
            $$

            $$\{ \boldsymbol x \ |\ ||\boldsymbol x - \boldsymbol x_c||_2 ≤ r < 0\}  \tag{Spherical Surface}\\$$

            Spherical Surface is a point set with a constant distance value $r$ from the center point $\boldsymbol x_c$.

          - Property
            - Sphere is a convex set

        * Ellipsoid & Ellipsoid Surface
          - Define  
            令$\boldsymbol A = \boldsymbol P^{-1}, \boldsymbol b = \boldsymbol 0, c = -1$是正定矩阵.
            $$
            \begin{align*}
              &\{ \boldsymbol x \ |\ (\boldsymbol x - \boldsymbol x_c)^T \boldsymbol P^{-1} (\boldsymbol x - \boldsymbol x_c) ≤ 1, \boldsymbol P = \boldsymbol P^T ⪰ 0\}  \tag{Ellipsoid}\\
              \Leftrightarrow\quad &\{ \boldsymbol x_c + \boldsymbol A \boldsymbol u \ |\ ||\boldsymbol u||_2 ≤ 1\}
            \end{align*}
            $$

            $$\{ \boldsymbol x \ |\ (\boldsymbol x - \boldsymbol x_c)^T P^{-1} (\boldsymbol x - \boldsymbol x_c) = 1, \boldsymbol P = \boldsymbol P^T ⪰ 0\}  \tag{Ellipsoid Surface}$$

          - Property
            - Ellipsoid is a convex set

        * Hyperboloid
          - Define  
            令$\boldsymbol A$是非正定矩阵.

        * Paraboloid
          - Define 

        * Cylinder & Cylinder Surface
          - Define
