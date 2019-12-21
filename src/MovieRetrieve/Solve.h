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
	AVLTree<CharString, DocList> recommendTree;
	AVLTree<int, CharString> movieIdToName;
public:
	//输入一个电影id，生成其html文件
    void generateHtml(int docId)
    {
        std::string filename = "./output/" + std::to_string(docId) + ".info";
        std::ifstream f(filename);
		assert(f.is_open());
        if (!f.is_open()) {
            return;
        }
        
        std::string line,target;
        std::getline(f,line);
        
    
        target += "<h1>";
        target += line;
        target += "</h1>";
        
        while(std::getline(f,line)) {
            target += line;
            target += "<br>";
        }
        target += "<br>";
        f.close();
        target += u8"<h1>推荐电影</h1>";
        
        DocList recommendlist;
        int count = 0;
        recommend(docId, recommendlist);
        
        auto currRecommend = recommendlist.headPtr();
        while(currRecommend && count < 5) {
            int currDocId = currRecommend->elem().docId;
            std::ifstream docFile("./output/" + std::to_string(currDocId) + ".info");
            assert(docFile.is_open());
            if (!docFile.is_open()) {
                currRecommend = currRecommend->next();
                continue;
            }
            count ++;
            std::getline(docFile,line);
            
            target += "<h2>";
            target += line;
            target += "</h2>";
            
            while(std::getline(docFile,line)) {
                target += line;
                target += "<br>";
            }
            target += "<br>";
            docFile.close();
            currRecommend = currRecommend->next();
        }
        
        fc.saveStringTo(target, "./html/" + std::to_string(docId) + ".html", true);
    }
    //为每一个电影生成html文件，用于gui
    void generateHtml() {
        MyList<CharString> filenames;
		fc.findFilenames("./input", filenames);
		int tot = filenames.length();
		int count = 0;
        auto currFile = filenames.headPtr();
        while(currFile) {
			count++;
			std::cout << std::setprecision(4) << float(count) / float(tot) * 100 << "%" << std::endl;

            int docId= atoi(currFile->elem().split(".html",true,1).headPtr()->elem().toCStr());
            generateHtml(docId);
			currFile = currFile->next();
        }
    }
    
	void extractWordsToFile() {
		MyList<CharString> filenames;
		fc.findFilenames("./input", filenames);

		Segmenter seg;
		seg.initDictionary("./词库.dic");
		seg.initStopUsedDic("./停用词.dic");

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
			CharString info, summary, summaryHtml;
			parser.extractInfo(info, summary);

			//将姓名/电影名加入词库
			MyList<CharString> names;
			MyList<CharString> segList;
			seg.getInfoWords(CharString::UTF8ToGB(info.toStr().c_str()), names);

			auto nameptr = names.headPtr();
			while (nameptr) {
				seg.addWordToDictionary(nameptr->elem());
				segList.add(nameptr->elem());
				nameptr = nameptr->next();
			}

			//保存文件
			fc.saveStringTo(CharString::join(segList, "\n"), (CharString("./movieInfo/") + currFile->elem().split(".html", true, 1).headPtr()->elem() + CharString(".info")).toStr());
			info += "\n";
			info += CharString::join(summary.split("\t \n", false));

			std::string infoFilename = (CharString("./output/") + currFile->elem().split(".html", true, 1).headPtr()->elem() + CharString(".info")).toStr();
			fc.saveStringTo(info, infoFilename, true);

			//分词
			seg.divideWords(CharString::UTF8ToGB(summary.toCStr()), segList);
			CharString segmentResult = CharString::join(segList, "\n");

			std::string dividewordFilename = (CharString("./output/") + currFile->elem().split(".html", true, 1).headPtr()->elem() + CharString(".txt")).toStr();
			fc.saveStringTo(segmentResult, dividewordFilename);

			
			//下一个文件
			currFile = currFile->next();
		}
		std::cout << "Finished! Press any key to quit!" << std::endl;
		//getchar();
	}

	//根据output里的分词结果创建词典
	void createAVLDictionary() {

		MyList<CharString> filenames;
		fc.findFilenames("./output", filenames, "txt");

		int tot = filenames.length(), count = 0;//计算进度百分比

		std::stringstream buf;
		std::ifstream docfile;

		auto currFile = filenames.headPtr();
		while (currFile) {
			count++;
			std::cout << std::setprecision(4) <<  float(count) / float(tot) * 100 << "%" << std::endl;

			CharString filename = CharString("./output/") + currFile->elem();
			int currDocId = atoi(currFile->elem().substring(0, currFile->elem().indexOf(".txt").m_headPtr()->elem()).toCStr());

			docfile.open(filename.toStr());
			assert(docfile.is_open());
			buf.clear();
			buf.str("");
			buf << docfile.rdbuf();
			auto wordList = CharString(buf.str()).split("\n");
			docfile.close();

			auto currWord = wordList.headPtr();
			while (currWord) {
				if (!docTree.search(currWord->elem())) {
					docTree.insert(currWord->elem(), DocList());
				}
				docTree.search(currWord->elem())->m_elem.edit(currDocId);

				currWord = currWord->next();
			}
			
			currFile = currFile->next();
		}
	}
	//根据movieInfo文件夹内的词表创建电影推荐树
	void createRecommendTree() {

		MyList<CharString> filenames;
		fc.findFilenames("./MovieInfo", filenames);

		int tot = filenames.length(), count = 0;//计算进度百分比

		std::stringstream buf;
		std::ifstream docfile;

		auto currFile = filenames.headPtr();
		while (currFile) {
			count++;
			std::cout << std::setprecision(4) << float(count) / float(tot) * 100 << "%" << std::endl;

			CharString filename = CharString("./MovieInfo/") + currFile->elem();
			int currDocId = atoi(currFile->elem().substring(0, currFile->elem().indexOf(".info").m_headPtr()->elem()).toCStr());

			docfile.open(filename.toStr());
			assert(docfile.is_open());
			buf.clear();
			buf.str("");
			buf << docfile.rdbuf();

			auto wordList = CharString(buf.str()).split("\n");
			docfile.close();
			
			movieIdToName.insert(currDocId, wordList.headPtr()->elem());
			auto currWord = wordList.headPtr();
			while (currWord) {
				if (!recommendTree.search(currWord->elem())) {
					recommendTree.insert(currWord->elem(), DocList());
				}
				recommendTree.search(currWord->elem())->m_elem.edit(currDocId);

				currWord = currWord->next();
			}
			currFile = currFile->next();
		}
	}
	//输入关键词链表list，将查询结果保存到ret
    void query(const MyList<CharString>& list, DocList& ret) {
        auto currWord = list.headPtr();
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
                ret.edit(docId, count);
                currDoc = currDoc->next();
            }
            currWord = currWord->next();
        }

    }
	//读取query1,生成result1
	void queryToFile() {
		std::ifstream queryFile("./query1.txt");
		std::ofstream resultFile("./result1.txt",std::ios::trunc | std::ios::out);
		if (!queryFile.is_open()) {
			std::cout << "Error::Solve::queryToFile()::Can't open query1.txt\n";
			return;
		}
		std::string line;
		while (std::getline(queryFile, line)) {
			DocList queryResult;
			query(CharString(CharString::UTF8ToGB(line.c_str())).split(" "), queryResult);
			

			auto currQuery = queryResult.headPtr();
			while (currQuery) {
				resultFile << currQuery->elem();
				currQuery = currQuery->next();
			}



			resultFile << "\n";
		}
		queryFile.close();
		resultFile.close();
	}
	//输入要推荐的电影id，将推荐结果保存到ret
	void recommend(int dstDocId, DocList& ret) {
		
		std::stringstream buf;
		std::ifstream docfile;

		std::string filename = "./movieInfo/" + std::to_string(dstDocId) + ".info";
		
		docfile.open(filename);
		assert(docfile.is_open());
		buf.clear();
		buf.str("");
		buf << docfile.rdbuf();
		auto wordList = CharString(buf.str()).split("\n");
		docfile.close();

		auto currWord = wordList.headPtr();
		while (currWord) {
			auto treeNode = recommendTree.search(currWord->elem());
			if (treeNode) {//遍历该词语所有文档，出现的文档及词语次数都计入推荐列表
				auto currDoc = treeNode->m_elem.headPtr();
				while (currDoc)
				{
					if (currDoc->elem().docId != dstDocId) {
						int docId = currDoc->elem().docId;
						int count = currDoc->elem().count;
						ret.edit(docId, count);
					}
					currDoc = currDoc->m_next;

				}
			}

			currWord = currWord->next();
		}
	}
	//读取query2，生成result2
	void recommendToFile() {
		std::ifstream queryFile("./query2.txt");
		std::ofstream resultFile("./result2.txt", std::ios::trunc | std::ios::out);
		if (!queryFile.is_open()) {
			std::cout << "Error::Solve::recommendToFile()::Can't open query2.txt\n";
			return;
		}
		int docId;
		std::string line;
		while (std::getline(queryFile, line)) {
			if (getMovieId(CharString::UTF8ToGB(line.c_str()), docId)) {
				DocList queryResult;
				recommend(docId, queryResult);
				
				auto currDoc = queryResult.headPtr();
				int currDocId;
				CharString currMovieName;
				int count = 0;
				while (currDoc && count < 5) {
					currDocId = currDoc->elem().docId;
					if (getMovieName(currDocId, currMovieName)) {
						count++;
						resultFile << "(" << currDocId << "," << currMovieName << ")";
					}
					currDoc = currDoc->next();
				}
				resultFile << "\n";
			}
			else {
				resultFile << "该电影不在数据库中，无法推荐\n";
			}
		}
		queryFile.close();
		resultFile.close();
	}
	//以电影名搜索电影id
	bool getMovieId(const CharString& name, int& id) {
		auto treeNode = recommendTree.search(name);
		if (treeNode) {
			id = treeNode->m_elem.headPtr()->elem().docId;
			return true;
		}
		else return false;
	}
	//以电影id搜索电影名
	bool getMovieName(int docId, CharString& name) {
		auto treeNode = movieIdToName.search(docId);
		if (treeNode) {
			name = treeNode->m_elem;
			return true;
		}
		else return false;
	}
};

