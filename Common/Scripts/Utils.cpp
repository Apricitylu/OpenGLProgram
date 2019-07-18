/************************************************************************/
/*							Create by LYujia				 			*/
/*								2019/7/17		 						*/
/*			Copyright 2019 by LYujia, All Rights Reserved.		 		*/
/************************************************************************/

#include "Utils.h"

Utils::Utils()
{
}


Utils::~Utils()
{
}

void Utils::_ShowError(string file, unsigned int line)
{
	cout << "error: " << file << ":" << line << endl;
}

bool Utils::ReadFile(const char* filePath, string& file)
{
	ifstream f(filePath);

	bool ret = false;
	if (f.is_open())
	{
		string line;
		while (getline(f, line))
		{
			file.append(line);
			file.append("\n");
		}
		f.close();
		ret = true;
	}
	else
	{
		ShowError();
		ret = false;
	}

	return ret;
}
