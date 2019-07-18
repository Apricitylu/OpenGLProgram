/************************************************************************/
/*								�������ļ�								*/
/*							Create by LYujia				 			*/
/*								2019/7/17		 						*/
/*			Copyright 2019 by LYujia, All Rights Reserved.		 		*/
/************************************************************************/

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

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); //�����ɫ���沢����Ϊ��ǰ�����ֵ

	glFlush();//ǿ��֮ǰ��OpenGL��������ִ��
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);//��ʼ��GLUT��
	glutInitDisplayMode(GLUT_RGBA);//���ó�����ʹ�õĴ�������
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("HelloWindow");

	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error:%s", glewGetErrorString(res));
		return 1;
	}

	glutDisplayFunc(display);//��ʾ�ص�����GLUT��ÿ�θ��´������ݻ��Զ����ô˺���

	glClearColor(0.0f, 1.0f, 0.0f, 0.0f);//���õ�ǰʹ�õ������ɫ����������OpenGLĬ��Ϊ��ɫ

	glutMainLoop();

	return 1;
}