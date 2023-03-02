# **Matrix**

# Note
[Linear Space](../../note/Linear_Space.md)

# Basic Struct  
|Structure|Mean|
|---|---|
|```vector<T>``` | Vector ($\mathbb R^{n}$) |
|```Mat<T>``` | Matrix ($\mathbb R^{m \times n}$) |
|||

## Vector
just like ```std::vector<T>```

## Matrix
* struct
  * data ```T*```
  * rows ```int```
  * cols ```int```
* constructor, destructor
  * $\boldsymbol 0^n$ ```(int)```
  * $\boldsymbol 0^{m \times n}$ ```(int, int)```
  * $\mathbb R^{m \times n}$ ```(int, int, T*)```
  * copy constructor ```(Mat&)```
* initial
  * $\mathbb R^n, \mathbb R^{m \times n}$ alloc memory space ```alloc(int)```, ```alloc(int, int)```
  * $\boldsymbol 0^n, \boldsymbol 0^{m \times n}$ clear the data ```zero()```, ```zero(int)```, ```zero(int, int)```
  * set the parameters with data existed ```set(int, int, T*)```, ```set_(int, int, T*)```
* property
  * size of elements ```size()```
  * whether empty ```empty()```
* index
  * $\boldsymbol v_i$ ```[int]```, ```(int)```
  * $\boldsymbol M_{i,j}$ ```(int, int)```
  * $i \to (r, c)$ ```i2rc(int, int&, int&)```, ```i2r(int)```, ```i2c(int)```
  * $(r, c) \to i$ ```rc2i(int, int&, int&)```
* assignment ```=```
  * matrix $\gets$ matrix
  * matrix $\gets$ vector
  * matrix $\gets$ ```std::initializer_list<T>```
  * fill ```fill(T&)```, ```= (T&)```
* basic operation
  * reshape ```reshape(int)```, ```reshape(int, int)```
  * compare ```==(Mat&)```
  * transfer data by pointers ```eatMat(Mat&)```
  * judge whether cross the border ```isOut(int, int)```

# Linear Space Operations
## Special matrix construction
* $\boldsymbol I$ ```E(Mat&)```
* $\boldsymbol 1$ ```ones(Mat&)```
* random matrix ```rands(Mat&, double st, double ed)```
* linear spacing vector ```rands(Mat&, double xs, double xe, int n = 100)```
* diagonal matrix ```diag(Mat&, Mat&)```
  
## Basic operations of matrix
* Unary operations
  * negative $- \boldsymbol M$ ```negative(Mat&, Mat&), -(Mat&)```
  * transpose $\boldsymbol M^T$ ```transpose(Mat&, Mat&)```
  * inverse $M^{-1}$ ```inv(Mat&, Mat&)```
  * pseudo-inverse $M^{+}$ ```pinv(Mat&, Mat&)```
* Binary operation
  * addition 
    * $\boldsymbol M = \boldsymbol M_1 + \boldsymbol M_2$ ```add(Mat&, Mat&, Mat&)```
    * $\boldsymbol v = \boldsymbol v_1 + \boldsymbol v_2$ ```add(vector&, vector&, vector&)```
  * subtraction  
    * $\boldsymbol M = \boldsymbol M_1 - \boldsymbol M_2$ ```sub(Mat&, Mat&, Mat&)```
    * $\boldsymbol v = \boldsymbol v_1 - \boldsymbol v_2$ ```sub(vector&, vector&, vector&)```
  * multiplication
    * $\boldsymbol M = \boldsymbol M_1 \times \boldsymbol M_2$ ```mul(Mat&, Mat&, Mat&)```
    * $\boldsymbol v = \boldsymbol M_1 \times \boldsymbol v_1$ ```mul(vector&, Mat&, vector&)```
    * $\boldsymbol M = a \boldsymbol M_1$ ```mul(Mat&, double, Mat&)```
    * $\boldsymbol v = a \boldsymbol v_1$ ```mul(Mat&, double, vector&)```
  * multiplication of elements, Hadamard product 
    * $\boldsymbol M = \boldsymbol M_1 \odot \boldsymbol M_2$ ```elementMul(Mat&, Mat&, Mat&)```
    * $\boldsymbol v = \boldsymbol v_1 \odot \boldsymbol v_2$ ```elementMul(vector&, vector&, vector&)```
  * division of elements 
    * for matrix ```elementDiv(Mat&, Mat&, Mat&)```
    * for vector ```elementDiv(vector&, vector&, vector&)```

## Eigen-value, eigen-vector
* eigen-value, eigen-vector $\lambda \boldsymbol v = \boldsymbol M \boldsymbol v$ ```eig(Mat& M, Mat& eigvec, Mat& eigvalue, double esp = 1E-4)```

