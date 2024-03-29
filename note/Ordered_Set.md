* Ordered Set
  - Include
    * Ordered Pair
      - Define
        $$(a, b) = \{\{a\}, \{a, b\}\}$$
        Ordered pair is a pair of two items in which order matters. Formally, it is defined as $\{\{a\}, \{a, b\}\}$.

  - Property
    * Permutation
      - Define  
        Permutation, is the arrangement of $k$ elements from a set of $n$ elements in a particular order. 

        The number of Conbination and Permutation Subsets
        $$A(n, k) = \frac{n!}{(n - k)!}  \tag{Permutation}$$

      - Property
        * Full Permutation
          - Define  
            Full Permutation refers to all possible permutations of all elements in a sequence.  

          - Problem: Generate Full Permutation
            $$\begin{align*}
              f(\{a_i | i\in 1:n\}) 
              &= \{(a_1, a_2, ..., a_n), (a_2, a_1, ..., a_n), (a_n, a_{n-1}, ..., a_1)\}  \\
              &= \cup_{i=1}^n (\{(a_i)\} × f(\{a_j\ |\ j ≠ i, j\in 1:n\}))  \tag{$×$: 序列合并}  \\
              f(\{a_1\}) &= \{(a_1)\}  \tag{initial}  \\
            \end{align*}$$
            
          - Property
            * Cantor Expansion  
              - Define  
                Cantor expansion is a bijection from a permutation sequence $a$ to a natural number $y$ that refer to the index of sequence in full permutation ordered by dictionary order. Where $f(a_i)$ is the number of elements smaller than $a_i$ and not appearing in $a_n:a_i$.
                $$y = \sum\limits_{i=1}^n f(a_i) (i-1)!$$