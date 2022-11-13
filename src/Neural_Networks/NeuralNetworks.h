#ifndef NEURAL_NETWORKS_H
#define NEURAL_NETWORKS_H

#include "NeuralLayer.h"
#include "ConvLayer.h"
#include "PoolLayer.h"
#include "BatchNorm.h

#include "LSTMLayer.h"

#include "BasicModule.h"

#include "ClassicalNeuralNetworks.h"

/*################################################################################################
核心类:
class NeuralLayer	(inSize, outSize)											//神经网络层
class ConvLayer		(inChannelNum, outChannelNum, kernelSize, padding, stride)	//卷积层
class PoolLayer		(kernelSize, padding, stride, poolType)						//下采样层
class LstmLayer		(inSize, outSize)											//长短期记忆层
class BatchNorm		(outChannelNum, _eps)										//归一化层
--------------------------------------------------------------------------------------------------
经典模块:
class BasicConv		(inChannelNum, outChannelNum, kernelSize, padding, stride)			//基础卷积层
class Inceptionn	(inChannelNum, n1x1, n3x3red, n3x3, n5x5red, n5x5, poolChannelNum)	//Inception模块 : 2014
class ResBlock		(inChannelNum, ChannelNum, stride)									//残差模块 : 2015
--------------------------------------------------------------------------------------------------
经典网络:
class BackPropagationNeuralNetworks()				//反向传播神经网络		: 1986.Rumelhart,McClelland
class LeNet()										//LeNet卷积神经网络		: 1998.Yann LeCun
class GoogLeNet()									//GoogLeNet卷积神经网络 : 2014.
class LstmNetwork()									//长短期记忆网络
################################################################################################*/

#endif