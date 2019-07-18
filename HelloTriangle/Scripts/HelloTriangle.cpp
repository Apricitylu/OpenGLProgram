/************************************************************************/
/*							Create by LYujia				 			*/
/*								2019/7/17		 						*/
/*			Copyright 2019 by LYujia, All Rights Reserved.		 		*/
/************************************************************************/

#include "HelloTriangle.h"

void init()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//顶点信息
	Vector3f vertices[6];
	vertices[0] = Vector3f(-0.90f, -0.90f, 0.0f);
	vertices[1] = Vector3f(0.85f, -0.90f, 0.0f);
	vertices[2] = Vector3f(-0.90f, 0.85f, 0.0f);
	vertices[3] = Vector3f(0.90f, -0.85f, 0.0f);
	vertices[4] = Vector3f(0.90f, 0.90f, 0.0f);
	vertices[5] = Vector3f(-0.85f, 0.90f, 0.0f);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//shader信息
	ShaderInfo shaderInfo[2];
	shaderInfo[0].shaderType = GL_VERTEX_SHADER;
	shaderInfo[0].shaderFilePath = "Shaders/triangle.vert";
	shaderInfo[1].shaderType = GL_FRAGMENT_SHADER;
	shaderInfo[1].shaderFilePath = "Shaders/triangle.frag";
	
	shader.CreateProgram(shaderInfo, sizeof(shaderInfo) / sizeof(shaderInfo[0]));
	shader.UseProgram();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); //清除颜色缓存并重设为当前的清除值

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glFlush();//强制之前的OpenGL命令立即执行
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);//初始化GLUT库
	glutInitDisplayMode(GLUT_RGBA);//设置程序所使用的窗口类型
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("HelloTriangle");

	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error:%s", glewGetErrorString(res));
		return 1;
	}

	init();
	glutDisplayFunc(display);//显示回调，即GLUT在每次更新窗口内容会自动调用此函数

	glClearColor(0.0f, 1.0f, 0.0f, 0.0f);//设置当前使用的清除颜色，若不设置OpenGL默认为黑色

	glutMainLoop();

	return 1;
}