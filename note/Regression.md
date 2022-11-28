* Regression
  - Purpose  
    Input: $\{y_i, \boldsymbol x_i\}_{i=1}^n, \boldsymbol x_i \in \mathbb R^{\dim}$

    Regression problems aims to study the relationship between the independent variable $\boldsymbol x$ and the dependent variable $y$ by finding a function form $y = f(\boldsymbol x, \boldsymbol w)$ and minimizing the error under certain criteria.  

    $$y_i = f(\boldsymbol x_i, \boldsymbol w) + \epsilon_i$$

    |Symbol|Significance|
    |---|---|
    | $\boldsymbol x$ | independent variable|
    | $y$| dependent variable|
    | $\boldsymbol w$| unknown parameter|
    | $\epsilon$| error|
    |||

  - Include
    * Linear Regression
      - Purpose  
        Linear Regression assumes that the relationship between $y$ and $\boldsymbol x$ is linear and models them with the form
        $$\begin{align*}
            y_i &= \left(w_0 + \sum_{d=1}^{\dim} w_d x_{id}\right) + \epsilon_i  \\
            \boldsymbol y &= \left(\begin{matrix} 1 & \boldsymbol x_1^T \\ \vdots & \vdots \\ 1 & \boldsymbol x_n^T \end{matrix}\right) \boldsymbol a + \boldsymbol \epsilon
        \end{align*}$$ 

        $\boldsymbol w \in \mathbb R^{\dim + 1}$: parameters.  
        $\epsilon \in \mathbb R^{n}$: error variable.

    * Gaussian Process Regression

  - Algorithm -- Solve the parameters $\boldsymbol w$
    * Least Square Method  
      $$\min_{\boldsymbol w} \quad \sum_{i=0}^n (f(\boldsymbol x_i, \boldsymbol w) - y_i)^2$$