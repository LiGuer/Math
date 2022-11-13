import torch
from torch import nn

class _DenseLayer(nn.Sequential):
	def __init__(self, num_input_features, growth_rate, bn_size, drop_rate):
		super().__init__()
		self.add_module('norm1', nn.BatchNorm2d(num_input_features))
		self.add_module('relu1', nn.ReLU(inplace=True))
		self.add_module('conv1', nn.Conv2d(
			num_input_features, bn_size * growth_rate,
			kernel_size=1, stride=1, bias=False
		))
		self.add_module('norm2', nn.BatchNorm2d(bn_size * growth_rate))
		self.add_module('relu2', nn.ReLU(inplace=True))
		self.add_module('conv2', nn.Conv2d(
			bn_size * growth_rate, growth_rate,
			kernel_size=3, stride=1, padding=1, bias=False
		))
		self.drop_rate = drop_rate

	def forward(self, x):
		new_features = super().forward(x)
		if self.drop_rate > 0:
			new_features = F.dropout(
				new_features, p=self.drop_rate,
				training=self.training
			)
		return torch.cat([x, new_features], 1)


class DenseNetUnit(nn.Sequential):
	def __init__(
		self, channels, nb_flows, layers=5, growth_rate=12,
		num_init_features=32, bn_size=4, drop_rate=0.2
	):
		super().__init__()

		if channels > 0:
			self.features = nn.Sequential(OrderedDict([
				('conv0', nn.Conv2d(channels, num_init_features, kernel_size=3, padding=1)),
				('norm0', nn.BatchNorm2d(num_init_features)),
				('relu0', nn.ReLU(inplace=True))
			]))

			# Dense Block
			num_features = num_init_features
			num_layers = layers
			block = _DenseBlock(
				num_layers=num_layers, num_input_features=num_features,
				bn_size=bn_size, growth_rate=growth_rate, drop_rate=drop_rate
			)
			self.features.add_module('denseblock', block)
			num_features = num_features + num_layers * growth_rate

			# Final batch norm
			self.features.add_module('normlast', nn.BatchNorm2d(num_features))
			self.features.add_module('convlast', nn.Conv2d(
				num_features, nb_flows,
				kernel_size=1, padding=0, bias=False
			))

			for m in self.modules():
				if isinstance(m, nn.Conv2d):
					nn.init.kaiming_normal_(m.weight.data)
				elif isinstance(m, nn.BatchNorm2d):
					m.weight.data.fill_(1)
					m.bias.data.zero_()
				elif isinstance(m, nn.Linear):
					m.bias.data.zero_()
		else:
			pass

	def forward(self, x):
		y = self.features(x)
		return y


class DenseNet(nn.Module):
	def __init__(self, nb_flows, drop_rate, channels):
		super().__init__()
		self.channels = channels

		self.close_feature  = DenseNetUnit(channels, nb_flows)
		self.relu = nn.ReLU(inplace=True)

	def forward(self, x):
		y = self.close_feature(x)
		return torch.sigmoid(y)

		