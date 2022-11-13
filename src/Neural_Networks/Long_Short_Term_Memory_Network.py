import numpy as np
import torch
import torch.nn as nn
from torch.autograd import Variable

class LSTM_Cell(nn.Module):
    def __init__(self, dim_x, dim_h, bias=True):
        super().__init__()

        self.x = nn.Linear(dim_x, 4 * dim_h, bias=bias)
        self.h = nn.Linear(dim_h, 4 * dim_h, bias=bias)

        self.dim_h =  dim_h
        self.reset_parameters()
    
    def forward(self, x, h, c):
        a = self.x(x) + self.h(h)
        f, i, o, g = a.chunk(4, 1)

        f = F.sigmoid(f)
        i = F.sigmoid(i)
        o = F.sigmoid(o)
        g = F.tanh   (g)
        c = torch.mul(c, f) + torch.mul(i, g)        
        h = torch.mul(o, F.tanh(c))
        
        return (h, c)

    def reset_parameters(self):
        std = 1.0 / np.sqrt(self.dim_h)
        for w in self.parameters():
            w.data.uniform_(-std, std)

class LSTM(nn.Module):
    def __init__(self, dim_x, dim_h, dim_y, bias=True):
        super().__init__()

        self.cell = LSTM_Cell(dim_x, dim_h)  
        self.fc = nn.Linear(dim_h, dim_y)
        self.dim_h = dim_h
    
    def forward(self, x):
        len_batch, len_seq, len_feature = x.shape

        h = Variable(torch.zeros(len_batch, self.dim_h)).cuda()
        c = Variable(torch.zeros(len_batch, self.dim_h)).cuda()
        y = []

        for seq in range(len_seq):
            h, c = self.cell(x[:, seq, :], h, c)
            y.append(h)

        y = y[-1].squeeze()
        y = self.fc(y) 
        return y
