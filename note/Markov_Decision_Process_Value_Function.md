* Markov Decision Process::Value Function & Action-Value Function
  - Define  
    $$
    \begin{align*}
      V_\pi(s) &= \mathbb E_\pi(G_t | S_t=s)  \tag{Value function}\\
      Q_\pi(s,a) &= \mathbb E_\pi(G_t | S_t = s, A_t = a)  \tag{Action-Value function}\\
      V_\pi(s) &= \sum_{a \in A} Q_\pi(s,a)
    \end{align*}
    $$ 

    Value function: 智能体在当前状态$S_t$和策略$\pi$下的期望的累计收益.  
    Action-Value function: 智能体在当前状态$S_t$和策略$\pi$下, 在做出动作$A_t$后的期望的累计收益.

  - Property
    - Bellman Equation
      $$
      \begin{align*}
        V_\pi(s) &= \mathbb E_\pi(R_{t+1} + \gamma V_\pi(s') | S_t = s)   \\
          &= \sum_a \mathbb P_\pi(a | s) \sum_{s',r} \mathbb P(s', r | s, a) (r + \gamma V_\pi (s'))
        \end{align*}
        $$
      其中, $V_\pi(s') = \mathbb E_\pi(G_{t+1} | S_{t+1} = s')$
      
      - Proof
        $$
        \begin{align*}
          V(s) &= \mathbb E(G_t | S_t=s)  \tag{Define}  \\
            &= \mathbb E(R_{t+1} + \gamma G_{t+1} | S_t=s)  \tag{substitution}  \\
            &= \sum_{a \in A} \mathbb P_\pi(a | s)  \left( \sum_{s' \in S} \sum_{r \in R} \mathbb P(s', r | s, a) · (r + \gamma \mathbb E(G_{t+1} | S_{t+1} = s')) \right)  \tag{formula of Expectation}  \\
            &= \sum_{a \in A} \mathbb P_\pi(a | s)  \left( \sum_{s' \in S} \sum_{r \in R} \mathbb P(s', r | s, a) · (r + \gamma V(s')) \right)  \tag{substitution}  \\
            &= \mathbb E(R_{t+1} + \gamma V(S_{t+1}) | S_t=s)
        \end{align*}
        $$

    - theorem
      $$\begin{align*}
        Q(s,a) &= \mathbb E(G_t | S_t = s, A_t = a)  \\
          &= \mathbb E \left(\sum_{k=0}^\infty \gamma^k R_{t+k+1} | S_t = s, A_t = a \right)  \\
          &= \mathbb E_{s'}(r+\gamma Q (s', a') | s, a)  \\
      \end{align*}$$

  - Problem: estimate the Value function
    - Dynamic Programming: 迭代寻找不动点
      $$V_{k+1} (s) = \sum_a \mathbb P_\pi(a | s) \sum_{s',r} \mathbb P(s', r | s, a) (r + \gamma V_k (s'))$$
      $V_\pi$即是迭代寻找的不动点

    - Monte Carlo Method: 随机采样估计  
      当环境转移概率$\mathbb P(s', r | s, a)$不知道时, 通过随机采样$G_t$以估计$V_\pi(s)$.
      $$V_\pi(s) \gets V_\pi(s) + \alpha (G_t - V_\pi(s))$$
        
    - Temporal Difference Learning
      $$\begin{align*}
        V_\pi(s)  &\gets V_\pi(s) + \alpha (r + \gamma V_\pi(s') - V_\pi(s))  \\
        Q(s, a) &\gets Q(s, a) + \alpha (r + \gamma Q(s', a') - Q(s, a))  \tag{on policy}\\
        Q(s, a) &\gets Q(s, a) + \alpha \left(r + \gamma · \max_{a_i' \in A} Q(s', a_i') - Q(s, a)\right)  \tag{off policy}
      \end{align*}$$
