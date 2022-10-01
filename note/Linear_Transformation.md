* Linear Transformation
  - Define
    $$T(k x + l y) = k(T x) + l(T y)$$
    A kind of mapping $T$ from linear space $V$ to itself.  
    For all $x \in V$, there is a unique $y \in V$ corresponding to it, and the linear condition is satisfied

    The linear transformation can be represented by a matrix composed of bases; linear transformation matrix.
    $$T X = X A  \quad ; X = (x_1, ... , x_n)$$ 

  - Property
    - operation 
      - $(T_1 + T_2) X = X (A + B)$
      - $(k\ T_1) X = X (k\ A)$
      - $(T_1 T_2) X = X AB$
      - $T_1^{-1} X = X A^{-1}$

    - Range 
      $$Range(T)=\{T x | x \in V\}$$
      In linear space,  
      the set of results of all vectors after linear transformation;   
      the linear space after linear transformation.

      - Rank
        $$rank(A) = \dim Range(A) = \dim Range(A^T)$$
        The dimension of space after transformation.
        The dimension of the range.

    - Null Space
      $$Null(T) = \{x | T x = 0\}$$
      In linear space, the set of all original vectors that are linearly transformed into zero vectors.

    - $\dim V = \dim Range(A) + \dim Null(A)$  
      变换前线性空间维数 = 值域维数 + 零空间维数. 

    - Invariant Subspace
      $$\forall x \in V_1, V_1 \subseteq V, T x \in V_1$$

    * Eigenvalues, Eigenvectors
      - Define
        $$T x = λ x$$
        - $x$: Eigenvectors, a vector whose direction does not change before and after linear transformation;  
        - $λ$: Eigenvalues, proportion of length change of eigenvector after linear transformation.

      - Property
        - Characteristic polynomial
          $$\varphi(λ) = |λ I - A| = λ^n + a_1 λ^{n-1} + ... + a_{n-1} λ + a_n$$

        - Theorem -- Hamilton-Cayley Theorem
          $$\varphi(A) = A^n + a_1 A^{n-1}+ ... +a_{n-1} A + a_n I = 0$$
          Matrix is the root of its characteristic polynomial.

    * Moore-Penrose Inverse
      - Define  
        The solution satisfying the following equation,
        $$ 
          \left\{\begin{matrix}
            A X A = A
            X A X = X
            (A X)^H = A X
            (X A)^H = A X
          \end{matrix}\right.
        $$
        When the rank of cols is full, $A^+ = (A^H A)^{-1} A^H$   
        When the rank of rows is full,  $A^+ = A^H (A A^H)^{-1}$

      - Property
        - $rank(A) = rank(A^H A) = rank(A A^H)$
        - 满秩分解算广义逆 $A^+ = G^H (F^H A G^H)^{-1} F^H$

    * Similarity
      - Define  
        $A$ is similar to $B$, $A ~ B$:
        $$\exists \text{Nonsingular Matrix}P \Rightarrow B = P^{-1} A P$$

      - Property 
        - $A ~ A$ 
        - $A ~ B \Leftrightarrow B ~ A$ 
        - $A ~ B, B ~ C \Leftrightarrow A ~ C$
        - The eigenvalues and eigenvectors of similar matrices are the same.
        - The trace of similar matrix is the same.

    - Transformation of linear transformation matrix under different bases
      $$A_Y = C^{-1} A_X C \quad ; Y = C X$$

      - Proof
        $$
        \begin{align*}
          T Y &= Y A_Y     \tag{Define}\\ 
          T X C &= X C A_Y   \tag{$ Y = X C $}\\
          X A_X C &= X C A_Y   \tag{$ T X = X A_X $}\\
          A_X C &= C A_Y  \\
          A_Y &= C^{-1} A_X C
        \end{align*}
        $$
        
  - Include
    * 恒等变换
      - Define
        $$T x = x \quad ;(\forall x \in V)$$

    * 零变换
      - Define  
        $$T x = 0 \quad ;(\forall x \in V)$$

    * 正交变换
      - Define
        $$<x, x> = <T x, T x>$$
        内积空间中, 保持任意向量的长度不变的线性变换.  
        正交矩阵:  
          $$A A^T = I$$
          $$A A^H = I$$

      * 初等旋转变换
        - Define  
          初等旋转变换矩阵:  
          $$T_{ij} = \left(\begin{matrix}
            \boldsymbol  I \\ & cos θ|_{(i,i)}&  & \sin θ|_{(i,j)} \\ & & \boldsymbol  I \\ & -\sin θ|_{(j,i)} & & \cos θ|_{(j,j)} \\ & & & & \boldsymbol  I
          \end{matrix}\right)$$

      * 初等反射变换
        - Define  
          $$y = H x = (I - 2 e_2 e_2^T) x$$
          - Proof  
            $$
            \begin{align*}
              x - y &= e_2 · (e_2^T x)  \\
              \Rightarrow y &= (I-2 e_2 e_2^T) x
            \end{align*}
            $$

    * 对称变换
      - Define
        $$<T x, y> = <x, T y>$$
        对称矩阵:
          $$A^T = A$$
          $$A^H = A$$

    * Projection transformation & Orthogonal Projection transformation
      - Define  
        Projection transformation: 设线性空间的子空间$L$及其补$M$, 投影变换是将线性空间沿$M$到$L$的投影的变换.  

        Projection matrix:  
        $$P_{L|M} = \left(\begin{matrix} X & 0 \end{matrix}\right) \left(\begin{matrix} X & Y \end{matrix}\right)^{-1}$$

        Orthogonal Projection transformation: 设线性空间的子空间$L$及其正交补$L_\bot$, 将线性空间沿$L_\bot$到$L$的投影的变换, 称Orthogonal projection transformation.  

        Orthogonal Projection matrix:  
          $$P_L = X(X^H X)^{-1}X^H$$
          Symbol: $X = (x_1, ... , x_r)$: 投影后子空间的基.

      - Property  
        - $P_{L|M}^2 = P_{L|M}$ 
        - $P_L (a \boldsymbol x + b \boldsymbol y) = a (P_L \boldsymbol x) + b (P_L \boldsymbol y)$
        - $x \in L \Leftrightarrow P_L x = x$
        - $x \in L_\bot \Leftrightarrow P_L x = 0$
        - if $H$ is a inner product space, and $L$ is a subspace of $H$ with Orthonormal Basis $\{u_1, ...u_n\}$, then the projection of $x \in H$ is
          $$\hat x = \sum_{i=1}^n \frac{u_i^T x}{u_i^T u_i} u_i$$ 

    * 斜切变换
      - Define  
        斜切变换矩阵:  
          单位矩阵的第(i,j)个元素改为斜切比率 $a_{ij}$

    * 缩放变换
      - Define  
        缩放变换矩阵:
        $$T = \left(\begin{matrix} Δx_1 \\ & Δx_2 \\ & & \ddots \\ & & & Δx_n \end{matrix}\right)$$