* Sequence 
  - Set
    ;
  - Problem 
    * Sort
    * Longest Subsequence Problem
    * Sequence Palindrome
      - Include
        * Longest Palindrome Subsequence 
          - Purpose 
            $$\begin{align*}
              \max_{x \subseteq a} \quad & n_x = \text{number}(x)  \\
              s.t. \quad & x_i = x_{n_x - i + 1}  \quad ; i = 1:n_x  \tag{Palindrome}
            \end{align*}$$

          - Algorithm  
            Dynamic programming,
            $$\begin{align*}
              f(s,e) &= \left\{\begin{matrix}
                f(s-1,e+1) + 2 \quad & f(s,e) > 0 \ and\  a_{s-1} = a_{e+1}  \\
                0 \quad & other.
                \end{matrix}\right.  \\
              f(s,s) &= 1  \tag{initial}  \\
              f(s,s+1) &= 2 \quad ;a_s = a_{s+1}  \\
            \end{align*}$$
            $f()$: $a_{s:e}$的回文字数, 不是回文序列则为0.

        * Maximum Palindrome Prefix
          - Purpose  
            $$\begin{align*}
              \max_{x \in 1:n_a} \quad & x  \\
              s.t. \quad & a_i = x_{x - i + 1}  \quad ; i = 1:x  \tag{Palindrome}
            \end{align*}$$

    * Sequence Matching
      - Purpose  
        (input) $a, b \quad ; b \subseteq a$  
        find $\min\ k$, let $b = a_{k:k+n_b-1}$

      - Property  
        if $b_{1:i} = a_{k:k+i-1}$, and $l_i = \text{Maximum public prefix length}(b_{1:i})$
        $$\begin{align*}
          b_{1:l_i} &= a_{((k+i-1)-l_i+1):(k+i-1)}  \\
          b_{1:l'} &≠ a_{((k+i-1)-l'+1):(k+i-1)}  \quad ; l' > l        
        \end{align*}$$

      - Algorithm
        * Knuth-Morris-Pratt Algorithm
          $$\begin{matrix}
            b_{1:i} = a_{k:k+i-1} &\ and\ & b_{i+1} &= a_{k+i} & \Rightarrow & b_{1:i+1} &=& a_{k:k+i}  \\
            b_{1:i} = a_{k:k+i-1} &\ and\ & b_{g(i)+1} &= a_{k+i} & \Rightarrow  & b_{1:g(i)+1} &=& a_{(k+i)-g(i)+1:k+i}  \\
            b_{1:i} = a_{k:k+i-1} &\ and\ & b_{g(g(i))+1} &= a_{k+i} & \Rightarrow  & b_{1:g(g(i))+1} &=& a_{(k+i)-g(g(i))+1:k+i}  \\
            &&&... & \Rightarrow &...&&  \\
            & & b_{1} &= a_{k+1} & \Rightarrow  & b_{1:1} &=& a_{k+i:k+i}  \\
            & & b_{1} &≠ a_{k+1} & \Rightarrow  & b_{1:1} &≠& a_{k+i:k+i}  \\
          \end{matrix}$$

  - Include 
    * Deque
      - Define
        Deque is a finite sequence that only allow to insert and delete elements from its head and tail.

      - Include
        * Queue
          - Define
            Queue is a finite sequence that only allow to insert elements from its tail and delete elements from its head. (First-in first-out.)

        * Stack
          - Define
            Stack is a finite sequence that only allow to insert and delete elements from its head. (First-in last-out.)

          - Include
            * Monotone Stack 
              - Define
                A stack that maintain its elements in a monotone order.