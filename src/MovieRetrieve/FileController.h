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
	void findFilenames(const char* lpPath, MyList<CharString>& fileList, const char* suffix = "*")
	{
		char szFind[MAX_PATH];
		WIN32_FIND_DATA FindFileData;

		strcpy(szFind, lpPath);
		//strcat(szFind, "/*.*");
		strcat(szFind, "/*.");
		strcat(szFind, suffix);

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
					findFilenames(szFile, fileList, suffix);
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
	void saveStringTo(const CharString& s, std::string filename, bool need_bom = false) {
		std::ofstream out(filename,std::ios::trunc);
		if (need_bom) out << char(0xEF) << char(0xBB) << char(0xBF);
		if (!out.is_open()) {
			return;
		}
		out << s;
		out.close();
	}
};