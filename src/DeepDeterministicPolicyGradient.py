import numpy as np
import torch
import torch.nn as nn
from torch.optim import Adam

from model import (Actor, Critic)
from memory import SequentialMemory
from random_process import OrnsteinUhlenbeckProcess
from util import *

class DDPG(object):
	def __init__():
		
		# Actor
		self.actor = Actor(self.nb_states, self.nb_actions, **net_cfg)
		self.actor_target = Actor(self.nb_states, self.nb_actions, **net_cfg)
		self.actor_optim  = Adam(self.actor.parameters(), lr=args.prate)

		# Critic
		self.critic = Critic(self.nb_states, self.nb_actions, **net_cfg)
		self.critic_target = Critic(self.nb_states, self.nb_actions, **net_cfg)
		self.critic_optim  = Adam(self.critic.parameters(), lr=args.rate)

		#Create replay buffer
		self.memory = SequentialMemory(limit=args.rmsize, window_length=args.window_length)
		self.random_process = OrnsteinUhlenbeckProcess(size=nb_actions, theta=args.ou_theta, mu=args.ou_mu, sigma=args.ou_sigma)

		# Hyper-parameters
		self.batch_size = args.bsize
		self.tau = args.tau
		self.discount = args.discount
		self.depsilon = 1.0 / args.epsilon
		self.epsilon = 1.0
		self.s_t = None # Most recent state
		self.a_t = None # Most recent action
		self.is_training = True

	def ActionSelection(self, state, decay_epsilon=True):
		action = to_numpy(
			self.actor(to_tensor(np.array([state])))
		).squeeze(0)
		action += self.is_training*max(self.epsilon, 0)*self.random_process.sample()
		action = np.clip(action, -1., 1.)

		if decay_epsilon:
			self.epsilon -= self.depsilon
		
		self.a_t = action
		return action

	def UpdatePolicy(self):
		# Sample batch
		state_batch, action_batch, reward_batch, next_state_batch, terminal_batch \
			= self.memory.sample_and_split(self.batch_size)

		# Critic update
		self.critic.zero_grad()

		q_batch = self.critic([ to_tensor(state_batch), to_tensor(action_batch) ])
		
		value_loss = criterion(q_batch, target_q_batch)
		value_loss.backward()
		self.critic_optim.step()

		# Actor update
		self.actor.zero_grad()

		policy_loss = -self.critic([
				to_tensor(state_batch),
				self.actor(to_tensor(state_batch))
		])

		policy_loss = policy_loss.mean()
		policy_loss.backward()
		self.actor_optim.step()

		# Target update
		soft_update(self.actor_target, self.actor, self.tau)
		soft_update(self.critic_target, self.critic, self.tau)
