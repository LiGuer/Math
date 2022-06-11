# LiGu's Mathematic Library
LiGu's Mathematic and Basic Algorithm Library.  

* 数学  Math  
	* 矩阵  Matrix
	* 微积分  Calculus
	* 复数  Complex
	* 图论  GraphTheory
	* 数论  NumberTheory
	* 统计  Statistics
	* 几何  Geometry
	* 大数  BigNum
  
*  算法 Algorithm
	* 链表  list
	* 红黑树  rb_tree  
	* Hash表  HashTable
	* 基础算法类  BasicAlgorithm  

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
T& operator[](int i);					// 索引元素
T& operator()(int i, int j);
T& operator()(int i);
T max();								// max/min
T max(int& index);
T min();
T min(int& index);
bool operator==	(const Mat& a);			//判断相等 [==]
Mat& operator=	(const Mat& a);			//赋矩阵 [=] //不能赋值自己
Mat& getData	(T* a);
Mat& getData	(T x, T y);
Mat& getData	(T x, T y, T z);
Mat& operator+=	(Mat& a);					//加法 [add +]
Mat& add		(Mat& a, Mat& b);
Mat& operator-=	(Mat& a);					//减法 [sub -]
Mat& sub		(Mat& a, Mat& b);
Mat& mul		(Mat& a, Mat& b);			//乘法 [mul ×]
Mat& operator*=	(const Mat& a);
Mat& operator*=	(const double a);			//数乘 [mul ×]
Mat& mul		(const double a, Mat& b);
Mat& div		(const double a, Mat& b);	//数除 [div /]
static T dot	(Mat& a, Mat& b);			//点乘 [dot ·]
T dot			(Mat& a);
Mat& crossProduct	(Mat& a, Mat& b);		//叉乘 [crossProduct ×]
Mat& crossProduct_	(Mat& a, Mat& b);
Mat& elementMul	(Mat& a, Mat& b);		//元素乘 [elementMul ×]
Mat& elementMul	(Mat& a);
Mat& elementDiv	(Mat& a, Mat& b);			//元素除 [elementDiv /]
Mat& elementDiv	(Mat& a);
Mat& negative	(Mat& ans);					//负 [negative -]
Mat& transpose	(Mat& ans);					//转置 [transpose T]
T sum			();							//求和 [sum Σ]
T sum			(Mat& a);
Mat& sum		(Mat& ans,int dim);
T product		();							//求积 [product Π]
T norm			();							//范数 [norm ||x||]
Mat& normalized	();							//归一化 [normalized]
T comi			(int i0, int j0);			//余子式 [comi]
Mat& inv		(Mat& ans);					//取逆 [inv x~¹]
T abs			();							//行列式 [abs |x|]
Mat& adjugate	(Mat& ans);					//伴随矩阵 [adjugate A*]
void eig		(T esp, Mat& eigvec, Mat& eigvalue);		//特征值特征向量 [eig]
Mat& solveEquations		(Mat& b, Mat& x);					//解方程组 [solveEquations]
void LUPdecomposition	(Mat& U, Mat& L, Mat<int>& P);		//LUP分解 [LUPdecomposition]
Mat& diag		(Mat& ans);									//构造对角矩阵 [diag]
Mat& conv		(Mat& a, Mat& b, int padding = 0, int stride = 1);	//卷积 [conv]
/**************** 特殊操作 ****************/  
Mat& getCol	(int _col, Mat& a)				//读/写一列 [getCol/setCol]
Mat& setCol	(int _col, Mat& a)
Mat& getRow	(int _row, Mat& a)				//读/写一行 [getRow/setRow]
Mat& block	(int rowSt, int rowEd, int colSt, int colEd, Mat& ans)	//子矩阵 [block]
Mat& horizStack	(Mat& a, Mat& b)            //水平向拼接 [horizStack ]
Mat& function	(Mat& x, T (*f)(T))			//函数操作
Mat& function	(T (*f)(T))	
```


## <BigNum.h> 大数类
(1) 2,16进制 赋值、输出  
(2) 位运算：与或非 异或 左移右移 补码  
(3) 比较运算：> < = >= <=  
(4) 数值计算：加减乘除余幂负  
```
/**************** 核心数据 ****************/
bool	sign = 1;
int8u*	data = NULL;			//堆叠顺序小低大高, 补码形式
int64	byte = 0;
/**************** 基础函数 ****************/  
// 基础函数
BigNum				()						//构造,析构
BigNum				(int64 _byte, bool _sign = 1)
BigNum				(const char* input)
~BigNum				()
BigNum& alloc		(int64 _byte)			//分配内存
BigNum& Realloc		(int64 _byte)
BigNum& zero		() 						//归零
BigNum& zero		(int64 _byte)
// 索引,赋值,输出
int8u&	operator[]	(int64 index) 			//索引
int8u&	operator()	(int64 index)
BigNum& operator=	(BigNum& input)  		//赋值
BigNum& operator=	(int64 input)
BigNum& operator=	(const char* input)
int usefulByte		()
int usefulBit		()
char* toStr			(int8u base = 10)		//输出字符串
// 位运算: 补与或非异或左移右移 [ & | ~ ^ << >> ]
BigNum&	comple		()  					//补码
BigNum&	icomple		()
BigNum	operator&	(BigNum& a)  			//与 &
BigNum&	operator&=	(BigNum& a)
BigNum	operator|	(BigNum& a) 			//或 |
BigNum& operator|=	(BigNum& a)
BigNum& operator~	() 						//非 ~
BigNum& operator^	(BigNum& a) 			//异或 ^
BigNum& operator^=	(BigNum& a)
BigNum& operator<<=	(int n)  				//左移右移 << >> 
BigNum& operator>>=	(int n)
// 比较运算: 大小等 [ > < = ]
char cmp(BigNum& a) 						
bool operator> 		(BigNum& a)
bool operator>=		(BigNum& a)
bool operator< 		(BigNum& a)
bool operator<=		(BigNum& a)
bool operator==		(BigNum& a)
// 数值运算: 负加减乘除余幂 [ + - * / % pow ]
BigNum	operator-	() 						//负 -
BigNum	operator+	(BigNum& a)  			//加 +
BigNum& add			(BigNum& a, BigNum& b)
BigNum& operator+=	(BigNum& a)
BigNum& operator+=	(int64 & a)
BigNum& operator++	()
BigNum  operator- 	(BigNum& a)				//减 -
BigNum& sub			(BigNum& a, BigNum& b)
BigNum& operator-=	(BigNum& a)
BigNum& operator-=	(int64 a)
BigNum& operator--	()
BigNum  operator* 	(BigNum& a) 			//乘 *
BigNum& mul			(BigNum& a, BigNum& b)
BigNum& operator*=	(BigNum& a)
BigNum& operator*=	(int64   a)
BigNum	operator/	(BigNum& a) 			//除 /
BigNum& div			(BigNum& a, BigNum& b)
BigNum& operator/=	(BigNum& a)
BigNum& operator/=	(int64   a)
BigNum	operator%	(BigNum& a) 			//余 %
BigNum& mod			(BigNum& a)
BigNum& operator%=	(BigNum& a)
BigNum& operator%=	(int64   a)
BigNum& pow			(BigNum& a) 			// 幂 pow
BigNum& pow			(int64 n)
```


## <Statistics.h> 统计学类
```
基础统计特征:
double Mean	(Mat<>& x);							//均值
Mat<>& Mean	(Mat<>& x, Mat<>& ans, int index);
double Var	(Mat<>& x);							//方差
Mat<>& Var	(Mat<>& x, Mat<>& ans, int index);
double PoissonDistrib	(int x, double mean);		//常见分布: 分布函数、密度函数
double  NormalDensity	(double x, double mean = 0, double var = 1);
double	NormalDistrib	(double x, double mean = 0, double var = 1);
double     ExpDensity	(double x, double mean);
double     ExpDistrib	(double x, double mean);
double   GammaDensity	(double x, double mean, double var);
double   GammaDistrib	(double x, double mean, double var);
假设检验:
double  X2Test	(Mat<>& x, Mat<>& expect);			//X²检验
double	X2Test	(Mat<>& x, double St, double Ed, int N, F&& DistribFunc);	//X²拟合检验
bool	SkewnessKurtosisTest	(Mat<>& x, double SignificanceLevel);		//偏度-峰度
其他:
Mat<int>& Histogram(Mat<>& x, int N, Mat<int>& frequency, double overFlow, double underFlow);	//直方图
void BoxPlot(Mat<>& x, Mat<>& MediQuartThreshold, std::vector<int>* OutlierIndex)	//箱形图
```

## <NumberTheory.h> 数论类
```
INT64S Factorial(INT64S n);										//阶乘
INT64S*FibonacciSequence(int N);								//Fibonacci数列
INT64S C		(INT64S n, INT64S m);							//组合
INT64S A		(INT64S n, INT64S m);							//排列
INT64S GCD		(INT64S a, INT64S b);							//最大公约数
INT64S LCM		(INT64S a, INT64S b);							//最小公倍数
INT64S GCDex	(INT64S a, INT64S b, INT64S& x, INT64S& y)		//拓展Euclid算法
bool   isPrime	(INT64S a);										//素数判断
INT64S PowMod	(INT64S a, INT64S k, INT64S m);					//幂次模
INT64S RSAPrivateKey(INT64S p, INT64S q, INT64S a);				//RSA密码
```
