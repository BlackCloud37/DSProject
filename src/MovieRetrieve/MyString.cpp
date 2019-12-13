#include "MyString.h"
std::ostream& operator<< (std::ostream& o,const CharString& s) {
	for (int i = 0; i < s.length(); i++) {
		o << s[i];
	}
	return o;
}
