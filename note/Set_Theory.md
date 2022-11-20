* Set Theory
  * Power Set
    - Define  
      The Power Set of a set $S$ is the set of all subset of $S$, including $\emptyset$ and $S$ itself.

  * $\sigma$-algebra
    - Define  
      For a set $S$ and its power set $P(S)$, a $\sigma$-algebra $\Sigma$ is a subset $\Sigma \subseteq P(S)$ such that
      - $S \in \Sigma$, and $S$ is considered to be the universal set in the following context.
      - $\Sigma$ closed under complementation, i.e. $A \in \Sigma$ implies that $A^C = S \backslash A \in \Sigma$. Meanwhile, base on the $S \in \Sigma$ (1) we have $\emptyset \in \Sigma$. 
      - $\Sigma$ is closed under countable unions, i.e. for any sequence $(A_1, ..., A_n), A_i \in \Sigma$, we have that 
        $$\bigcup_i A_i \in \Sigma$$