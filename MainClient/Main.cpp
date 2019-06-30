
/*���ӿ⣬���ڹ��������������������Ŀ��÷���һ����*/
#pragma comment(lib,"freeglut.lib")
#pragma comment(lib,"glew32.lib")

/*����ͷ�ļ�*/
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>	//ע��glew.h��ǰ�棬Ȼ�����freeglut.h
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
	glutInit(&argc, argv);//��ʼ��GLUT��
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);//���ó�����ʹ�õĴ�������
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL�ͻ���");

	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error:%s", glewGetErrorString(res));
		return 1;
	}

	glutDisplayFunc(RenderSceneCB);//��ʾ�ص���display callback������GLUT��ÿ�θ��´������ݻ��Զ����ô˺���
	glutIdleFunc(RenderSceneCB);//����ʱÿ֡����

	glClearColor(0.0f, 1.0f, 0.0f, 0.0f);

	glutMainLoop();
	return 1;
}