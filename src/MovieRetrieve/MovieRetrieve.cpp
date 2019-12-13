
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

void testSeg() {
	Segmenter seg;
	seg.initDictionary("C:\\Users\\zml05\\Documents\\DS\\第二阶段\\2018013400_叶宸_实验1\\exe\\词库.dic");

	HtmlParser parser;

	//解析得到info 和 summary;
	parser.setHtmlByFileName(CharString("C:\\Users\\zml05\\Desktop\\input\\1.html"));


	CharString info, summary;
	parser.extractInfo(info, summary);

	std::cout << CharString::UTF8ToGB(info.toCStr()) << std::endl;
	std::cout << CharString::UTF8ToGB(summary.toCStr()) << std::endl;

	//将姓名加入词库
	MyList<CharString> names;
	seg.getWords(CharString::UTF8ToGB(info.toCStr()), names);
	auto nameptr = names.headPtr();
	while (nameptr) {
		seg.addWord(nameptr->elem());
		nameptr = nameptr->next();
	}

	//分词
	MyList<CharString> segList;
	seg.divideWords(CharString::UTF8ToGB(summary.toCStr()), segList);
	CharString segmentResult = CharString::join(segList, "\n");
	std::cout << segmentResult;
}


void testPunc() {
	CharString a = CharString::UTF8ToGB(u8"﹑");
	CharString b = "「";
	CharString c = u8"励﹑陪";
	MyList<CharString> segList;
	Segmenter seg;
	seg.initDictionary("C:\\Users\\zml05\\Documents\\DS\\第二阶段\\2018013400_叶宸_实验1\\exe\\词库.dic");
	seg.divideWords(CharString::UTF8ToGB(c.toCStr()), segList);
	CharString segmentResult = CharString::join(segList, "\n");
}

void testAVL() {
	AVLTree<int,int> t;
	for (int i = 0; i <= 20000000; i++) {
		t.insert(i, i);
	}
	for (int i = 0; i < 40000000; i+=10000) {
		printf("%d\n",bool(t.search(i)));
	}
	return;
}

void testDocList() {
	DocList dl;
	for (int i = 0; i<10; i++)
		dl.add(DocNode(i));
	//dl.addPos(1, 5, 1);
	
}

void testAVLandDoclist() {
	AVLTree<CharString, DocList> t;


	int currDocId = 1;
	int currUrlId = 8292;
	const CharString testWordList1[] = { "铁","铜", "铝", "船", "铜", "铝" };
	for (int currPos = 0; currPos < 6; currPos++) {
		CharString currWord = testWordList1[currPos];
		if (!t.search(currWord)) {
			t.insert(currWord, DocList());
		}
		//t.search(currWord)->m_elem.addPos(currDocId, currPos);
	}

	currDocId = 2;
	currUrlId = 8232;
	const CharString testWordList2[] = { "铁","铁"};
	for (int currPos = 0; currPos < 2; currPos++) {
		CharString currWord = testWordList2[currPos];
		if (!t.search(currWord)) {
			t.insert(currWord, DocList());
		}
		//t.search(currWord)->m_elem.addPos(currDocId, currPos);
	}

	currDocId = 3;
	currUrlId = 8293;
	const CharString testWordList3[] = { "铁","铁", "铁" };
	for (int currPos = 0; currPos < 3; currPos++) {
		CharString currWord = testWordList3[currPos];
		if (!t.search(currWord)) {
			t.insert(currWord, DocList());
		}
		//t.search(currWord)->m_elem.addPos(currDocId, currPos);
	}
}
/*
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
	end = clock();
	std::cout << "Time Used: " << (end - start) / 1000 << "s\n";
	getchar();
}
*/
