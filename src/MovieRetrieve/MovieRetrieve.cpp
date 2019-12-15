
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
// 控制台显示乱码纠正
void init() {
	system("chcp 65001"); //设置字符集 （使用SetConsoleCP(65001)设置无效，原因未知）

	CONSOLE_FONT_INFOEX info = { 0 }; // 以下设置字体
	info.cbSize = sizeof(info);
	info.dwFontSize.Y = 16; // leave X as zero
	info.FontWeight = FW_NORMAL;
	wcscpy(info.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
}

void testListIter() {
	MyList<int> l;
	for (int i = 0; i < 3; i++) {
		l.add(i);
	}

	std::cout << (*l.begin()).elem() << " " << (*l.end()).elem() << "\n";
	for (ListNode<int> numNode : l) {
		std::cout << numNode.elem() << "\n";
	}
}
void testUpdate() {
	AVLTree<CharString, DocList> t;
	t.insert("悲剧", DocList());
	auto p = t.search("悲剧");
	p->m_elem.addCount(0);
	p->m_elem.addCount(1);
	p->m_elem.addCount(1);
	p->m_elem.addCount(2);
	p->m_elem.addCount(3);
	p->m_elem.addCount(3);
}


int main()
{
	Solve s;
	//init();
	clock_t start, end;
	start = clock();
	//testAVL();
	//testDocList();
	//testSeg();
	//solve();
	//testPunc();
	//testAVLandDoclist();
	//s.extractWordsToFile();
	s.createAVLDictionary();
	s.createRecommendTree();
	s.queryToFile();
	//s.recommendToFile();

	
	//std::cout << "\n";
	//s.recommend(2).printList();
	//s.queryToFile();
	//testListIter();
	end = clock();
	std::cout << "Time Used: " << (end - start) / 1000 << "s\n";
}



//todo: 修改getname 去重