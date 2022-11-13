* Convolution Nerual Network ; CNN
  * 卷积层
    - Forward Propagation -- Convolution
      $$
      \begin{align*}
        x * k = \left[y_{ij} | y_{ij} = \sum_{c=1}^{C_x} \sum_{p=1}^{H_x} \sum_{q=1}^{W_x} x_{c,i+p-1,j+q-1} × k_{cpq}\right]  \\
        y_c = x * k_c + b_c  \\
        y = {y_1, y_2, ... , y_n}  \\
      \end{align*}
      $$

      - Symbols 
        - $\{H, W, C\}$ 张量高度、宽度、通道数, kernel 卷积核
        - padding 加边框宽度
          $$
          \begin{align*}
            H_{out} = \frac{H_{in} - H_{kernel} + 2 · padding}{stride + 1}  \\
            W_{out} = \frac{W_{in} -  W_{kernel} + 2 · padding}{stride + 1}   
          \end{align*}
          $$

    - Back Propagation -- Convolution
      $$
      \begin{align*}
        \frac{∂E}{∂y_l} &= \frac{∂E}{∂y_L}·\frac{∂y_L}{∂y{L-1}}· ... ·\frac{∂y_{l+1}}{∂y_l}  \\
          &= δ_l = δ_{l+1}·\frac{∂y_{l+1}}{∂x_{l+1}} = δ_{l-1} * Rot_{π}(w_l)  \\
        \frac{∂E}{∂k_l} &= \frac{∂E}{∂y_l} · \frac{∂y_l}{∂k_l} = δ_l·\frac{∂y_l}{∂k_l} = δ_l * x_l  \\
        \frac{∂E}{∂b_l} &= δ_l · \frac{∂y_l}{∂b_l} = \sum_x \sum_y δ_l
      \end{align*}
      $$
      其中,
      $$
      \begin{align*}
        \frac{∂y_{lcij}}{∂k_{lcpq}} = x_{l,c,i+p-1,j+q-1}
        \frac{∂y_{lc}}{∂k_{lcpq}} = \sum_{i=1}^{H_y} \sum_{j=1}^{W_y} x_{l,c,i+p-1,j+q-1}  \\
        \frac{∂E}{∂k_{lcpq}} = \sum_{i=1}^{H_y} \sum_{j=1}^{W_y} δ_{lcij} × x_{l,c,i+p-1,j+q-1}  = δ_{lc} * x_{lc}  \\
        \frac{∂E}{∂b_{lc}} = \sum_{i=1}^{H_y} \sum_{j=1}^{W_y} δ_{lcij} = \sum_x \sum_y δ_l
      \end{align*}
      $$

  \Include
    * Residual Module
      - Forward Propagation
        $$y = x + F(x, ω)$$
        $$
        \begin{align*}
          x_{l+1} &= x_l + F(x_l, ω_l)  \\
          x_{l+2} &= x_{l+1} + F(x_{l+1}, ω_{l+1}) = x_l + F(x_l, ω_l) + F(x_{l+1}, ω_{l+1})  \\
          x_L &= x_l + \sum_{i=l}^{L-1} F(x_{i+1}, ω_{i+1})
        \end{align*}
        $$

      - Back Propagation
        $$
          \frac{∂ \epsilon}{∂ x_l} = \frac{\epsilon}{x_L} \frac{∂ x_L}{x_l} = ∂\frac{∂ \epsilon}{∂ x_L} \left(1 + \frac{∂}{∂ x_l} \sum_{i=l}^{L-1} F(x_i, w_i)\right)
        $$