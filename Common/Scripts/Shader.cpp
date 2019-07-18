/************************************************************************/
/*							Create by LYujia				 			*/
/*								2019/7/17		 						*/
/*			Copyright 2019 by LYujia, All Rights Reserved.		 		*/
/************************************************************************/

#include "Shader.h"
#include "Utils.h"


Shader::Shader()
{
	m_pShaderProgram = 0;
}


Shader::~Shader()
{
	if (m_pShaderProgram == 0)
	{
		glDeleteProgram(m_pShaderProgram);
		m_pShaderProgram = 0;
	}
}

void Shader::CreateProgram(ShaderInfo* shaderInfo, GLuint size)
{
	if (!InitProgram())
	{
		return;
	}

	for (GLuint i = 0; i < size; i++)
	{
		if (!AttachShader(shaderInfo[i].shaderType, shaderInfo[i].shaderFilePath))
		{
			return;
		}
	}

	if (!LinkProgram())
	{
		return;
	}
}

GLuint Shader::GetProgram()
{
	return m_pShaderProgram;
}

bool Shader::InitProgram()
{
	m_pShaderProgram = glCreateProgram();
	if (m_pShaderProgram == 0)
	{
		fprintf(stderr, "ShaderProgram����ʧ�ܣ�����");
		return false;
	}

	return true;
}

bool Shader::AttachShader(GLenum shaderType, const char* shaderFilePath)
{
	string shaderText;
	if (!Utils::ReadFile(shaderFilePath, shaderText))
	{
		fprintf(stderr, "��ȡshader�ļ�ʧ�ܣ�");
		return false;
	}

	GLuint shaderObj = glCreateShader(shaderType);//����shader����
	if (shaderObj == 0)
	{
		fprintf(stderr, "����shaderʧ�ܣ�");
		return false;
	}

	const GLchar *p = shaderText.c_str();
	GLint l = strlen(shaderText.c_str());
	glShaderSource(shaderObj, 1, &p, &l);//����shader

	glCompileShader(shaderObj);//����shader

	GLint ret;
	glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &ret);
	if (ret == 0)
	{
		GLchar log[1024];
		glGetShaderInfoLog(shaderObj, sizeof(log), NULL, log);
		fprintf(stderr, "shader%d����ʧ�ܣ�%s", shaderType, log);
		exit(1);
	}

	glAttachShader(m_pShaderProgram, shaderObj);//����shader��program

	return true;
}

bool Shader::LinkProgram()
{
	GLint ret;
	GLchar logInfo[1024];

	glLinkProgram(m_pShaderProgram);//����program
	glGetProgramiv(m_pShaderProgram, GL_LINK_STATUS, &ret);
	if (ret == 0)
	{
		glGetProgramInfoLog(m_pShaderProgram, sizeof(logInfo), NULL, logInfo);
		fprintf(stderr, "����shaderProgramʧ�ܣ�");
		return false;
	}

	glValidateProgram(m_pShaderProgram);//ʹprogram��Ч
	glGetProgramiv(m_pShaderProgram, GL_VALIDATE_STATUS, &ret);
	if (ret == 0)
	{
		glGetProgramInfoLog(m_pShaderProgram, sizeof(logInfo), NULL, logInfo);
		fprintf(stderr, "shaderProgram��Чʧ�ܣ�");
		exit(1);
	}

	return glGetError() == GL_NO_ERROR;
}

void Shader::UseProgram()
{
	glUseProgram(m_pShaderProgram);//ʹ��program
}