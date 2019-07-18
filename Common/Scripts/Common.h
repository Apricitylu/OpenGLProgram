/************************************************************************/
/*							Create by LYujia				 			*/
/*								2019/7/17		 						*/
/*			Copyright 2019 by LYujia, All Rights Reserved.		 		*/
/************************************************************************/

#pragma once

/*链接库，跟在工程属性里设置链接器的库用法是一样的*/
#pragma comment(lib,"freeglut.lib")
#pragma comment(lib,"glew32.lib")

/*导入头文件*/
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>	//注意glew.h在前面，然后才是freeglut.h
#include <GL/freeglut.h>
#include <iostream>

using namespace std;