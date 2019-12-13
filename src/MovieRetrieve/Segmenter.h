#pragma once
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
#include "MyString.h"
#include "MyHashTable.h"
#include "MyList.h"
#define CHINESEPUNCNUM 22
//�ִ���
class Segmenter
{
	MyHashTable<CharString> m_hashTable;//�ʵ�
	int m_maxLengthOfWords;//�ʵ��д������󳤶�
	int m_lengthOfACharacter;//һ���ֵĳ��ȣ�GB��Ϊ2
	CharString Chinesepuncs[CHINESEPUNCNUM] = { "��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","�p","��","��" };
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


	//�����articlesource������GB������ַ���������ִʽ�����浽ret
	void divideWords(const CharString& articlesource, MyList<CharString>& ret) {
		CharString article = articlesource;

		//Ԥ�����Ȱ�����Ӣ�ĵ��ʽ�ȡ�����棬��ȥ�����з����ı����ַ�
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

			//split, join����ȥ��Ӣ���ַ�
		auto noEngPuncList = article.split(" \t\n~,./-=[]{};:'!?@#$%^&*()_+<>`|\\qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890", false);
		article = CharString::join(noEngPuncList);

		//MyList<CharString> noEngPuncList;


		//�������ƥ��ķִ��㷨
		int currPos = article.length() - 1;
		while (currPos >= m_lengthOfACharacter) {
			//bool singleWord = true;
			//����ɨ��
			for (int i = m_maxLengthOfWords; i >= m_lengthOfACharacter; i = i - m_lengthOfACharacter) {
				CharString wordPossible = article.substring(currPos - i + 1, i);//��ȡ��Ҫɨ��Ĵ�
				if (m_hashTable.has(wordPossible)) {//���������
					ret.add(wordPossible);
					//currPos -= i;
					//singleWord = false;
					//break;
				}
			}
			currPos -= m_lengthOfACharacter;
			/*
			if (singleWord) {//���ֳɴ�
				CharString singleWordPossible = article.substring(currPos - m_lengthOfACharacter + 1, m_lengthOfACharacter);
				ret.add(singleWordPossible);
				currPos -= m_lengthOfACharacter;
			}
			*/
			
		}

		if (!currPos) {
			ret.add(article.substring(0, m_lengthOfACharacter));
		}


		//���˵����е����ı��
		for (int i = 0; i < CHINESEPUNCNUM; i++) {
			auto search = ret.search(Chinesepuncs[i]);
			while (search) {
				ret.remove(search);
				search = ret.search(Chinesepuncs[i]);
			}
		}
	}

	//��info�н����������ȴ���
	void getInfoWords(const CharString& info, MyList<CharString>& wordlist) {
		const auto& noEngList = info.split("1234567890 \t\n~,./:-=[]{};'!?@#$%^&*()_+<>`|\\qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM", false);
		auto curr = noEngList.headPtr();
		while (curr)
		{
			auto text = curr->elem();
			auto index = text.indexOf("��", 1);
			while (index.length())
			{
				auto pos = index.headPtr()->elem();
				wordlist.add(text.substring(0, pos));
				text.cutstring(0, pos+2);
				index = text.indexOf("��", 1);
			}
			wordlist.add(text);
			curr = curr->next();
		}
	}
};

