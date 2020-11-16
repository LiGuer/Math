# LiGu_AlgorithmLib
LiGu's Basic Algorithm Library.  
* <Mat.h>:					矩阵类  
* <list.h>:					链表类  

## <Mat.h> 矩阵类  
```
/**************** 基础数据 ****************/
T* data = NULL;
int rows = 0, cols = 0;
/**************** 基础函数 ****************/
Mat();													//构造析构函数  
Mat(const int _rows, const int _cols);  
Mat(const Mat& a);  
~Mat();
void clean();											//清零  
void error();											//报错						
/**************** 基础矩阵 ****************/  
void zero(const int _rows, const int _cols);			//零元  
void E(const int _rows);								//单位元  
/**************** 运算 ****************/  
T& operator[](int i);									// '[]' 取元素  
T max();												//max / min  
T max(int index);  
T min();  
void assign(const Mat& a);								//赋矩阵  
void add(Mat& a, Mat& b, Mat& ans);						//加  
void mult(const Mat& a, const Mat& b, Mat& ans);		//乘  
void sum(int dim, Mat& ans);							//元素求和  
void transposi(Mat& ans);								//转置  
void eig(T esp, Mat& eigvec, Mat& eigvalue);			//特征值特征向量 
```