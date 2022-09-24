* Statistics
  - Define
    - Population 
    - Sampling 

  - Problem 
    * Parameter Estimation
      - Include 
        * Point Estimation
        * Interval estimation
    * Hypothesis Testing


* Point Estimation  
  - Define  
    Best estimate with a single value $\hat \theta$ of an unknown population parameter $\theta$ from samples $\{X_1, ..., X_n\}$.

  - Property
    - Biasness
      $$bias(\hat \theta) = \mathbb E(\hat \theta) - \theta$$ 

      $$
      \begin{align*}
        \mathbb E(\hat \theta) - \theta = 0  \tag{unbiased}\\
        \left(\lim_{m \to \infty} \mathbb E(\hat \theta_m)\right) - \theta = 0  \tag{asymptotically unbiased}\\
      \end{align*}
      $$

      - Proof  
        Sample Mean:
        $$
        \begin{align*}
          E({\hat \mu}^2)
          &= \mathbb E\left(\frac{1}{n} \sum_{i=1}^n X_i \right)  \\
          &= \frac{1}{n} \sum_{i=1}^n \mathbb E\left(X_i \right)  \\
          &= \frac{1}{n} \sum_{i=1}^n \mu  \\
          &= \mu
        \end{align*}
        $$

        Sample Variance:
        $$
        \begin{align*}
          E({\hat \sigma}^2)
          &= \mathbb E\left(\frac{1}{n} \sum_{i=1}^n (X_i - \hat \mu)^2 \right)  \\
          &= \mathbb E\left(\frac{1}{n} \sum_{i=1}^n ((X_i - \mu) - (\hat \mu - \mu))^2 \right)  \\
          &= \mathbb E \left( \frac{1}{n} \sum_{i=1}^n (X_i - \mu)^2 - \frac{2}{n} \sum_{i=1}^n (X_i - \mu)(\hat \mu - \mu) + \frac{1}{n} \sum_{i=1}^n (\hat \mu - \mu)^2 \right)  \\
          &= \mathbb E \left( \frac{1}{n} \sum_{i=1}^n (X_i - \mu)^2 - 2 (\hat \mu - \mu)\left(\frac{1}{n}\sum_{i=1}^n(X_i) - \mu \right) + (\hat \mu - \mu)^2 \right)  \\
          &= \mathbb E \left( \frac{1}{n} \sum_{i=1}^n (X_i - \mu)^2 - 2 (\hat \mu - \mu)^2 + (\hat \mu - \mu)^2 \right)  \\
          &= \frac{1}{n} \sum_{i=1}^n \left( \mathbb E \left( (X_i - \mu)^2 \right) \right) - \mathbb E \left((\hat \mu - \mu)^2 \right)  \\
          &= \sigma^2 - Var \left(\hat \mu \right)  \\
          &= \sigma^2 - \frac{1}{n} \sigma^2  \\
          &= \frac{n-1}{n} \sigma^2
        \end{align*}
        $$

        $$
          \Rightarrow {\tilde \sigma}^2 = \frac{1}{n-1} \sum_{i=1}^n (X_i - \hat \mu)^2
        $$

    - Efficiency

  - Example
    |Name|Expression| 
    |---|---|
    |Sample Mean|$\hat \mu = \frac{1}{n} \sum\limits_{i=0}^n X_i$|
    |Sample Variance|$\hat \sigma =  \frac{1}{n-1} \sum\limits_{i=1}^n (X_i - \hat \mu)$|
    |Sample k-order Moment|$\hat A_k = \frac{1}{n} \sum\limits_{i=0}^n X_i^k$|
    |Sample k-order Central Moment|$\hat B_k = \frac{1}{n} \sum\limits_{i=0}^n (X_i - \hat \mu)^k$|
    |||

  - Algorithm -- Solve Point Estimation  
    * Moment Estimation
      
    * Maximum Likelihood Estimation