import torch
import torch.nn as nn
import torch.nn.functional as F
from torch.autograd import Variable

class GRU_Cell(nn.Module):
    def __init__(self, dim_x, dim_h, bias=True):
        super().__init__()

        self.rz_x = nn.Linear(dim_x, 2 * dim_h, bias=bias)
        self.rz_h = nn.Linear(dim_h, 2 * dim_h, bias=bias)
        self.h_x  = nn.Linear(dim_x, dim_h, bias=bias)
        self.h_h  = nn.Linear(dim_h, dim_h, bias=bias)

        self.dim_h =  dim_h
        self.reset_parameters()
    
    def forward(self, x, h):
        a = F.sigmoid(self.rz_x(x) + self.rz_h(h))
        r, z = a.chunk(2, 1)

        h_ = F.tanh(self.h_x(x) + self.h_h(r * h))
        h = torch.mul(z, h_) + torch.mul((1.0 - z), h)
        
        return h

    def reset_parameters(self):
        std = 1.0 / np.sqrt(self.dim_h)
        for w in self.parameters():
            w.data.uniform_(-std, std)


class GRU(nn.Module):
    def __init__(self, dim_x, dim_h, dim_y, bias=True):
        super().__init__()

        self.cell = GRU_Cell(dim_x, dim_h)  
        self.fc = nn.Linear(dim_h, dim_y)
        self.dim_h = dim_h
    
    def forward(self, x):
        len_batch, len_seq, len_feature = x.shape

        h = Variable(torch.zeros(len_batch, self.dim_h)).cuda()
        y = []

        for seq in range(len_seq):
            h = self.cell(x[:, seq, :], h)
            y.append(h)

        y = y[-1].squeeze()
        y = self.fc(y) 
        return y