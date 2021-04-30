#ifndef _LIST_H
#define _LIST_H
/*********************************************************************************
						链表 List
*********************************************************************************/
#include<stdlib.h>
/*********************************************************************************
*						_list_node 节点
*	successor node, or first element if head
	predecessor node, or last element if head
*********************************************************************************/
template<class T>
struct _list_node {
	T data;
	_list_node<T>* next = NULL, prev = NULL;
};
/*********************************************************************************
*						_list_iterator 迭代器
*	迭代器必须能指向1ist 的节点，并能进行正确的递增、递减、取值、成员存取等操作。
*********************************************************************************/
template<class T>
class _list_iterator {
public:
	typedef _list_iterator<T> iterator;
	_list_node<T>* node;
	/*---------------- 构造函数 ----------------*/
	_list_iterator() {}
	_list_iterator(_list_node<T>* node) { this->node = node; }
	/*---------------- 运算符重载 ----------------*/
	bool operator== (const iterator& x) { return node == x.node; }		//比较[==]
	bool operator!= (const iterator& x) { return node != x.node; }		//比较[!=]
	iterator& operator++ () { node = node->next; return *this; }		//递增[++] 
	iterator& operator-- () { node = node->prev; return *this; }		//递减[--]
	T operator* ()const { return (*node).data; }						//指针[*]
	T*operator->()const { return &(operator*()); }						//指针[->]
};
/*********************************************************************************
*						list 环状双向链表(主)
----------------------------------------------------------------------------------
*							核心数据
node* head;										//链表头地址
----------------------------------------------------------------------------------
*							基础性质
list();											//构造函数
iterator begin();								//首地址
iterator end();									//末地址
bool empty();									//判断为空
int size();										//元素数量
----------------------------------------------------------------------------------
*							基础操作
node* insert(iterator position, T& x);			//插入
void erase(iterator position);					//删除
void push_front(const T& x);					//首末插入删除
void push_back(const T& x);
void pop_front();
void pop_back();
void clear();									//全清
iterator search(T& x);							//查找
*********************************************************************************/
template<class T>
class list
{
public:
	typedef _list_iterator<T> iterator;
	typedef _list_node<T> node;
	node* head;															//链表头地址
	/******************************************************************************
	*                    基础性质
	******************************************************************************/
	/*---------------- 构造函数 ----------------*/
	list() {
		head = new node;
		head->next = head;
		head->prev = head;
	}
	/*---------------- 首末地址 ----------------*/
	iterator begin() { return iterator(head->next); }
	iterator end  () { return iterator(head); }
	/*---------------- 判断为空 ----------------*/
	bool empty() { return head.next == head; }
	/*---------------- 元素数量 ----------------*/
	int size() {
		int ans = 0;
		for (iterator it = begin(); it != end(); ++it) ans++;
		return ans;
	}
	/******************************************************************************
	*                    基础操作
	******************************************************************************/
	/*---------------- 插入 ----------------*/
	node* insert(iterator position, const T& x) {
		node* tmp = new node();
		node* old = position.node;
		tmp->data = x;
		tmp->next = old;
		tmp->prev = old->prev;
		old->prev->next = old->prev = tmp;
		return tmp;
	}
	/*---------------- 删除 ----------------*/
	void erase(iterator position) {
		node* prev = position.node->prev;
		node* next = position.node->next;
		prev->next = next;
		next->prev = prev;
		delete position.node;
	}
	/*---------------- 首末插入删除 ----------------*/
	void push_front(const T& x) { insert(begin(), x); }
	void push_back (const T& x) { insert(end(), x); }
	void pop_front() { erase(begin()); }
	void pop_back () { erase(--end()); }
	/*---------------- 全清 ----------------*/
	void clear() {
		for (iterator it = begin(); it != end(); it++) 
			erase(it);
	}
	/*---------------- 查找 ----------------*/
	iterator search(const T& x) {
		for (iterator it = begin(); it != end(); ++it) 
			if (it.node->data == x) return it;
		return NULL;
	}
};
#endif