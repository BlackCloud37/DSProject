#pragma once
#include "MyList.h"
#include "MyString.h"


template <class Valuetype>
class MyHashTable;

template <class Valuetype>
class HashNode {
	friend class MyHashTable<Valuetype>;
	CharString key;
	Valuetype val;
public:
	HashNode(const CharString& k):key(k) {
	}
	HashNode(const CharString& k, const Valuetype& v) :key(k), val(v) {

	}
	Valuetype value() const {
		return val;
	}
	bool operator==(const HashNode& other) {
		return key == other.key;
	}
};

template <class Valuetype>
class MyHashTable {
	int m_size;
	MyList<HashNode<Valuetype> > *m_hashNodes;//array of list
public:
	MyHashTable(const int size = 300000):m_size(size) {
		m_hashNodes = new MyList<HashNode<Valuetype> >[size]();
	}
	~MyHashTable() {
		delete[] m_hashNodes;
	}
	void insert(const CharString& key) {
		int index = hash(key);
		m_hashNodes[index].add(HashNode<Valuetype>(key));
	}
	void insert(const CharString& key, const Valuetype& val) {
		int index = hash(key);
		m_hashNodes[index].add(HashNode<Valuetype>(key, val));
	}
	void del(const CharString& key) {
		int index = hash(key);
		m_hashNodes[index].remove(HashNode<Valuetype>(key));
	}
	//判断是否存在此词
	bool has(const CharString& key) {
		return m_hashNodes[hash(key)].search(HashNode<Valuetype>(key));
	}

	Valuetype get(const CharString& key) {
		return m_hashNodes[hash(key)].search(HashNode<Valuetype>(key))->elem().value();
	}

	//哈希算法，直接求和，乘以大素数再取模
	int hash(const CharString& key) {
		int sum = 0;
		for (int i = 0; i < key.length(); i++) {
			sum += key[i];
		}
		return abs((sum - 1) * 40503 % m_size);
	}
};

