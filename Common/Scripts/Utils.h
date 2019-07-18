/************************************************************************/
/*							Create by LYujia				 			*/
/*								2019/7/17		 						*/
/*			Copyright 2019 by LYujia, All Rights Reserved.		 		*/
/************************************************************************/

#pragma once

/*导入头文件*/
#include <string>
#include <iostream>
#include <fstream>

#define ShowError() _ShowError(__FILE__, __LINE__)

using namespace std;

class Utils
{
public:
	Utils();
	~Utils();

	static void _ShowError(string file, unsigned int linenum);
	static bool ReadFile(const char* filePath, string& file);
};

