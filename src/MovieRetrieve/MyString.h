#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <assert.h>
#include <iostream>
#undef UNICODE
#include <Windows.h>
#include "MyList.h"
#define RESTORESIZE 10

class CharString
{
	char* m_data;//字符数组
	int m_length;//数组长度
public:
	CharString() {
		m_length = 0;
		m_data = nullptr;
	}
	CharString(const char& c) {
		m_data = new char[1 + RESTORESIZE]();
		m_data[0] = c;
		m_length = 1;
		m_data[m_length] = '\0';
	}
	CharString(const std::string& str) {
		m_data = new char[str.length()+RESTORESIZE]();
		m_length = str.length();
		for (int i = 0; i < str.length();i++) {
			m_data[i] = str[i];
			if (m_data[i] == '\0')
				m_data[i] = ' ';
		}

		m_data[m_length] = '\0';

	}
	CharString(const char* str) {
		if (!str)
			return;

		m_length = 0;
		while (str[++m_length] != '\0') {
			continue;
		}
		m_data = new char[m_length+RESTORESIZE]();
		for (int i = 0; i < m_length; i++) {
			m_data[i] = str[i];
			if (m_data[i] == '\0')
				m_data[i] = ' ';
		}
		m_data[m_length] = '\0';
	}
	CharString(const CharString& str) {

		m_data = new char[str.length()+RESTORESIZE]();
		m_length = str.length();
		for (int i = 0; i < m_length; i++) {
			m_data[i] = str[i];
		}
		m_data[m_length] = '\0';
	}
	CharString(CharString&& str) {
		m_data = new char[str.length()+RESTORESIZE]();
		m_length = str.length();
		for (int i = 0; i < m_length; i++) {
			m_data[i] = str[i];
		}
		m_data[m_length] = '\0';
	}
	char& operator[](int index) {
		if (index < 0 || index > m_length)
			return m_data[m_length];
		assert(index >= 0 && index <= m_length);
		return m_data[index];
	}
	const char& operator[](int index) const {
		assert(index >= 0 && index <= m_length);
		return m_data[index];
	}
	CharString& operator=(const CharString& other) {

		char* newData = new char[other.length() + RESTORESIZE]();
		
		for (int i = 0; i < other.length(); i++) {
			newData[i] = other[i];
		}

		m_length = other.length();
		newData[m_length] = '\0';
		if (m_data)
			delete[] m_data;

		m_data = newData;
		return *this;
	}
	CharString& operator=(CharString&& other) {
		
		char* newData = new char[other.length() + RESTORESIZE]();
		
		for (int i = 0; i < other.length(); i++) {
			newData[i] = other[i];
		}
		//strcpy_s(newData, other.m_data);
		m_length = other.length();
		newData[m_length] = '\0';

		if (m_data)
			delete[] m_data;
		m_data = newData;
		
		return *this;
	}
	CharString& operator+(const CharString& other) {
		this->concat(other);
		return *this;
	}
	CharString& operator+=(const CharString& other) {
		this->concat(other);
		return *this;
	}
	CharString& operator+(CharString& other) {
		this->concat(other);
		return *this;
	}
	CharString& operator+=(CharString& other) {
		this->concat(other);
		return *this;
	}
	CharString& operator+=(std::string other) {
		this->concat(other);
		return *this;
	}
	CharString& operator+(const char* other) {
		this->concat(other);
		return *this;
	}
	CharString& operator+=(const char* other) {
		this->concat(other);
		return *this;
	}
	bool operator==(const CharString& other) const{
		
		int min = m_length < other.length() ? m_length : other.length();
		for (int i = 0; i < min; i++) {
			if (m_data[i] != other[i]) {
				return false;
			}
		}
		if (m_data[min] != other[min] || m_data[min] != '\0')
			return false;
		return true;
		
		//return strcmp(m_data, other.m_data)==0;
	}
	bool operator<(const CharString& other) const {
		return strcmp(m_data, other.m_data)<0;
	}
	bool operator>(const CharString& other) const {
		return strcmp(m_data, other.m_data)>0;
	}
	~CharString() {
		if (m_data)
			delete[] m_data;
	}

	int length() const {
		return m_length;
	}
	
	void concat(const CharString& other) {
		if (other.length() == 0) {
			return;
		}
		if (!m_data) {
			assert(m_length == 0);
			m_data = new char[other.length() + RESTORESIZE]();
			m_length = other.length();
			for (int i = 0; i < other.length(); i++) {
				m_data[i] = other[i];
			}
			m_data[m_length] = '\0';
		}
		else {
			char* newData = new char[m_length + other.length() + RESTORESIZE]();
			for (int i = 0; i < m_length; i++) {
				newData[i] = m_data[i];
			}
			for (int j = 0; j < other.length(); j++) {
				newData[m_length + j] = other[j];
			}
			m_length += other.length();
			delete[] m_data;
			m_data = newData;
			m_data[m_length] = '\0';
		}

	}

	//不改变原字符串的截取
	CharString substring(int start, int len = -1)  const {
		if (start < 0 || start >= m_length)
			start = 0;
		if (len < 0 || len + start > m_length) {
			len = m_length - start;
		}
		char* retString = new char[len + RESTORESIZE]();
		for (int i = 0; i < len; i++) {
			retString[i] = m_data[start + i];
		}
		retString[len] = '\0';
		CharString ret = CharString(retString);
		delete[] retString;
		return ret;
	}

