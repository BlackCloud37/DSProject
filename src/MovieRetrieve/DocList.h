#pragma once
#include "MyList.h"

class DocList;


//文档节点，包含了文档id和一计数，计数表示了此文档中，某个词出现的次数
class DocNode {
public:
	friend class DocList;
	int docId;
	int count;
     
	DocNode(int docid) :docId(docid), count(0){

	}
    DocNode(DocNode& other):docId(other.docId), count(other.count) {
        
    }
    DocNode(const DocNode& other):docId(other.docId), count(other.count) {
        
    }
	bool operator==(const DocNode& other) {
		return count == other.count;
	}
	bool operator<(const DocNode& other) {
		return count < other.count;
	}
	bool operator>(const DocNode& other) {
		return count > other.count;
	}
	friend std::ostream& operator<<(std::ostream&, const DocNode&);
};


//文档链表，每个链表对应一个词，链表中的节点分别记录了每个文档中该词出现的次数
class DocList :
	//继承链表类
	public MyList<DocNode>
{	
	//为维持“关键词出现次数最多的靠前”的排序，每次插入/修改某节点后都让它上浮到合适位置
	bool update(ListNode<DocNode>* p) {
		if (!p)
			return false;
		while (p->m_prev && p->m_prev->m_elem < p->m_elem) {
			ListNode<DocNode>* prev_prev = p->m_prev->m_prev;
			ListNode<DocNode>* prev = p->m_prev;
			ListNode<DocNode>* next = p->m_next;
			p->m_prev->m_prev = p;
			if (next)
				next->m_prev = p->m_prev;
			p->m_prev->m_next = next;
			p->m_prev = prev_prev;
			p->m_next = prev;
			if (prev_prev)
				prev_prev->m_next = p;
		}
		if (!p->m_prev) {
			m_headPtr() = p;
		}
		while (p->next()) {
			p = p->next();
		}
		m_tailPtr() = p;
		return true;
	}

public:
    DocList() {
        
    }
    DocList(DocList& other): MyList<DocNode>(other) {
        
    }
    DocList(const DocList& other): MyList<DocNode>(other) {
        
    }
	//在链表中搜索文档
	//成功返回节点指针，失败返回nullptr
    ListNode<DocNode>* searchByDocID(int docId) {
		ListNode<DocNode>* curr = m_headPtr();
		while (curr) {
			if (curr->m_elem.docId == docId) {//'ve found the Node, return curr
				return curr;
			}
			curr = curr->m_next;
		}
		return nullptr;
	 }

	//增加某文档计数（即Edit）
	 void addCount(int docId, int count = 1) {
		 ListNode<DocNode>* node = searchByDocID(docId);
		 if (!node) {
			 add(DocNode(docId))->m_elem.count += count;
		 }
		 else {
			 node->m_elem.count += count;
			 update(node);//todo fix update
		 }
		 return;
	 }

	 //返回该链表文档总数
	 int howManyDocs() {
		 return length();
	 }
	 //返回该词在所有文档中出现总次数
	 int totCount() {
		 int c = 0;
		 auto curr = headPtr();
		 while (curr) {
			 c += curr->elem().count;
			 curr = curr->next();
		 }
	 }
};

