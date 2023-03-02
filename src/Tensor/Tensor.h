#ifndef TENSOR_H
#define TENSOR_H

#include <vector>

template<class T = double>
class Tensor {
public:
	/*
	 * 核心数据, Data堆叠方向: 低维优先
	 */
	std::vector<T> data;
	std::vector<int> dim;
	
	/*---------------- 构造/析构函数 ----------------*/
	Tensor() { ; }
	Tensor(int dimNum, int* dimLength) { zero(dimNum, dimLength); }
	Tensor(int x0, int y0, int z0)     { zero(x0, y0, z0); }
	Tensor(Tensor& a) { *this = a; }
	~Tensor() { delete data; }


	/*---------------- porperty ----------------*/
	inline int size() {
		return data.size();
	}

	inline bool empty() {
		return data.empty();
	}

	/*---------------- 分配空间 ----------------*/
	Tensor& alloc(std::vector<int>& _dim) {
		int _dimNum = _dim.size(), 
			 dimNum =  dim.size();

		if (dimNum != _dimNum
		||  memcmp(dim, _dim, dimNum * sizeof(int)) != 0
		) {
			dimNum = _dimNum;
			dim = (int*)malloc(dimNum * sizeof(int));
			memcpy(dim, _dim, dimNum * sizeof(int));

			num = 1;
			for (int i = 0; i < dimNum; i++) {
				num *= dim[i];
			}

			if (data != NULL)
				delete data;
			data = (T*)malloc(num * sizeof(T));
		}
		return *this;
	}

	Tensor& alloc(int x0)							{ int t[] = { x0 };				alloc(1, t); return *this; }
	Tensor& alloc(int x0, int y0)					{ int t[] = { x0, y0 };			alloc(2, t); return *this; }
	Tensor& alloc(int x0, int y0, int z0)			{ int t[] = { x0, y0, z0 };		alloc(3, t); return *this; }
	Tensor& alloc(int x0, int y0, int z0, int w0)	{ int t[] = { x0, y0, z0, w0 }; alloc(4, t); return *this; }

	/*---------------- 零元/清零 ----------------*/
	Tensor& zero() { memset(data, 0, sizeof(T) * num); return *this; }	//清零 
	Tensor& zero(int _dimNum, int* _dim)		{ alloc(_dimNum, _dim); zero(); return *this; }
	Tensor& zero(int x0)						{ alloc(x0);			zero(); return *this; }
	Tensor& zero(int x0, int y0)				{ alloc(x0, y0);		zero(); return *this; }
	Tensor& zero(int x0, int y0, int z0)		{ alloc(x0, y0, z0);	zero(); return *this; }
	Tensor& zero(int x0, int y0, int z0, int w0){ alloc(x0, y0, z0, w0);zero(); return *this; }

	/*---------------- "[]"取元素 ----------------
	*	[坐标]: { x, y, z, ...} = data[ x + y·X0 + z·X0·Y0 + ... ]
	*	[Data堆叠方向]: 满x,一列 => 满xy,一矩阵 => 满xyz,一方块 => ....
	**-------------------------------------------*/
	T& operator[](int i)				{ return data[i]; }
	T& operator()(int x)				{ return data[x]; }
	T& operator()(int x, int y)			{ return data[x + y * dim[0]]; }
	T& operator()(int x, int y, int z)	{ return data[x + y * dim[0] + z * dim[1] * dim[0]]; }

	T& operator()(int* dimIndex) {
		int index = 0, step = 1;
		for (int i = 0; i < dimNum; i++) {
			index += step * dimIndex[i];
			step  *= dim[i];
		} 
		return data[index];
	}

	T& operator()(std::initializer_list<T> list) {
		int index = 0, step = 1, i = 0;
		for (auto& item : list) {
			index += step * item;
			step  *= dim[i++];
		} 
		return data[index];
	}

	inline int i2x(int i) { return i % dim[0]; }
	inline int i2y(int i) { return dimNum == 2 ? i / dim[0]          : i %(dim[1] * dim[0])/ dim[0]; }
	inline int i2z(int i) { return dimNum == 3 ? i /(dim[1] * dim[0]): i %(dim[2] * dim[1] * dim[0])/(dim[1] * dim[0]); }
	inline int xyz2i(int x, int y, int z) { return x + y * dim[0] + z * dim[1] * dim[0]; }

	/*----------------赋值 [ = ]----------------*/ //不能赋值自己
	Tensor& operator=(Tensor& a) {
		if (a.data == NULL) 
			exit(-1);
		alloc(a.dim);
		memcpy(data, a.data, num * sizeof(T));
		return *this;
	}

	Tensor& eat(Tensor& a) {
		if (a.data == NULL) 
			exit(-1);
		if (data != NULL)
			delete data;
		data = a.data; a.data = NULL; dim.eatMat(a.dim);
		return *this;
	}

};
#endif