#pragma once
#include "MyList.h"

class DocList;

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



class DocList :
	public MyList<DocNode>
{
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


	 int howManyDocs() {
		 return length();
	 }

	 int totCount() {
		 int c = 0;
		 auto curr = headPtr();
		 while (curr) {
			 c += curr->elem().count;
			 curr = curr->next();
		 }
	 }
};

