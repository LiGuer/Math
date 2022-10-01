* Probability Space
  - Define
    $$<Ω, \mathcal F, \mathbb P>$$
    Probability Space is a Triplet. 

    - $Ω$: sample space
    - $\mathbb P$: Probability
    - $\mathcal F$: σ-algebra, a set of subsets of $Ω$, called a set of events, and satisfying
      - $\emptyset, Ω \in \mathcal F$ Including empty set and full set of sample space
      - closed under complements
        $$A \in \mathcal F \Rightarrow A^C \in \mathcal F$$
      - closed under countable unions 
        $$A_1, A_2, ... \in \mathcal F \Rightarrow \bigcup_{i=1}^∞ A_i \in \mathcal F$$

  - Include
    * Probability
      - Define
        $$\mathbb P: \mathcal F \to [0, 1]$$
        
        Probability is a set function, a measure of a set, and satisfies:

        - Nonnegativity $\mathbb P(A) \in [0, 1] \quad ; \forall A \in F$
        - Normative $\mathbb P(Ω) = 1$
        - Countable Additivity 
          $$\mathbb P \left(\bigcup_i A_i \right) = \sum_i \mathbb P(A_i)$$

      - Property
        - Theorem -- large number theorem
          $$
          \begin{align*}
          \lim\limits_{n \to ∞} \mathbb P \left(\left|\frac{1}{N} \sum_{k=1}^n X_k-μ \right|<ε \right) = 1  \tag{Weak large number theorem}\\
          \lim\limits_{n \to ∞} \mathbb P\left(\left|\frac{f_A}{n}-p \right|<ε\right) = 1  \tag{Bernoulli large number theorem}
          \end{align*}
          $$

        - Theorem -- Central Limit Theorem
          $$
          \begin{align*}
            \lim\limits_{n \to ∞} F_n(x) &= \lim\limits_{n \to ∞} \mathbb P(\frac{\sum\limits_{k=1}^n X_k - n μ}{\sqrt{n} σ} ≤ x)  \\
            &= \int_{-∞}^x \frac{1}{\sqrt{2 π}} e^{-t^2 / 2} \mathrm d t  \\
            &= \Phi(x)
          \end{align*}
          $$

      - Include
        * Joint probability
          - Define 
            $$\mathbb P(A B)$$
            The probability of A and B occurring together.

        * Conditional probability
          - Define
            $$\mathbb P(B | A)$$
            Probability of occurrence of $B$ under the condition that $A$ occurs.

          - Property
            - Independence 
              $$Independence \Leftrightarrow \mathbb P(A B) = \mathbb P(A) \mathbb P(B)$$
              The occurrence of $A$ and $B$ does not affect each other.

            - relationship between Joint \& Conditional probability
              $$
              \begin{align*}
                \mathbb P(B | A) &= \frac{\mathbb P(A B)}{\mathbb P(A)}  \\
                \mathbb P(A B) &= \mathbb P(B | A) \mathbb P(A) = \mathbb P(A | B) \mathbb P(B)
              \end{align*}
              $$

            - Theorem -- Total Probability Theorem
              $$\mathbb P(A) = \sum_i \mathbb P(A|B_i) \mathbb P(B_i) \quad; \sum_i A_i = Ω$$

            - Theorem -- Bayes formula
              $$
              \begin{align*}
                \mathbb P(A | B) &= \frac{\mathbb P(B | A) \mathbb P(A)}{\mathbb P(B)}  \\
                \mathbb P(A_i | B) &= \frac{\mathbb P(B | A_i) \mathbb P(A_i)}{\sum\limits_j \mathbb P(B|A_j) \mathbb P(A_j)}; \sum_j A_j = Ω
              \end{align*}
              $$