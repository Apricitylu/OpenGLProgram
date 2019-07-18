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
		fprintf(stderr, "ShaderProgram创建失败！！！");
		return false;
	}

	return true;
}

bool Shader::AttachShader(GLenum shaderType, const char* shaderFilePath)
{
	string shaderText;
	if (!Utils::ReadFile(shaderFilePath, shaderText))
	{
		fprintf(stderr, "读取shader文件失败！");
		return false;
	}

	GLuint shaderObj = glCreateShader(shaderType);//创建shader对象
	if (shaderObj == 0)
	{
		fprintf(stderr, "创建shader失败！");
		return false;
	}

	const GLchar *p = shaderText.c_str();
	GLint l = strlen(shaderText.c_str());
	glShaderSource(shaderObj, 1, &p, &l);//加载shader

	glCompileShader(shaderObj);//编译shader

	GLint ret;
	glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &ret);
	if (ret == 0)
	{
		GLchar log[1024];
		glGetShaderInfoLog(shaderObj, sizeof(log), NULL, log);
		fprintf(stderr, "shader%d编译失败：%s", shaderType, log);
		exit(1);
	}

	glAttachShader(m_pShaderProgram, shaderObj);//链接shader至program

	return true;
}

bool Shader::LinkProgram()
{
	GLint ret;
	GLchar logInfo[1024];

	glLinkProgram(m_pShaderProgram);//链接program
	glGetProgramiv(m_pShaderProgram, GL_LINK_STATUS, &ret);
	if (ret == 0)
	{
		glGetProgramInfoLog(m_pShaderProgram, sizeof(logInfo), NULL, logInfo);
		fprintf(stderr, "链接shaderProgram失败！");
		return false;
	}

	glValidateProgram(m_pShaderProgram);//使program生效
	glGetProgramiv(m_pShaderProgram, GL_VALIDATE_STATUS, &ret);
	if (ret == 0)
	{
		glGetProgramInfoLog(m_pShaderProgram, sizeof(logInfo), NULL, logInfo);
		fprintf(stderr, "shaderProgram生效失败！");
		exit(1);
	}

	return glGetError() == GL_NO_ERROR;
}

void Shader::UseProgram()
{
	glUseProgram(m_pShaderProgram);//使用program
}