
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

//��GLSL����ʧ��ʱ����ʾOpenGL��־����
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

//��GLSL����ʧ��ʱ����ʾOpenGL��־����
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

//���OpenGL�����־�����Ƿ���OpenGL����
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

	//���ж�����ɫ������ҪĿ�궼�ǽ����㷢�͸�����
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

	//��ʹӦ�ó�����ȫû���õ��κλ�������
	//OpenGL��Ȼ��Ҫ��ʹ����ɫ����ʱ��������һ�������õ�VAO��
	//������������������OpenGLҪ���VAO��
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window, double currentTime)
{
	//glClearColor(1.0, 0.0, 0.0, 1.0);	//ָ�����������ʱ�õ���ɫֵ
	//glClear(GL_COLOR_BUFFER_BIT);	//ʵ��ʹ�ú�ɫ����ɫ�������������

	glUseProgram(renderingProgram);	//�����������ѱ�����ɫ���ĳ�������OpenGL���߽׶�
	//glPointSize(30.0f);//���ô�СΪ30���صĵ�
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//�����߿�ģ��
	glDrawArrays(GL_TRIANGLES, 0, 3);//�������ߴ������
}

int main(int argc, char** argv)
{
	//��ʼ��GLFW��
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//ָ���˻���������OpenGL�汾4.3����
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//ʵ����GLFWwindow
	GLFWwindow* window = glfwCreateWindow(600, 600, "HelloWorld", NULL, NULL);
	glfwMakeContextCurrent(window);//��window�뵱ǰOpenGL�����Ĺ�������

	//��ʼ��GLEW�⡪��GLFW����Ĭ����˫�����
	if (glewInit() != GLEW_OK)
	{
		exit(EXIT_FAILURE);
	}

	//������ֱͬ����Vsync��
	glfwSwapInterval(1);

	//����һ��init()����
	init(window);

	//�ظ�����display()����
	while (!glfwWindowShouldClose(window))
	{
		display(window, glfwGetTime());	//glfwGetTime()�����᷵��GLFW��ʼ��֮�󾭹���ʱ��
		glfwSwapBuffers(window);//������Ļ
		glfwPollEvents();//����������¼����簴���¼���
	}

	//֪ͨGLFW���ٴ����Լ���ֹ����
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);

	return 0;
}