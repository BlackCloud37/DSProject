#pragma once
#include "MyString.h"
#include "MyStack.h"
#include <iostream>
#include <fstream>
#include <sstream>
#define NODEMAXSIZE 100000
#define SINGLETAGNUM 3
#define FILTERWORDSNUM 7

//标明Node类型
enum ExprNodeType {
	NODE_NONE = 0,
	NODE_BAD,
	NODE_START_TAG,
	NODE_CLOSE_TAG,
	NODE_SINGLE_TAG,
	NODE_CONTENT,
};

//Tag属性键值对
struct Attribute
{
	CharString attrKey;
	CharString attrValue;

	Attribute(const CharString& key, const CharString& val) :attrKey(key), attrValue(val) {

	}
};



//语法节点类
class ExprNode
{
public:
	ExprNodeType type;
	CharString text;

	CharString tagName;
	MyList<Attribute> attributes;

	ExprNode() {
		type = NODE_NONE;
		tagName = "";
		text = "";
	}
	ExprNode(const ExprNode& other) {
		type = other.type;
		text = other.text;
		tagName = other.tagName;
		attributes = other.attributes;
	}
	~ExprNode() {
	}
	void addAttribute(const Attribute& a) {
		attributes.add(a);
	}
	CharString operator[](const CharString& key) {
		const ListNode<Attribute>* curr = attributes.headPtr();
		while (curr) {
			if (curr->elem().attrKey == key) {
				return curr->elem().attrValue;
			}
			curr = curr->next();
		}
		return "";
	}
	
	friend std::ostream& operator<<(std::ostream&, const ExprNode&);
	friend std::ofstream& operator<< (std::ofstream&, const ExprNode&);
};


//解析类
class HtmlParser {
	
	CharString m_currHtml;//当前解析的html
	Stack<ExprNode> m_nodeStack;//储存节点的栈
	int m_tmpPos;//用于解析html时标记当前位置
	const CharString SINGLETAGSET[SINGLETAGNUM] = { "!DOCTYPE", "meta", "link" };
	const CharString FILTERWORDS[FILTERWORDSNUM] = { "IMDb", "tt", u8"官方网站", "com", "www", "net", "org"};
	
	
	
	/*
		输入一个节点，对其内容进行解析
		"<text>","<text",">","text"
	*/
	void parseNode(ExprNode& node) {
		const CharString& text = node.text;
		if (text[0] == '<' && text[text.length() - 1] == '>') {//形如"<text>", 是一个Tag

			const MyList<CharString>& splited = text.split(" <>\n\t", false);
			const ListNode<CharString>* head = splited.headPtr();

			if (head->elem()[0] == '/') {//close tag
				node.type = NODE_CLOSE_TAG;
				node.tagName = head->elem().substring(1);//remove '/'
			}
			else {//start tag or single tag
				node.tagName = head->elem();
				node.type = getNodeTypeByText(node.tagName);
				ListNode<CharString>* curr = head->next();
				while (curr) {
					auto keyAndValue = curr->elem().split("=\"\'", false, 2);
					if (keyAndValue.length() != 2) {
						curr = curr->next();
						continue;
					}

					node.addAttribute(Attribute(keyAndValue.headPtr()->elem(), keyAndValue.tail()));
					curr = curr->next();
				}
			}
		}
		else if (text[0] == '<' && text[text.length() - 1] != '>') {//形如"<text"，是个错误标签
			node.type = NODE_BAD;
		}
		else if (text[0] == '>') {//形如">" 同上
			assert(text.length() == 1);
			node.type = NODE_BAD;
		}
		else if (text[0] != '<') {//形如"text"，是一个content节点
			if (text.split(" \n\t", false, 1).length())
				node.type = NODE_CONTENT;
		}
	}


	//从当前的html文本中获取下一个Node
	ExprNode getNextNode() {
		int charCount = 0;
		ExprNode node;
		
		while (m_currHtml[m_tmpPos] != '<' && m_currHtml[m_tmpPos] != '>' && m_tmpPos < m_currHtml.length()) {//go and find the first '<' or '>'
			m_tmpPos++;
			charCount++;
		}

		if (charCount != 0) {//it's a content node "xxxxxx", cut the string before '<' or '>' as the text of content
			node.text = m_currHtml.substring(m_tmpPos - charCount, charCount);
			return node;
		}
		else {//it may be a tag node
			if (m_currHtml[m_tmpPos] == '>') {//bad node has a form of ">"
				m_tmpPos++;
				charCount++;
				node.text = m_currHtml.substring(m_tmpPos - charCount, charCount);
				return node;
			}
			else {
				//skip the current '<'
				m_tmpPos++;
				charCount++;

				//find another '<' or '>'
				while (m_currHtml[m_tmpPos] != '<' && m_currHtml[m_tmpPos] != '>') {
					m_tmpPos++;
					charCount++;
				}

				if (m_currHtml[m_tmpPos] == '<') {//bad node has the form of "<xxxxxxx <", only get the part of "<xxxxx ",but not include the second"<"
					node.text = m_currHtml.substring(m_tmpPos - charCount, charCount);
					return node;
				}
				else {//good node
					m_tmpPos++;
					charCount++;
					node.text = m_currHtml.substring(m_tmpPos - charCount, charCount);
					return node;
				}
			}
		}
	}


