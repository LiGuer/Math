* Partial Order & Strict Partial Order
  - Define  
    Partial Order is a homogeneous relation $\le$ on a set $S$ that is reflexive, antisymmetric, transitive.
    - Reflexivity. Every element is related to itself.
      $$\forall x \in S, x \le x$$ 
    - Antisymmetry. 
      $$\forall x, y \in S, x \le y, y \le x \quad\Rightarrow\quad x = y$$
    - Transitivity. 
      $$\forall x, y, z \in S, x \le y, y \le z \quad\Rightarrow\quad x \le z$$

    Strict Partial Order is a homogeneous relation $<$ on a set $S$ that is irreflexivity, antisymmetric, transitive. Where the irreflexivity is not $a < a$ (no element is related to itself).

  - Property
    * Least Element & Greatest Element
      - Define  
        $$m \le a \quad, m \in S, \forall a \in S  \tag{Least element}$$ 
        $$a \le g \quad, g \in S, \forall a \in S  \tag{Greatest element}$$ 

    * Minimal Element & Maximal Element  
      - Define  
        $$\nexists a \in S, \text{ let } a < m \quad, m \in S \tag{Minimal element}$$ 
        $$\nexists a \in S, \text{ let } g < a \quad, g \in S \tag{Maximal element}$$ 

  - Include
    * Total Order
      - Define  
        Total Order is a partial order in which any two elements are comparable, such that, 
        - strongly connected, formerly called total: $a \le b$ or $b \le a$.

  - Problem
    * Optimization Problem 