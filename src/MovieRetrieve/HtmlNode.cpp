#include "HtmlNode.h"
std::ostream& operator<< (std::ostream& o, const ExprNode& l) {
	std::cout << "Text: " << l.text;
	return o;
}
std::ofstream& operator<< (std::ofstream& o, const ExprNode& l) {
	o << "Text: " << l.text;
	return o;
}