#ifndef BASIC_ALGORITHM_H
#define BASIC_ALGORITHM_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
namespace BasicAlgorithm {
/******************************************************************************
*                    基础算法
-------------------------------------------------------------------------------
//约定: 左闭右开[first,last)
T& min(const T& a, const T& b);
T& max(const T& a, const T& b);
bool next_permutation(T* first, T* last);
bool prev_permutation(T* first, T* last);
void reverse(T* first, T* last);
void sort(T* first, T* last);
void swap(T* a, T* b);
******************************************************************************/
/*--------------------------------[ min/max 最大/小 ]--------------------------------*/
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
//template <class T> const T& min(const T& a, const T& b) { return (a < b) ? a : b; }	// or: return !comp(b,a)?a:b; for version (2)
//template <class T> const T& max(const T& a, const T& b) { return (a > b) ? a : b; }	// or: return comp(a,b)?b:a; for version (2)
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
void sort(T* first, T* last, bool*(cmp)(T a, T b))
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
	sort(first, border, cmp);
	sort(border + 1, last, cmp);
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
/*--------------------------------[ Huffman编码 ]--------------------------------
*	[定义]: Huffman Tree: 带权路径长度最短的树，权值较大的叶结点离根较近。
*	[节点结构]: [1]权值	[2]该节点编码值	[3]父节点指针
*	[建树流程]:
		[1] 初始w1,w2,…,wn，为各自均为根节点的森林。
		[2] 从森林中选出2个根结点权值最小的树合并，作为一棵新树的左右子树，
			且新树的根结点权值为其左、右子树根结点权值之和。
		[3] 从森林中删除选取的两棵树，并将新树加入森林。
		[4] 重复[2][3]，直至森林中只剩一棵树。
		//左节点编码0, 右节点编码1
**-----------------------------------------------------------------------*/
template<class T>
struct HuffmanTreeNode {
	T weight;
	bool binary = 0;
	HuffmanTreeNode* parent = NULL, * child[2] = { NULL,NULL };
};

template<class T>
HuffmanTreeNode<T>* HuffmanCode(T weight[], int N, int codeLen[], unsigned long long codeHuffman[]) {
	// [Tree] 建树 build Huffman tree
	// [1]
	HuffmanTreeNode<T>* HuffmanTree = (HuffmanTreeNode<T>*)calloc(N * 2 + 1, sizeof(HuffmanTreeNode<T>));
	for (int i = 0; i < N; i++) HuffmanTree[i].weight = weight[i];
	for (int i = N; i < N * 2; i++) HuffmanTree[i].weight = 1e9;
	// [2] find two smallest nodes 'min1, min2'
	int min1, min2, pos1 = N - 1, pos2 = N;
	for (int i = 0; i < N - 1; i++) {
		if (pos1 >= 0 && HuffmanTree[pos1].weight < HuffmanTree[pos2].weight) min1 = pos1--;
		else min1 = pos2++;
		if (pos1 >= 0 && HuffmanTree[pos1].weight < HuffmanTree[pos2].weight) min2 = pos1--;
		else min2 = pos2++; 
		// [3] combine to new tree 
		HuffmanTree[N + i].child[0] = &HuffmanTree[min1];	HuffmanTree[N + i].child[1] = &HuffmanTree[min2];
		HuffmanTree[N + i].weight = HuffmanTree[min1].weight + HuffmanTree[min2].weight;
		HuffmanTree[min1].parent = HuffmanTree[min2].parent = &HuffmanTree[N + i];
		HuffmanTree[min2].binary = 1;
	}
	HuffmanTreeNode<T>* root = &HuffmanTree[N * 2 - 2];
	// [Code] Huffman编码 // assign binary code
	for (int i = 0; i < N; i++) {
		HuffmanTreeNode<T>* treeCur = &HuffmanTree[i];
		int cur = 0;
		unsigned long long codeTemp = 0;
		while (treeCur != root) {
			if (treeCur->binary)codeTemp += (unsigned long long)1 << cur;
			cur++;
			treeCur = treeCur->parent;
		}
		codeLen[i] = cur;
		for (int j = 0; j < codeLen[i]; j++) //反序
			if (((unsigned long long)1 << (codeLen[i] - 1 - j)) & codeTemp)
				codeHuffman[i] += (unsigned long long)1 << j;
	}
	return root;
}
}
#endif 
