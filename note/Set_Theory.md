* Set Theory
  * Set
    - Define
      A set is a collection of distinct objects.

    - Include
      * Empty Set
        $$\emptyset$$

      * Subset

      * Power Set
        - Define  
          The Power Set of a set $S$ is the set of all subset of $S$, including $\emptyset$ and $S$ itself.

        - Include
          * $\sigma$-algebra
            - Define  
              For a set $S$ and its power set $P(S)$, a $\sigma$-algebra $\Sigma$ is a subset $\Sigma \subseteq P(S)$ such that
              - $S \in \Sigma$, and $S$ is considered to be the universal set in the following context.
              - $\Sigma$ closed under complementation, i.e. $A \in \Sigma$ implies that $A^C = S \backslash A \in \Sigma$. Meanwhile, base on the $S \in \Sigma$ (1) we have $\emptyset \in \Sigma$. 
              - $\Sigma$ is closed under countable unions, i.e. for any sequence $(A_1, ..., A_n), A_i \in \Sigma$, we have that 
                $$\bigcup_i A_i \in \Sigma$$

            - Perporty
              - The maximum $\sigma$-algebra is Power Set of $S$,  
                The minimum $\sigma$-algebra is $\{\emptyset, S\}$

              - Countable intersection set closure, if $A_1, ... , A_n \in Σ$, then $\bigcap_i A_i  \in Σ$
                - Proof  
                  De Morgan's law

    - Include
      * Ordered Set

  * Mapping , Function
    - Define  
      $$f: X \to Y$$
      $$\forall x \in X, \exists_{= 1} y \in Y, f(x) = y$$
      Mapping, refer to a relationship from the element of set $X$ to that of set $Y$, and satisfy $X$中的任意元素都有唯一的$Y$中元素与之对应. (One-to-many is not allowed.)

      $X$: domain of Definition.

    - Include
      * Injective
        - Define  
          $$\forall x, x', f(x) = f(x') \Rightarrow x = x'$$
          Each mapped element $y$ has a unique element $x$ corresponding to it.

      * Surjection
        - Define
          $$\forall y \in Y, \exists x \in X, f(x) = y$$
          Each element $y$ in set $Y$ has a element $x$ in set $X$ corresponding to it.

      * Bijection , One-to-One Correspondence
        - Define
          $$\forall y \in Y, \exists_{= 1} x \in X, f(x) = y$$
          A map that is both injective and surjective. Each element $y$ in set $Y$ has a unique element $x$ in set $X$ corresponding to it. Meanwhile, each element $x$ in set $X$ has a unique element $y$ in set $Y$ corresponding to it.

        - Property
          * Inverse Function
            - Define  
              if $f$ is a bijection, its inverse function is $f^{-1}(b) = a \Leftrightarrow f(a) = b$