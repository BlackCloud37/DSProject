
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <iomanip>
#include <ctime>

#include "AVLTree.h"
#include "FileController.h"
#include "MyList.h"
#include "MyStack.h"
#include "MyString.h"
#include "HtmlNode.h"
#include "MyHashTable.h"
#include "Segmenter.h"
#include "DocList.h"
#include "Solve.h"


int main()
{
	Solve s;
	clock_t start, end;
	start = clock();
	s.extractWordsToFile();
	s.createAVLDictionary();
	s.createRecommendTree();
	s.queryToFile();
	s.recommendToFile();
	s.generateHtml();
	end = clock();
	std::cout << "Time Used: " << (end - start) / 1000 << "s\n";
}
