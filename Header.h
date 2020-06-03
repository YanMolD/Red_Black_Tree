#pragma once
#include "usefull_func.h"
using namespace std;

#define BLACK true;

template <typename T> class RedBlackTree
{
public:
	void Add(int key, T data);
	void Remove(int key);
	T Find_data(int key);
	void print();
	bool get_node_color(int key);
	RedBlackTree()
	{
		Root = NULL;
		Size = 0;
	}
	~RedBlackTree()
	{
		clear();
	}
	Node<T>* Get_Root();
	void clear();
private:
	Node<T>* Root;
	Node<T>& find_free_place(int key);
	void printing(Node<T>* Root, int depth);
	void distructon(Node<T>* p);
	int Size;
};

template <typename T>
void RedBlackTree<T>::clear()
{
	if (Root)
		distructon(Root);
	Size = 0;
	Root = NULL;
}
template <typename T>
void RedBlackTree<T>::distructon(Node<T>* p)
{
	if (p->left) distructon(p->left);
	if (p->right) distructon(p->right);
	delete p;
}

template <typename T>
void RedBlackTree<T>::Add(int key, T data)
{
	int size = Size;
	if (Size == 0)
		Root = new Node<T>(key, data);
	else
	{
		Node<T>* cur = &find_free_place(key);
		if (key > cur->key)	cur->right = new Node<T>(cur, key, data);
		else cur->left = new Node<T>(cur, key, data);
		while (cur != NULL)
		{
			cur = balance(cur);
			if (Root->parent != NULL)
				Root = Root->parent;
			cur = cur->parent;
		}
	}
	Size++;
}

template <typename T>
T RedBlackTree<T>::Find_data(int key)
{
	if (Size == 0)
		throw out_of_range("No elements in tree");
	Node<T>* cur = Root;
	while (cur != NULL)
	{
		if (cur->key > key) cur = cur->left;
		else if (cur->key < key) cur = cur->right;
		else return cur->data;
	}
	return NULL;
}

template <typename T>
void RedBlackTree<T>::Remove(int key)
{
	if (Size == 0)
		throw out_of_range("No elements in tree");
	if (Size == 1)
	{
		clear();
		return;
	}
	Node<T>* cur = Root;
	while (cur)
		if (cur->key > key) cur = cur->left;
		else if (cur->key < key) cur = cur->right;
		else break;
	if (!cur)
		throw out_of_range("No elements with this key");
	Node<T>* key_node = cur;
	if (cur->right)
	{
		cur = cur->right;
		while (cur->left)
			cur = cur->left;
		key_node->key = cur->key;
	}
	if (have_sons(cur))
		if (cur->right)
		{
			swap_nodes(cur, cur->right);
			delete cur->right;
		}
		else
		{
			swap_nodes(cur, cur->left);
			delete cur->left;
		}
	else
	{
		if (cur->parent->left == cur)
		{
			cur = cur->parent;
			delete cur->left;
		}
		else
		{
			cur = cur->parent;
			delete cur->right;
		}
	}
	while (cur != NULL)
	{
		cur = balance(cur);
		if (Root->parent != NULL)
			Root = Root->parent;
		cur = cur->parent;
	}
	Size--;
}

template <typename T>
Node<T>& RedBlackTree<T>::find_free_place(int key)
{
	Node<T>* cur = Root;
	while (have_sons(cur))
		if (cur->key > key)
			if (cur->left) cur = cur->left;
			else break;
		else
			if (cur->right) cur = cur->right;
			else break;
	return *cur;
}

template <typename T>
void RedBlackTree<T>::print()
{
	printing(Root, 0);
}

template <typename T>
void RedBlackTree<T>::printing(Node<T>* root, int depth)
{
	for (int i = 0; i < depth; i++)
		cout << "    ";
	cout << root->key;
	if (get_node_color(root->key)) cout << " b" << endl;
	else cout << " r" << endl;
	if (root->right)
		printing(root->right, depth + 1);
	if (root->left)
		printing(root->left, depth + 1);
}

template <typename T>
bool RedBlackTree<T>::get_node_color(int key) //is node black
{
	Node<T>* cur = Root;
	if (key == Root->key) return true; //Root of the tree
	int counter = 0, bl_counter = 0;
	while (cur)
	{
		if (cur->key > key) cur = cur->left;
		else if (cur->key < key) cur = cur->right;
		else break;
		counter++;
	}
	if ((!cur->left || !cur->right) && (cur->left || cur->right))//have only one child
		return true;
	if (counter == Root->height - 1)//lowest leaf
		return false;
	if (!cur->left && !cur->right) //normal leaf
	{
		Node<T>* check = cur->parent;
		if (!(get_node_color(check->key))) return true;
		bl_counter++;
		check = check->parent;
		while (check != NULL)
		{
			if ((get_node_color(check->key))) bl_counter++;
			check = check->parent;
		}
		if (bl_counter != (!(Root->height % 2) ? Root->height / 2 : Root->height / 2 + 1))
			return true;
		else
			return false;
	}
	int buf_counter = counter;
	Node<T>* buf_cur = cur;
	while (buf_cur->left && buf_cur->right) //normal node
	{
		if (buf_cur->left->height < buf_cur->right->height) buf_cur = buf_cur->left;
		else buf_cur = buf_cur->right;
		buf_counter++;
	}
	if (Root->height % 2) bl_counter = buf_counter / 2 + 1;
	else bl_counter = buf_counter / 2 + (buf_counter % 2 ? 0 : 1);
	if (bfactor(cur) != 0) //node with one subtree bigger then other
	{
		Node<T>* check = cur->parent;
		int bl_counter2 = 0;
		while (check != NULL)
		{
			if ((get_node_color(check->key))) bl_counter2++;
			check = check->parent;
		}
		if (bl_counter2 == (!(Root->height % 2) ? Root->height / 2 : Root->height / 2 + 1) - 1)
			return false;
	}
	if ((cur->height <= 2) && (bl_counter != (!(Root->height % 2) ? Root->height / 2 : Root->height / 2 + 1)))
		return true;
	if (Root->height % 2)
		if (counter % 2) return true;
		else return false;
	else if (counter % 2) return false;
	else return true;
}

template <typename T>
Node<T>* RedBlackTree<T>::Get_Root()
{
	return Root;
}