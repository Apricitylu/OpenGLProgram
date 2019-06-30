
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

static void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);//初始化GLUT库
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);//设置程序所使用的窗口类型
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL客户端");

	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error:%s", glewGetErrorString(res));
		return 1;
	}

	glutDisplayFunc(RenderSceneCB);//显示回调（display callback），即GLUT在每次更新窗口内容会自动调用此函数
	glutIdleFunc(RenderSceneCB);//空闲时每帧调用

	glClearColor(0.0f, 1.0f, 0.0f, 0.0f);

	glutMainLoop();
	return 1;
}