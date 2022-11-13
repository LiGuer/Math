* Differential Equations
  * Ordinary Differential Equation  

    - Algorithm: Runge Kutta Method  
      常微分方程组
        $$\begin{align*}
          \boldsymbol y' &= f(\boldsymbol  x, \boldsymbol  y)  \\
          \boldsymbol  y(\boldsymbol  x_0) &= \boldsymbol  y_0
        \end{align*}$$
        迭代求解 $\boldsymbol  y(x)$ 的一点/一区间的数值解.
        $$\begin{align*}
          y(x + dx) &= y(x) + \frac{dx}{6} · (k_1 + 2 k_2 + 2 k_3 + k_4)  \\
          k_1 &= f \left(x_n , y_n \right)            \tag{区间开始斜率}  \\
          k_2 &= f \left(x_n + \frac{dx}{2}, y_n + \frac{dx}{2}·k_1 \right)    \tag{区间中点斜率,通过欧拉法采用$k_1$决定y在$x_n+\frac{dx}{2}$值}  \\
          k_3 &= f \left(x_n + \frac{dx}{2}, y_n + \frac{dx}{2}·k_2 \right)    \tag{区间中点斜率,采用$k_2$决定y值}  \\
          k_4 &= f \left(x_n + dx, y_n + dx·k_3 \right)      \tag{区间终点斜率}
        \end{align*}$$

      - Property  
        RK4法是四阶方法，每步误差是h⁵阶，总积累误差为h⁴阶

  * Partial Differential Equation
    - Define  
      $$F(D^k u(x), ... , D^2 u(x), u(x), x) = 0$$
      $$F: R^{n^k} × R^{n^{k-1}} × R^n × R × \Omega \to R \quad; x \in \Omega; u: \Omega \to R$$

    - Include
      * Linear partial differential equation
        - Define
          $$\sum_{k=0}^K a_k(x) D^k u(x) = f(x)$$

      * Second order nonlinear partial differential equation
        - Define
          $$\sum_{ij} a_{ij}(x) \frac{∂^2 u}{∂ x_i ∂ x_j} + \sum_i b_i(x) \frac{∂ u}{∂ x_i} + c(x) u(x) = f(x)$$
          系数矩阵 $A(x) = (a_{ij}(x))_{m×m}$

        - Include

          * Elliptic partial differential equations
            - Define  
              指$A(x)$负定.

            - Include
              * Poisson's equations
                - Define
                  $$-\nabla^2 \phi = f$$
                * Laplace's方程
                  $$\nabla^2 \phi = 0$$

          * Hyperbolic partial differential equation
            - Define  
              指$A(x)$的特征值由1个0和其他负数组成.

            - Include
              * Diffusion equation
                - Define
                  $$\frac{∂u}{∂t} - a \nabla^2 u = f(x,t)$$

          * Parabolic partial differential equation
            - Define
              指$A(x)$的特征值由1个正数和其他负数组成.

            - Include
              * Wave equation
                - Define
                  $$\frac{∂^2 u}{∂t^2} - a \nabla^2 u = f(x,t)$$

      * 半线性偏微分方程
      * 拟线性偏微分方程
      * 完全非线性偏微分方程





