#include "DocList.h"
std::ostream& operator<<(std::ostream& o, const DocNode& d) {
	o << "(" << d.docId << "," << d.count << ")";
	return o;
}