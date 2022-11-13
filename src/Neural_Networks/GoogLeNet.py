import torch
import torch.nn as nn

class BasicConv2d(nn.Module):
    def __init__(self, in_channels, out_channals, **kwargs):
        super().__init__()

        self.conv = nn.Conv2d(in_channels, out_channals, **kwargs)
        self.bn   = nn.BatchNorm2d(out_channals)
        self.relu = nn.ReLU(inplace=True)

    def forward(self, x):
        x = self.conv(x)
        x = self.bn(x)
        x = self.relu(x)
        return x


class Inception(nn.Module):
    def __init__(self, in_planes,n1x1, n3x3red, n3x3, n5x5red, n5x5, pool_planes):
        super().__init__()

        self.b1       = BasicConv2d(in_planes, n1x1, kernel_size=1)
        self.b2_1x1_a = BasicConv2d(in_planes, n3x3red, kernel_size=1)
        self.b2_3x3_b = BasicConv2d(n3x3red, n3x3, kernel_size=3, padding=1)
        self.b3_1x1_a = BasicConv2d(in_planes, n5x5red, kernel_size=1)
        self.b3_3x3_b = BasicConv2d(n5x5red, n5x5, kernel_size=3, padding=1)
        self.b3_3x3_c = BasicConv2d(n5x5, n5x5, kernel_size=3, padding=1)
        self.b4_pool  = nn.MaxPool2d(3, stride=1, padding=1)
        self.b4_1x1   = BasicConv2d(in_planes, pool_planes, kernel_size=1)
        
    def forward(self, x):
        y1 = self.b1(x)
        y2 = self.b2_3x3_b(self.b2_1x1_a(x))
        y3 = self.b3_3x3_c(self.b3_3x3_b(self.b3_1x1_a(x)))
        y4 = self.b4_1x1(self.b4_pool(x))

        y = torch.cat([y1, y2, y3, y4], 1)
        return y


class GoogLeNet(nn.Module):
    def __init__(self):
        super().__init__()

        self.pre_layers = BasicConv2d(1, 64, kernel_size=5,padding=2)

        self.a3 = Inception(64,  64,  96, 128, 16, 32, 32)
        self.b3 = Inception(256, 128, 128, 192, 32, 96, 64)

        self.maxpool = nn.MaxPool2d(3, stride=2, padding=1)

        self.a4 = Inception(480, 192,  96, 208, 16,  48,  64)
        self.b4 = Inception(512, 160, 112, 224, 24,  64,  64)
        self.c4 = Inception(512, 128, 128, 256, 24,  64,  64)
        self.d4 = Inception(512, 112, 144, 288, 32,  64,  64)
        self.e4 = Inception(528, 256, 160, 320, 32, 128, 128)
        self.a5 = Inception(832, 256, 160, 320, 32, 128, 128)
        self.b5 = Inception(832, 384, 192, 384, 48, 128, 128)

        self.avgpool = nn.AvgPool2d(8, stride=1)
        self.linear = nn.Linear(1024 * 5 * 5, 7)

    def forward(self, x):
        y = self.pre_layers(x)
        y = self.a3(y)
        y = self.b3(y)
        y = self.maxpool(y)
        y = self.a4(y)
        y = self.b4(y)
        y = self.c4(y)
        y = self.d4(y)
        y = self.e4(y)
        y = self.maxpool(y)
        y = self.a5(y)
        y = self.b5(y)
        y = self.avgpool(y)
        y = y.view(y.size(0), -1)
        y = self.linear(y)
        return y