* Self-Attention
  - Process  
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
      &= \text{softmax}\left(\frac{\boldsymbol q_i^T \boldsymbol k_i}{\sqrt{d_k}}\right)  \tag{scaled dot-product similarity}
    \end{align*}$$

    $$\boldsymbol Y = \boldsymbol W \boldsymbol V = \boldsymbol W \left(\begin{matrix} \boldsymbol v_1^T\\ \vdots \\  \boldsymbol v_{L_\text{seq}}^T \end{matrix}\right) = \left(\begin{matrix} \sum\limits_{i=1}^{L_\text{seq}} w_{1,i} \boldsymbol v_i^T \\ \vdots \\ \sum\limits_{i=1}^{L_\text{seq}} w_{L_\text{seq},i} \boldsymbol v_i^T\end{matrix}\right)$$
    
    $d_k$ refers to the dimensionality of the Key vectors.

  - Property
    * Multi-Head Self-Attention
      - Process  
        $$\text{MultiHead}(Q,K,V) = \left(\begin{matrix}\text{Attention}(Q_1, K_1, V_1) & \cdots & \text{Attention}(Q_h, K_h, V_h)\end{matrix}\right) W_O$$

        Multi-Head Self-Attention allows the model to focus on multiple aspects of the input sequence simultaneously. Multi-Head Self-Attention is to perform self-attention multiple times in parallel, with different sets of learned query, key, and value matrices.

        $$\begin{align*}
          \left(\begin{matrix}Q_1 & \cdots & Q_h\end{matrix}\right) &= X W_Q  \\
          \left(\begin{matrix}K_1 & \cdots & K_h\end{matrix}\right) &= X W_K  \\
          \left(\begin{matrix}V_1 & \cdots & V_h\end{matrix}\right) &= X W_V
        \end{align*}$$

        |Symbol|Means|
        |---|---|
        |$X \in \mathbb R^{L_\text{seq} \times d}$|input|
        |$W_Q, W_K, W_V, W_O \in \mathbb R^{d \times d}$|weight|
        |$Q_i, K_i, V_i \in \mathbb R^{L_\text{seq} \times \frac{d}{N_\text{head}}}$||

  - Include
    * Transformer
      - Process
        - Basic structure
          - Multi-Head Self-Attention
          - Position-wise Feedforward Layer 
            $$y = W_2 (\text{ReLU}(W_1 x + b_1)) + b_2$$ 
        - Encoder
          $$\text{EncoderLayer}_i(x) = \text{LayerNorm}(x + \text{MultiHead}(\text{LayerNorm}(x)))$$
          $$\text{Encoder}(x) = \text{Position-wiseFeedforward}(\text{EncoderLayer}_N(...(\text{EncoderLayer}_1(x))))$$
        - Decoder  
          $$\text{DecoderLayer}_i(x, y) = \text{LayerNorm}(x + \text{MultiHead}(\text{LayerNorm}(\text{MaskedSelfAttention}(x, y))))$$
          $$\text{Decoder}(x, y) = \text{Position-wiseFeedforward}(\text{DecoderLayer}_N(...(\text{DecoderLayer}_1(x, y))))$$
    * BERT