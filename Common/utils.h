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
	static GLuint loadTexture(const char* texImagePath);

    // �ƽ���� �� �����⡢�����䡢���淴��͹���
    float* goldAmbient();
    float* goldDiffuse();
    float* goldSpecular();
    float goldShininess();

    // �������� �� �����⡢�����䡢���淴��͹���
    float* silverAmbient();
    float* silverDiffuse();
    float* silverSpecular();
    float silverShininess();

    // ��ͭ���� �� �����⡢�����䡢���淴��͹���
    float* bronzeAmbient();
    float* bronzeDiffuse();
    float* bronzeSpecular();
    float bronzeShininess();
};