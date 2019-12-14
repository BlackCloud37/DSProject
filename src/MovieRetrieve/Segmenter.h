#pragma once
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
#include "MyString.h"
#include "MyHashTable.h"
#include "MyList.h"
//#define CHINESEPUNCNUM 22
//�ִ���
class Segmenter
{
	MyHashTable<CharString> m_dictionary;//�ʵ�
	MyHashTable<CharString> m_stopUsedDictionary;//ͣ�ôʵ�
	int m_maxLengthOfWords;//�ʵ��д������󳤶�
	int m_lengthOfACharacter;//һ���ֵĳ��ȣ�GB��Ϊ2
	//const CharString Chinesepuncs[CHINESEPUNCNUM] = { "��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","�p","��","��" };
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

	//�����articlesource������GB������ַ���������ִʽ�����浽ret
	void divideWords(const CharString& articlesource, MyList<CharString>& ret) {
		CharString article = articlesource;
		
		//Ԥ�����Ȱ�����Ӣ�ĵ��ʽ�ȡ������
		extractEngAndNum(articlesource, ret);

		//split, join����ȥ��Ӣ���ַ�
		auto noEngPuncList = article.split(Englishpuncs, false);
		article = CharString::join(noEngPuncList);

		//�������ƥ��ķִ��㷨
		int currPos = article.length() - 1;
		while (currPos >= m_lengthOfACharacter) {
			//bool singleWord = true;
			//����ɨ��
			for (int i = m_maxLengthOfWords; i >= m_lengthOfACharacter; i = i - m_lengthOfACharacter) {
				CharString wordPossible = article.substring(currPos - i + 1, i);//��ȡ��Ҫɨ��Ĵ�
				if (m_dictionary.has(wordPossible)) {//���������
					ret.add(wordPossible);

				}
			}
			currPos -= m_lengthOfACharacter;
		}

		//���˵����е�ͣ�ô�
		removeStopUsedWords(ret);
	}

	//��info�н����������ȴ���
	void getInfoWords(const CharString& info, MyList<CharString>& wordlist) {
		//extractEngAndNum(info, wordlist);
		const auto& noEngList = info.split(Englishpuncs, false);
		auto curr = noEngList.headPtr();
		while (curr)
		{
			auto text = curr->elem();
			auto index = text.indexOf("��", 1);
			while (index.length())
			{
				auto pos = index.headPtr()->elem();
				wordlist.add(text.substring(0, pos));
				text.cutstring(0, pos + 2);
				index = text.indexOf("��", 1);
			}
			wordlist.add(text);
			curr = curr->next();
		}
		removeStopUsedWords(wordlist);
	}

};
