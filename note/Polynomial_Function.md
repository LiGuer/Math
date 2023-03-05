* Polynomial Function
  - Define
    $$
    \begin{align*}
      f(x) &= \sum_{i=0}^{n} a_i x^i  \tag{one variable}  \\
      f(\boldsymbol x) &= \sum_{\boldsymbol i=(0,...,0)_n}^{(\dim,...,\dim)_n} \left(a_{\boldsymbol i} · \prod_{i_j \in \boldsymbol i, x_0 = 1}x_{i_j} \right)  \tag{multi-variate}  
    \end{align*}
    $$

  - Include
    * Linear Function  
    * Quadratic Function
    * Cubic Function
      - Define  
        $$
          f(x) = \sum_{i=0}^{3} a_i x^i  \tag{Univariate}
        $$
        
      - Property
        * 三次函数的零点集 (三次曲面)
          - Define

          - 解三次方程
            - Problem

            - Answer
              一元三次


    * Forth-Order Function
      - Define
        $$\begin{align*}
          f(x) &= \sum_{i=0}^{4} a_i x^i  \tag{Univariate}  \\
          f(\boldsymbol x) =& \sum_{i_1=1}^{\dim} \sum_{i_2=1}^{\dim} \sum_{i_3=1}^{\dim} \sum_{i_4=1}^{\dim} a_{i_1 i_2 i_3 i_4} · x_{i_1} x_{i_2} x_{i_3} x_{i_4} +    \\
            &\sum_{i_1=1}^{\dim} \sum_{i_2=1}^{\dim} \sum_{i_3=1}^{\dim} b_{i_1 i_2 i_3} · x_{i_1} x_{i_2} x_{i_3} +    \\
            &\sum_{i_1=1}^{\dim} \sum_{i_2=1}^{\dim} c_{i_1 i_2} · x_{i_1} x_{i_2} +    \\
            &\sum_{i_1=1}^{\dim} d_{i_1} x_{i_1} +    \\
            &e  \tag{Multivariate}
        \end{align*}$$

      - Property
        * 四次函数的零点集 , Forth-Order Surface
          - Define

          - Solve Forth-Order equation
            - Problem

            - Algorithm
              - 一元四次

          - Include
            * Torus
              - Define  
                $$\{\boldsymbol x \ |\ (R^2 - r^2 + \boldsymbol x^T \boldsymbol x)^2 - 4 R^2 (\boldsymbol x^T \boldsymbol x - x_i^2) = 0\}  \tag{Torus}$$  

            * Tanglecube
              - Define
                $$\sum_{i=1}^3 \left(x_i^4 - 5 x_i^2 \right) + 11.8 = 0  \tag{Tanglecube}$$   
          