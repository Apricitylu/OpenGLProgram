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
    static float toRadians(float degrees);

    // 黄金材质 — 环境光、漫反射、镜面反射和光泽
    static float* goldAmbient();
    static float* goldDiffuse();
    static float* goldSpecular();
    static float goldShininess();

    // 白银材质 — 环境光、漫反射、镜面反射和光泽
    static float* silverAmbient();
    static float* silverDiffuse();
    static float* silverSpecular();
    static float silverShininess();

    // 青铜材质 — 环境光、漫反射、镜面反射和光泽
    static float* bronzeAmbient();
    static float* bronzeDiffuse();
    static float* bronzeSpecular();
    static float bronzeShininess();
};