## Inner product space
* inner product 
  * $a = \boldsymbol M_1 \cdot \boldsymbol M_2$ ```dot(Mat&, Mat&)```
  * $a = \boldsymbol v_1^T \  \boldsymbol v_2$ ```dot(vector&, vector&)```
* norm 
  * $||\boldsymbol v||_2$ ```norm(vector&)```
  * $||\boldsymbol v||_n$ ```norm(Mat&, int)```
  * $||\boldsymbol M||_2$ ```norm(Mat&)```
  * $||\boldsymbol M||_c$ ```norm(Mat&, char)```
* normalize
  * $\hat {\boldsymbol v} = \frac{\boldsymbol v}{||\boldsymbol v||_2}$ ```normalize(vector&)```
  * $\hat {\boldsymbol M} = \frac{\boldsymbol M}{||\boldsymbol M||_2}$ ```normalize(Mat&)```

## Matrix Decomposition
* lower–upper (LU) decomposition ```LUP(Mat& M, Mat& L, Mat& U)```
* lower–upper-pivot (LUP) decomposition ```LU(Mat& M, Mat& L, Mat& U, Mat& P)```
* QR decomposition, orthogonal triangular decomposition ```QR(Mat& M, Mat& Q, Mat& R)```
* Singular Value Decomposition (SVD) ```SVD(Mat& A, Mat& U, Mat& S, Mat& V)```
* Full rank decomposition

## Construction of linear transformation matrix
* rotate
* shear
* scale
* reflect
* project

## other Linear Space Operations
* determinant $|\boldsymbol M^{n \times n}|$ ```det(Mat&)```
* adjugate matrix $\boldsymbol M^*$ ```adjugate(Mat&, Mat&)```
* cofactor ```comi(Mat&, int, int)```
* cross product $\boldsymbol v = \boldsymbol v_1 \times \boldsymbol v_2$ ```Mat& cross(Mat& ans, Mat& a, Mat& b)```, ```cross(vector& ans, vector& a, vector& b)```

# Memory Operations
* swap two matrix ```swap(Mat&, Mat&)```
* submatrix


# Files 

* Matrix  
  <[Matrix.h](Matrix.h)>
  * Basic Struct  
    <[Mat.h](Mat.h)>
    * Basic Matrix initialization  
      <[Init.h](Init.h)>
  * Property
    * Eigenvalue / Eigenvector $(\lambda x = A x)$  
      <[Eig.h](Eig.h)>
  * Operation  
    <[Operate.h](Operate.h)>
    * Basic Operation $(\cdot^T,+,-,\times,/)$   
      <[BasicOperate.h](BasicOperate.h)>
    * Inner Product, Norm $(\|\cdot\|)$, Normalize   
      <[Inner.h](Inner.h)>
    * Cross Product  
      <[CrossProduct.h](CrossProduct.h)>
    * Determinant $(|\cdot|)$, Cofactor matrix, Adjoint matrix  
      <[Determinant.h](Determinant.h)>
    * Inverse Matrix $(\cdot^-)$  
      <[Inv.h](Inv.h)>
    * Summation $(\Sigma_{i = 1}^N)$  
      <[Sum.h](Sum.h)>
    * Convolution  
      <[Conv.h](Conv.h)>
  * Other
    * Matrix Decompose  
      <[Decompose.h](Decompose.h)>
    * Function  
      <[Function.h](Function.h)>
    * Solve Liner Equations $(A x = b)$  
      <[SolveEquations.h](SolveEquations.h)>
    * Submatrix  
      <[Submatrix.h](Submatrix.h)>
    * Transform  
      <[Transform.h](Transform.h)>


| File | Significance |
|---|---|
| <[Mat.h](Mat.h)> | Basic Struct |
| <[Matrix.h](Matrix.h)> | Complete Main Header |
| <[Init.h](Init.h)> | Basic Matrix initialization |
| <[Decompose.h](Decompose.h)> | Matrix Decompose |
| <[Eig.h](Eig.h)> | Eigenvalue / Eigenvector |
| <[Function.h](Function.h)> | Function |
| <[SolveEquations.h](SolveEquations.h)> | Solve Liner Equations |
| <[Submatrix.h](Submatrix.h)> | Submatrix |
| <[Transform.h](Transform.h)> | Transform |
|**Operate**||
| <[Operate.h](Operate.h)> | Operate Header |
| <[BasicOperate.h](BasicOperate.h)> | Basic Operate |
| <[Inner.h](Inner.h)> | Inner |
| <[CrossProduct.h](CrossProduct.h)> | Cross Product |
| <[Conv.h](Conv.h)> | Convolution |
| <[Determinant.h](Determinant.h)> | Determinant |
| <[Inv.h](Inv.h)> | Inverse Matrix |
| <[Sum.h](Sum.h)> | Summation |
|||
