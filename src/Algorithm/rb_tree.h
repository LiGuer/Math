#ifndef _RB_TREE_
#define _RB_TREE_
/*********************************************************************************
*						红黑树  Red-Black Tree
*********************************************************************************/
#define NULL 0

typedef bool _rb_tree_color_tpye;
const _rb_tree_color_tpye red = false;
const _rb_tree_color_tpye black = true;
/*********************************************************************************
*						红黑树 节点 Red-Black Tree Node
*********************************************************************************/
template<class T>
struct _rb_tree_node
{
	typedef _rb_tree_color_tpye color_tpye;
	typedef _rb_tree_node node;
	color_tpye color = black;
	node* 
		parent = NULL, 
		left   = NULL, 
		right  = NULL;
	T key;
};
/*********************************************************************************
*						红黑树 迭代器 Red-Black Tree Iterator
*********************************************************************************/
template<class T>
struct _rb_tree_iterator
{

};
/*********************************************************************************
*						红黑树  Red-Black Tree
* red-black properties:
1. Every node is either red or black.
2. The root is black.
3. Every leaf (NIL) is black.
4. If a node is red, then both its children are black.
5. For each node, all simple paths from the node to
descendant leaves contain the same number of black nodes.
----------------------------------------------------------------------------------
* API:
	min、max、search						//搜索
	left_rotate、right_rotate				//旋转
	insert									//插入
	erase									//删除
*********************************************************************************/
template<class T>
class rb_tree {
	typedef _rb_tree_color_tpye color_tpye;
	typedef _rb_tree_node node;
	node* root = NULL, nil = NULL;
	/*---------------- 构造函数 ----------------*/
	rb_tree() { nil = new node; root = nil; }
	/*---------------- 最大/最小值 ----------------*/
	static node* min(node* x) {
		while (x->left  != nil) x = x->left;
		return x;
	}
	static node* max(node* x) {
		while (x->right != nil) x = x->left;
		return x;
	}
	/*---------------- 搜索 ----------------*/
	void search(node* x) {
		node* y = root;
		while (y != nil) {
			if (x.key == y.key) return y;
			else if (x.key < y.key) y = y->left;
			else y = y->right;
		} return NULL;
	}
	/*---------------- 左转 ----------------*/
	void left_rotate(node* x) {
		node* y = x->right;
		// y->left
		x->right = y->left;
		if (y->left != nil) y->left->parent = x;
		y->left = x;
		// x->parent
		y->parent = x->parent;
		if (x->parent == nil) root = y;
		else if (x == x->parent->left) x->parent->left = y;
		else x->parent->right = y;
		x->parent = y;
	}
	/*---------------- 右转 ----------------*/
	void right_rotate(node* x) {
		node* y = x->left;
		// y->right
		x->left = y->right;
		if (y->right != nil) y->right->parent = x;
		y->right = x;
		// x->parent
		y->parent = x->parent;
		if (x->parent == nil) root = y;
		else if (x == x->parent->left) x->parent->left = y;
		else x->parent->right = y;
		x->parent = y;
	}
	/*---------------- 插入 ----------------*/
	void insert(node* x) {
		node* y;
		for (node* t = root; t != nil;) {
			y = t;
			if (x.key == t.key) {						//case2: key exist
				t.key = x.key; return;
			}
			else if (x.key < t.key) 
				 t = t->left;
			else t = t->right;
		}
		x->parent = y;
		x->left  = nil;
		x->right = nil;
		x->color = red;
		//if red with black parent,black balance won't be broken
		//if black,the child tree which insert always 1 more 
		//in black total,must to balance.
		if (y == nil) {									//case1: empty tree
			root = x; root->color = black; return;
		}
		else if (x->key < y.key) y->left = x;
		else y->right = x;

		if (x->parent->color == red) insert_fixup(x);	//case4: parent is red
		return;								//case3: parent is black:return
	}

	void insert_fixup(node* x) {
		//case4: parent is red
		//if parent of insert is red, then the parent isn't root,
		//thus must exist parent of the parent, and p->p must be black
		while (x->parent->color == red) {
			if (x->parent == x->parent->parent->left) {	//P is P-P's left
				y = x->parent->parent->right;			//P-b:parent's brother
				//case 4.1: P-b exist and is red
				//I.P、P-b => black	//II.P-P => red	//III.P-P as insert
				if (y->color == red) {
					x->parent->color == balck;			//I
					y->color = black;					//I
					x->parent->parent->color = red;		//II
					x = x->parent->parent;				//III
				}
				//case 4.2: P-b is black or Nil
				//P->b must be Nil, cannot exist black
				//if not P->b will 1 more black than P child trees

				//case 4.2.1: insert is P'left
				//I.P => black	//II.P-P => red	//III.P-P right_rotate

				//case 4.2.2: insert is P'right
				//I.P left_rotate II. P as insert III.to 4.2.1
				else {
					if (x == x->parent->right) {		//case 4.2.2
						x == x->parent;
						left_rotate(x);
					}
					x->parent->color = black;			//case 4.2.1
					x->parent->parent->color = red;
					right_rotate(x->parent->parent);
					break;
				}
			}
			else {										//P is P-P's right
				if (y->color == red) {					//case 4.1
					x->parent->color == balck;			//I
					y->color = black;					//I
					x->parent->parent->color = red;		//II
					x = x->parent->parent;				//III
				}
				//case 4.3:P-b is black or Nil,and P is P-P's right
				else {
					if (x == x->parent->right) {		//case 4.3.2
						x == x->parent;
						right_rotate(x);
					}
					x->parent->color = black;			//case 4.3.1
					x->parent->parent->color = red;
					left_rotate(x->parent->parent);
					break;
				}
			}
		} root->color = black;
	}
	/*---------------- 删除 ----------------*/
	void erase(node* x) { }
	void erase_fixup(node* x) { }
};

#endif