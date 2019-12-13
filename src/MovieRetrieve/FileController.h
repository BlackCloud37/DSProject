#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "MyString.h"
#include "MyList.h"
#include <string>
#include <io.h>
#include <direct.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <iterator>
class FileController
{
public:
	//递归搜索文件夹下的所有文件，将文件名保存到fileList
	//参考：https://www.jb51.net/article/134028.htm
	void findFilenames(const char* lpPath, MyList<CharString>& fileList)
	{
		char szFind[MAX_PATH];
		WIN32_FIND_DATA FindFileData;

		strcpy(szFind, lpPath);
		strcat(szFind, "/*.*");

		HANDLE hFind = ::FindFirstFile(szFind, &FindFileData);
		if (INVALID_HANDLE_VALUE == hFind)    return;

		while (true)
		{
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (FindFileData.cFileName[0] != '.')
				{
					char szFile[MAX_PATH];
					strcpy(szFile, lpPath);
					strcat(szFile, "\\");
					strcat(szFile, (char*)(FindFileData.cFileName));
					findFilenames(szFile, fileList);
				}
			}
			else
			{
				fileList.add(FindFileData.cFileName);
			}
			if (!FindNextFile(hFind, &FindFileData))    break;
		}
		FindClose(hFind);
	}
	//保存一个MyString到目标文件
	void saveStringTo(CharString& s, std::string filename) {
		std::ofstream out(filename,std::ios::trunc);
		if (!out.is_open()) {
			return;
		}
		out << s;
		out.close();
	}

};