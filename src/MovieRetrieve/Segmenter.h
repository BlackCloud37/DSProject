#pragma once
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
#include "MyString.h"
#include "MyHashTable.h"
#include "MyList.h"
#define CHINESEPUNCNUM 22
//分词类
class Segmenter
{
	MyHashTable<CharString> m_hashTable;//词典
	int m_maxLengthOfWords;//词典中词语的最大长度
	int m_lengthOfACharacter;//一个字的长度，GB下为2
	CharString Chinesepuncs[CHINESEPUNCNUM] = { "·","　","。","！","…","（","）","—","；","，","《","》","？","：","“","”","、","【","】","﹑","「","」" };
public:

	void addWord(const CharString& word) {
		if (!m_hashTable.has(word)) {
			m_hashTable.insert(word);
			if (word.length() > m_maxLengthOfWords) {
				m_maxLengthOfWords = word.length();
			}
		}
			
	}

	void initDictionary(const char* filename, const CharString& codetype = "UTF-8") {
		if (codetype == "UTF-8") {
			std::ifstream dic(filename);
			std::string temp;
			while (getline(dic, temp)) {
				CharString word = CharString::UTF8ToGB(temp.c_str());
				if (m_maxLengthOfWords < word.length())
					m_maxLengthOfWords = word.length();
				m_hashTable.insert(word);
			}
		}
		else
		{
			std::ifstream dic(filename);
			std::string temp;
			while (getline(dic, temp)) {
				m_hashTable.insert(CharString(temp));
			}
		}
		m_lengthOfACharacter = 2;
	}


	//输入的articlesource必须是GB编码的字符串，将其分词结果保存到ret
	void divideWords(const CharString& articlesource, MyList<CharString>& ret) {
		CharString article = articlesource;

		//预处理，先把所有英文单词截取并保存，再去除所有非中文标点和字符
		int leftI = articlesource.length(), rightI = articlesource.length();
		for (int i = articlesource.length() - 1 ; i >= 0; i--) {
			if ((articlesource[i] >= 'a' && articlesource[i] <= 'z') || (articlesource[i] >= 'A' && articlesource[i] <= 'Z') || (articlesource[i] <='9' && articlesource[i] >='0')) {
				leftI = i;
			}
			else
			{
				if (rightI > leftI) {
					ret.add(article.substring(leftI, rightI - leftI));
				}
				rightI = i;
				leftI = rightI;
			}
		}

		if (rightI > leftI) {
			ret.add(article.substring(leftI, rightI - leftI));
		}

			//split, join方法去除英文字符
		auto noEngPuncList = article.split(" \t\n~,./-=[]{};:'!?@#$%^&*()_+<>`|\\qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890", false);
		article = CharString::join(noEngPuncList);

		//MyList<CharString> noEngPuncList;


		//后向最大匹配的分词算法
		int currPos = article.length() - 1;
		while (currPos >= m_lengthOfACharacter) {
			//bool singleWord = true;
			//逆向扫描
			for (int i = m_maxLengthOfWords; i >= m_lengthOfACharacter; i = i - m_lengthOfACharacter) {
				CharString wordPossible = article.substring(currPos - i + 1, i);//截取的要扫描的词
				if (m_hashTable.has(wordPossible)) {//存在这个词
					ret.add(wordPossible);
					//currPos -= i;
					//singleWord = false;
					//break;
				}
			}
			currPos -= m_lengthOfACharacter;
			/*
			if (singleWord) {//单字成词
				CharString singleWordPossible = article.substring(currPos - m_lengthOfACharacter + 1, m_lengthOfACharacter);
				ret.add(singleWordPossible);
				currPos -= m_lengthOfACharacter;
			}
			*/
			
		}

		if (!currPos) {
			ret.add(article.substring(0, m_lengthOfACharacter));
		}


		//过滤掉所有的中文标点
		for (int i = 0; i < CHINESEPUNCNUM; i++) {
			auto search = ret.search(Chinesepuncs[i]);
			while (search) {
				ret.remove(search);
				search = ret.search(Chinesepuncs[i]);
			}
		}
	}

	//从info中解析出人名等词语
	void getInfoWords(const CharString& info, MyList<CharString>& wordlist) {
		const auto& noEngList = info.split("1234567890 \t\n~,./:-=[]{};'!?@#$%^&*()_+<>`|\\qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM", false);
		auto curr = noEngList.headPtr();
		while (curr)
		{
			auto text = curr->elem();
			auto index = text.indexOf("·", 1);
			while (index.length())
			{
				auto pos = index.headPtr()->elem();
				wordlist.add(text.substring(0, pos));
				text.cutstring(0, pos+2);
				index = text.indexOf("·", 1);
			}
			wordlist.add(text);
			curr = curr->next();
		}
	}
};

