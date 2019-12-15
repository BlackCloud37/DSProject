#pragma once
#include <iostream>
#include <iterator>





//普通双向链表实现
template<class Elemtype>
class MyList;

template<class Elemtype>
class ListNode {
	friend class MyList<Elemtype>;
	friend class DocList;
public:
	Elemtype m_elem;
	ListNode* m_next;
	ListNode* m_prev;
	ListNode(Elemtype e):m_elem(e) {
		m_next = nullptr;
		m_prev = nullptr;
	}
	Elemtype elem()const {
		return m_elem;
	}
	ListNode* next() const {
		return m_next;
	}
	ListNode* prev() const {
		return m_prev;
	}
};



template<class Elemtype>
class MyListIterator : public std::iterator<std::input_iterator_tag, Elemtype> {
	ListNode<Elemtype>* _ptr;
public:
	MyListIterator(ListNode<Elemtype>* p) {
		_ptr = p;
	}
	MyListIterator& operator = (const MyListIterator& iter) {
		_ptr = iter._ptr;
	}
	bool operator != (const MyListIterator& iter)
	{
		return _ptr != iter._ptr;
	}
	bool operator == (const MyListIterator& iter)
	{
		return _ptr == iter._ptr;
	}

	MyListIterator& operator ++ ()
	{
		_ptr = _ptr->m_next;
		return *this;
	}

	MyListIterator operator ++ (int)
	{
		MyListIterator tmp = *this;
		_ptr = _ptr->m_next;
		return tmp;
	}

	ListNode<Elemtype>& operator * ()
	{
		return *_ptr;
	}
};






template<class Elemtype>
class MyList
{
	ListNode<Elemtype>* m_head;
	ListNode<Elemtype>* m_tail;
	int m_length;
public:
	typedef MyListIterator<Elemtype> iterator;
	iterator begin() {
		return iterator(m_head);
	}
	iterator end() {
		return iterator(m_tail);
		//return iterator(nullptr);
		//return iterator(m_tail->m_next);
	}
	MyList() {
		m_head = nullptr;
		m_tail = nullptr;
		m_length = 0;
	}

	~MyList() {
		ListNode<Elemtype>* curr = m_head;
		while (curr) {
			ListNode<Elemtype>* next = curr->m_next;
			delete curr;
			curr = next;
		}
	}

	MyList(const MyList& other) {
		m_head = nullptr;
		m_tail = nullptr;
		m_length = 0;
		const ListNode<Elemtype>* curr = other.headPtr();
		while (curr) {
			this->add(curr->m_elem);
			curr = curr->m_next;
		}
	}

	MyList(MyList&& other) {
		m_head = nullptr;
		m_tail = nullptr;
		m_length = 0;
		const ListNode<Elemtype>* curr = other.headPtr();
		while (curr) {
			this->add(curr->m_elem);
			curr = curr->m_next;
		}
	}

	MyList& operator=(MyList&& other) {
		clear();
		const ListNode<Elemtype>* curr = other.headPtr();
		while (curr) {
			this->add(curr->m_elem);
			curr = curr->m_next;
		}
		return *this;
	}
	MyList& operator=(const MyList& other) {
		clear();
		const ListNode<Elemtype>* curr = other.headPtr();
		while (curr) {
			this->add(curr->m_elem);
			curr = curr->m_next;
		}
		return *this;
	}

	ListNode<Elemtype>* add(const Elemtype& elem) {
		if (m_head == nullptr) {
			m_head = new ListNode<Elemtype>(elem);
			if (!m_head)
				return nullptr;
			m_tail = m_head;
			m_length = 1;
			return m_tail;
		}
		else {
			ListNode<Elemtype>* tmp = new ListNode<Elemtype>(elem);
			if (!tmp)
				return nullptr;
			tmp->m_prev = m_tail;
			m_tail->m_next = tmp;
			m_tail = tmp;
			m_length++;
			return m_tail;
		}
	}

	ListNode<Elemtype>* tailPtr() const {
		return m_tail;
	}
	Elemtype tail() const {
		if (!m_tail)
			return Elemtype();
		return m_tail->m_elem;
	}

	const int length()const {
		return m_length;
	}

	ListNode<Elemtype>* search(const Elemtype& elem) const{
		ListNode<Elemtype>* curr = m_head;
		while (curr) {
			if (curr->m_elem == elem) {//'ve found the Node, return curr
				return curr;
			}
			curr = curr->m_next;
		}
		return nullptr;
	}

	bool remove(const Elemtype& elem) {
		return remove(search(elem));
	}
	//bool remove(ListNode<Elemtype>* elemPtr) {
	//	return remove(elemPtr);
	//}
	bool remove(ListNode<Elemtype>* elemPtr) {
		if (!elemPtr) {
			return false;
		}
		ListNode<Elemtype>* prev = elemPtr->m_prev;
		ListNode<Elemtype>* next = elemPtr->m_next;
		if (prev) {
			prev->m_next = next;
		}
		if (next) {
			next->m_prev = prev;
		}
		if (elemPtr == m_head) {
			m_head = next;
		}
		if (elemPtr == m_tail) {
			m_tail = prev;
		}
		m_length--;
		delete elemPtr;
		return true;
	}

	bool removeTail() {
		return remove(m_tail);
	}
	
	ListNode<Elemtype>* insert(Elemtype elem, ListNode<Elemtype>* pos) {
		ListNode<Elemtype>* tmp = new ListNode<Elemtype>(elem);
		if (!tmp)
			return nullptr;
		tmp->m_next = pos->next;
		tmp->m_prev = pos;
		pos->m_next = tmp;
		tmp->m_next->m_prev = tmp;
		return tmp;
	}

	const ListNode<Elemtype>* headPtr() const{
		return m_head;
	}
	ListNode<Elemtype>*& m_headPtr(){
		return m_head;
	}
	ListNode<Elemtype>*& m_tailPtr() {
		return m_tail;
	}
	void clear() {
		ListNode<Elemtype>* curr = m_head;
		while (curr) {
			ListNode<Elemtype>* next = curr->m_next;
			delete curr;
			curr = next;
		}
		m_head = nullptr;
		m_tail = nullptr;
		m_length = 0;
	}
	void printList() const {
		ListNode<Elemtype>* curr = m_head;
		while (curr) {
			std::cout << curr->m_elem << " ";
			curr = curr->m_next;
		}
		std::cout << std::endl;
	}
	//template<class Elemtype> friend std::ostream& operator<< (std::ostream&, const MyList&);
	//template<class Elemtype> friend std::ofstream& operator<< (std::ofstream&, const MyList&);
    //friend std::ostream& operator<< (std::ostream&, const MyList&);
    //friend std::ofstream& operator<< (std::ofstream&, const MyList&);
};


