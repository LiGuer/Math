* Number Theory
  * Integer Ring
    - Define
      $$(\mathbb Z, +, ·)$$

  * Division with Remainder & Factor
    - Define    
      For $a,b \in \mathbb Z, b \neq 0$, there are unique $q,r\in \mathbb Z, 0 ≤ r < b$ satisfy
      $$a = q \times b + r$$

      we called $q$ is Division, $r$ is Remainder.
      $$\begin{align*}
        a / b = q  \tag{Division}\\
        a \% b = r  \tag{Remainder}
      \end{align*}$$

      If $r = 0$, then we called the $q$ and $b$ is two Factors of $a$, and $b$ divides $a$, $b | a$. For any $a$, $1, a$ are always Factors of $a$.
      $$b | a \Leftrightarrow (\exists c \in \mathbb Z) a = b \times c$$

    - Concept
      * Common Divisor & Common Multiple
      * Prime
        - Define  
          A number that can only be divided by $1$ and itself.

        - Property
          -  The Fundamental Theorem of Arithmetic  
            Any integer $n$ greater than $1$ can be uniquely expressed in the form of prime $p_i$ product.   
            $$n = \prod_i p_i^{\alpha_i} \quad n \in \mathbb Z, n > 1$$

          - Resolving prime factor  
            Pollard Rho algorithm

    - Property  
      - /
        $$\begin{align*}
          (a + b) \% c &= (a \% c + b \% c) \% c  \\
          (a - b) \% c &= (a \% c - b \% c + c) \% c  \\
          (a · b) \% c &= ((a \% c) · (b \% c)) \% c  \\
          (a / b) \% c &= (a · b^{-1}) \% c = ((a \% c) · (b^{-1} \% c)) \% c  \tag{$b^{-1}$:$b$的逆元}
        \end{align*}$$

      - Inverse element
        $(a · c) % b = 1$, 则$c$是$a$在$mod\ b$下的逆元$a^{-1}$

    - Problem
      * Linear Diophantine Equation
      * Congruence Equation
        - Define  
          Solve the unknown $0 \le x < m, x \in \mathbb Z$ 
          $$f(x) \% m = b$$ 

        - Include
          * Linear Congruence Equation
            - Define  
              A type of Congruence Equation with $f(x) = a x$.
              $$(a x) \% m = b$$

            - Include
              * System of Linear Congruence Equations
                - Purpose
                  $$\begin{align*}
                    \left\{\begin{matrix} x \% m_1 = a_1 \\ \vdots \\ x \% m_n = a_n \end{matrix}\right.
                  \end{align*}$$

                  $$x = k \prod_{i=1}^n m_i + \sum_{i=1}^n a_i \left(\prod_{j=1, j≠i}^n m_j\right) \left(\prod_{j=1, j≠i}^n m_j\right)^{-1}$$

      * Power Module
        - Purpose
          $$b = (a^k) \% m$$

        - Algorithm
          - 逐次平方法
            - 将 k 二进制展开
              $$k = \sum_{i=0}^r u_i·2^i$$

              - Note
                计算机里, k内存天然是二进制

            - 逐次平方制作模$m$的$a$幂次表, $i\in[0,r]$
              $$\begin{align*}
                a^{2^0} &= a = A_0 \% m  \\
                a^{2^i} &= (a^{2^{i-1}})^2 = A^{2(i-1)} = A_i % m
              \end{align*}$$

            - 乘积
              $$\prod_{i=0}^r A_i^{u_i} \% m$$
            
            - proof
              $$a^k = a^{\sum_{i=0}^r u_i·2^i}$$

  * Multiplicative Function
    - Define  
      A mapping $f: \mathbb Z \to \mathbb R$, such that
      $$f(a \times b) = f(a) f(b) \quad when\ a, b \in \mathbb Z, gcd(a, b) = 1$$

    - Property
      - $f(1) = 1$

    - Include
      * Eular Function
        - Define  
          The number of coprimes with $n$ in positive integers less than $n$.
          $$\phi(n) = number(\{i\ |\ i \in 1:n, gcd(i, n) = 1\})$$

        - Property
          $$\begin{align*}
            n &= \prod_i p_i^{k_i}  \\
            \phi(n) &= n \prod_{p|n} (1 - 1/p)  
          \end{align*}$$
