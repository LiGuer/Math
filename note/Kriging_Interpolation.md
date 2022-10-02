* Kriging Interpolation 
  - Include
    * Ordinary Kriging Interpolation 


* Ordinary Kriging Interpolation 
  - Purpose  
    Input: $\{\boldsymbol x_1, ..., \boldsymbol x_n\}, \{Z_1(\boldsymbol x_1), ..., Z_n(\boldsymbol x_n)\}, \boldsymbol x$

    Spatial Interpolation, and meets the assumptions: (Tobler's First Law of Geography) the Space attribute $Z$ is uniform. For any point in space, there are same Expectation $\mu$, Variance $\sigma ^2$.
    $$Z (\boldsymbol x) = \mu + \epsilon$$
    $$
    \begin{align*}
      \mathbb E\left(Z (\boldsymbol x)\right) &= \mu \\
      Var(Z (\boldsymbol x)) &= Var(\epsilon) = \sigma ^2
    \end{align*}
    $$    

    This problem is to solve the $\tilde Z(\boldsymbol x)$ by finding weights $\boldsymbol w = (w_1, ..., w_n)^T$ with the Interpolation point $\boldsymbol x$, and minimizing the difference between Estimated value $\tilde Z(\boldsymbol x)$ and Actual value $Z(\boldsymbol x)$.  

    $$\tilde Z(\boldsymbol x) = \sum_i w_i(x) Z_i(\boldsymbol x_i)$$
    $$
    \begin{align*}
      \min_{\boldsymbol w} \quad & Var(\tilde Z - Z)  \tag{Minimize Estimation Error}\\
      s.t. \quad & \mathbb E(\tilde Z - Z) = 0  \tag{Unbiased Estimation}
    \end{align*}
    $$

    |Symbol|Mean|
    |---|---|
    |$n$|the number of sample points|
    |$\boldsymbol x$|position of purpose Interpolation point|
    |$\boldsymbol x_i$|position of sample points|
    |$\tilde Z$|Estimated value of $\boldsymbol x$|
    |$Z$|Actual value of $\boldsymbol x$|
    |$Z_i$|Sample value of position $\boldsymbol x_i$|
    |$\mu$|Expectation|
    |$\sigma ^2$|Variance|
    |$\boldsymbol w = (w_1, ..., w_n)^T$|weights|
    |$\gamma_{ij}$|the Semi-Variogram $\gamma(x_i, x_j)$|
    |$\boldsymbol \Gamma \in \mathbb R^{n \times n}$|the Matirx of Semi-Variogram $\gamma_{ij}$|
    |$\boldsymbol \gamma_x \in \mathbb R^{n}$|the Vector of Semi-Variogram $\gamma(x_i, x)$|
    |$\gamma_{00}$|the Semi-Variogram $\gamma(x, x)$|
    |||

    Set the Semi-Variogram $\gamma(x_i, x_j)$
    $$
    \begin{align*}
      γ(x_i, x_j)
      &= \frac{1}{2} \mathbb E((Z_i - Z_j)^2)  \tag{Semi-Variogram}\\
      &= σ^2 - Corr(Z_i, Z_j)  \\
    \end{align*}
    $$

    Therefore, 
    $$ 
    \begin{align*}
      \Rightarrow \min_{\boldsymbol w} \quad& 2 \boldsymbol w^T \boldsymbol \gamma_x - \boldsymbol w^T  \boldsymbol \Gamma \boldsymbol w - \gamma_{00}\\
      s.t. \quad 
      & \boldsymbol 1^T \boldsymbol w = 1
    \end{align*}
    $$ 

    - Proof  
      $\mathbb E\left(\tilde Z - Z\right)$:
      $$
      \begin{align*}
        \mathbb E\left(\tilde Z - Z\right) 
        &= \mathbb E\left(\left(\sum_i w_i Z_i\right) - Z\right)  \\
        &= \mu \left(\sum_i w_i\right) - \mu    \\
        &= 0  \\
        \Rightarrow \sum w_i &= 1
      \end{align*}
      $$

      $Var(\tilde Z - Z)$:
      $$
      \begin{align*}
        Var(\tilde Z - Z) 
        &= \mathbb E\left(\left(\left(\sum_i w_i Z_i\right) - Z\right)^2\right)  \\
        &= \mathbb E \left(\left(\sum_i w_i Z_i\right)^2\right) - 2·\mathbb E \left(Z · \sum_i w_i Z_i \right) + \mathbb E(Z^2)  \\
        &= \mathbb E \left(\left(\sum_i w_i Z_i\right) \left(\sum_j w_j Z_j\right) \right) - 2·\left(\sum_i w_i · \mathbb E \left(Z Z_i \right)\right) + \mathbb E(Z^2)  \\
        &= \left(\sum_i \sum_j w_i w_j \mathbb E \left( Z_i   Z_j\right) \right) - 2·\left(\sum_i w_i · \mathbb E \left(Z Z_i \right)\right) + \mathbb E(Z^2)  \\
        &= \left(\sum_i \sum_j w_i w_j Corr\left(Z_i,Z_j\right)\right) - 2\left(\sum w_i Corr\left(Z_i,Z\right)\right) + Corr\left(Z,Z\right)
      \end{align*}
      $$

      Define the Semi-Variogram $γ_{ij}$
      $$
      \begin{align*}
        γ_{ij} 
        &= \frac{1}{2} \mathbb E((Z_i - Z_j)^2)  \\
        &= \frac{1}{2} (\mathbb E(Z_i^2) + \mathbb E(Z_j^2) - 2 \mathbb E(Z_i Z_j))  \\
        &= σ^2 - Corr(Z_i, Z_j)  \\

        \Rightarrow Corr(Z_i, Z_j) &= σ^2 - γ_{ij}  \\
      \end{align*}
      $$

      $$
      \begin{align*}
        \Rightarrow Var(\tilde Z - Z) 
        =& \left(2\left(\sum_i w_i \gamma_{i0}\right) - \left(\sum_i \sum_j w_i w_j \gamma_{ij}\right) - \gamma_{00}\right)  -\\
        & \left(2\left(\sum_i w_i σ^2\right) - \left(\sum_i \sum_j w_i w_j σ^2\right) - σ^2\right)  \tag{Substitution}\\
        =& 2\left(\sum_i w_i \gamma_{i0}\right) - \left(\sum_i \sum_j w_i w_j \gamma_{ij}\right) - \gamma_{00}  \tag{$2 σ^2 - σ^2 - σ^2 = 0$}\\
        =& 2 \boldsymbol w^T \boldsymbol \gamma_x - \boldsymbol w^T  \boldsymbol \Gamma \boldsymbol w - \gamma_{00}
      \end{align*}
      $$


  - Solution  
    Calculate weights $\{w_1, ..., w_n\}$  
    $$
    \begin{align*}
      \left(\begin{matrix} w_1 \\ \vdots \\ w_n \\ \mu  \end{matrix}\right) = \left(\begin{matrix} \gamma \left(x_1,x_1\right) & ... & \gamma \left(x_1,x_n\right) & 1 \\ \vdots & \ddots & \vdots & \vdots \\  \gamma \left(x_n,x_n\right) & ... & \gamma \left(x_n,x_n\right) & 1 \\ 1 & ... & 1 & 0 \end{matrix}\right)^{-1} \left(\begin{matrix} \gamma \left(x_1,x\right) \\ \vdots \\ \gamma \left(x_n,x\right) \\ 1 \end{matrix}\right)
    \end{align*}
    $$

    - Proof
      $$
      \begin{align*}
        L(w_i, λ_i) &= (2 \boldsymbol w^T \boldsymbol \gamma_x - \boldsymbol w^T  \boldsymbol \Gamma \boldsymbol w - \gamma_{00}) + λ_i \left(\sum_i w_i - 1 \right)  \tag{Lagrange function}\\
        λ_i &\ge 0\\
        G(λ) &= \inf L(w_i, λ)  \tag{Lagrangian dual}
      \end{align*}
      $$

      $L(W,λ)$求导, 当导数为0时, 取得极值. 即. 得到权值计算方程.

  - Procedure
    - Calculate the Semi-Variogram $γ_{ij}, \Gamma, γ_{x}, γ_{00}$
    - Calculate weights $\{w_1, ..., w_n\}$  
    - 计算 Interpolation 点结果 $Z(x) = \sum_i w_i(x) Z_i$
