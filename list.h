#ifndef _LIST_H
#define _LIST_H
/*
API:	begin, end, empty, size,				//基本属性
		insert, push_front, push_back,			//插入
		erase, pop_front, pop_back, clear		//删除
		search,									//查找
*/
#include<stdlib.h>
/***********************************************
*				_list_node 节点
************************************************/
template<class T>
struct _list_node {
	_list_node<T>* next = NULL;	// successor node, or first element if head
	_list_node<T>* prev = NULL;	// predecessor node, or last element if head
	T data;
};
/***********************************************
*				_list_iterator 迭代器
*	迭代器必须能指向1ist 的节点，并能进行正确的递增、递减、取值、成员存取等操作。
************************************************/
template<class T>
struct _list_iterator {
	typedef _list_iterator<T> self;

	_list_node<T>* node;

	_list_iterator() {}
	_list_iterator(_list_node<T>* node) { this->node = node; }
	/***********************************************
	*				运算符重载
	************************************************/
	/*----------------比较 [ == ][ != ]----------------*/
	bool operator== (const self& x) const { return node == x.node; }
	bool operator!= (const self& x) const { return node != x.node; }
	/*----------------递增 [ ++ ]----------------*/
	self& operator++ () {
		node = node->next;
		return *this;
	}
	/*----------------递减 [ -- ]----------------*/
	self& operator-- () {
		node = node->prev;
		return *this;
	}
	/*----------------指针 [ * ][ -> ]----------------*/
	T operator* ()const { return (*node).data; }
	T* operator-> ()const { return &(operator*()); }
};
/***********************************************
*				list (主) 环状双向链表
*
************************************************/
template<class T>
class list
{
public:
	typedef _list_iterator<T> iterator;
	typedef _list_node<T> node;
private:
	node* head;		//环状双向链表，头地址

public:
	list() {
		head = new node;
		head->next = head;
		head->prev = head;
	}
	/*----------------首末地址 [ begin ][ end ]----------------*/
	iterator begin() { return iterator(head->next); }	//链表开始地址
	iterator end() { return iterator(head); }			//链表结束地址
	/*----------------是否为空 [ empty ]----------------*/
	bool empty() { return head.next == head; }
	/*----------------元素多少 [ size ]----------------*/
	int size() {
		int ans = 0;
		for (iterator it = begin(); it != end(); ++it) {
			ans++;
		}return ans;
	}
	/*----------------插入 [ insert ]----------------*/
	node* insert(iterator position, const T& x) {
		node* tmp = new node();
		node* old = position.node;
		tmp->data = x;
		tmp->next = old;
		tmp->prev = old->prev;
		old->prev->next = tmp;
		old->prev = tmp;
		return tmp;
	}
	/*----------------删除 [ erase ]----------------*/
	void erase(iterator position) {
		node* prev = position.node->prev;
		node* next = position.node->next;
		prev->next = next;
		next->prev = prev;
		delete position.node;
	}
	/*----------------首末元素 插入,删除----------------*/
	void push_front(const T& x) { insert(begin(), x); }
	void push_back(const T& x) { insert(end(), x); }
	void pop_front() { erase(begin()); }
	void pop_back() { erase(--end()); }
	/*----------------全清 [ clear ]----------------*/
	void clear() {
		for (iterator it = begin(); it != end();) {
			iterator temp = it;
			++it;
			erase(temp);
		}
	}
	/*----------------查找 [ search ]----------------*/
	iterator search(const T& x) {
		for (iterator it = begin(); it != end(); ++it) {
			if (it.node->data == x)return it;
		}return NULL;
	}
};
#endif
/*/============ example ============
#include<iostream>
using namespace std;
struct STU
{
	int age;
	bool operator == (const STU& x) { return age == x.age; }
};
int main()
{
	list<STU> stulist;
	cout << "size = " << stulist.size() << endl;
	stulist.push_back({ 12 }); stulist.push_back({ 4 });
	stulist.push_back({ 15 }); stulist.push_front({ 16 });
	stulist.push_front({ 19 });
	cout << "size = " << stulist.size() << endl;
	for (list<STU>::iterator it = stulist.begin(); it != stulist.end(); ++it) {
		cout << (*it.node).data.age << ' ';
	}cout << endl;

	stulist.pop_front();
	stulist.pop_back();
	cout << "size = " << stulist.size() << endl;
	for (list<STU>::iterator it = stulist.begin(); it != stulist.end(); ++it) {
		cout << (*it.node).data.age << ' ';
	}cout << endl;

	list<STU>::iterator a = stulist.search({ 4 });
	cout << "search: " << (*a.node).data.age << '\n';
	stulist.erase(a);
	for (list<STU>::iterator it = stulist.begin(); it != stulist.end(); ++it) {
		cout << (*it.node).data.age << ' ';
	}cout << endl;

	stulist.clear();
	cout << "size = " << stulist.size() << endl;
	for (list<STU>::iterator it = stulist.begin(); it != stulist.end(); ++it) {
		cout << (*it.node).data.age << ' ';
	}cout << endl;
	return 0;
}
/*/