#ifndef MATRIX_MAT_H
#define MATRIX_MAT_H

#include <vector>
#include <initializer_list>

template <class T = double>
class Mat {
public:
	/*---------------- 元素 (数据堆叠 -- 行优先) ----------------*/
	T* data = NULL;
	int rows = 0, 
		cols = 0;

	/*---------------- constructor, destructor ----------------*/
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

	Mat(const Mat& a) {			//copy constructor
		*this = a;
	}

	~Mat() { 
		delete data; 
	}

	/*---------------- property ----------------*/
	// size of elements
	inline int size() const {
		return rows * cols;
	}

	inline bool empty() {
		return size() == 0 ? true : false;
	}

	/*---------------- index (先纵再横)----------------*/
	inline T& operator[](int i) { 
		return data[i];
	}

	inline T& operator()(int x) { 
		return data[x]; 
	}

	inline T& operator()(int x, int y) { 
		return data[x * cols + y]; 
	}

	inline void i2rc(int i, int& r, int& c) {
		r = i / cols;
		c = i % cols;
	}

	inline int  i2r(int i) {
		return i / cols;
	}

	inline int  i2c(int i) {
		return i % cols;
	}

	inline int rc2i(int r, int c) {
		return r * cols + c;
	}

	/*---------------- initial ----------------*/
	// alloc memory space
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

	// clear the data
	inline Mat& zero() {
		memset(data, 0, sizeof(T) * size());
		return *this;
	}

	Mat& zero(const int _rows, const int _cols = 1) {
		alloc(_rows, _cols);
		zero();
		return *this;
	}

	// set the parameters with data existed 
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

	/*---------------- assignment ----------------*/ //不能赋值自己
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

	Mat& operator=(const std::vector<T>& x) {
		int n = x.size();
		alloc(n);

		for (int i = 0; i < n; i++) {
			data[i] = x[i];
		}

		return *this;
	}

	Mat& operator=(T x) {
		return fill(x);
	}

	inline Mat& fill(const T& a) {
		for (int i = 0; i < size(); i++)
			data[i] = a;
		return *this;
	}

	/*---------------- basic operation ----------------*/
	// 改变矩阵形状
	Mat& reshape(int _rows, int _cols = 1) {
		if(_rows * _cols != size())
			exit(-1);
		
		rows = _rows;
		cols = _cols;

		return *this;
	}

	// compare
	bool operator==(const Mat& a) {
		if (rows != a.rows || cols != a.cols)
			return false;
		return memcmp(data, a.data, size() * sizeof(T)) == 0 ? true : false;
	}

	// judge whether cross the border
	inline bool isOut(int _rows, int _cols) {
		if (_rows < 0 || _rows >= rows) return true;
		if (_cols < 0 || _cols >= cols) return true;
		return false;
	}

	// transfer data by pointers
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

};


#endif