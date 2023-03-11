* Neural Network

  - Include
    * Back Propagation Neural Network
      * 全连接层
        - Forward Propagation
          $$\boldsymbol y = σ (\boldsymbol w \boldsymbol x + \boldsymbol b)$$
          $σ()$ 激活函数.

        - Back Propagation -- Gradient Descent Optimization
          $$ 
          \begin{align*}
            δ_l &= (\boldsymbol w_{l+1}^T·δ_{l+1})·σ'(z_l)  \tag{每层误差}  \\
            δ_L &= ∇ E · σ'(z_{outl})  \tag{输出层误差}  \\
            ∇ E(\boldsymbol w_l) &= δ_l \boldsymbol x_l^T  \tag{每层参数误差}  \\
          \end{align*}
          $$
          
          $$\boldsymbol w^{τ+1} = \boldsymbol w^τ - η ∇ E(\boldsymbol w^τ)$$

          - Symbols 
            - $z = \boldsymbol w^T \boldsymbol x + \boldsymbol b$
            - $E_{total} = \sum (target_i - out_i)^2$ 误差·损失函数
            - $E(\boldsymbol w)=\frac{1}{2} \sum_{n=1}^N ||\boldsymbol y(\boldsymbol x_n, \boldsymbol w)-\boldsymbol t_n||^2$

          - Proof
            每层参数误差, 由链式法则得(其中$L$指输出层),
            $$
            \begin{align*}
              \frac{∂ E}{∂ w_l} &= \frac{∂ z_l}{∂ w_l} \left(\frac{∂ y_l}{∂ z_l}\frac{∂ z_l}{∂ y_{l+1}}\right) ... \left(\frac{∂ y_{L-1}}{∂ z_{L-1}} \frac{∂ z_{L-1}}{∂ y_L} \right) \frac{∂ y_L}{∂ z_L} \frac{∂ E}{∂ y_L}  \\
              &= \frac{∂ z_l}{∂ w_l}  \left(\prod_{i = l}^{L} \frac{∂ y_i}{∂ z_i}\frac{∂ z_i}{∂ y_{i+1}} \right) \frac{∂ y_L}{∂ z_L} \frac{∂ E}{∂ y_L}  \\
            \end{align*}
            $$
            其中,
            $$
            \begin{align*}
              \frac{∂ y_l}{∂ z_l} &= σ'(z_l)  \\
              \frac{∂ z_l}{∂ y_{l+1}} &= \boldsymbol w_{l+1}^T  \\
            \end{align*}
            $$
            令$δ_l$为每层误差,
            $$
            \begin{align*}
              δ_l &=\left(\prod_{i = l}^{L} \frac{∂ y_i}{∂ z_i}\frac{∂ z_i}{∂ y_{i+1}} \right) \frac{∂ y_L}{∂ z_L} \frac{∂ E}{∂ y_L}  \\
              &= \left(\prod_{i = l}^{L} σ'(z_i)\boldsymbol w_{i+1}^T\right) σ '(z_L)\frac{∂ E}{∂ y_L}  \\
            \end{align*}
            $$
            得到每层参数误差结果,
            $$
            \begin{align*}
              => \frac{∂ E}{∂ w_l} &= δ_l \frac{∂ z_l}{∂ w_l} = δ_l x_l^T  \\
              δ_L &= \frac{∂ E}{∂ y_L} ·σ '(z_L)  \\
            \end{align*}
            $$

    * Convolution Nerual Network ; CNN

    * Recurrent Neural Network ; RNN

    * Graph Nerual Network ; GNN

    * Self-Attention


