/************************************************************************/
/*								主函数文件								*/
/*							Create by LYujia				 			*/
/*								2019/7/17		 						*/
/*			Copyright 2019 by LYujia, All Rights Reserved.		 		*/
/************************************************************************/

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

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); //清除颜色缓存并重设为当前的清除值

	glFlush();//强制之前的OpenGL命令立即执行
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);//初始化GLUT库
	glutInitDisplayMode(GLUT_RGBA);//设置程序所使用的窗口类型
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("HelloWindow");

	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error:%s", glewGetErrorString(res));
		return 1;
	}

	glutDisplayFunc(display);//显示回调，即GLUT在每次更新窗口内容会自动调用此函数

	glClearColor(0.0f, 1.0f, 0.0f, 0.0f);//设置当前使用的清除颜色，若不设置OpenGL默认为黑色

	glutMainLoop();

	return 1;
}