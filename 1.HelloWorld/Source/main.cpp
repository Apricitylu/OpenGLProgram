
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

GLuint createShaderProgram()
{
	//���ж�����ɫ������ҪĿ�궼�ǽ����㷢�͸�����
	const char* vshaderSource =
		"#version 430 \n"
		"void main(void) \n"
		"{ gl_Position = vec4(0.0, 0.0, 0.0, 1.0); }";

	const char* fshaderSource =
		"#version 430 \n"
		"out vec4 color; \n"
		"void main(void) \n"
		"{ \n"
		"	if(gl_FragCoord.x < 300)  \n"
		"		color = vec4(0.0, 0.0, 1.0, 1.0); \n"
		"	else \n"
		"		color = vec4(1.0, 0.0, 0.0, 1.0); \n"
		"}";

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vShader, 1, &vshaderSource, NULL);
	glShaderSource(fShader, 1, &fshaderSource, NULL);

	glCompileShader(vShader);
	glCompileShader(fShader);

	GLuint vfProgram = glCreateProgram();

	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);

	glLinkProgram(vfProgram);

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
	glPointSize(30.0f);//���ô�СΪ30���صĵ�
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//�����߿�ģ��
	glDrawArrays(GL_POINTS, 0, 1);//�������ߴ������
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