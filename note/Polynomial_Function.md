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
      - Define
        $$
        \begin{align*}
          f(x) &= a x + b \tag{一元一次}  \\
          f(\boldsymbol x) &= \boldsymbol A \boldsymbol x + \boldsymbol b  \tag{多元一次}
        \end{align*}
        $$

      - Property
        - 线性函数的零点集, Solving Linear Equations
          - Problem  
            $$\boldsymbol A \boldsymbol x = \boldsymbol b$$

            Know $\boldsymbol A, \boldsymbol b$ and Solve $\boldsymbol x$. Where, $\boldsymbol A \in \mathbb R^{m \times n}$, $\boldsymbol x \in \mathbb R^{n}$, $\boldsymbol b \in \mathbb R^m$, $n$ is the number of unknown number, $m$ is the number of linear equations.

          - Property : Existence of Solutions  
            $$\begin{align*}
              \left\{\begin{matrix}
                rank((\boldsymbol A\ \boldsymbol b)) = rank(\boldsymbol A) &= n& \quad \text{Unique Solution}  \\
                rank((\boldsymbol A\ \boldsymbol b)) = rank(\boldsymbol A) &< n& \quad \text{Infinite Solutions}  \\
                rank((\boldsymbol A\ \boldsymbol b)) > rank(\boldsymbol A) &&\quad \text{Unsolvable}
              \end{matrix}\right.
            \end{align*}$$

            - Proof  
              if $rank((\boldsymbol A\ \boldsymbol b)) > rank(\boldsymbol A)$  
              $\Rightarrow$ $(\boldsymbol a_1,...,\boldsymbol a_n)$ and $b$ is linearly independent  
              $\Rightarrow$ $\nexists \boldsymbol x \in \mathbb R^{n}$ let $x_1 \boldsymbol a_1 + ... + x_n \boldsymbol a_n = b$.  
              $\Rightarrow$ Unsolvable

              if $rank((\boldsymbol A\ \boldsymbol b)) = rank(\boldsymbol A) < n$  
              $\Rightarrow$ $\exists u \in \mathbb R^{n-1}, k \in [1, n]$ let $\boldsymbol a_k = \sum\limits_{i=1,i\neq k}^n u_i \boldsymbol a_i$  
              $\Rightarrow$ $\exists c_1, c_2 \in \mathbb R, c_1 + c_2 = 1$ and $\exists x^*$ is a special solution, let $b = \left(c_1 x_k^* a_k + c_2 x_k^* \sum\limits_{i=1,i\neq k}^n u_i \boldsymbol a_i \right) + \sum\limits_{i=1,i\neq k}^n x_i^* \boldsymbol a_i$  
              $\Rightarrow$ Infinite Solutions

              for the same reason, if $rank((\boldsymbol A\ \boldsymbol b)) = rank(\boldsymbol A) = n$  
              $\Rightarrow$ Unique Solution

          - Solving  
            - Unique Solution  
              if $rank(\boldsymbol A) = n$ , then $\boldsymbol A^{-1}$ exists and
              $$\boldsymbol x = \boldsymbol A^{-1} \boldsymbol b$$
              
            - Infinite Solutions    
              General Solution $\boldsymbol x = \boldsymbol A^{-^{\{1\}}} \boldsymbol b + (\boldsymbol I - \boldsymbol A^{-^{\{1\}}} \boldsymbol A) \boldsymbol c$  
              Special Solution $\boldsymbol x = \boldsymbol A^{-^{\{1\}}} \boldsymbol b$  

              Minimal Norm Solution. The minimum norm solution is unique.
              $$\begin{align*}
                \min_{\boldsymbol x} \quad& ||\boldsymbol x||_2  \\
                s.t. \quad& \boldsymbol A \boldsymbol x = \boldsymbol b
              \end{align*}$$
              $$\boldsymbol x = \boldsymbol A^{-^{\{1,3\}}} \boldsymbol b$$
                
            - Unsolvable  
              Approximate solution $\tilde{\boldsymbol x}$ by least square,
              $$\min_{\tilde{\boldsymbol x}} \quad ||\boldsymbol A \tilde{\boldsymbol x} - \boldsymbol b||_2$$
              $$\tilde{\boldsymbol x} = \boldsymbol A^{-^{\{1,4\}}} \boldsymbol b$$

              Minimal norm Approximate solution $\tilde{\boldsymbol x}$ by least square,
              $$\begin{align*}
                \min_{\tilde{\boldsymbol x}} \quad& ||\boldsymbol A \tilde{\boldsymbol x} - \boldsymbol b||_2  \\
                \min_{\tilde{\boldsymbol x}} \quad& ||\tilde{\boldsymbol x}||_2
              \end{align*}$$
              
              $$\tilde{\boldsymbol x} = \boldsymbol A^+ \boldsymbol b$$

    * 二次函数
      - Define
        $$
        \begin{align*}
          f(x) &= a x^2 + b x + c  \tag{一元二次}  \\
          f(\boldsymbol x) &= \boldsymbol x^T \boldsymbol A \boldsymbol x + \boldsymbol b \boldsymbol x + c  \tag{多元二次, 矢量式}  \\
           &= \sum_{i=1}^{\dim} \sum_{j=1}^{\dim} a_{ij} · x_i x_j + \sum_{i=1}^{\dim} b_i x_i + c  \tag{分量式}
        \end{align*}
        $$

      - Property
        * 二次函数的零点集 (二次曲面)
          - Define  
            二次曲面:  
            $$
            \begin{align*}
              &\{ \boldsymbol x \ |\ \boldsymbol x^T \boldsymbol A \boldsymbol x + \boldsymbol b \boldsymbol x + c = 0\} \\
            \Leftrightarrow &\{ \boldsymbol x' \ |\ \boldsymbol x'^T \boldsymbol A' \boldsymbol x' = 0 \ |\  \boldsymbol x' = \left(\begin{matrix} \boldsymbol x \\ 1 \end{matrix}\right)\}
            \end{align*}
            $$

          - 解二次方程
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
            * Euclid Sphere
              - Define  
                令$\boldsymbol A = \boldsymbol I, \boldsymbol b = \boldsymbol 0, c = -r^2$
                $$
                \begin{align*}
                  &\{ \boldsymbol x \ |\ ||\boldsymbol x - \boldsymbol x_c||_2 ≤ r, r>0\}  \\
                \Leftrightarrow &\{ \boldsymbol x \ |\ (\boldsymbol x - \boldsymbol x_c)^T (\boldsymbol x - \boldsymbol x_c) ≤ r^2, r>0\}  \tag{等价}  \\
                \Leftrightarrow &\{ \boldsymbol x_c + r \boldsymbol u \ |\ ||\boldsymbol u||_2 ≤ r, r>0\}  \tag{等价}
                \end{align*}
                $$
                距离中心点$\boldsymbol x_c$为恒定值$r$的点的集合.

              - Property
                - 是凸集

            * 椭球
              - Define  
                令$\boldsymbol A = \boldsymbol P^{-1}, \boldsymbol b = \boldsymbol 0, c = -1$是正定矩阵.
                $$
                \begin{align*}
                  &\{ \boldsymbol x \ |\ (\boldsymbol x - \boldsymbol x_c)^T P^{-1} (\boldsymbol x - \boldsymbol x_c) ≤ 1, \boldsymbol P = \boldsymbol P^T ⪰ 0\}  \\
                  \Leftrightarrow &\{ \boldsymbol x_c + A \boldsymbol u \ |\ ||\boldsymbol u||_2 ≤ 1\}  \tag{等价}
                \end{align*}
                $$

              - Property
                - 是凸集

            * 双曲体
              - Define  
                令$\boldsymbol A$是非正定矩阵.

            * 柱体
              - Define

    * 三次函数
      - Define  
        $$
          f(x) = \sum_{i=0}^{3} a_i x^i  \tag{一元}
        $$
        
      - Property
        * 三次函数的零点集 (三次曲面)
          - Define

          - 解三次方程
            - Problem

            - Answer
              一元三次


    * 四次函数
      - Define
        $$
        \begin{align*}
          f(x) &= \sum_{i=0}^{4} a_i x^i  \tag{一元}  \\
          f(\boldsymbol x) =& \sum_{i_1=1}^{\dim} \sum_{i_2=1}^{\dim} \sum_{i_3=1}^{\dim} \sum_{i_4=1}^{\dim} a_{i_1 i_2 i_3 i_4} · x_{i_1} x_{i_2} x_{i_3} x_{i_4} +    \\
            &\sum_{i_1=1}^{\dim} \sum_{i_2=1}^{\dim} \sum_{i_3=1}^{\dim} b_{i_1 i_2 i_3} · x_{i_1} x_{i_2} x_{i_3} +    \\
            &\sum_{i_1=1}^{\dim} \sum_{i_2=1}^{\dim} c_{i_1 i_2} · x_{i_1} x_{i_2} +    \\
            &\sum_{i_1=1}^{\dim} d_{i_1} x_{i_1} +    \\
            &e  \tag{多元, 分量式}
        \end{align*}
        $$

      - Property
        * 四次函数的零点集 (四次曲面)
          - Define

          - 解四次方程
            - Problem

            - Algorithm
              - 一元四次

          - Include
            * 圆环
          