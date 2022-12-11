* Set Theory
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

  * Mapping , Function
    - Define  
      $$f: X \to Y$$
      $$\forall x \in X, \exists_{= 1} y \in Y, f(x) = y$$
      映射, 是指从集合$X$到集合$Y$的元素之间的一类对应关系, 且满足$X$中的任意元素都有唯一的$Y$中元素与之对应. (不允许一对多.)

      $X$ 定义域

    - Include
      * Injective
        - Define  
          $$\forall x, x', f(x) = f(x') \Rightarrow x = x'$$
          每一个被映射的y都有唯一的x与之对应.

      * Surjection
        - Define
          $$\forall y \in Y, \exists x \in X, f(x) = y$$
          每一个y都至少有一个x与之对应.

      * Bijection , One-to-One Correspondence
        - Define
          $$\forall y \in Y, \exists_{= 1} x \in X, f(x) = y$$
          既是单射又是满射的映射. 每一个x都有唯一的y与之对应, 每一个y都有唯一的x与之对应.

        - Property
          * Inverse Function
            - Define  
              若$f$是双射, 则其反函数为 $f^{-1}(b) = a \Leftrightarrow f(a) = b$