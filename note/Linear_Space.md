* Linear Space
  - Define  
    A non empty set with addition and number multiplication, and satisfying:
    |Axiom|Meaning|
    |---|---|
    | 加法封闭 | $x+y \in V$ |
    | 数乘封闭 | $k x \in V$ |
    | Identity element of vector addition | $x+0=x$ |
    | Inverse elements of vector addition | $x+(-x) = 0$ |
    | Identity element of scalar multiplication | $1x = x$ |
    | Commutativity of vector addition | $x+y = y+x$ |
    | Associativity of vector addition | $x+(y+z) = (x+y) +z$ |
    | Compatibility of scalar multiplication with field multiplication | $a(bx) = (a b)x$ |
    | Distributivity of scalar multiplication with respect to vector addition | $a(x+y) = a x + a y$ |
    | Distributivity of scalar multiplication with respect to field addition | $(a+b)x = ax+bx$ |
    |||

  - Property
    - Linear Independence / Linear Correlation
      $$\nexists / \exists\ a ≠ 0 \Rightarrow x = \sum_{i=1}^n a_i x_i = 0$$ 

    - Dimension  
      In linear space, the maximum number of vectors contained in a linearly independent vector group.

    * Base 
      - Define
        $$\ x = \sum_{i=1}^n a_i x_i, \forall x \in V$$
        Basis is a linearly independent vector group $X = (x_1, ... , x_n)$, All vectors in the linear space are linear combinations of the vector group.

        Symbol:
        - $x_i$: Base vector
        - $a_i$: coordinate

      - Property 
        - Base transformation  
          $$Y = X C$$
          Transformation matrix between new and old bases.

          - Property  
            The base transformation matrix is nonsingular.

        - Coordinate transformation
          $$a_x = C a_y$$
          
          - Proof
            $$v = X a_x = Y a_y = X C a_y \Rightarrow a_x = C a_y$$

        - Span
          $$Span(x_1,...,x_n) = \{x | x = \sum_{i=1}^n a_i x_i\}$$ 
          A representation of a linear space given by a basis vector.

    * Linear Subspace
      - Define
        A nonempty set in a linear space that is closed to linear operations.
        - 加法封闭 $x,y\in V_1 ,\quad x+y \in V_1$
        - 数乘封闭 $x \in V_1, k x \in V_1$

    * Linear Transformation

  - Include
    * Normed Linear Space
    
  - Problem
    * Matrix Decomposition