* Recurrent Neural Network ; RNN
  - Purpose  
    Mining time information of time series.
    
  - Include
    * Long Short Term Memory ; LSTM

    * Gate Recurrent Unit ; GRU

* Long Short Term Memory ; LSTM
  - Define  
    Input: a time series $x \in \mathbb R^{\dim_x × T_{seq}}$  
    Output: a time series $h \in \mathbb R^{\dim_x × T_{seq}}, h = \{h_1, ...,h_{T_{seq}}\}$. Sometimes, we can take a part or the last element $h_{T_{seq}}$ of the sequence $h$ as the output.  
    Hyperparameter: the dimension of hidden state $h_t$: $\dim_h$

    For a moment $t$, the define of the cell of LSTM:
    $$h_t = f_{Cell}(x_t, h_{t-1})  \tag{a cell of LSTM in moment $t$}$$

    $$
    \begin{align*}
      f_t &= \text{sigmoid}\left(W_f \left(\begin{matrix} h_{t-1} \\ x_t \end{matrix}\right) + b_f\right)  \tag{forget gate}\\
      i_t &= \text{sigmoid}\left(W_i \left(\begin{matrix} h_{t-1} \\ x_t \end{matrix}\right) + b_i\right)  \tag{input gate}\\
      o_t &= \text{sigmoid}\left(W_o \left(\begin{matrix} h_{t-1} \\ x_t \end{matrix}\right) + b_o\right)  \tag{output gate}\\
      g_t &= \text{tanh}\ \ \ \ \ \left(W_g \left(\begin{matrix} h_{t-1} \\ x_t \end{matrix}\right) + b_g\right)  \tag{cell input}\\
      c_t &= f_t \odot c_{t-1} + i_t \odot g_t  \tag{cell state}\\
      h_t &= o_t \odot \text{tanh}(c_t)  \tag{hidden state}
    \end{align*}
    $$  

    $$
    \begin{align*}  
      \Rightarrow\quad  \left(\begin{matrix} \tilde f_t \\ \tilde i_t \\ \tilde o_t \\ \tilde g_t \end{matrix}\right)  
      &= W \left(\begin{matrix} h_{t-1} \\ x_t \end{matrix}\right) + b  \\
      &= W_h h_{t-1} + W_x x_t + b \\
      \left(\begin{matrix} f_t \\ i_t \\ o_t \end{matrix}\right) &= \text{sigmoid}\left(\left(\begin{matrix} \tilde f_t \\ \tilde i_t \\ \tilde o_t \end{matrix}\right)\right) \\
      g_t &= \text{tanh} (\tilde g_t) 
    \end{align*}  
    $$  

    [Long Short Term Memory Network (Code)](./files/code/Long_Short_Term_Memory_Network.py)

    |Symbols|Mean|
    |---|---|  
    |$x_t \in \mathbb R^{\dim_x}$| input; the value of the time series $x$ at time $t$|
    |$\dim_x$| the dimension of input $x_t$|
    |$\dim_h$| the dimension of hidden state $h_t$|
    |$f_t \in (0,1)^{\dim_h}$| forget gate| 
    |$i_t \in (0,1)^{\dim_h}$| input gate| 
    |$o_t \in (0,1)^{\dim_h}$| output gate| 
    |$g_t \in (-1,1)^{\dim_h}$| cell input| 
    |$c_t \in \mathbb R^{\dim_h}$| cell state| 
    |$h_t \in \mathbb R^{\dim_h}$| hidden state| 
    |$\odot$| multiply by element|
    |$W \in \mathbb R^{4 \dim_h × (\dim_h + \dim_x)}$|Weights|
    |$W_{g/f/i/o} \in \mathbb R^{\dim_h × (\dim_h + \dim_x)}$| Weights|
    |$W_{h} \in \mathbb R^{4 \dim_h × \dim_h}$| Weights|
    |$W_{x} \in \mathbb R^{4 \dim_h × \dim_x}$| Weights|
    |||

  - Property
    - Back Propagation:   
      $$
      \begin{align*}
        \Delta_{f, t} &= c_{t-1} · \Delta_{c, t-1} · \text{sigmoid}'(f_t)  \\
        \Delta_{i, t} &= g_t · \Delta_{c, t-1} · \text{sigmoid}'(i_t)  \\
        \Delta_{o, t} &= s_t · \Delta_{h_{t+1}, t} · \text{sigmoid}'(o_t)  \\
        \Delta_{g, t} &= i_t · \Delta_{c, t-1} · \text{tanh}'(g_t)  \\

        \Delta_{W_{fiog}, t} &= \Delta_{W_{fiog}, t - 1} + \Delta_{fiog, t} × {x_c}^T  \\
        \Delta_{b_{fiog}, t} &= \Delta_{b_{fiog}, t - 1} + \Delta_{fiog, t}  \\

        \Delta_{c, t} &= o_t · \Delta_{h_{t+1}} + \Delta_{c_{t+1}}  \\
        \Delta_{h, t} &= \Delta_{x_c} [\Delta_{x, t}, \Delta_{h, t}]  \\
        \Delta_{x_c, t} &= \sum W^T × \Delta_{fiog, t}  \\
      \end{align*}
      $$  

* Gate Recurrent Unit ; GRU
  - Define   
    Input: a time series $x \in \mathbb R^{\dim_x × T_{seq}}$  
    Output: a time series $h \in \mathbb R^{\dim_x × T_{seq}}, h = \{h_1, ...,h_{T_{seq}}\}$. Sometimes, we can take a part or the last element $h_{T_{seq}}$ of the sequence $h$ as the output.  
    Hyperparameter: the dimension of hidden state $h_t$: $\dim_h$

    For a moment $t$, the define of the cell of GRU:
    $$h_t = f_{Cell}(x_t, h_{t-1})  \tag{a cell of GRU in moment $t$}$$


    $$
    \begin{align*}
      \left(\begin{matrix} z_t \\ r_t\end{matrix}\right) &= \text{sigmoid}\left(W_{rz} \left(\begin{matrix} h_{t-1} \\ x_t \end{matrix}\right) + b_{rz}\right)  \tag{reset \& input gate}\\
      \tilde h_t &= \text{tanh}\left(W_h \left(\begin{matrix}r_t \odot h_{t-1} \\ x_t \end{matrix}\right) + b_h\right)  \tag{cell input}\\
      h_t &= (1 - z_t) \odot h_{t-1} + z_t \odot \tilde h_t  \tag{hidden state}
    \end{align*}
    $$  

    [Gate Recurrent Unit Network (Code)](./files/code/Gated_Recurrent_Unit_Network.py)

    |Symbols|Mean|
    |---|---|  
    |$x_t \in \mathbb R^{\dim_x}$| input; the value of the time series $x$ at time $t$|
    |$\dim_x$| the dimension of input $x_t$|
    |$\dim_h$| the dimension of hidden state $h_t$|
    |$r_t \in (0,1)^{\dim_h}$| reset gate |
    |$z_t \in (0,1)^{\dim_h}$| input gate |
    |$\tilde h_t \in (-1,1)^{\dim_h}$| cell input |
    |$h_t \in (-1,1)^{\dim_h}$| hidden state |
    |$W_{rz} \in \mathbb R^{2 \dim_h \times (\dim_h + \dim_x)}$| weights|
    |$W_{h} \in \mathbb R^{\dim_h \times (\dim_h + \dim_x)}$| weights|
    |$\odot$| multiply by element|
    |||

    - Note  
      GRU is a simple version of LSTM with lower complexity and Slightly poor performance.