import torch
import torch.nn as nn
import torch.nn.functional as F

# Self-Attention
class SelfAttention(nn.Module):
    def __init__(self, dim):
        super().__init__()
        self.dim = dim
        
        self.Q_linear = nn.Linear(dim, dim)
        self.K_linear = nn.Linear(dim, dim)
        self.V_linear = nn.Linear(dim, dim)
    
    def forward(self, X, mask=None):
        Q = self.Q_linear(X)
        K = self.K_linear(X)
        V = self.V_linear(X)
        
        W = torch.matmul(Q, K.transpose(-2, -1)) / torch.sqrt(torch.tensor(self.d_k).float())

        if mask is not None:
            W = W.masked_fill(mask == 0, -1e9)
        
        attention = F.softmax(W, dim=-1)
        Y = torch.matmul(attention, V)

        return Y, attention
    