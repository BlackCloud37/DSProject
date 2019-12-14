#pragma once
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
#include "MyString.h"
#include "MyHashTable.h"
#include "MyList.h"
//#define CHINESEPUNCNUM 22
//分词类
class Segmenter
{
	MyHashTable<CharString> m_dictionary;//词典
	MyHashTable<CharString> m_stopUsedDictionary;//停用词典
	int m_maxLengthOfWords;//词典中词语的最大长度
	int m_lengthOfACharacter;//一个字的长度，GB下为2
	//const CharString Chinesepuncs[CHINESEPUNCNUM] = { "·","　","。","！","…","（","）","—","；","，","《","》","？","：","“","”","、","【","】","﹑","「","」" };
	const CharString Englishpuncs = " \t\n~,./-=[]{};:'!?@#$%^&*()_+<>`|\\qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890";
public:

	void addWordToDictionary(const CharString& word) {
		if (!m_dictionary.has(word)) {
			m_dictionary.insert(word);
			if (word.length() > m_maxLengthOfWords) {
				m_maxLengthOfWords = word.length();
			}
		}

	}
	void addStopUsedWord(const CharString& word) {
		if (!m_stopUsedDictionary.has(word)) {
			m_stopUsedDictionary.insert(word);
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
				m_dictionary.insert(word);
			}
		}
		else
		{
			std::ifstream dic(filename);
			std::string temp;
			while (getline(dic, temp)) {
				m_dictionary.insert(CharString(temp));
			}
		}
		m_lengthOfACharacter = 2;
	}

	void initStopUsedDic(const char* filename, const CharString& codetype = "UTF-8") {
		if (codetype == "UTF-8") {
			std::ifstream dic(filename);
			std::string temp;
			while (getline(dic, temp)) {
				CharString word = CharString::UTF8ToGB(temp.c_str());
				m_stopUsedDictionary.insert(word);
			}
		}
		else
		{
			std::ifstream dic(filename);
			std::string temp;
			while (getline(dic, temp)) {
				m_stopUsedDictionary.insert(CharString(temp));
			}
		}
	}

	void extractEngAndNum(const CharString& articlesource, MyList<CharString>& ret) {
		int leftI = articlesource.length(), rightI = articlesource.length();
		for (int i = articlesource.length() - 1; i >= 0; i--) {
			if ((articlesource[i] >= 'a' && articlesource[i] <= 'z') || (articlesource[i] >= 'A' && articlesource[i] <= 'Z') || (articlesource[i] <= '9' && articlesource[i] >= '0')) {
				leftI = i;
			}
			else
			{
				if (rightI > leftI) {
					ret.add(articlesource.substring(leftI, rightI - leftI));
				}
				rightI = i;
				leftI = rightI;
			}
		}

		if (rightI > leftI) {
			ret.add(articlesource.substring(leftI, rightI - leftI));
		}
	}

	void removeStopUsedWords(MyList<CharString>& ret) {
		auto wordPtr = ret.m_headPtr();
		while (wordPtr) {
			if (m_stopUsedDictionary.has(wordPtr->elem())) {
				auto tmp = wordPtr;
				wordPtr = wordPtr->next();
				ret.remove(tmp);
				continue;
			}
			wordPtr = wordPtr->next();
		}
	}

	//输入的articlesource必须是GB编码的字符串，将其分词结果保存到ret
	void divideWords(const CharString& articlesource, MyList<CharString>& ret) {
		CharString article = articlesource;
		
		//预处理，先把所有英文单词截取并保存
		extractEngAndNum(articlesource, ret);

		//split, join方法去除英文字符
		auto noEngPuncList = article.split(Englishpuncs, false);
		article = CharString::join(noEngPuncList);

		//后向最大匹配的分词算法
		int currPos = article.length() - 1;
		while (currPos >= m_lengthOfACharacter) {
			//bool singleWord = true;
			//逆向扫描
			for (int i = m_maxLengthOfWords; i >= m_lengthOfACharacter; i = i - m_lengthOfACharacter) {
				CharString wordPossible = article.substring(currPos - i + 1, i);//截取的要扫描的词
				if (m_dictionary.has(wordPossible)) {//存在这个词
					ret.add(wordPossible);

				}
			}
			currPos -= m_lengthOfACharacter;
		}

		//过滤掉所有的停用词
		removeStopUsedWords(ret);
	}

	//从info中解析出人名等词语
	void getInfoWords(const CharString& info, MyList<CharString>& wordlist) {
		//extractEngAndNum(info, wordlist);
		const auto& noEngList = info.split(Englishpuncs, false);
		auto curr = noEngList.headPtr();
		while (curr)
		{
			auto text = curr->elem();
			auto index = text.indexOf("·", 1);
			while (index.length())
			{
				auto pos = index.headPtr()->elem();
				wordlist.add(text.substring(0, pos));
				text.cutstring(0, pos + 2);
				index = text.indexOf("·", 1);
			}
			wordlist.add(text);
			curr = curr->next();
		}
		removeStopUsedWords(wordlist);
	}

};
