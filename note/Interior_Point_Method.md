* Interior Point Method
  - Include 
    * Barber Function Method
      - Algorithm  
        利用障碍函数$-\frac{1}{t} \log (-f_i(\boldsymbol x))$, 将不等式约束转换到目标函数中去, 从而转化为等式约束凸优化问题, 求解其最优解$\boldsymbol x^*(t)$. 同时, $\boldsymbol x^*(t)$是与原问题最优解偏差在$\frac{m}{t}$以内的次优解, $t \to \infty, \boldsymbol x^*(t) \to \boldsymbol x^*$.

        $$
        \begin{align*}
          \min \quad & f_0(\boldsymbol x) - \frac{1}{t} \sum_i \log (-f_i(\boldsymbol x))  \\
          s.t. \quad & \boldsymbol A \boldsymbol x = \boldsymbol b
        \end{align*}
        $$

        - 序列无约束极小化方法: 顺序求解一系列障碍函数新问题$\boldsymbol x^*(t)$(无约束极小化问题), 每次用所获得的最新点组为求解下一个问题的初始点, 并逐渐增加$t = t + Δt$直到$t ≥ \frac{m}{ε}$.

        - 步骤 (序列无约束极小化方法)
          - 初始化
            严格可行点$\boldsymbol x$, $t_{(0)}>0$, $Δt$, 误差阈值$ε>0$
          - 循环 ($\frac{m}{t} < ε$时,退出循环)
            从$\boldsymbol x$开始, 求解障碍函数新问题的解 $\boldsymbol x^*(t)$. 改进 $\boldsymbol x = \boldsymbol x^*(t)$, $t = t + Δt$.

      - Note
        - 将不等式约束转换到目标函数中去. $I_{-}(\boldsymbol x)$是指示函数, (1) 当$\boldsymbol x ≤ 0$ 满足原不等式约束, $I_{-}(\boldsymbol x) = 0$对原目标函数不产生影响; (2) 当$\boldsymbol x > 0$ 违背原不等式约束, $f_0(\boldsymbol x) + \sum_i I_{-}(f_i(\boldsymbol x)) = \infty$, 所以该点无法作为最优点. 综上所述, 指示函数的新优化问题与原问题等价.

          $$
          \begin{align*}
            \Rightarrow \min \quad & f_0(\boldsymbol x) + \sum_i I_{-}(f_i(\boldsymbol x))   \\
              s.t. \quad & \boldsymbol A \boldsymbol x = \boldsymbol b
          \end{align*}
          $$

          $$
          \begin{align*}
            I_{-}(\boldsymbol x) = \left\{\begin{matrix}
              0  \quad , \boldsymbol x ≤ 0  \\
              \infty  \quad , \boldsymbol x > 0
            \end{matrix}\right.  \tag{Indicator Function}
          \end{align*}
          $$

        - 但是, 指示函数不可微, 因此想办法用可微的函数去拟合指示函数, 来达到相同效果. 对于每一个设置的$t$, 都有一个问题的最优解$x^*(t)$与之对应. $t$越大, 对$I_{-}(x)$拟合的越好, 新的优化问题的解就越接近于原问题.
          $$
            I_{-}(x) \approx -\frac{1}{t} \log (-x)  \tag{$t>0$}
          $$

          $$
          \begin{align*}
            \Rightarrow \min \quad & f_0(\boldsymbol x) - \frac{1}{t} \sum_i \log (-f_i(\boldsymbol x))  \\
              s.t. \quad & \boldsymbol A \boldsymbol x = \boldsymbol b
          \end{align*}
          $$

        - KKT condition (障碍函数新问题)  
          $$
          \begin{align*}
            \boldsymbol A \boldsymbol x^*(t) &= \boldsymbol b  \\
            ∇ L &= ∇ f_0(\boldsymbol x^*(t)) + \frac{1}{t} ∇(- \sum_i \log (-f_i(\boldsymbol x))) + \boldsymbol A^T \boldsymbol v^*(t)  \\
              &= ∇ f_0(\boldsymbol x^*(t)) + \frac{1}{t} \sum_{i=1}^m \frac{∇ f_i(\boldsymbol x^*(t))}{-f_i(\boldsymbol x^*(t))} + \boldsymbol A^T \boldsymbol v^*(t)  \\
              &= 0
          \end{align*}
          $$

          原问题KKT condition, 与障碍函数新问题 KKT condition, 对比如下. 令$λ_i^*(t) = \frac{1}{-t f_i(\boldsymbol x^*(t))}$, 则二者表达式一样.
          $$
          \begin{align*}
            \boldsymbol A \boldsymbol x^*(t) &= \boldsymbol b  \\
            ∇ f_0(\boldsymbol x^*(t)) + \frac{1}{t} \sum_{i=1}^m \frac{∇ f_i(\boldsymbol x^*(t))}{-f_i(\boldsymbol x^*(t))} + \boldsymbol A^T \boldsymbol v^*(t) &= 0  \\
            ∇ f_0(\boldsymbol x^*) + \sum_{i=1}^m λ_i^* ∇ f_i(\boldsymbol x^*) + \boldsymbol A^T \boldsymbol v^* &= 0
          \end{align*}
          $$

        - $\boldsymbol x^*(t)$是与原问题最优解偏差在$\frac{m}{t}$以内的次优解, $t \to \infty, \boldsymbol x^*(t) \to \boldsymbol x^*$
          - Proof  
            只要证 $0 ≤ f_0(\boldsymbol x^*(t)) - f_0(\boldsymbol x^*) ≤ \frac{m}{t}$.  
            上文原问题与障碍函数新问题KKT condition对比可得, $\boldsymbol x^*(t)$使得$L(\boldsymbol x, \boldsymbol λ^*(t), \boldsymbol v^*(t))$在其位置达到极小值, 
            $$∇ L(\boldsymbol x^*(t), \boldsymbol λ^*(t), \boldsymbol v^*(t)) = 0$$

            $$
            \begin{align*}
              \Rightarrow \quad  g(\boldsymbol λ^*(t), \boldsymbol v^*(t)) &= \inf_{x \in D} L(\boldsymbol x, \boldsymbol λ^*(t), \boldsymbol v^*(t))  \\
                &= L(\boldsymbol x^*(t), \boldsymbol λ^*(t), \boldsymbol v^*(t))  \\
                &= f_0(\boldsymbol x^*(t)) + \sum_{i=1}^m λ^*_i(t) f_i(\boldsymbol x^*(t)) + \boldsymbol v^*(t)^T (\boldsymbol A^T \boldsymbol x^*(t) - \boldsymbol b)  \\
                &= f_0(\boldsymbol x^*(t)) + \sum_{i=1}^m \frac{1}{-t \boldsymbol x^*(t)} f_i(\boldsymbol x^*(t))  \tag{$\boldsymbol A^T \boldsymbol x^*(t) - \boldsymbol b = 0$}  \\
                &= f_0(\boldsymbol x^*(t)) - \frac{m}{t}
            \end{align*}
            $$

            又$\because$ 强对偶性有, $g(\boldsymbol λ^*, \boldsymbol v^*) = f_0(\boldsymbol x^*)$
            $$
            \begin{align*}
              \therefore \quad & g(\boldsymbol λ^*(t), \boldsymbol v^*(t)) ≤ g(\boldsymbol λ^*, \boldsymbol v^*) = f_0(\boldsymbol x^*)  \\
              \Rightarrow \quad &  f_0(\boldsymbol x^*(t)) - \frac{m}{t} ≤ f_0(\boldsymbol x^*)  \tag{Substitution}  \\
              \Rightarrow \quad &  f_0(\boldsymbol x^*(t)) - f_0(\boldsymbol x^*) ≤ \frac{m}{t}
            \end{align*}
            $$

            又$\because$ 原问题目标函数是$\min f_0(\boldsymbol x)$
            $$
            \begin{align*}
              \Rightarrow \quad &  f_0(\boldsymbol x^*(t)) ≥ f_0(\boldsymbol x^*)  \\
              \Rightarrow \quad &  0 ≤ f_0(\boldsymbol x^*(t)) - f_0(\boldsymbol x^*) ≤ \frac{m}{t}  \\
            \end{align*}
            $$

    * Original Dual Method
