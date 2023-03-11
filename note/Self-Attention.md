* Self-Attention
  - Purpose  
    For a input $X \in \mathbb R^{L_{seq} \times \dim}$, self-attention mechanism aims to capture relationships between defferent elements of the input sequence through computering a weight $W$ for each vector based on its similarity to other vectors in the sequence.

    $$Y = \text{Attention}(Q, K, V) = \text{softmax}\left(\frac{Q K^T}{\sqrt{d_k}} \right) V  \tag{Self-Attention}$$  
    $$\begin{align*}
      Q &= W_Q X  \tag{Quary}\\
      K &= W_K X  \tag{Key}\\
      V &= W_V X  \tag{Value}  
    \end{align*}$$

    $$\boldsymbol W \in \mathbb R^{L_\text{seq} \times L_\text{seq}}  \tag{similar matrix}$$

    $$\begin{align*}
      \boldsymbol W_{ij} 
      &= \text{similar}(\boldsymbol x_i, \boldsymbol x_j)  \\
      &= \text{softmax}\left(\frac{\boldsymbol q_i^T \cdot \boldsymbol k_i}{\sqrt{d_k}}\right)  \tag{scaled dot-product similarity}
    \end{align*}$$

    $$\boldsymbol Y = \boldsymbol W \boldsymbol V = \boldsymbol W \left(\begin{matrix} \boldsymbol v_1^T\\ \vdots \\  \boldsymbol v_{L_\text{seq}}^T \end{matrix}\right) = \left(\begin{matrix} \sum\limits_{i=1}^{L_\text{seq}} w_{1,i} \boldsymbol v_i^T \\ \vdots \\ \sum\limits_{i=1}^{L_\text{seq}} w_{L_\text{seq},i} \boldsymbol v_i^T\end{matrix}\right)$$
    
    $d_k$ refers to the dimensionality of the Key vectors.