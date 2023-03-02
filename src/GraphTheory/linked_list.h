#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include<stdlib.h>

/******************************************
 * 
 *		Linked List, Circular Doubly linked list
 *
 *****************************************/

/*
 *  list_node 节点
 */
template<class T>
struct list_node {
	T data;
	list_node<T>
		* next = NULL, 
		* prev = NULL;
};

/*
 *  list_iterator
 */
template<class T>
class list_iterator {
public:
	typedef list_iterator<T> iterator;
	list_node<T>* node;
	
	// 构造函数 
	list_iterator() {}
	list_iterator(list_node<T>* node) { this->node = node; }

	// 运算符重载
	bool operator== (const iterator& x) { return node == x.node; }
	bool operator!= (const iterator& x) { return node != x.node; }

	iterator& operator++ () { node = node->next; return *this; }
	iterator& operator-- () { node = node->prev; return *this; }

	T operator* ()const { return (*node).data; }
	T*operator->()const { return &(operator*()); }
};

/*
 *  Circular Doubly linked list
 */
template<class T>
class list
{
public:
	list_node* head;		//head node of linked list

	typedef list_iterator<T> iterator;
	typedef list_node<T> node;


	/*---------------- constructor / destructor ----------------*/
	list() {
		head = new node;
		head->next = head;
		head->prev = head;
	}

	~list() {
		while (head != NULL) {
			list_node* p = head;
			head = head->next;
			delete p;
		}
	}

	/*---------------- index ----------------*/
	iterator begin() { return iterator(head->next); }
	iterator end  () { return iterator(head); }

	/*---------------- property ----------------*/
	bool empty() { 
		return head.next == head; 
	}

	int size() {
		int ans = 0;
		for (iterator it = begin(); it != end(); ++it) ans++;
		return ans;
	}

	/*---------------- add ----------------*/
	node* insert(iterator position, const T& x) {
		node* tmp = new node();
		node* old = position.node;
		tmp->data = x;
		tmp->next = old;
		tmp->prev = old->prev;
		old->prev->next = old->prev = tmp;
		return tmp;
	}
	
	void push_front(const T& x) { insert(begin(), x); }
	void push_back (const T& x) { insert(end(), x); }

	/*---------------- delete ----------------*/
	void erase(iterator position) {
		node* prev = position.node->prev;
		node* next = position.node->next;
		prev->next = next;
		next->prev = prev;
		delete position.node;
	}

	void pop_front() { erase(begin()); }
	void pop_back () { erase(--end()); }

	// clear all elements
	void clear() {
		for (iterator it = begin(); it != end(); it++) 
			erase(it);
	}

	/*---------------- search ----------------*/
	iterator search(const T& x) {
		for (iterator it = begin(); it != end(); ++it) 
			if (it.node->data == x) return it;
		return NULL;
	}

};
#endif