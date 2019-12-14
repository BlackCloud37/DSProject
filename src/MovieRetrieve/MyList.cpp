#include "MyList.h"

template<class Elemtype>
std::ostream& operator<<(std::ostream& o, const MyList<Elemtype>& l) {
	const ListNode<Elemtype>* curr = l.headPtr();
	while (curr) {
		o << curr->elem() << " ";
		curr = curr->next();
	}
	return o;
}
template<class Elemtype>
std::ofstream& operator<<(std::ofstream& o, const MyList<Elemtype>& l) {
	const ListNode<Elemtype>* curr = l.headPtr();
	while (curr) {
		o << curr->elem() << " ";
		curr = curr->next();
	}
	return o;
}
