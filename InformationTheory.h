/*
Copyright 2020,2021 LiGuer. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
	http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#ifndef INFORMATION_THEORY_H
#define INFORMATION_THEORY_H
#include "Mat.h"
namespace InformationTheory {
typedef unsigned int INT64U;
/******************************************************************************
*					卷积码
*	[算法]:
	每次输入k bit => 【|1|2|.|k|】  【|1|2|.|k|】 ...【|1|2|.|k|】	mNum个iNum级移存器
						X X X X        X X X X          X X X X
						⊕    ⊕    ⊕    ⊕    ⊕    ...    ⊕		oNum个模2加法器
						\     \     \     |     /            /
											⇱		1个旋转开关(每输入iNum bit旋转一周)
											=>	输出
-------------------------------------------------------------------------------
*	[Example]:
		Mat<unsigned int> memSize(1), ploy(1, 3);
		InformationTheory::ConvCodeTrellis trellis(memSize.getData(3), ploy.getData(7, 5, 1));
		int t[] = { 1, 0, 1 }, ans[3];
		for (int i = 0; i < 3; i++) ans[i] = InformationTheory::ConvCode(t[i], &trellis);
******************************************************************************/
struct ConvCodeTrellis {
	INT64U iNum, oNum;
	Mat<INT64U> ploy, mem, memNum;
	ConvCodeTrellis(Mat<INT64U> _memNum, Mat<INT64U>& _ploy) {
		memNum	= _memNum;
		ploy	= _ploy;
		iNum	= ploy.rows;
		oNum	= ploy.cols;
		mem.zero(iNum);
	}
};
INT64U ConvCode(INT64U x, ConvCodeTrellis* trellis) {
	//寄存器移位
	for (INT64U i = 0; i < trellis->mem.size(); i++) {
		trellis->mem[i]<<= 1;
		trellis->mem[i] &= ~(0xFFFFFFFF << trellis->memNum[i]);
		trellis->mem[i] += (x & (1 << i)) ? 1 : 0;
	}
	//编码
	INT64U code = 0;
	for (INT64U i = 0; i < trellis->oNum; i++) {
		INT64U t = 0;
		for (INT64U j = 0; j < trellis->iNum; j++) {
			INT64U memtmp = trellis->mem[j] & trellis->ploy(j, i);
			for (INT64U k = 0; k < trellis->memNum[j]; k++)
				t ^= (memtmp & (1 << k)) ? 1 : 0;
		}
		code<<= 1;
		code += t;
	}return code;
}

}
#endif