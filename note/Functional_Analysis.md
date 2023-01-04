* Functional Analysis

  * Laplace Transform


  * Wavelet Transform
    - Define
      $$W_{\phi, f}(a, b) = \frac{1}{\sqrt{|a|}} \int_{-\infty}^{\infty} f(x) \overline{\phi \left(\frac{x - b}{a} \right)} \mathrm d x$$
      $$\phi_{jk}(x) = 2^{\frac{j}{2}} \phi(2^j x - k)$$
      $$c_{jk} =  W_{\phi, f}(2^{-j}, k 2^{-j})$$

  * Cross Correlation & Convolution
    - Define  
      Cross Correlation and Convolution, are a measure of similarity (through inner product) of two funcions $f, g$ of the displacement $t \in \mathbb R$ of one relative to the other.
        
      $$\begin{align*}
      Corr(f(t), g(t)) 
        &= \sum_{t = -\infty}^{\infty} f(\tau) g(t + \tau)  \tag{Cross Correlation}  \\
        &= \sum_{t = -\infty}^{\infty} f(-t+\tau) g(\tau)  \\
        &= \int_{-\infty}^{\infty} f(\tau) g(t + \tau) \mathrm d \tau   \\
        &= \int_{-\infty}^{\infty} f(-t+\tau) g(\tau) \mathrm d \tau  \\

      Conv(f(t), g(t)) 
        &= \sum_{t = -\infty}^{\infty} f(\tau) g(t - \tau)  \tag{Convolution}  \\
        &= \sum_{t = -\infty}^{\infty} f(t-\tau) g(\tau)  \\
        &= \int_{-\infty}^{\infty} f(\tau) g(t - \tau) \mathrm d \tau   \\
        &= \int_{-\infty}^{\infty} f(t-\tau) g(\tau) \mathrm d \tau  \\
      \end{align*}$$ 

    - Property
      - Commutativity & Associativity & Distributivity
      $$\begin{align*}
        Conv(f, g)  &= Conv(g, f)  \tag{Commutativity}  \\
        Conv(f, Conv(g, h))  &= Conv(Conv(f, g), h)  \tag{Associativity}  \\
        Conv(f, g+h)  &= Conv(f, g) + Conv(f, h)  \tag{Distributivity}  \\
        a·Conv(f, g)  &= Conv(a·f, g)  \tag{Associativity with scalar multiplication}  \\
      \end{align*}$$ 

    - Application  
      - cross-correlation is a measure of similarity of two series as a function of the displacement of one relative to the other. 