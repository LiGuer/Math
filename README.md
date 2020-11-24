# LiGu_AlgorithmLib
LiGu's Basic Algorithm Library.  
* <Mat.h>                   矩阵类  
* <list.h>                  链表类  
* <GraphTheory.h>           图论类
* <NumberTheory.h>          数论类
* <BasicAlgorithm.h>        基础算法类
* <ComputationalGeometry.h> 计算几何类
* <PartialDifferentialEquation.h>   偏微分方程类
* <PrincipalComponentsAnalysis.h>:  主成分分析|数据降维
* <K_mean.h>                K-Mean聚类算法  
* <rb_tree.h>               红黑树
* <BigNum.h>				大数类
* <LSTM.h>                  长短期记忆网络

## <Mat.h> 矩阵类  
```
/**************** 核心数据 ****************/
T* data = NULL;
int rows = 0, cols = 0;
/**************** 基础函数 ****************/
Mat();                                      //构造析构函数  
Mat(const int _rows, const int _cols);  
Mat(const Mat& a);  
~Mat();
void clean();                               //清零  
void error();                               //报错  
void eatMat(Mat& a);                        //
/**************** 基础矩阵 ****************/  
[1] 零元 zero		[2] 单位元 E		[3] 随机元 rands  
/**************** 基础运算 ****************/  
T& operator[](int i)                        // "[]"取元素  
T& operator()(int i, int j)                 // "()"取元素  
T& operator()(int i) 
T max()                                     // max/min 
T max(int& index) 
T min()  
T min(int& index)  
Mat& operator=(const Mat& a)                //赋矩阵 [ = ]  //不能赋值自己
Mat& add(Mat& a, Mat& b)                    //加法 [ add ]
Mat& mult(const Mat& a, const Mat& b)       //乘法 [ mult ]
Mat& mult(const double a, const Mat& b)     //数乘 [ mult ]
Mat& dot(const Mat& a, const Mat& b)        //点乘 [ dot ]
Mat& negative(Mat& ans)                     //负 [ negative ]
Mat& transposi(Mat& ans)                    //转置 [ trans ]
void sum(int dim, Mat& ans)                 //元素求和 [ sum ]
T norm()                                    //范数 [ norm ]
T comi(int i0, int j0)                      //余子式 [ comi ]
T abs()                                     //行列式 [ abs ]
Mat& adjugate(Mat& ans)                     //伴随矩阵 [ adjugate ]
void eig(T esp, Mat& eigvec, Mat& eigvalue) //特征值特征向量 [ eig ]
/**************** 特殊操作 ****************/  
Mat& horizStack(Mat& a, Mat& b)             //水平向拼接 [ horizStack() ]
void swap(Mat& a)                           //交换数据 [ swap() ]
Mat& getCol(int _col, Mat& a)               //得到一列 [ getCol() ]
```