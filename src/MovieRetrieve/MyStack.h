#pragma once
#include "MyString.h"
#include "MyList.h"



//基于List，以适配器模式实现栈
template<class Elemtype>
class Stack
{
	MyList<Elemtype> m_list;
public:
	Stack() {
		m_list = MyList<Elemtype>();
	}
	~Stack() {
	}
	Stack(const Stack& other) {
		this->m_list = other.m_list;
	}
	Stack(Stack&& other) {
		this->m_list = other.m_list;
	}
	Stack& operator=(Stack&& other) {
		this->m_list = other.m_list;
		return *this;
	}
	Stack& operator=(const Stack& other) {
		this->m_list = other.m_list;
		return *this;
	}
	bool push(const Elemtype& elem) {
		return m_list.add(elem);
	}
	bool pop() {
		return m_list.removeTail();
	}
	Elemtype top() {
		return m_list.tail();
	}
	bool empty() {
		return m_list.length() == 0;
	}
	int size() {
		return m_list.length();
	}
	void printStack() {
		m_list.printList();
	}
	void clear() {
		m_list.clear();
	}
	//template<class Elemtype> friend std::ostream& operator<< (std::ostream&, const Stack<Elemtype>&);
	//template<class Elemtype> friend std::ofstream& operator<< (std::ofstream&, const Stack<Elemtype>&);
    friend std::ostream& operator<< (std::ostream&, const Stack<Elemtype>&);
    friend std::ofstream& operator<< (std::ofstream&, const Stack<Elemtype>&);
};

template<class Elemtype>
std::ostream& operator<< (std::ostream& o, const Stack<Elemtype>& l) {
	o << l.m_list;
	return o;
}
template<class Elemtype>
std::ofstream& operator<< (std::ofstream& o, const Stack<Elemtype>& l) {
	o << l.m_list;
	return o;
}