	//根据tag的文本判断是开始标签还是单独标签
	ExprNodeType getNodeTypeByText(CharString& tagtext) {
		for (int i = 0; i < SINGLETAGNUM; i++) {
			if (tagtext == SINGLETAGSET[i])
				return NODE_SINGLE_TAG;
		}
		return NODE_START_TAG;
	}

public:
	//读取文件中的html
	void setHtmlByFileName(std::string filename) {
		std::ifstream htmlfile;
		htmlfile.open(filename);
		assert(htmlfile.is_open());
		std::stringstream buf;
		buf << htmlfile.rdbuf();
		m_currHtml = buf.str();
		htmlfile.close();
	}
	//读取文件中的html
	void setHtmlByFileName(const CharString& filenameMyStr) {
		std::string filename;
		filename = filenameMyStr.toStr();
		setHtmlByFileName(filename);
	}
	//读取文本中的html
	void setHtmlByString(const CharString& html) {
		m_currHtml = html;
		m_nodeStack = Stack<ExprNode>();
		m_tmpPos = 0;
	}


	//解析html，将info和summary分别保存
	void extractInfo(CharString& targetInfo, CharString& targetSummary) {
		int length = m_currHtml.length();
		m_tmpPos = 0;
		m_nodeStack.clear();
		ExprNode prevDivtag;//储存上一个Div标签的内容，方便特判
		ExprNode prevSpan;//储存上一个Span标签的内容，方便特判

		
		Stack<CharString> contents;//暂存结束标签与开始标签间的内容
		//解析
		while (m_tmpPos < length) {//如果没有解析完
			//获取下一个Node
			ExprNode node = getNextNode();
			//解析这个node
			parseNode(node);

			//特判，决定入栈，出栈或丢弃
			switch (node.type) {
			case NODE_SINGLE_TAG: {
				break;
			}

			case NODE_START_TAG: {
				m_nodeStack.push(node);

				if (node.tagName == "div")
					prevDivtag = node;
				else if (node.tagName == "span")
					prevSpan = node;
				break;
			}


			case NODE_CLOSE_TAG: {
				//根据标签类型和属性值特判
				if ((node.tagName == "div" && prevDivtag["id"] == "info") ||
					(node.tagName == "span" && prevSpan["property"] == "v:itemreviewed") )//info部分
				{
					Stack<ExprNode> contentsStack;
					while (!m_nodeStack.empty()) {
						ExprNode topNode = m_nodeStack.top();
						if (topNode.tagName == node.tagName && topNode.type == NODE_START_TAG) {
							m_nodeStack.pop();
							break;
						}
						contentsStack.push(topNode);
						m_nodeStack.pop();
					}

					while (!contentsStack.empty()) {
						auto topnode = contentsStack.top();
						if (topnode.type == NODE_CONTENT) {
							bool ifFiltered = false;
							for (int i = 0; i < FILTERWORDSNUM; i++) {
								if (topnode.text.indexOf(FILTERWORDS[i], 1).length())
									ifFiltered = true;
							}
							if (!ifFiltered)
								targetInfo.concat(topnode.text);
						}
						else if(topnode.tagName.indexOf("br",1).length())
						{
							targetInfo.concat("\n");
						}
						contentsStack.pop();
					}
					if (node.tagName == "span" && prevSpan["property"] == "v:itemreviewed")
						targetInfo.concat("\n");
				}
				
				else if (node.tagName == "span" && (prevSpan["property"] == "v:summary" || prevSpan["class"] == "all"))//summary部分
				{
					Stack<ExprNode> contentsStack;
					while (!m_nodeStack.empty()) {
						ExprNode topNode = m_nodeStack.top();
						if (topNode.tagName == node.tagName && topNode.type == NODE_START_TAG) {
							m_nodeStack.pop();
							break;
						}
						contentsStack.push(topNode);
						m_nodeStack.pop();
					}

					while (!contentsStack.empty()) {
						auto topnode = contentsStack.top();
						if (topnode.type == NODE_CONTENT) {
							
							const auto& l = topnode.text.split("\t", false);
							auto curr = l.headPtr();
							while (curr)
							{
								targetSummary.concat(curr->elem());
								curr = curr->next();
							}
						}
						contentsStack.pop();
					}
				}
				
			}


			case NODE_CONTENT: {
				m_nodeStack.push(node);
				break;
			}
			default:
				break;
			}
		}
	}
};