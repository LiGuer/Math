#ifndef COMPLEX_H
#define COMPLEX_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

class Complex {
    double real = 0, imag = 0;

    // 赋值
    Mat& operator=(Complex& a) { 
        real = a.real;
        imag = a.imag;
        return *this; 
    }

    Mat& set(double _real, double _imag){
        real = _real;
        imag = _imag;
        return *this; 
    }
}

namespace ComplexLib {

/* 
 * 基础构造
 */

Complex& zero(Complex& a){
    a.real = a.imag = 0;
    return a;
}

Complex& I(Complex& a){
    a.real = 0;
    a.imag = 1;
    return a;
}


/* 
 * 基础运算
 */

//加
Complex& add(Complex& a, Complex& b, Complex& ans) {
    ans.real = a.real + b.real;
    ans.imag = a.imag + b.imag;
    return ans;
}

//减
Complex& sub(Complex& a, Complex& b, Complex& ans) {
    ans.real = a.real - b.real;
    ans.imag = a.imag - b.imag;
    return ans;
}

//乘
Complex& mul(Complex& a, Complex& b, Complex& ans) {
    double _real, _imag;
    _real = a.real * b.real - a.imag * b.imag;
    _imag = a.real * b.imag + a.imag * b.real;
    ans.real = _real;
    ans.imag = _imag;
    return ans;
}

//除
Complex& div(Complex& a, Complex& b, Complex& ans) {
    double _real, _imag, t;
    t = b.real * b.real + b.imag * b.imag
    _real = (a.real * b.real + a.imag * b.imag) / t;
    _imag = (a.imag * b.real - a.real * b.imag) / t;
    ans.real = _real;
    ans.imag = _imag;
    return ans;
}

//共轭
Complex& conjugate (Complex& a, Complex& ans){
    ans.real =  a.real;
    ans.imag = -a.imag;
    return ans;
}

}



#endif