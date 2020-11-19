#ifndef BASIC_ALGORITHM_H
#define BASIC_ALGORITHM_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//约定: 左闭右开[first,last) 
template <class T> const T& min(const T& a, const T& b);
template <class T> const T& max(const T& a, const T& b);
template <class T> bool next_permutation(T* first, T* last);
template <class T> bool prev_permutation(T* first, T* last);
template <class T> void reverse(T* first, T* last);
template <class T> void sort(T* first, T* last);
template <class T> void swap(T* a, T* b);

/*--------------------------------[ min/max 最大/小 ]--------------------------------*/
template <class T> const T& min(const T& a, const T& b) { return !(b < a) ? a : b; }	// or: return !comp(b,a)?a:b; for version (2)
template <class T> const T& max(const T& a, const T& b) { return (a < b) ? b : a; }	// or: return comp(a,b)?b:a; for version (2)
/*--------------------------------[ swap 交换 ]--------------------------------*/
template <class T> void swap(T* a, T* b){T c = *a; *a = *b; *b = c;}
/*--------------------------------[ sort 排序 ]--------------------------------
*	[算法]: 快速排序	Quick Sort
*	[输入]: [1]first: 最初元素地址	[2]last: 最末元素地址 + 1
*	[时间复杂度]: 上限n²，大概率平均值n log(n)
*	[流程]: 
*		[1] 最后元素作为参考元素[Ref]，[border]分割比参考元素大或小的界限，指向大的最初段
		[2] [ai] 从第一元素开始，直至 Ref 前一元素位置
		[3]	1).若 ai >=Ref, 不处理
			2).若 ai < Ref, 
				ai 与 border元素交换位置, border++
		[4] 遍历结束后, 交换Ref 与 border
		[5] Ref 左右比他大/小的两方元素，分别从头开始。
**-----------------------------------------------------------------------*/
template <class T>
void sort(T* first, T* last)
{
	if (last - 1 <= first)return;
	T* ref = last - 1;			//最后元素作为参考元素
	T* border = first;			//分割比参考元素大或小的界限，指向大的最初段
	for (T* i = first; i != ref; i++) {
		if (*i < *ref) {				//[3]
			T temp = *border;
			*border = *i; *i = temp;
			border++;
		}

	}
	T temp = *border;
	*border = *ref; *ref = temp;
	sort(first, border);
	sort(border + 1, last);
}
/*--------------------------------[ reverse 反序 ]--------------------------------*/
template <class T> void reverse(T* first, T* last) { last--; while (first < last) { swap(first, last); first++; last--; } }
/*--------------------------------[ next/prev_permutation 全排列 ]--------------------------------
*
*	输入: [first,last)
*	流程
		[1] Start from last, 找到非降序点[i]
			[2] find the pos[i2] that bigger than i_th element firstly from last
			[3] swap i & i2
			[4] reverse from i+1 to last, 因为后[i+1.last)的元素是降序，肯定得倒换的。
*		[1]若全降序系列,则无next,返回全增序
**-----------------------------------------------------------------------*/
template <class T>
bool next_permutation(T* first, T* last) {
	if (last - 1 <= first)return false;
	T* i = last - 2;
	while (true) {
		if (*i < *(i + 1)) {		//[1]
			T* i2 = last;
			while (!(*i < *--i2));	//找到第一个大于i元素的位置
			swap(i, i2);
			reverse(i + 1, last);
			return true;
		}
		if (i == first) { reverse(first, last); return false; }	//[1]全降序系列
		i--;
	}
}
#endif // ! SORT_H
