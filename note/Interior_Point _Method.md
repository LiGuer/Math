* Interior Point Method
  - Porpose    
    Simplify the problem by trasforming inequality constraints $f_i(x) \le 0$ into the objective function $f_0(x)$ through Indicator function $I_- (x)$. Therefore, the simplified problem is an Optimization Problems only with Equality Constraints.

    $$\begin{align*}
      I_- (x) = \left\{\begin{matrix}  
        0\quad & ; x \le 0\\
        \infty \quad &; x > 0
      \end{matrix}\right.  \tag{Indicator Function}
    \end{align*}$$
    
    $$\begin{align*}
      \Rightarrow \min_{\boldsymbol x} \quad & f_0 (\boldsymbol x) - \sum_i I_- (f_i(\boldsymbol x)) \\
      s.t. \quad & A x = b
    \end{align*}$$

    However, since the Indicator function $I_- (x)$ is non-differentiable, we use the Barrier function to approximate the Indicator function. Meanwhile, the optimal solution of the new problem $\boldsymbol x^*(t)$ is a suboptimal solution whose deviation from the original optimal solution $x^*$ within $\frac{m}{t}$, and $t \to \infty \Rightarrow \boldsymbol x^*(t) \to \boldsymbol x^*$,
    $$I_- (x) \simeq \frac{1}{t} \log(-x)$$

    $$\begin{align*}
      \Rightarrow \min_{\boldsymbol x^*(t)} \quad & f_0 (\boldsymbol x) - \frac{1}{t} \sum_i \log(-f_i(\boldsymbol x)) \\
      s.t. \quad & A x = b
    \end{align*}$$