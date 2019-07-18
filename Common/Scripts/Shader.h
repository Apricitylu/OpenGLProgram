/************************************************************************/
/*							Create by LYujia				 			*/
/*								2019/7/17		 						*/
/*			Copyright 2019 by LYujia, All Rights Reserved.		 		*/
/************************************************************************/

#include "Common.h"

class ShaderInfo
{
public:
	GLenum shaderType;
	const char* shaderFilePath;

	ShaderInfo()
	{
		shaderType = 0;
		shaderFilePath = "";
	}

	ShaderInfo(GLenum type, const char* path)
	{
		shaderType = type;
		shaderFilePath = path;
	}
};

class Shader
{
public:
	Shader();
	~Shader();

	void CreateProgram(ShaderInfo* shaderInfo, GLuint size);
	GLuint GetProgram();

	bool InitProgram();
	bool AttachShader(GLenum shaderType, const char* shaderFilePath);
	bool LinkProgram();
	void UseProgram();

private:
	GLuint m_pShaderProgram;
};

