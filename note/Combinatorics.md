* Combinatorics
  * Counting

    - Property
      * Addition theorem  
        for $S = \cap_{i=1}^n S_i, S_i \cap S_j = \emptyset (i ≠ j)$
        $$\Rightarrow \text{number}(S) = \sum_{i=1}^n \text{number}(S_i)$$

      * Multiplication theorem  
        for sets $S_A, S_B$, and
        $$\begin{align*}
          S &= \{(a, b) | a \in S_A, b \in S_B\}  \\
            &= S_A × S_B  \tag{Cartesian积}  \\
        \end{align*}$$
        $$\Rightarrow \text{number}(S) = \text{number}(S_A) × \text{number}(S_B)$$

        - Proof
          $$\begin{align*}
            S 
            &= \{(a, b) | a \in S_A, b \in S_B\}  \\
            &= \cap_{a_i \in S_A} \{(a_i, b) | b \in S_B\}  \\
            \Rightarrow \text{number}(S) &= \sum_{i=1}^{\text{number}(S_A)} \text{number}(S_B)  \tag{Addition theorem}  \\
            &= \text{number}(S_A) × \text{number}(S_B)  \\
          \end{align*}$$

      * Principle of Inclusion-Exclusion  
        for $A_1,...,A_n \subseteq S$
        $$\begin{align*}
          \text{number}(\cup_{i=1}^n A_i) &= \sum_{k=1}^n ((-1)^{k-1} \sum_{\substack{i_1,...,i_k \in 1:n \\ i_1≠...≠i_k}} \text{number}(\cap_{i\in\{i_1,...,i_k\}} A_i))
        \end{align*}$$

      - Special Counting Sequence
        * Catalan Numbers 
          - Define  
            $$\begin{align*}
              f_n 
              &= \frac{C(2n, n)}{n+1}\quad, n \ge 0  \\
              &= C(2n, n) - C(2n, n - 1)  \\
              &= C(2n, n) - C(2n, n + 1)  \\
              &= \frac{(2n)!}{(n+1)! n!}\\
              &= \left\{\begin{matrix}
                \sum\limits_{i=1}^n f_{i-1}f_{n-i}  & n \ge 2\\
                1 & n = 0, 1
              \end{matrix}\right. \tag{recurrence form}\\
              &= \frac{4n-2}{n+1} f_{n-1}
            \end{align*}$$
            Catalan Numbers are a sequence of natural numbers.
          
      * Pigeonhole Principle  
        for $A_1, ..., A_n \subseteq A, \text{number}(A) = n + 1$, $\Rightarrow \exists A_i, \text{number}(A_i) ≥ 2$.

  * Permutation & Conbination
    - Define  
      Conbination is the selection of $k$ elements from a set of $n$ elements without any regard to the order in which they are chosen.  
      Permutation: is the arrangement of $k$ elements from a set of $n$ elements in a particular order. 

      The number of Conbination and Permutation Subsets
      $$\begin{align*}
        C(n, r) &= \frac{n!}{(n - m)!·m!}  \tag{Conbination}\\
        A(n, r) &= \frac{n!}{(n - m)!}  \tag{Permutation}
      \end{align*}$$

    - Property
      - $C(n,r) = C(n-1,r-1) + C(n-1,r)$
      - $C(n,r) = C(n,n-r)$
      - 
        $$\begin{align*}
          \sum_{i=0}^n C(n,i) &= 2^n  \\
          \sum_{i=0}^n C(n,i)^2 &= C(2n, n)  \\
          \sum_{i=0}^n C(k+i,k)^2 &= C(n+k+1, k+1) 
        \end{align*}$$

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

