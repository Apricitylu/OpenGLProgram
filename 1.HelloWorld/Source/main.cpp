
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

//当GLSL编译失败时，显示OpenGL日志内容
void printShaderLog(GLuint shader) 
{
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetShaderInfoLog(shader, len, &chWrittn, log);
		cout << "Shader Info Log: " << log << endl;
		free(log);
	}
}

//当GLSL链接失败时，显示OpenGL日志内容
void printProgramLog(int prog) 
{
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetProgramInfoLog(prog, len, &chWrittn, log);
		cout << "Program Info Log: " << log << endl;
		free(log);
	}
}

//检查OpenGL错误标志，即是否发生OpenGL错误
bool checkOpenGLError() 
{
	bool foundError = false;
	int glErr = glGetError();
	while (glErr != GL_NO_ERROR) {
		cout << "glError: " << glErr << endl;
		foundError = true;
		glErr = glGetError();
	}
	return foundError;
}

string readShaderSource(const char* filePath)
{
	string content;
	ifstream fileStream(filePath, ios::in);
	string line = "";
	while (!fileStream.eof())
	{
		getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();

	return content;
}

GLuint createShaderProgram()
{
	GLint vertCompiled;
	GLint fragCompiled;
	GLint linked;

	//所有顶点着色器的主要目标都是将顶点发送给管线
	string vertShaderStr = readShaderSource("Source/vertShader.glsl");
	string fragShaderStr = readShaderSource("Source/fragShader.glsl");

	const char* vshaderSource = vertShaderStr.c_str();
	const char* fshaderSource = fragShaderStr.c_str();

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vShader, 1, &vshaderSource, NULL);
	glShaderSource(fShader, 1, &fshaderSource, NULL);

	glCompileShader(vShader);
	checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1)
	{
		cout << "vextex complication failed" << endl;
		printShaderLog(vShader);
	}

	glCompileShader(fShader);
	checkOpenGLError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1)
	{
		cout << "fragment complication failed" << endl;
		printShaderLog(fShader);
	}

	GLuint vfProgram = glCreateProgram();

	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);

	glLinkProgram(vfProgram);
	checkOpenGLError();
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
	if (linked != 1)
	{
		cout << "linking failed" << endl;
		printProgramLog(vfProgram);
	}

	return vfProgram;
}

void init(GLFWwindow* window) 
{
	renderingProgram = createShaderProgram();

	//即使应用程序完全没有用到任何缓冲区，
	//OpenGL仍然需要在使用着色器的时候至少有一个创建好的VAO，
	//所以这两行用来创建OpenGL要求的VAO。
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window, double currentTime)
{
	//glClearColor(1.0, 0.0, 0.0, 1.0);	//指定了清除背景时用的颜色值
	//glClear(GL_COLOR_BUFFER_BIT);	//实际使用红色对颜色缓冲区进行填充

	glUseProgram(renderingProgram);	//将含有两个已编译着色器的程序载入OpenGL管线阶段
	//glPointSize(30.0f);//设置大小为30像素的点
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//呈现线框模型
	glDrawArrays(GL_TRIANGLES, 0, 3);//启动管线处理过程
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

	//实例化GLFWwindow
	GLFWwindow* window = glfwCreateWindow(600, 600, "HelloWorld", NULL, NULL);
	glfwMakeContextCurrent(window);//将window与当前OpenGL上下文关联起来

	//初始化GLEW库——GLFW窗口默认是双缓冲的
	if (glewInit() != GLEW_OK)
	{
		exit(EXIT_FAILURE);
	}

	//开启垂直同步（Vsync）
	glfwSwapInterval(1);

	//调用一次init()函数
	init(window);

	//重复调用display()函数
	while (!glfwWindowShouldClose(window))
	{
		display(window, glfwGetTime());	//glfwGetTime()，它会返回GLFW初始化之后经过的时间
		glfwSwapBuffers(window);//绘制屏幕
		glfwPollEvents();//处理窗口相关事件（如按键事件）
	}

	//通知GLFW销毁窗口以及终止运行
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);

	return 0;
}