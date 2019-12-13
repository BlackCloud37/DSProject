#pragma once
#include "MyString.h"
#include "MyStack.h"
#include <iostream>
#include <fstream>
#include <sstream>
#define NODEMAXSIZE 100000
#define SINGLETAGNUM 3
#define FILTERWORDSNUM 7

//����Node����
enum ExprNodeType {
	NODE_NONE = 0,
	NODE_BAD,
	NODE_START_TAG,
	NODE_CLOSE_TAG,
	NODE_SINGLE_TAG,
	NODE_CONTENT,
};

//Tag���Լ�ֵ��
struct Attribute
{
	CharString attrKey;
	CharString attrValue;

	Attribute(const CharString& key, const CharString& val) :attrKey(key), attrValue(val) {

	}
};



//�﷨�ڵ���
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


//������
class HtmlParser {
	
	CharString m_currHtml;//��ǰ������html
	Stack<ExprNode> m_nodeStack;//����ڵ��ջ
	int m_tmpPos;//���ڽ���htmlʱ��ǵ�ǰλ��
	const CharString SINGLETAGSET[SINGLETAGNUM] = { "!DOCTYPE", "meta", "link" };
	const CharString FILTERWORDS[FILTERWORDSNUM] = { "IMDb", "tt", u8"�ٷ���վ", "com", "www", "net", "org"};
	
	
	
	/*
		����һ���ڵ㣬�������ݽ��н���
		"<text>","<text",">","text"
	*/
	void parseNode(ExprNode& node) {
		const CharString& text = node.text;
		if (text[0] == '<' && text[text.length() - 1] == '>') {//����"<text>", ��һ��Tag

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
		else if (text[0] == '<' && text[text.length() - 1] != '>') {//����"<text"���Ǹ������ǩ
			node.type = NODE_BAD;
		}
		else if (text[0] == '>') {//����">" ͬ��
			assert(text.length() == 1);
			node.type = NODE_BAD;
		}
		else if (text[0] != '<') {//����"text"����һ��content�ڵ�
			if (text.split(" \n\t", false, 1).length())
				node.type = NODE_CONTENT;
		}
	}


	//�ӵ�ǰ��html�ı��л�ȡ��һ��Node
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


	//����tag���ı��ж��ǿ�ʼ��ǩ���ǵ�����ǩ
	ExprNodeType getNodeTypeByText(CharString& tagtext) {
		for (int i = 0; i < SINGLETAGNUM; i++) {
			if (tagtext == SINGLETAGSET[i])
				return NODE_SINGLE_TAG;
		}
		return NODE_START_TAG;
	}

public:
	//��ȡ�ļ��е�html
	void setHtmlByFileName(std::string filename) {
		std::ifstream htmlfile;
		htmlfile.open(filename);
		assert(htmlfile.is_open());
		std::stringstream buf;
		buf << htmlfile.rdbuf();
		m_currHtml = buf.str();
		htmlfile.close();
	}
	//��ȡ�ļ��е�html
	void setHtmlByFileName(const CharString& filenameMyStr) {
		std::string filename;
		filename = filenameMyStr.toStr();
		setHtmlByFileName(filename);
	}
	//��ȡ�ı��е�html
	void setHtmlByString(const CharString& html) {
		m_currHtml = html;
		m_nodeStack = Stack<ExprNode>();
		m_tmpPos = 0;
	}


	//����html����info��summary�ֱ𱣴�
	void extractInfo(CharString& targetInfo, CharString& targetSummary) {
		int length = m_currHtml.length();
		m_tmpPos = 0;
		m_nodeStack.clear();
		ExprNode prevDivtag;//������һ��Div��ǩ�����ݣ���������
		ExprNode prevSpan;//������һ��Span��ǩ�����ݣ���������

		
		Stack<CharString> contents;//�ݴ������ǩ�뿪ʼ��ǩ�������
		//����
		while (m_tmpPos < length) {//���û�н�����
			//��ȡ��һ��Node
			ExprNode node = getNextNode();
			//�������node
			parseNode(node);

			//���У�������ջ����ջ����
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
				//���ݱ�ǩ���ͺ�����ֵ����
				if ((node.tagName == "div" && prevDivtag["id"] == "info") ||
					(node.tagName == "span" && prevSpan["property"] == "v:itemreviewed") )//info����
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
				
				else if (node.tagName == "span" && (prevSpan["property"] == "v:summary" || prevSpan["class"] == "all"))//summary����
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