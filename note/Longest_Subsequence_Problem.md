* Longest Subsequence Problem
  - Include
    * Longest Common Subsequence
      - Purpose  
        input: sequence $a, b$
        $$\begin{align*}
          \max_{x}  \quad & \text{number}(x)  \\
          s.t. \quad & x \subseteq a  \\
            & x \subseteq b
        \end{align*}$$

      - Algorithm  
        Dynamic Programming  
        $$\begin{align*}
          f(a_{1:n}, b_{1:m}) &= \left\{\begin{matrix}
            f(a_{1:n-1}, b_{1:m-1}) + 1 \quad &;  a_n = b_m  \\
            \max(f(a_{1:n}, b_{1:m-1}), f(a_{1:n-1}, b_{1:m})) \quad &;  a_n ≠ b_m  \\
          \end{matrix}\right.  \\

          f(a_1, b_1) &= \left\{\begin{matrix}
            1    \quad ; a_1 = b_1  \\
            0   \quad ; a_1 ≠ b_1  \\
          \end{matrix}\right.  \tag{initial}
        \end{align*}$$

      - Include
        * Longest Continuous Common Subsequence
          - Algorithm  
            Dynamic Programming  
            $$\begin{align*}
              f(a_{1:n}, b_{1:m}) &= \left\{\begin{matrix}
                f(a_{1:n-1}, b_{1:m-1}) + 1 \quad &;  a_n = b_m  \\
                0  \quad &;  a_n ≠ b_m
              \end{matrix}\right.  \\

              f(a_i, b_1) &= \left\{\begin{matrix}
                1  \quad ; a_i = b_1  \\
                0  \quad ; a_i ≠ b_1
              \end{matrix}\right.  \tag{initial}  \\

              f(a_1, b_i) &= \left\{\begin{matrix}
                1  \quad ; a_1 = b_i  \\
                0  \quad ; a_1 ≠ b_i
              \end{matrix}\right.
            \end{align*}$$

    * Longest Ascending Subsequence
      - Purpose  
        (input) sequence $a$
        $$\begin{align*}
          \max_{x \subseteq a}  \quad &  number(x)  \\
          s.t. \quad & x_i < x_{i+1} \quad ; i \in 1:number(x)
        \end{align*}$$
        
      - Algorithm  
        $$\begin{align*}
          f(n) = max(f(i), max(f(j)) + 1) \quad ; j < i \ and\ a_j < a_i  \\
          f(1) = 1  \tag{initial}
        \end{align*}$$
        $f(n)$: 以$a_n$为结尾的最长上升子序列的长度.

    * Longest Public Prefix
      - Purpose  
        (input) sequence $a$
        $$\begin{align*}
          \max \quad & k  \\
          s.t. \quad & a_{1:k} = a_{n-k+1:n}
        \end{align*}$$

      - Algorithm  
        $$\begin{align*}
          f(n) = \left\{\begin{matrix}
            f(n-1) + 1  \quad &;  a_n = a_{f(n-1) + 1}  \\
            f(f(n-1)) + 1 \quad &; a_n = a_{f(f(n-1)) + 1}  \\
            \vdots & \vdots  \\
            0  \quad &; other
          \end{matrix}\right.
        \end{align*}$$
