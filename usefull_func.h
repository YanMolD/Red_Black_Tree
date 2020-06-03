#pragma once
#include <iomanip>
#include <iostream>

using namespace std;

template <class T> struct Node
{
	T data;
	int key;
	int height;
	Node<T>* parent;
	Node<T>* left;
	Node<T>* right;
	Node(int key, T data)//Root initialization
	{
		this->data = data;
		this->key = key;
		height = 1;
		parent = NULL;
		left = NULL;
		right = NULL;
	}
	Node(Node<T>* prnt, int key, T data)
	{
		this->data = data;
		this->key = key;
		height = 1;
		parent = prnt;
		left = NULL;
		right = NULL;
	}
	~Node()
	{
		if (parent != NULL)
		{
			if (parent->left == this)
				parent->left = NULL;
			else
				parent->right = NULL;
		}
	}
};

template <class T> void swap_nodes(Node<T>* node1, Node<T>* node2)
{
	int t = node1->key;
	T swap = node1->data;
	node1->key = node2->key;
	node2->key = t;
	node1->data = node2->data;
	node2->data = swap;
}

template <class T> bool have_sons(Node<T>* cur)
{
	return (cur->right || cur->left);
}

template <class T> int height(Node<T>* p)
{
	return p ? p->height : 0;
}

template <class T> int bfactor(Node<T>* p)
{
	return height(p->right) - height(p->left);
}

template <class T> void fixheight(Node<T>* p)
{
	unsigned char hl = height(p->left);
	unsigned char hr = height(p->right);
	p->height = (hl > hr ? hl : hr) + 1;
}

template <class T> Node<T>* rotateright(Node<T>* rot_node)
{
	Node<T>* left_ch = rot_node->left;
	rot_node->left = left_ch->right;
	if (rot_node->left) rot_node->left->parent = rot_node;
	left_ch->right = rot_node;
	left_ch->parent = rot_node->parent;
	if (left_ch->parent)
		if (left_ch->parent->left == rot_node) left_ch->parent->left = left_ch;
		else left_ch->parent->right = left_ch;
	rot_node->parent = left_ch;
	fixheight(rot_node);
	fixheight(left_ch);
	return left_ch;
}

template <class T> Node<T>* rotateleft(Node<T>* rot_node)
{
	Node<T>* right_ch = rot_node->right;
	rot_node->right = right_ch->left;
	if (rot_node->right) rot_node->right->parent = rot_node;
	right_ch->left = rot_node;
	right_ch->parent = rot_node->parent;
	if (right_ch->parent)
		if (right_ch->parent->left == rot_node)	right_ch->parent->left = right_ch;
		else right_ch->parent->right = right_ch;
	rot_node->parent = right_ch;
	fixheight(rot_node);
	fixheight(right_ch);
	return right_ch;
}

template <class T> Node<T>* balance(Node<T>* p)
{
	fixheight(p);
	if (bfactor(p) == 2)
	{
		if (bfactor(p->right) < 0) p->right = rotateright(p->right);
		return rotateleft(p);
	}
	if (bfactor(p) == -2)
	{
		if (bfactor(p->left) > 0) p->left = rotateleft(p->left);
		return rotateright(p);
	}
	return p;
}

template <class T>void inOrder_key(Node<T>* node)
{
	if (node == NULL) return;
	inOrder_key(node->left);
	cout << node->key << " ";
	inOrder_key(node->right);
}

template <class T>void inOrder_data(Node<T>* node)
{
	if (node == NULL) return;
	inOrder_data(node->left);
	cout << node->data << " ";
	inOrder_data(node->right);
}