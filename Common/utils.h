#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <SOIL2/SOIL2.h>

using namespace std;

class Utils
{
public:
	static void printShaderLog(GLuint shader);
	static void printProgramLog(int prog);
	static bool checkOpenGLError();
	static string readShaderSource(const char* filePath);
	static GLuint createShaderProgram(const char* vp, const char* fp);
	static void createShaderProgram2(const char* vp, const char* fp, GLuint& pro);
};