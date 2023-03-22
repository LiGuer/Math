* Wavelet Transform
  - Define
    $$W_{\phi, f}(a, b) = \frac{1}{\sqrt{|a|}} \int_{-\infty}^{\infty} f(x) \overline{\phi \left(\frac{x - b}{a} \right)} \mathrm d x$$
    $$\phi_{jk}(x) = 2^{\frac{j}{2}} \phi(2^j x - k)$$
    $$c_{jk} =  W_{\phi, f}(2^{-j}, k 2^{-j})$$
