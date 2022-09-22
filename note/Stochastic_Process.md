* Stochastic Process  
  - Define  
    The Probability Space $Ω, \mathcal F, P$, Mapping $X(t, ζ)$ from $Ω$ to a Continuous Time Function Space.

    - Note
      - When the time $t$ is fixed, the random process degenerates into a random variable.
      - When the random sample $ζ$ is determined, the random process degenerates into a continuous time function

  - Property
    - Correlation  
      When not relevant,
      $$R_{XY}(t_1, t_2) = μ_X(t_1) μ_Y^*(t_2), \quad \forall t_1, t_2$$

    - Orthogonality  
      $$R_{XY}(t_1, t_2) = 0, \quad \forall t_1, t_2$$

    - Independence

    - Stationarity
      $$\mathbb P (x_{t_1}, ... , x_{t_n}) = \mathbb P (x_{t_1+τ}, ..., x_{t_n+τ}) \quad ;\forall τ, t_1, ..., t_n \in \mathbb R, \quad n \in \mathbb N$$
      Time shift invariance, Statistical properties do not change over time.
      
      - Note  
        The random sampling at any interval and in any order on the time series has time shift invariance.

    - Generalized Stationarity   
      - Power Spectral Density  
        Power Spectral Density, is the Fourier transform of autocorrelation function of generalized stationary process.

        - Property
          - is real numbers
          - $≥ 0$

    - Markov property

  - Example
    - Simple process
      $$X(t, ζ) = X(ζ) f(t)$$

    - Random Sine Wave
      $$X(t, ζ) = A(ζ) \sin(\omega_0 t + \Theta(ζ))$$
      
    - Possion Counting process
      $$N(t) = \sum_{n=1}^\infty u(t - T(n)) \quad; f_T(t;n) = \frac{(λ t)^{n-1}}{(n-1)!} λ e^{-λ t} u(t)$$
