# LiGu_AlgorithmLib
LiGu's Basic Algorithm Library.  
*  框架 (算法 + 数据结构)
* <Mat.h>						矩阵类
* <NeuralNetworks.h>			神经网络类
* <BasicMachineLearning.h>		基础机器学习类
*  算法 Algorithm
* <BasicAlgorithm.h>			基础算法类  
* <GraphTheory.h>				图论类 
* <NumberTheory.h>			数论类
*  数据结构 DataStructure
* <list.h>						链表类  
* <rb_tree.h>					红黑树  
* <Tensor.h>					张量类  
*  Other
* <PartialDifferentialEquation.h>	偏微分方程类  
* <LSTM.h>						长短期记忆网络  

## <Mat.h> 矩阵类  
```
/**************** 核心数据 ****************/
T* data = NULL;
int rows = 0, cols = 0;
/**************** 基础函数 ****************/
Mat();                                      //构造/析构函数  
Mat(const int _rows, const int _cols);  
Mat(const int _rows);
Mat(const Mat& a);  
~Mat();
void error();                               //报错  
int size();                                 //Size
Mat& fill(T a);                             //填充
void eatMat(Mat& a);                        //吃掉另一个矩阵(指针操作)
void swap(Mat& a);                          //交换数据 [ swap ]
/**************** 基础矩阵 ****************/  
[0]alloc	分配空间   
[1]zero		零元/清零   
[2]E		单位元   
[3]ones		全1元  
[4]rands	随机元 
/**************** 基础运算 ****************/  
T& operator[](int i);                    // 索引元素
T& operator()(int i, int j);
T& operator()(int i);
T max();                                // max/min
T max(int& index);
T min();
T min(int& index);
bool operator==(const Mat& b);          //判断相等 [==]
Mat& operator=(const Mat& a);           //赋矩阵 [=] //不能赋值自己
Mat& getData(T* a);
Mat& getData(T x, T y);
Mat& getData(T x, T y, T z);
Mat& operator+=(Mat& a);                //加法 [add +]
Mat& add(Mat& a, Mat& b);
Mat& operator-=(Mat& a);                //减法 [sub -]
Mat& sub(Mat& a, Mat& b);
Mat& mult(Mat& a, Mat& b);              //乘法 [mult ×]
Mat& operator*=(const Mat& a);
Mat& operator*=(const double a);        //数乘 [mult ×]
Mat& mult(const double a, Mat& b);
Mat& divide(const double a, Mat& b);    //数除 [divide /]
static T dot(Mat& a, Mat& b);           //点乘 [dot ·]
T dot(Mat& a);
Mat& crossProduct(Mat& a, Mat& b);      //叉乘 [crossProduct ×]
Mat& crossProduct_(Mat& a, Mat& b);
Mat& elementMult(Mat& a, Mat& b);       //元素乘 [elementMult ×]
Mat& elementMult(Mat& a);
Mat& elementDivide(Mat& a, Mat& b);     //元素除 [elementDivide /]
Mat& elementDivide(Mat& a);
Mat& negative(Mat& ans);                //负 [negative -]
Mat& transposi(Mat& ans);               //转置 [transposi T]
T sum();                                //求和 [sum Σ]
static T sum(Mat& a);
Mat& sum(Mat& ans,int dim);
T product();                            //求积 [product Π]
T norm();                               //范数 [norm ||x||]
Mat& normalized();                      //归一化 [normalized]
T comi(int i0, int j0);                 //余子式 [comi]
Mat& inv(Mat& ans);                     //取逆 [inv x~¹]
T abs();                                //行列式 [abs |x|]
Mat& adjugate(Mat& ans);                //伴随矩阵 [adjugate A*]
void eig(T esp, Mat& eigvec, Mat& eigvalue);    //特征值特征向量 [eig]
Mat& solveEquations(Mat& b, Mat& x);            //解方程组 [solveEquations]
void LUPdecomposition(Mat& U, Mat& L, Mat<int>& P);                     //LUP分解 [LUPdecomposition]
Mat& diag(Mat& ans);                                                    //构造对角矩阵 [diag]
Mat<double>& conv(Mat& a, Mat& b, int padding = 0, int stride = 1);     //卷积 [conv]
/**************** 特殊操作 ****************/  
Mat& getCol(int _col, Mat& a)               //读/写一列 [getCol/setCol]
Mat& setCol(int _col, Mat& a)
Mat& getRow(int _row, Mat& a)               //读/写一行 [getRow/setRow]
Mat& block(int rowSt, int rowEd, int colSt, int colEd, Mat& ans)	//子矩阵 [block]
Mat& horizStack(Mat& a, Mat& b)             //水平向拼接 [horizStack ]
```

## <NeuralNetworks.h> 神经网络类
```
/*----------------[ 核心类 ]----------------*/
class NeuralLayer(int inputSize, int outputSize)    //神经网络层
class ConvLayer(int _inChannelNum, int _outChannelNum,int kernelSize,int _padding,int _stride)  //卷积层
class PoolLayer(int _kernelSize, int _padding, int _stride, int _poolType)      //下采样层
/*----------------[ 经典结构 ]----------------*/
class BackPropagation_NeuralNetworks()              //反向传播神经网络 : 1986.Rumelhart,McClelland
class LeNet_NeuralNetworks()                        //LeNet卷积神经网络 : 1998.Yann LeCun
class Inception()                                   //Inception模块: 2014.Google
```
