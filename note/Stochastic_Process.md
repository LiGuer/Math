* Stochastic Process  
  - Define  
    A Stochastic Process $X(t, \omega), \omega \in \Omega, t \in T$ is defined as a collection of random variables defined on a Probability Space $(Ω, \mathcal F, P)$, and these random variables indexed by set $T$.

    - Note
      - When the time $t$ is fixed, the random process degenerates into a random variable.
      - When the random sample $ζ$ is determined, the random process degenerates into a continuous time function

  - Property
    * Correlation function & Covariance function
      - Define  
        $$
        \begin{align*} 
          Corr(X(t_1), Y(t_2)) 
          &= \mathbb E(X(t_1) Y(t_2))  \tag{Correlation function}\\

          Corr(X(t_1), X(t_2)) 
          &= \mathbb E(X(t_1) X(t_2))  \tag{AutoCorrelation function}\\

          Cov(X(t_1), Y(t_2)) 
          &= \mathbb E((X(t_1) - \mu_X(t_1)) (Y(t_2) - \mu_Y(t_2)))  \tag{Covariance function}\\
          &= \mathbb E(X(t_1) X(t_2)) - \mu_X(t_1) \mu_Y(t_2)  \\
          &= Corr(X(t_1), Y(t_2)) - \mu_X(t_1) \mu_Y(t_2)  \\

          Cov(X(t_1), X(t_2)) 
          &= \mathbb E((X(t_1) - \mu(t_1)) (X(t_2) - \mu(t_2)))  \tag{AutoCovariance function}\\
          &= \mathbb E(X(t_1) X(t_2)) - \mu(t_1) \mu(t_2)  \\
          &= Corr(X(t_1), X(t_2)) - \mu(t_1) \mu(t_2) 
        \end{align*} 
        $$

      - Property  
        - $X, Y \ \text{Uncorrelated} \Leftrightarrow Cov(X(t_1), Y(t_2)) = 0, \quad \forall t_1, t_2$

        - $X, Y \ \text{Orthogonality} \Leftrightarrow Corr(X(t_1), Y(t_2)) = 0, \quad \forall t_1, t_2$

    - Independence

    * Strictly Stationarity
      - Define 
        $$\mathbb P (x(t_1), ... , x(t_n)) = \mathbb P (x(t_1+τ), ..., x_(t_n+τ)) \quad ;\forall τ, t_1, ..., t_n \in \mathbb R, \quad n \in \mathbb N$$
        Time shift invariance, Statistical properties do not change over time.

        - Note  
          The random sampling at any interval and in any order on the time series has time shift invariance.

    * Weak-sense Stationarity
      - Define 
        $$R_{XX}(t_1 - t_2 ,0) = R_{XX}(t_1, t_2) \quad; \forall t_1, t_2 \in \mathbb R$$ 
  
      - Property
        - Power Spectral Density  
          Power Spectral Density is the Fourier transform of autocorrelation function of Weak-sense stationary process.

          Property:
          - is real numbers
          - $≥ 0$

  - Include
    * Markov Chain 
    * Gaussian Process

  - Example
    - Simple process
      $$X(t, ζ) = X(ζ) f(t)$$

    - Random Sine Wave
      $$X(t, ζ) = A(ζ) \sin(\omega_0 t + \Theta(ζ))$$
      
    - Possion Counting process
      $$N(t) = \sum_{n=1}^\infty u(t - T(n)) \quad; f_T(t;n) = \frac{(λ t)^{n-1}}{(n-1)!} λ e^{-λ t} u(t)$$
