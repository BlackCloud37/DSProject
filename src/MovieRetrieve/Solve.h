#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <iomanip>
#include <ctime>
#include <utility>
#include "AVLTree.h"
#include "FileController.h"
#include "MyList.h"
#include "MyStack.h"
#include "MyString.h"
#include "HtmlNode.h"
#include "MyHashTable.h"
#include "Segmenter.h"
#include "DocList.h"
class Solve
{
    FileController fc;
    AVLTree<CharString, DocList> docTree;
public:
	void extractWordsToFile() {
		MyList<CharString> filenames;
		fc.findFilenames("./input", filenames);


		Segmenter seg;
		seg.initDictionary("./词库.dic");

		int tot = filenames.length(), count = 0;//计算进度百分比

		HtmlParser parser;
		auto currFile = filenames.headPtr();
		//遍历文件
		while (currFile) {
			count++;
			std::cout << std::setprecision(4) << float(count) / float(tot) * 100 << "%" << std::endl;

			CharString filename = CharString("./input/") + currFile->elem();
			//解析得到info 和 summary;
			parser.setHtmlByFileName(filename);
			CharString info, summary;
			parser.extractInfo(info, summary);

			//将姓名加入词库
			MyList<CharString> names;
			seg.getWords(CharString::UTF8ToGB(info.toStr().c_str()), names);

			auto nameptr = names.headPtr();
			while (nameptr) {
				seg.addWord(nameptr->elem());
				nameptr = nameptr->next();
			}

			//保存文件
			info += "\n";
			info += CharString::join(summary.split("\t \n", false));

			std::string infoFilename = (CharString("./output/") + currFile->elem().split(".html", true, 1).headPtr()->elem() + CharString(".info")).toStr();
			fc.saveStringTo(info, infoFilename);

			//分词
			MyList<CharString> segList;
			seg.divideWords(CharString::UTF8ToGB(summary.toCStr()), segList);
			CharString segmentResult = CharString::join(segList, "\n");

			std::string dividewordFilename = (CharString("./output/") + currFile->elem().split(".html", true, 1).headPtr()->elem() + CharString(".txt")).toStr();
			fc.saveStringTo(segmentResult, dividewordFilename);
			//std::cout << segmentResult << std::endl;

			//下一个文件
			currFile = currFile->next();
		}
		std::cout << "Finished! Press any key to quit!" << std::endl;
		//getchar();
	}

	void createAVLDictionary() {

		MyList<CharString> filenames;
		//fc.findFilenames("./output", filenames);
		fc.findFilenames("C:\\Users\\zml05\\Desktop\\output", filenames);
	
		int tot = filenames.length(), count = 0;//计算进度百分比


		std::stringstream buf;
		std::ifstream docfile;

		auto currFile = filenames.headPtr();
		while (currFile) {
			count++;
			std::cout << std::setprecision(4) << float(count) / float(tot) * 100 << "%" << std::endl;
			buf.clear();

			if (currFile->elem().indexOf(".txt").length()) {
				//CharString filename = CharString("./input/") + currFile->elem();
				CharString filename = CharString("C:\\Users\\zml05\\Desktop\\output\\") + currFile->elem();
				int currDocId = atoi(currFile->elem().substring(0, currFile->elem().indexOf(".txt").m_headPtr()->elem()).toCStr());


				docfile.open(filename.toStr());
				assert(docfile.is_open());
				buf << docfile.rdbuf();
				auto wordList = CharString(buf.str()).split("\n");
				docfile.close();

				
				auto currWord = wordList.headPtr();
				while (currWord) {
					if (currWord->elem().length() <= 2) {
						currWord = currWord->next();
						continue;
					}

                    if (!docTree.search(currWord->elem())) {
                        docTree.insert(currWord->elem(), DocList());
					}
                    docTree.search(currWord->elem())->m_elem.addCount(currDocId);

					currWord = currWord->next();
				}
			}
			currFile = currFile->next();
		}
	}

    DocList query(const MyList<CharString>& list) {
        auto currWord = list.headPtr();
        DocList ret;
        while(currWord) {
            CharString word = currWord->elem();
            AVLTreeNode<CharString, DocList>* treeNode = docTree.search(word);
            if (!treeNode) {
                currWord = currWord->next();
                continue;
            }
            DocList& doclist = treeNode->m_elem; 
            auto currDoc = doclist.headPtr();
            while(currDoc) {
                int docId = currDoc->elem().docId;
                int count = currDoc->elem().count;
                ret.addCount(docId, count);
                currDoc = currDoc->next();
            }
            currWord = currWord->next();
        }
        return ret;
    }


};

