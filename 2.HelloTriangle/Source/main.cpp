
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

void init(GLFWwindow* window) {}

void display(GLFWwindow* window, double currentTime)
{
	glClearColor(1.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

int main(int argc, char** argv)
{
	//初始化GLFW库
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//指定了机器必须与OpenGL版本4.3兼容
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(600, 600, "HelloTriangle", NULL, NULL);//实例化GLFWwindow
	glfwMakeContextCurrent(window);//将window与当前OpenGL上下文关联起来

	//初始化GLEW库
	if (glewInit() != GLEW_OK)
	{
		exit(EXIT_FAILURE);
	}

	//开启垂直同步（Vsync）
	glfwSwapInterval(1);

	//调用一次init()函数
	init(window);

	while (!glfwWindowShouldClose(window))
	{
		display(window, glfwGetTime());	//重复调用display()函数
		glfwSwapBuffers(window);//绘制屏幕
		glfwPollEvents();//处理窗口相关事件（如按键事件）
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}