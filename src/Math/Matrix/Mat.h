#ifndef MATRIX_MAT_H
#define MATRIX_MAT_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <initializer_list>

template <class T = double>
class Mat {
public:
	/*---------------- 元素 (数据堆叠 -- 行优先) ----------------*/
	T* data = NULL;
	int rows = 0, 
		cols = 0;

	/*---------------- 构造/析构 ----------------*/
	Mat() { 
		; 
	}

	Mat(const int _rows) { 
		zero(_rows, 1); 
	}

	Mat(const int _rows, const int _cols) { 
		zero(_rows, _cols); 
	}

	Mat(const int _rows, const int _cols, T* _data) { 
		set(_rows, _cols, _data); 
	}

	~Mat() { 
		delete data; 
	}

	/*---------------- 元素数量 ----------------*/
	inline int size() const {
		return rows * cols;
	}

	/*---------------- 索引元素 (先纵再横)----------------*/
	inline T& operator[](int i) { 
		return data[i];
	}

	inline T& operator()(int x) { 
		return data[x]; 
	}

	inline T& operator()(int x, int y) { 
		return data[x * cols + y]; 
	}

	inline void i2xy(int& i, int& x, int& y) {
		x = i / cols;
		y = i % cols;
	}

	inline int  i2x(int i) {
		return i / cols;
	}

	inline int  i2y(int i) {
		return i % cols;
	}

	inline int xy2i(int x, int y) {
		return x * cols + y;
	}

	/*---------------- 分配空间 ----------------*/
	Mat& alloc(const int _rows, const int _cols = 1) {
		if (_rows != rows || _cols != cols) {
			if (data != NULL)
				delete data;

			data = (T*) malloc(_rows * _cols * sizeof(T));
			rows = _rows;
			cols = _cols;
		}
		return *this;
	}

	/*---------------- 赋值 ----------------*/ //不能赋值自己
	Mat& operator=(const Mat& a) {
		if (a.data == NULL)
			return *this;

		alloc(a.rows, a.cols);
		memcpy(data, a.data, sizeof(T) * size());
		return *this;
	}

	Mat& operator=(std::initializer_list<T> list) {
		int i = 0;
		for (auto& item : list)
			data[i++] = item;
		return *this;
	}

	Mat& operator=(T  x) {
		return fill(x);
	}

	Mat& set(const int _rows, const int _cols, T* _data) {
		zero(_rows, _cols);
		memcpy(data, _data, sizeof(T) * size());
		return *this;
	}

	Mat& set_(const int _rows, const int _cols, T* _data) {
		rows = _rows;
		cols = _cols;
		data = _data;
		return *this;
	}

	/*---------------- 零元/清零 ----------------*/
	inline Mat& zero() {
		memset(data, 0, sizeof(T) * size());
		return *this;
	}

	Mat& zero(const int _rows, const int _cols = 1) {
		alloc(_rows, _cols);
		zero();
		return *this;
	}

	/*---------------- 填充  ----------------*/
	inline Mat& fill(const T& a) {
		for (int i = 0; i < size(); i++)
			data[i] = a;
		return *this;
	}

	/*----------------判断相等 [ ==/!= ]----------------*/
	bool operator==(const Mat& a) {
		if (rows != a.rows || cols != a.cols)
			return false;
		return memcmp(data, a.data, size() * sizeof(T)) == 0 ? true : false;
	}

	/*---------------- 判断出界 ----------------*/
	inline bool isOut(int _rows, int _cols) {
		if (_rows < 0 || _rows >= rows) return true;
		if (_cols < 0 || _cols >= cols) return true;
		return false;
	}

	/*---------------- 吃掉另一个矩阵(指针操作)  ----------------*/
	inline Mat& eatMat(Mat& a) {
		if (data != NULL)
			delete data;
		data = a.data;
		a.data = NULL;

		rows = a.rows;
		cols = a.cols;
		a.rows = a.cols = 0;

		return *this;
	}

	/*---------------- 交换数据 ----------------*/
	Mat& swap(Mat& a) {
		T* tmp = a.data;
		a.data = data;
		data = tmp;

		int t = a.rows;
		a.rows = rows;
		rows = t;

		t = a.cols;
		a.cols = cols;
		cols = t;

		return *this;
	}

};


#endif