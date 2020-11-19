#ifndef ALGORITHM_H
#define ALGORITHM_H
/*--------------------------------[ swap 交换 ]--------------------------------*/
template <class T> void swap(T* a, T* b){T c = *a; *a = *b; *b = c;}
/*--------------------------------[ sort 排序 ]--------------------------------
*	快速排序	Quick Sort
*	输入: [1]first: 最初元素地址	[2]last: 最末元素地址 + 1
*	时间复杂度上: 上限n²，大概率平均值n log(n)
*	流程: 
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
#endif // ! SORT_H
