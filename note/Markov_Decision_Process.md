* Markov Decision Process
  - Define  
    Markov decision process is a discrete-time stochastic control process with a 4-tuple 
    $$<S, A, \mathbb P(S_t = s_t | S_{t-1} = s_{t-1}, A_{t-1} = a_{t-1}), \mathbb P(R_t = r_t| S_t = s_t, S_{t-1} = s_{t-1}, A_{t-1} = a_{t-1})>$$
    , and the state transition $S_{t-1} \to S_{t}$ only depends on the last moment $S_{t-1}, A_{t-1}$.  

    Markov Reinforcement Learning is a problem aimed at finding the optimal strategy $\pi^*$ to maximize Cumulative Reward $G$ of a Markov decision process.
    $$
    \begin{align*}
      \max_{\pi} \quad & \mathbb E(G) = \sum_{t = 0}^\infty \mathbb E(R_t)  \tag{objective}\\
      s.t. \quad
      & \mathbb P(S_t = s_t | S_{t-1} = s_{t-1}, A_{t-1} = a_{t-1})    \tag{Environmental Response}\\
      & \mathbb P(R_t = r_t| S_t = s_t, S_{t-1} = s_{t-1}, A_{t-1} = a_{t-1})  \\
      & a_t = \pi(s_t) \tag{Agent's Strategy}
    \end{align*}
    $$

    |Symbol|Means|
    |---|---|
    |$t$|Time|
    |$S$|State set|
    |$S_t$|State of time $t$; a random variable|
    |$A$|Action set|
    |$A_t$|Action of time $t$; a random variable|
    |$R_t$|Immediate Reward of time $t$; a random variable|
    |$G$|Cumulative Reward of all steps; a random variable|
    |$\mathbb P(S_t = s_t \| S_{t-1} = s_{t-1}, A_{t-1} = a_{t-1})$|Transition Probability|
    |$\mathbb P(R_t = r_t\| S_t = s_t, S_{t-1} = s_{t-1}, A_{t-1} = a_{t-1})$|Probability of Immediate Reward|
    |$\pi : S_t \to A_t$|Strategy, Policy function|
    |||
    |$T$|the Termination time|
    |$G_t$|Cumulative Reward from time $t$ to the Termination time; a random variable|
    |$\gamma$|Discount of Reward|
    |||

  - Property
    - Cumulative Reward of time $t$ & Reword discount
      $$G_t= \sum_{\tau = t+1}^T R_\tau  \tag{Cumulative Reward of $t$}$$  
      In order to avoid Termination time $T \to ∞, G_t \to ∞$, introduce the discount $γ \in [0,1]$,  
      $$G_t = \sum_{k=0}^∞ γ^k R_{t+k+1} = R_{t+1} + γ G_{t+1}$$

    * Value function & Action-Value function
      - Define  
        $$
        \begin{align*}
          V_π(s) &= \mathbb E_π(G_t | S_t=s)  \tag{Value function}\\
          Q_π(s,a) &= \mathbb E_π(G_t | S_t = s, A_t = a)  \tag{Action-Value function}\\
          V_π(s) &= \sum_{a \in A} Q_π(s,a)
        \end{align*}
        $$ 

        Value function: 智能体在当前状态$S_t$和策略$π$下的期望的累计收益.  
        Action-Value function: 智能体在当前状态$S_t$和策略$π$下, 在做出动作$A_t$后的期望的累计收益.

      - Property
        - Theorem -- Bellman Equation
          $$
          \begin{align*}
            V_π(s) &= \mathbb E_π(R_{t+1} + γ V_π(s') | S_t = s)   \\
              &= \sum_a \mathbb P_π(a | s) \sum_{s',r} \mathbb P(s', r | s, a) (r + γ V_π (s'))
            \end{align*}
            $$
          其中, $V_π(s') = \mathbb E_π(G_{t+1} | S_{t+1} = s')$
          
          - Proof
            $$
            \begin{align*}
              V(s) &= \mathbb E(G_t | S_t=s)  \tag{Define}  \\
                &= \mathbb E(R_{t+1} + γ G_{t+1} | S_t=s)  \tag{substitution}  \\
                &= \sum_{a \in A} \mathbb P_π(a | s)  \left( \sum_{s' \in S} \sum_{r \in R} \mathbb P(s', r | s, a) · (r + γ \mathbb E(G_{t+1} | S_{t+1} = s')) \right)  \tag{formula of Expectation}  \\
                &= \sum_{a \in A} \mathbb P_π(a | s)  \left( \sum_{s' \in S} \sum_{r \in R} \mathbb P(s', r | s, a) · (r + γ V(s')) \right)  \tag{substitution}  \\
                &= \mathbb E(R_{t+1} + γ V(S_{t+1}) | S_t=s)
            \end{align*}
            $$

        - theorem
          $$
          \begin{align*}
            Q(s,a) &= \mathbb E(G_t | S_t = s, A_t = a)  \\
              &= \mathbb E \left(\sum_{k=0}^∞ γ^k R_{t+k+1} | S_t = s, A_t = a \right)  \\
              &= \mathbb E_{s'}(r+γ Q (s', a') | s, a)  \\
          \end{align*}
          $$

      - Problem -- estimate the Value function
        - Dynamic Programming -- 迭代寻找不动点
          $$V_{k+1} (s) = \sum_a \mathbb P_π(a | s) \sum_{s',r} \mathbb P(s', r | s, a) (r + γ V_k (s'))$$
          $V_π$即是迭代寻找的不动点

        - Monte Carlo Method -- 随机采样估计  
          当环境转移概率$\mathbb P(s', r | s, a)$不知道时, 通过随机采样$G_t$以估计$V_π(s)$.
          $$V_π(s) \gets V_π(s) + α (G_t - V_π(s))$$
            
        - Temporal Difference Learning
          $$
          \begin{align*}
            V_π(s)  &\gets V_π(s) + α (r + γ V_π(s') - V_π(s))  \\
            Q(s, a) &\gets Q(s, a) + α (r + γ Q(s', a') - Q(s, a))  \tag{on policy}\\
            Q(s, a) &\gets Q(s, a) + α \left(r + γ · \max_{a_i' \in A} Q(s', a_i') - Q(s, a)\right)  \tag{off policy}
          \end{align*}
          $$

    - the selection of Policy function  
      Deterministic strategy $a = π(s)$  
      Random strategy $\mathbb P_π(a | s)$  

      - Problem -- finding the optimal policy
        - 策略梯度上升方法
          - 原理
            $$
            \begin{align*}
              \mathbb P_π (a | s, θ)  \\
              θ_{t+1} = θ_{t+1} + α \widehat{∇ J(θ_t)}
            \end{align*}
            $$
            通过策略梯度上升更新随机性策略的参数$θ$. $J(θ)$是对策略的性能度量函数, 可定义为$J(θ) = V_{π_\theta} (s_{t=0})$.

          - Theorem -- 策略梯度定理  
            $$
            \begin{align*}
              ∇ J(\.θ) &= ∇_θ V(s)  \\
                &\propto \sum_{s' \in S} \mathbb P_{μ,π}(s') \sum_{a \in A} Q_π(s', a) ∇_θ \mathbb P_π(a | s', \.θ)   \\
                &\propto \mathbb E_{s' ~ \mathbb P_{μ,π}, a ~ π}(Q_π(s', a) ∇_θ \ln \mathbb P_π (a | s'))
            \end{align*}
            $$

            - Proof
              $$
              \begin{align*}
                ∇_θ V_π(s) &= ∇_θ\ \mathbb E(G_t|S_t = s)  \tag{价值函数定义}  \\
                  &= ∇_θ \left(\sum_{a \in A} \mathbb P_π(a|s)\ Q(s,a) \right)  \tag{期望公式}  \\
                  &= \sum_{a \in A} ∇_θ(\mathbb P_π(a|s)\ Q(s,a))  \tag{分配律}  \\
                  &= \sum_{a \in A}(Q(s,a)\ ∇_θ \mathbb P_π(a|s) + \mathbb P_π(a|s)\ ∇_θ Q(s,a))  \tag{微分乘法法则}  \\
                  &= \sum_{a \in A}(Q(s,a)\ ∇_θ \mathbb P_π(a|s) + \mathbb P_π(a|s)\ ∇_θ \sum_{s', r} \mathbb P(s', r | s, a)(r + γ V_π(s')))  \tag{代入}  \\
                  &= \sum_{a \in A}(Q(s,a)\ ∇_θ \mathbb P_π(a|s) + \mathbb P_π(a|s)\ \sum_{s', r} \mathbb P(s', r | s, a) γ ∇_θ V_π(s'))  \tag{分配律}  \\
                  &= \sum_{a \in A} Q(s,a)\ ∇_θ \mathbb P_π(a|s) + \sum_{s'} γ ∇_θ V_π(s') \sum_{a \in A} \mathbb P_π(a|s)\sum_r \mathbb P(s', r | s, a)  \tag{加法性质}  \\
                  &= \sum_{a \in A} Q(s,a)\ ∇_θ \mathbb P_π(a|s) + \sum_{s'} γ ∇_θ V_π(s') \sum_{a \in A} \mathbb P(s', a| s)  \tag{条件概率公式}  \\
                  &= \sum_{a \in A} Q(s,a)\ ∇_θ \mathbb P_π(a|s) + \sum_{s'} γ \mathbb P(s'| s) ∇_θ V_π(s')  \tag{概率求和}  \\
                  &= \sum_{s' \in S} \sum_{t=0}^∞ γ^t \mathbb P^{(t)}_π(s' | s) \sum_{a \in A} Q_π(s', a) ∇_θ \mathbb P_π(a|s')  \tag{递推$ ∇_θ V_π(s')$展开, $\mathbb P^{(t)}$是t步转移概率}  \\
                  &=\left(\sum_{s''\in S} \sum_{t=0}^∞ γ^t \mathbb P_π^{(t)}(s''| s)\right) · \sum_{s' \in S} \frac{\sum_{t=0}^∞ γ^t \mathbb P_π^{(t)}(s'| s)}{\sum_{s''\in S}(\sum_{t=0}^∞ γ^t \mathbb P_π^{(t)}(s''| s))} \sum_{a \in A} Q_π(s', a) ∇_θ \mathbb P_π(a|s')  \\ \tag{提项,将$\sum_{t=0}^∞ \mathbb P_π^{(t)}(s'| s)$归一化}  \\
                  &= α \sum_{s' \in S} \mathbb P_{μ,π}(s') \sum_{a \in A} Q_π(s', a) ∇_θ \mathbb P_π(a|s')  \\ \tag{\small 简写,$(\sum_{s''\in S} \sum_{t=0}^∞ γ^t \mathbb P_π^{(t)}(s''| s))$简为系数,$\mathbb P_{μ,π}(s')$指状态在策略下整个时间序列出现的概率}  \\
                  &= α \mathbb E_{s' ~ \mathbb P_{μ,π}}\left(\sum_{a \in A} Q_π(s', a) ∇_θ \mathbb P_π(a|s')\right)  \tag{期望形式}  \\
                  &= α \mathbb E_{s' ~ \mathbb P_{μ,π}}\left(\sum_{a \in A} \mathbb P_π(a|s') Q_π(s', a) \frac{∇_θ \mathbb P_π(a|s')}{\mathbb P_π(a|s')}\right)  \tag{提项$\mathbb P_π(a|s')$, 方便后面对$a$写期望}  \\
                  &= α \mathbb E_{s' ~ \mathbb P_{μ,π}, a ~ π}(Q_π(s', a)\ ∇_θ \ln \mathbb P_π(a|s'))  \tag{期望形式, 微分公式}
              \end{align*}
              $$



