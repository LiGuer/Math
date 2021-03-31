/*
Copyright 2020 LiGuer. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
	http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Reference.
[1]Introduction Algorithms.THOMAS H.CORMEN,CHARLES E.LEISERSON,RONALD L.RIVEST,CLIFFORD STEIN
==============================================================================*/
#ifndef _DATA_FRAME_H
#define _DATA_FRAME_H
#include "Mat.h"
/*--------------------------------[ DataFrame ]--------------------------------
**------------------------------------------------------------------------*/
class DataFrame {
public:
	union DataCell { int d; float f; char c; char* s; };
	Mat<DataCell> dataList;
	/*--------------------------------[ 基本函数 ]--------------------------------*/
	// 构造析构
	DataFrame(int rows, int cols) {
		dataList.zero(rows, cols);
	}
	~DataFrame() { }
	//-------------------------------- DataRead --------------------------------
	void DataRead(const char* input, const char* dataListModel) {
		FILE* fin = fopen(input, "r");
		char model[] = "%x";
		for (int i = 0; i < dataList.rows; i++) {
			for (int j = 0; dataListModel[j] != '\0'; j++) {
				model[1] = dataListModel[j];
				if (dataListModel[j] == ' ')continue;
				else if (dataListModel[j] == 's') {
					dataList(i, j).s = (char*)malloc(50 * sizeof(char));
					fscanf(fin, model, dataList(i, j).s);
				}
				else fscanf(fin, model, &dataList(i, j));
				fgetc(fin);
			}
		}fclose(fin);
	}
	//-------------------------------- getMatrix --------------------------------
	template<class T>
	void getMatrix(int colst, int coled, char model, Mat<T>& output) {
		output.zero(dataList.rows, coled - colst + 1);
		for (int i = 0; i < dataList.rows; i++) {
			for (int j = colst; j <= coled; j++) {
				switch (model) {
				case 'd': output(i, j - colst) = dataList(i, j).d; break;
				case 'f': output(i, j - colst) = dataList(i, j).f; break;
				}
			}
		}
	}
};
#endif