	//会改变原字符串的剪切
	CharString cutstring(int start, int len = -1){
		if (start < 0 || start >= m_length)
			start = 0;
		if (len < 0 || len + start > m_length) {
			len = m_length - start;
		}
		char* retString = new char[len + RESTORESIZE]();
		for (int i = 0; i < len; i++) {
			retString[i] = m_data[start + i];
		}
		retString[len] = '\0';
		CharString ret = CharString(retString);
		delete[] retString;

		for (int i = start; i < m_length - len; i++) {
			m_data[i] = m_data[i + len];
		}
		for (int i = m_length - len; i < m_length; i++) {
			m_data[i] = '\0';
		}
		m_length = m_length - len;
		return ret;
	}

	//sub为子串内容，howmany为搜索个数，采用KMP算法
	//将所有找到的子串坐标作为链表返回
	MyList<int> indexOf(const CharString& sub, const int howmany = -1) const{
		MyList<int> indexes;
		
		int* next = new int[sub.length()]();
		memset(next, 0, sub.length() * sizeof(int));
		next[0] = -1;
		int i = 0, j = -1;
		while (i < sub.length() - 1) {
			if (j == -1 || sub[i] == sub[j]) {
				i++;
				j++;
				next[i] = j;
			}
			else {
				j = next[j];
			}
		}

		i = 0, j = 0;
		while (i < m_length) {
			if (j == -1 || m_data[i] == sub[j]) {
				
				if (j == sub.length() - 1) {
					indexes.add(i - j);
					if (indexes.length() >= howmany && howmany != -1) {
						delete[] next;
						return indexes;
					}
					j = next[j];
				}
				i++;
				j++;
			}
			else {
				j = next[j];
			}
		}
		delete[] next;
		return indexes;
	}

	/*
	* 如果if_separator_is_successive 为 true，则以separator整个字符串作为分隔符分割
	* 否则，以separator中的每个字符作为分隔符进行分割（类似js的split）
	* 返回分割结果链表
	* howmany决定要分割成多少个，默认-1为全部分割
	*/
	MyList<CharString> split(const CharString& separator = "", const bool if_separator_is_successive = true, const int howmany = -1) const{
		MyList<CharString> splited;
		
		if (!separator.length()) {
			int left = 0, len = 1;
			while ((splited.length() <= howmany || howmany == -1) && left + len <= length()) {
				splited.add(substring(left++, len));
			}
		}
		else if (separator.length() == 1) {
			int left = 0, right = 0;
			const char sep = separator[0];
			
			while ((splited.length() <= howmany || howmany == -1) && right <= length())
			{
				while (m_data[right] != sep && right < length() ) {
					if (m_data[right] < 0) {
						right += 2;
					}
					else {
						right++;
					}
				}
				if (right > left) {
					splited.add(substring(left, right - left));
				}
				right++;
				left = right;
			}
		}
		else {
			if (if_separator_is_successive) {
				const int len = separator.length();
				//const CharString& copystr = CharString(*this);
				const MyList<int>& indexes = indexOf(separator);
				const ListNode<int>* curr = indexes.headPtr();
				int left = 0;
				while (curr) {
					splited.add(substring(left, curr->elem() - left));
					left = curr->elem() + len;
					curr = curr->next();
				}
				if (left < length()) {
					splited.add(substring(left, length() - left));
				}
			}
			else {
				int left = 0, right = 0;

				while ((splited.length() <= howmany || howmany == -1) && right <= length())
				{
					while (separator.indexOf(m_data[right]).length() == 0 && right < length()) {
						if (m_data[right] < 0) {
							right += 2;
						}
						else {
							right++;
						}	
					}

					if (right > left) {
						splited.add(substring(left, right - left));
					}



					right++;
					left = right;
				}
			}

		}
		return splited;
	}

	const char* getDataPtr() const {
		return m_data;
	}
	//转化为string
	std::string toStr() const {
		std::string str(m_data);
		return str;
	}
	const char* toCStr() const {
		return m_data;
	}
	//将UTF8编码转换为GB
	//参考自CSDN:https://blog.csdn.net/huixingshao/article/details/80524070
	static std::string UTF8ToGB(const char* utf8)
	{
		int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
		wchar_t* wstr = new wchar_t[len + 1];
		memset(wstr, 0, len + 1);
		MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
		len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
		char* str = new char[len + 1];
		memset(str, 0, len + 1);
		WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
		if (wstr) delete[] wstr;
		return str;
	}
	     
	 //将l的字符串逐个拼接，并以seg作为分隔符
	 static CharString join(const MyList<CharString>& l, const CharString& seg = "") {
		 CharString ret;
		 if (seg == "") {
			 auto curr = l.headPtr();
			 while (curr) {
				 ret.concat(curr->elem());
				 curr = curr->next();
			 }
		 }
		 else {
			 auto curr = l.headPtr();
			 while (curr) {
				 ret.concat(curr->elem());
				 curr = curr->next();
				 if (curr) {
					 ret.concat(seg);
				 }
			 }
		 }
		 return ret;
	 }

	 
	friend std::ostream& operator<<(std::ostream&,const CharString&);
};

