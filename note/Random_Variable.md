* Random Variable & Random Vector
  - Define  
    Random Variable:
    $$X: Ω \to \mathbb R  \tag{Random Variable}$$
    a random variable $X$ is a function from a sample space $Ω$ to the real numbers $\mathbb R$.

    Symbol:
      - $Ω$: sample space
      - $\mathbb R$: the field of real numbers

    - Note  
      event set $\{ζ | X(ζ) ≤ x\}$, shorthand as $\{X ≤ x\}$.

    Random Vector ; Multivariate random variable:
    $$\boldsymbol X = (X_1, ... , X_n)^T  \tag{Random Vector}$$ 

  - Property
    * Probability Mass function  
      - Define   
        for a discrete Random Variable $X$
        $$\mathbb P(X = x)$$

      - Property
        - Nonnegative
          $$\mathbb P(X = x) > 0$$

        - Sums to 1
          $$\sum_i \mathbb P(X = x_i) = 1$$ 

    * Cumulative Distribution function
      - Define  
        $$
        \begin{align*}
          F_X(x) &= \mathbb P(X ≤ x)  \\
          F_{\boldsymbol X}(x) &= \mathbb P(X_1 ≤ x_1, ..., X_n ≤ x_n) = \mathbb P(\boldsymbol X ≤ \boldsymbol x)
        \end{align*}
        $$

    * Probability Generating Function
      - Define  
        For a discrete random variable $X$ taking values in the non-negative integers $x \in \mathbb Z$, the Probility Generating Function is  
        $$G(z) = \mathbb E(z^x) = \sum_{i=0}^\infty z^x \mathbb P(x)$$ 

      - Property
        - $G(1) = 1$
        - $\mathbb P(X = x) = \frac{G^{(k)}(0)}{x!}$
        - $\mathbb E(X) = G'(1)$
        - $Var(X) = G''(1) + G'(1)(1 - G'(1))$ 
        - $\mathbb E(X^k) = \left(z \frac{\partial }{\partial z}\right) G(z)|_{z = 1}$

    * Probability Density Function
      - Define  
        $$
        \begin{align*}
          f_X(x) &= \frac{\mathrm d F_X(x)}{\mathrm d x}  \\
          f_{\boldsymbol X}(\boldsymbol x) &= \frac{∂^n F_{\boldsymbol X}(x)}{∂ x_1 ... ∂ x_n}  \\
          f_{X_i}(x_i) &= \int_{-\infty}^\infty ... \int_{-\infty}^\infty f_{\boldsymbol X}(\boldsymbol x) \mathrm d x_1 ... \mathrm d x_j ... \mathrm d x_n |_{j ≠ i}  \tag{Marginal Probability Density}
        \end{align*}
        $$

    * Moment
      - Define  
        K-order Moment & K-order Central Moment:
        $$
        \begin{align*}
          \mathbb E(X^k) &= \sum_i x_i^k \mathbb P_X(x_i)  \tag{Moment (Discrete)}  \\
            &= \int_{-\infty}^\infty x^k f_X(x) \mathrm d x  \tag{Moment (Continuous)}  \\
          \mathbb E((X-μ_x)^k) &= \sum_i (x_i-μ_x)^k \mathbb P_X(x_i)  \tag{Central Moment (Discrete)}  \\
            &= \int_{-\infty}^\infty (x-μ_x)^k f_X(x) \mathrm d x  \tag{Central Moment (Continuous)}
        \end{align*}
        $$

        Symbol: $\mu_x = \mathbb E(X)$
          
      - Include
        * Expectation
          - Define
            $$
            \begin{align*}
              \mathbb E(X) = \mu_x &= \sum x_i \mathbb P_X(x_i)  \tag{Discrete}  \\
                &= \int_{-\infty}^\infty x f_X(x) \mathrm d x  \tag{Continuous}  \\
              \mathbb E(\boldsymbol X) &= \left(\begin{matrix} \bar X_i \\ \vdots \end{matrix}\right) 
            \end{align*}
            $$

          - Property
            $$
            \begin{align*}
              \mathbb E(Y) &= \int_{-\infty}^\infty g(x) f_X(x) \mathrm d x  \\
              Y &= g(X)  \tag{$g$ is a measurable function}
            \end{align*}
            $$

        * Variance & Standard Deviation
          - Define
            $$
            \begin{align*}
              Var(X) = \sigma_x^2 &= \mathbb E((X - \mu_x)^2)  \tag{Variance}\\
              \sigma_x &= \sqrt{\mathbb E((X - \mu_x)^2)}  \tag{Standard Deviation}
            \end{align*}
            $$
            $$$$ 

          - Property
            - $Var(X) = \mathbb E((X - \mu_x)^2) = \mathbb E(X^2) - \mathbb E(X)^2$
      
        * Skewness  
          $$\mathbb E\left(\frac{(X - \mu_x)^3}{\sigma_x^3}\right)  \tag{3-order Central Moment}$$ 
          
        * Kurtosis  
          $$\mathbb E\left(\frac{(X - \mu_x)^4}{\sigma_x^4}\right)  \tag{4-order Central Moment}$$  

        * Mixed Moment  
          - Define  
            $$
            \begin{align*}
              &\mathbb E(X^i Y^j) \tag{$ij$-order Joint Moment}  \\
              &\mathbb E((X-\bar X)^i (Y-\bar Y)^j) \tag{$ij$-order Joint Central Moment}
            \end{align*}
            $$

          - Include
            * Correlation & Covariance  
              - Define 
                $$
                \begin{align*}
                  Corr(X,Y) &= \mathbb E(X Y)  &\tag{Correlation}  \\

                  Cov(X,Y) 
                  &= \mathbb E((X-\bar X) (Y-\bar Y))   \tag{Covariance}  \\
                  &= \mathbb E(X Y) - \mathbb E(X) \mathbb E(Y)  \\
                  &= Corr(X,Y) - \mathbb E(X) \mathbb E(Y)  \\

                  R_{\boldsymbol X\boldsymbol X} &= \mathbb E(\boldsymbol X \boldsymbol X^T)  \\
                  &= \left(\begin{matrix} E(X_i X_j) & ... \\ \vdots & \ddots \end{matrix}\right)  \tag{AutoCorrelation matrix}  \\


                  \boldsymbol Σ_{\boldsymbol X\boldsymbol X} &= \mathbb E((\boldsymbol X - \bar{\boldsymbol X}) (X - \bar{\boldsymbol X})^T)  \tag{AutoCovariance matrix}\\
                  &= \left(\begin{matrix} E((X_i - \bar X_i) (X_j - \bar X_j)) & ... \\ \vdots & \ddots \end{matrix}\right)  
                \end{align*}
                $$

              - Include
                * Correlation Coefficient
                  $$\rho = \frac{Cov(X,Y)}{Cov(X,X) Cov(Y,Y)}$$

              - Property
                - $Cov(X,Y) = Cov(Y,X)$
                - $Cov(X,X) = Var(X,X)$
                - relation between AutoCorrelation & AutoCovariance matrix
                  $$\boldsymbol Σ_{\boldsymbol X\boldsymbol X} = \boldsymbol R_{\boldsymbol X\boldsymbol X} - \bar{\boldsymbol X} \bar{\boldsymbol X}^T$$
                - Autocovariance matrix $Σ_{\boldsymbol X\boldsymbol X}$ is a positive semi-definite matrix.
                - $\boldsymbol Y = \boldsymbol A \boldsymbol X \Rightarrow \boldsymbol Σ_{\boldsymbol Y\boldsymbol Y} = \boldsymbol A \boldsymbol Σ_{\boldsymbol X\boldsymbol X} \boldsymbol A^T$

