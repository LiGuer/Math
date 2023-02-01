import numpy as np
import torch
import torch.nn as nn

from CNN import Net

# --------------------------------

# 				Deep QNetwork 

# --------------------------------
class DeepQNetwork:
	def __init__(
		self, actionDim, stateDim, 
		learningRate=0.01, gamma=0.9, epsilonMax=1.2, epsilonDelta = 0.001, ReplaceIter=100, MemorySize=2000, BatchSize=32
	):
		self.actionDim	= actionDim
		self.stateDim 	= stateDim
		
		self.learningRate = learningRate					#学习率
		self.gamma = gamma				#参数gamma
		self.epsilonMax = epsilonMax				#最大贪婪值
		self.ReplaceIter = ReplaceIter
		self.BatchSize = BatchSize
		self.epsilonDelta = epsilonDelta
		self.epsilon = epsilonMax
		self.StepCur = 0						#学习步数指针
		self.MemorySize = MemorySize			#记忆最大个数
		self.MemoryCur = 0						#记忆指针
		self.Memory = np.zeros((self.MemorySize, stateDim * 2 + 2))	#记忆
		
		self.EvalNet	 = Net(stateDim, actionDim)				#评价网络
		self.TargetNet = Net(stateDim, actionDim)				#目标网络
		
		self.TargetNet.load_state_dict(self.EvalNet.state_dict())
		self.optimizer = torch.optim.Adam(self.EvalNet.parameters(), lr = self.learningRate)
		self.LossFunc = nn.MSELoss()			#均方损失函数

	# A = F(S)
	def ActionSelect(self, state):
		state = torch.FloatTensor(state[np.newaxis,:])

		# random
		if np.random.uniform() < self.epsilon:
			action = np.random.choice(self.actionDim)
			self.epsilon -= self.epsilonDelta

		# determine
		else:
			q = self.EvalNet(state)
			action = torch.max(q,1)[1].data.numpy()

		return action

	# 记忆库存储
	def MemorySave(self, stateOld, stateNew, action, reward):
		self.Memory[self.MemoryCur % self.MemorySize, :] = np.hstack((stateOld, stateNew, [action, reward]))
		self.MemoryCur += 1

	# update Q(S,A)
	def Train(self):
		if self.StepCur % self.ReplaceIter == 0:
			self.TargetNet.load_state_dict(self.EvalNet.state_dict())	#更新TargetNet，同EvalNet一致
		self.StepCur += 1

		# 取值
		BatchMemory = np.random.choice(min(self.MemoryCur,self.MemorySize), size=self.BatchSize)
		BatchMemory = self.Memory[BatchMemory,:]

		S = torch.FloatTensor(BatchMemory[:,0:self.stateDim])
		S_= torch.FloatTensor(BatchMemory[:,self.stateDim : 2 * self.stateDim])
		A = torch.LongTensor (BatchMemory[:, 2 * self.stateDim].astype(int))
		R = torch.FloatTensor(BatchMemory[:, 2 * self.stateDim + 1])

		Qeval = self.EvalNet(S)
		print(Qeval)
		print(A)
		Qeval = Qeval.gather(1, A)		#eval评价网络,状态S的Q表
		Qnext = self.TargetNet(S_).detach()			#target目标网络,状态S_的Q表
		QTarget = R + self.gamma * Qnext.max(1)[0].view(self.BatchSize, 1)
		print(QTarget)

		loss = self.LossFunc(Qeval, QTarget)

		self.optimizer.zero_grad()
		loss.backward()
		self.optimizer.step()


