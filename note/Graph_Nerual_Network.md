* Graph Nerual Network ; GNN
  - Include
    * Graph Convolutional Network ; GCN
      - Define 
        $$
        \begin{align*}
          H_{l+1} 
          &= f(H_l, A)   \tag{l$^{th}$ NN layer}\\
          &= \sigma(\tilde A H_l W_l^T)  \\
          H_{out} 
          &= f(X, A)   \tag{the whole NN}\\
          &= \text{softmax}(\tilde A\ \text{ReLU}(\tilde A X W_{(0)}^T) W_{(1)}^T)\\
          \tilde A &= \tilde D^{1/2}\ (A + I_N)\ \tilde D^{-1/2}\\
          \tilde D_{ii} &= \sum_j (A + I_N)_{ij} 
        \end{align*}
        $$  
        
        | Symbol | Significance |
        |---|---|
        | $G$ | Graph |
        | $N$ | number of nodes |
        | $F_{in}$ | number of features per node |
        | $F_{out}$ | number of output features per node |
        | $X \in \mathbb R^{N×F_{in}}$ | feature matrix ; $x_i$ is a feature description for node $i$ | 
        | $A \in \mathbb R^{N×N}$ | adjacency matrix of the Graph $G$ |
        | $H_{out} \in \mathbb R^{N×F_{out}}$ | output matrix |
        | $H_l \in \mathbb R^{N×F_{l}}$ | the input of $l^{th}$ neural network layer|
        | $W_l \in \mathbb R^{F_{l+1}×F_{l}}$ | the wights of $l^{th}$ neural network layer |
        | $H_{l+1} = f(H_l, A)$ | $l^{th}$ neural network layer ; $H_0 = X, H_L = H_{out}$ |
        | $\tilde D$ | diagonal node degree matrix of $(A + I_N)$ |
        |||

        - Note
          - $(A + I_N)$: if only $(A H)$ means sum up all the feature vectors of all neighboring nodes but not the node itself (unless there are self-loops in the graph)

          - $\tilde D^{1/2} (A + I_N) \tilde D^{-1/2}$: normalized 

    * Graph Attention Network
      - Define 
        $$
        \begin{align*}
          h_i &= \sigma\left(\sum_{j \in N_i} \alpha_{ij} W h_j\right)  \tag{the layer of NN}\\
          H_{l+1} &= \sigma\left(A H_l W^T\right)    \\
          \Rightarrow h_i &= \underset{k}{\|} \sigma\left(\sum_{j \in N_i} \alpha_{ij,k} W_k h_j\right)  \tag{multi-head attention}\\
          &= \sigma\left(\frac{1}{K} \sum_{k=1}^K \sum_{j \in N_i} \alpha_{ij,k} W_k h_j\right)
        \end{align*}
        $$

        $$
        \begin{align*}
          \alpha_{ij} 
          &= \text{softmax}(e_{ij})  \tag{Attention Wights}\\
          &= \text{softmax}(a(W h_i, W h_j))  \\
          &= \text{softmax}(\text{LeakyReLU}(\boldsymbol a^T (W h_i\ ||\ W h_j)))  \\
        \end{align*}
        $$

        | Symbol | Significance |
        |---|---|
        | $G$ | Graph |
        | $N$ | number of nodes |
        | $F_{in}$ | number of features per node |
        | $F_{out}$ | number of output features per node |
        | $X \in \mathbb R^{N \times F_{int}}$ | input |
        | $H_{out} \in \mathbb R^{N \times F_{out}}$ | output |
        | $H_{l} \in \mathbb R^{N \times F_{l}}$ | input of every layer of NN |
        | $W_{l} \in \mathbb R^{F_{l+1} \times F_{l}}$ | Wights of every layer of NN |
        | $A_{l} \in \mathbb R^{N \times N}$ | Attention Wights of every layer of NN |3
        | $\alpha_{ij,l} \in \mathbb R$ | the Attention Wights of edge $ij$ of every layer of NN |
        |$\boldsymbol a \in \mathbb R^{2 F_{l+1}}$| a weight vector |
        |||