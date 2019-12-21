#pragma once
#include <assert.h>
template<class KeyType, class ElemType>
class AVLTreeNode {
public:
	KeyType m_key;
	ElemType m_elem;
	int m_bf;
	AVLTreeNode<KeyType, ElemType>* m_lChild;
	AVLTreeNode<KeyType, ElemType>* m_rChild;
public:
	AVLTreeNode(const KeyType& k, const ElemType& e) :
		m_key(k),
		m_elem(e),
		m_bf(0),
		m_lChild(nullptr),
		m_rChild(nullptr)
	{}
};

enum DIRECTION {
	LEFT,
	RIGHT
};


template<class KeyType, class ElemType>
class AVLTree {
	

private:
	//左旋
	void L_Rotate(AVLTreeNode<KeyType, ElemType>*& p) {
		AVLTreeNode<KeyType, ElemType>* rc = nullptr;
		rc = p->m_rChild;
		assert(rc);
		p->m_rChild = rc->m_lChild;
		rc->m_lChild = p;
		p = rc;
	}
	//右旋
	void R_Rotate(AVLTreeNode<KeyType, ElemType>*& p) {
		AVLTreeNode<KeyType, ElemType>* lc = nullptr;
		lc = p->m_lChild;
		assert(lc);
		p->m_lChild = lc->m_rChild;
		lc->m_rChild = p;
		p = lc;
	}
	//左平衡调整
	void leftBalance(AVLTreeNode<KeyType, ElemType>*& t) {
		AVLTreeNode<KeyType, ElemType>* lc = nullptr;
		AVLTreeNode<KeyType, ElemType>* rd = nullptr;
		lc = t->m_lChild;
		assert(lc);
		switch (lc->m_bf)
		{
		case 1:
			t->m_bf = 0;
			lc->m_bf = 0;
			R_Rotate(t);
			break;
		case 0:
			t->m_bf = 1;
			lc->m_bf = -1;
			R_Rotate(t);
			break;
		case -1:
			rd = lc->m_rChild;
			assert(rd);
			switch (rd->m_bf)
			{
			case 1:
				t->m_bf = -1;
				lc->m_bf = 0;
				break;
			case 0:
				t->m_bf = 0;
				lc->m_bf = 0;
				break;
			case -1:
				t->m_bf = 0;
				lc->m_bf = 1;
				break;
			}
			rd->m_bf = 0;
			L_Rotate(t->m_lChild);
			R_Rotate(t);
			break;
		}
	}
	//右平衡调整
	void rightBalance(AVLTreeNode<KeyType, ElemType>*& t) {
		AVLTreeNode<KeyType, ElemType>* rc = nullptr;
		AVLTreeNode<KeyType, ElemType>* ld = nullptr;

		rc = t->m_rChild;
		assert(rc);
		switch (rc->m_bf)
		{
		case 1:
			ld = rc->m_lChild;
			assert(ld);
			switch (ld->m_bf)
			{
			case 1:
				t->m_bf = 0;
				rc->m_bf = -1;
				break;
			case 0:
				t->m_bf = 0;
				rc->m_bf = 0;
				break;
			case -1:
				t->m_bf = 1;
				rc->m_bf = 0;
				break;
			}
			ld->m_bf = 0;
			R_Rotate(t->m_rChild);
			break;
		case 0:
			t->m_bf = -1;
			rc->m_bf = 1;
			L_Rotate(t);
			break;
		case -1:
			t->m_bf = 0;
			rc->m_bf = 0;
			L_Rotate(t);
			break;
		}
	}
	//总体平衡调整
	bool adjust(AVLTreeNode<KeyType, ElemType>*& t, DIRECTION d) {
		if (!t)
			return false;
		if (d == LEFT) {
			switch (t->m_bf)
			{
			case 1:
				leftBalance(t);
				return false;
			case 0:
				t->m_bf = 1;
				return true;
			case -1:
				t->m_bf = 0;
				return false;
			}
		}
		else {
			switch (t->m_bf) {
			case 1:
				t->m_bf = 0;
				return false;
			case 0:
				t->m_bf = -1;
				return true;
			case -1:
				rightBalance(t);
				return false;
			}
		}
	}
	//析构以t为根的树
	void destroy(AVLTreeNode<KeyType, ElemType>*& t) {
		if (t) {
			destroy(t->m_lChild);
			destroy(t->m_lChild);
			delete t;
			t = nullptr;
		}
	}
	//在以t为根的树中插入一个关键字为key，元素为elem的节点
	//成功返回true,否则返回false
	bool insert(AVLTreeNode<KeyType, ElemType>*& t, const KeyType& key, const ElemType& elem, bool& taller) {
		if (!t) {
			t = new AVLTreeNode<KeyType, ElemType>(key, elem);
			taller = true;
		}
		else {
			if (key == t->m_key) {
				taller = false;
				return false;
			}
			else if (key < t->m_key) {
				if (!insert(t->m_lChild, key, elem, taller)) {
					return false;
				}
				if (taller) {
					taller = adjust(t, LEFT);
				}
			}
			else {
				if (!insert(t->m_rChild, key, elem, taller)) {
					return false;
				}
				if (taller) {
					taller = adjust(t, RIGHT);
				}
			}
		}
		return true;
	}
	//在以t为根的树中搜索key节点
	//成功返回节点指针，否则返回nullptr
	AVLTreeNode<KeyType, ElemType>* search(AVLTreeNode<KeyType, ElemType>* t, const KeyType& key)  {
		if (!t)
			return nullptr;
		if (t->m_key == key)
			return t;
		else if (key < t->m_key)
			return search(t->m_lChild, key);
		else
			return search(t->m_rChild, key);
	}

public:
	AVLTreeNode<KeyType, ElemType>* m_root;
	AVLTree() : m_root(nullptr) {}
	~AVLTree() {
		destroy(m_root);
	}

	//对外开放的insert，作为树的成员函数，不必再传入树根参数
	bool insert(const KeyType& key, const ElemType& elem) {
		bool taller = false;
		return insert(m_root, key, elem, taller);
	}
	//对外开放的search，作为树的成员函数，不必再传入树根参数
	AVLTreeNode<KeyType, ElemType>* search(const KeyType& key) {
		return search(m_root, key);
	}
};


