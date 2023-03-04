* Set Theory
  * Set
    - Define  
      A set $S$ is a collection of distinct objects.

      If an object $x$ is a member of a set $S$, we write $x \in S$. Otherwise, we write $x \notin S$.

    - Property
      - relationship between sets
        * Subset & Proper Subset 
          $$A \subseteq B \quad\Leftrightarrow\quad x \in B, \forall x \in A \tag{Subset}$$ 
          If all the elements of set $A$ are contained in a set $B$, then we say $A$ is a subset of $B$.

          $$A \subset B \Leftrightarrow x \in B, \forall x \in A \text{ and } \exist x \notin A, x \in B \tag{Proper Subset}$$ 

          A set $A$ is a proper subset of $B$, if $A \subseteq B$, but not $A = B$.

        - equal  
          $$\begin{align*}
            A = B &\quad\Leftrightarrow\quad x \in B, \forall x \in A \text{ and } x \in A, \forall x \in B  \tag{equal}\\
            &\quad\Leftrightarrow\quad A \subseteq B, B \subseteq A
          \end{align*}$$
          Two sets are equal, if they contain the same elements.

        * Disjoint
          $$A, B \text{ is Disjoint } \quad\Leftrightarrow\quad A \cap B = \emptyset$$   

      - operations
        * Intersection
          - Define
            $$A \cap B = \{x \ |\ x \in A, x \in B\}  \tag{Intersection}$$

          - Property
            - idempotency law: $A \cap A = A$
            - commutative law: $A \cap B = B \cap A$  
            - associative law: $A \cap (B \cap C) = (A \cap B) \cap C$

        * Union
          - Define  
            $$A \cup B = \{x \ |\ x \in A \text{ or } x \in B \}  \tag{Union}$$

          - Property
            - idempotency law: $A \cup A = A$
            - commutative law: $A \cup B = B \cup A$  
            - associative law: $A \cup (B \cup C) = (A \cup B) \cup C$
             
        * Difference
          - Define
            $$A - B = \{x \ |\ x \in A \text{ and } x \notin B\}  \tag{Difference}$$    

        * Complement of A Set
          - Define  
            $$\bar A = U - A = \{x \ |\ x \in U, x \notin A\}  \tag{Complement of A Set}$$  

            For a universal set $U$, the complement of a set $A$ is $U - A$.

          - Property
            - $\bar{\bar A} = A$ 

        - Property
          - distributive laws 
            $$A \cap (B \cup C) = (A \cap B) \cup (A \cap C)$$
            $$A \cup (B \cap C) = (A \cup B) \cap (A \cup C)$$
          - absorption laws
            $$A \cap (A \cup B) = A$$
            $$A \cup (A \cap B) = A$$ 
          - DeMorgan's laws
            $$A - (B \cap C) = (A - B) \cup (A - C)$$ 
            $$A - (B \cup C) = (A - B) \cap (A - C)$$ 
            $$\overline{A \cap B} = \bar A \cup \bar B$$ 
            $$\overline{A \cup B} = \bar A \cap \bar B$$ 

      - Special Operations
        * Cartesian Product
          - Define
            $$A \times B = \{(a, b) \ | a \in A \text{ and } b \in B\}$$  
            For two sets $A, B$, cartesian product is the set of all ordered pairs such that the first element of the pair is an element of $A$ and the second one is from $B$.

          - Property
            - $\text{number}(A \times B) = \text{number}(A) \cdot \text{number}(B)$
    - Include
      * Empty Set
        - Define 
          $$\emptyset$$
          Empty Set is a set without any element. 

        - Property 
          - $\emptyset \in S, \forall \text{ set } S$
          - $A \cap \emptyset = \emptyset$
          - $A \cup \emptyset = A$

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
        - Include
          * Ordered Pair
            - Define
              $$(a, b)$$    

  * Sequence

  * Mapping , Function
    - Define  
      $$f: X \to Y  \tag{Function}$$
      $$\forall x \in X, \exists_{= 1} y \in Y, f(x) = y$$
      Mapping, refer to a relationship from the element of set $X$ to that of set $Y$, and satisfy any element in $X$ has a unique element in $Y$ corresponding to it (One-to-many is not allowed).

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