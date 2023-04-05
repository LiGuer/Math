* Markov Decision Process
  - Define  
    $$<S, A, P, R, \gamma>  \tag{Markov decision process}$$

    $$\begin{align*}
      P &= \mathbb P(S_t = s_t | S_{t-1} = s_{t-1}, A_{t-1} = a_{t-1})  \tag{state transition probability}\\
      R &= \mathbb P(R_t = r_t| S_t = s_t, S_{t-1} = s_{t-1}, A_{t-1} = a_{t-1})  \tag{reward function}
    \end{align*}$$

    Markov decision process is a discrete-time stochastic control process with a tuple, and the state transition $S_{t-1} \to S_{t}$ only depends on the last moment $S_{t-1}, A_{t-1}$.  

    Markov Reinforcement Learning is a problem aimed at finding the optimal strategy $\pi^*$ to maximize Cumulative Reward $G$ of a Markov decision process.
    $$\begin{align*}
      \max_{\pi} \quad & \mathbb E(G) = \sum_{t = 0}^\infty \mathbb E(R_t)  \tag{objective}\\
      s.t. \quad
      & \mathbb P(S_t = s_t | S_{t-1} = s_{t-1}, A_{t-1} = a_{t-1})    \tag{Environmental Response}\\
      & \mathbb P(R_t = r_t| S_t = s_t, S_{t-1} = s_{t-1}, A_{t-1} = a_{t-1})  \\
      & a_t = \pi(s_t) \tag{Agent's Strategy}
    \end{align*}$$

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
      In order to avoid Termination time $T \to \infty, G_t \to \infty$, introduce the discount $\gamma \in [0,1]$,  
      $$G_t = \sum_{k=0}^\infty \gamma^k R_{t+k+1} = R_{t+1} + \gamma G_{t+1}$$

    * Markov Decision Process::Value Function & Action-Value Function 
    - the selection of Policy function  
      Deterministic strategy $a = \pi(s)$  
      Random strategy $\mathbb P_\pi(a | s)$  

      - Problem: finding the optimal policy
        - 策略梯度上升方法
          - 原理
            $$\begin{align*}
              \mathbb P_\pi (a | s, \theta)  \\
              \theta_{t+1} = \theta_{t+1} + \alpha \widehat{\nabla J(\theta_t)}
            \end{align*}$$
            通过策略梯度上升更新随机性策略的参数$\theta$. $J(\theta)$是对策略的性能度量函数, 可定义为$J(\theta) = V_{\pi_\theta} (s_{t=0})$.

          - Strategy gradient theorem  
            $$\begin{align*}
              \nabla J(\boldsymbol \theta) &= \nabla_\theta V(s)  \\
                &\propto \sum_{s' \in S} \mathbb P_{μ,\pi}(s') \sum_{a \in A} Q_\pi(s', a) \nabla_\theta \mathbb P_\pi(a | s', \boldsymbol \theta)   \\
                &\propto \mathbb E_{s' \sim \mathbb P_{μ,\pi}, a \sim \pi}(Q_\pi(s', a) \nabla_\theta \ln \mathbb P_\pi (a | s'))
            \end{align*}$$

            - Proof
              $$\begin{align*}
                \nabla_\theta V_\pi(s) 
                &= \nabla_\theta\ \mathbb E(G_t|S_t = s)  \tag{价值函数定义}  \\
                &= \nabla_\theta \left(\sum_{a \in A} \mathbb P_\pi(a|s)\ Q(s,a) \right)  \tag{期望公式}  \\
                &= \sum_{a \in A} \nabla_\theta(\mathbb P_\pi(a|s)\ Q(s,a))  \tag{分配律}  \\
                &= \sum_{a \in A}(Q(s,a)\ \nabla_\theta \mathbb P_\pi(a|s) + \mathbb P_\pi(a|s)\ \nabla_\theta Q(s,a))  \tag{微分乘法法则}  \\
                &= \sum_{a \in A}(Q(s,a)\ \nabla_\theta \mathbb P_\pi(a|s) + \mathbb P_\pi(a|s)\ \nabla_\theta \sum_{s', r} \mathbb P(s', r | s, a)(r + \gamma V_\pi(s')))  \tag{代入}  \\
                &= \sum_{a \in A}(Q(s,a)\ \nabla_\theta \mathbb P_\pi(a|s) + \mathbb P_\pi(a|s)\ \sum_{s', r} \mathbb P(s', r | s, a) \gamma \nabla_\theta V_\pi(s'))  \tag{分配律}  \\
                &= \sum_{a \in A} Q(s,a)\ \nabla_\theta \mathbb P_\pi(a|s) + \sum_{s'} \gamma \nabla_\theta V_\pi(s') \sum_{a \in A} \mathbb P_\pi(a|s)\sum_r \mathbb P(s', r | s, a)  \tag{加法性质}  \\
                &= \sum_{a \in A} Q(s,a)\ \nabla_\theta \mathbb P_\pi(a|s) + \sum_{s'} \gamma \nabla_\theta V_\pi(s') \sum_{a \in A} \mathbb P(s', a| s)  \tag{条件概率公式}  \\
                &= \sum_{a \in A} Q(s,a)\ \nabla_\theta \mathbb P_\pi(a|s) + \sum_{s'} \gamma \mathbb P(s'| s) \nabla_\theta V_\pi(s')  \tag{概率求和}  \\
                &= \sum_{s' \in S} \sum_{t=0}^\infty \gamma^t \mathbb P^{(t)}_\pi(s' | s) \sum_{a \in A} Q_\pi(s', a) \nabla_\theta \mathbb P_\pi(a|s')  \tag{递推$ \nabla_\theta V_\pi(s')$展开, $\mathbb P^{(t)}$是t步转移概率}  \\
                &=\left(\sum_{s''\in S} \sum_{t=0}^\infty \gamma^t \mathbb P_\pi^{(t)}(s''| s)\right) · \sum_{s' \in S} \frac{\sum_{t=0}^\infty \gamma^t \mathbb P_\pi^{(t)}(s'| s)}{\sum_{s''\in S}(\sum_{t=0}^\infty \gamma^t \mathbb P_\pi^{(t)}(s''| s))} \sum_{a \in A} Q_\pi(s', a) \nabla_\theta \mathbb P_\pi(a|s')  \\ \tag{提项,将$\sum_{t=0}^\infty \mathbb P_\pi^{(t)}(s'| s)$归一化}  \\
                &= \alpha \sum_{s' \in S} \mathbb P_{μ,\pi}(s') \sum_{a \in A} Q_\pi(s', a) \nabla_\theta \mathbb P_\pi(a|s')  \\
                &= \alpha \mathbb E_{s' \sim \mathbb P_{μ,\pi}}\left(\sum_{a \in A} Q_\pi(s', a) \nabla_\theta \mathbb P_\pi(a|s')\right)  \tag{期望形式}  \\
                &= \alpha \mathbb E_{s' \sim \mathbb P_{μ,\pi}}\left(\sum_{a \in A} \mathbb P_\pi(a|s') Q_\pi(s', a) \frac{\nabla_\theta \mathbb P_\pi(a|s')}{\mathbb P_\pi(a|s')}\right)  \tag{提项$\mathbb P_\pi(a|s')$, 方便后面对$a$写期望}  \\
                &= \alpha \mathbb E_{s' \sim \mathbb P_{μ,\pi}, a \sim \pi}(Q_\pi(s', a)\ \nabla_\theta \ln \mathbb P_\pi(a|s'))  \tag{期望形式, 微分公式}
              \end{align*}$$



