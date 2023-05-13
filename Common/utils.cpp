
#include "utils.h"


//当GLSL编译失败时，显示OpenGL日志内容
void Utils::printShaderLog(GLuint shader)
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
void Utils::printProgramLog(int prog)
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
bool Utils::checkOpenGLError()
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

string Utils::readShaderSource(const char* filePath)
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

GLuint Utils::createShaderProgram(const char* vp, const char* fp)
{
	GLint vertCompiled;
	GLint fragCompiled;
	GLint linked;

	//所有顶点着色器的主要目标都是将顶点发送给管线
	string vertShaderStr = readShaderSource(vp);
	string fragShaderStr = readShaderSource(fp);

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
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != 1)
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

GLuint Utils::loadTexture(const char* texImagePath)
{
	GLuint textureID;

	textureID = SOIL_load_OGL_texture(texImagePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (textureID == 0)
	{
		cout << "could not find texture file" << texImagePath << endl;
	}

	//启用多级渐远纹理贴图（Mipmapping）
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	// 如果还启用各向异性过滤
	if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) //显卡是否支持AF
	{
		GLfloat anisoSetting = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting);//获取最大采样程度
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting);//应用于激活纹理对象
	}

	return textureID;
}

GLuint Utils::loadCubeMap(const char* mapDir) {
	GLuint textureRef;

	// 假设6个纹理图像文件xp、xn、yp、yn、zp、zn都是JPG格式图像
	string xp = mapDir; xp = xp + "/xp.jpg";
	string xn = mapDir; xn = xn + "/xn.jpg";
	string yp = mapDir; yp = yp + "/yp.jpg";
	string yn = mapDir; yn = yn + "/yn.jpg";
	string zp = mapDir; zp = zp + "/zp.jpg";
	string zn = mapDir; zn = zn + "/zn.jpg";

	textureRef = SOIL_load_OGL_cubemap(xp.c_str(), xn.c_str(), yp.c_str(), yn.c_str(),
		zp.c_str(), zn.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	if (textureRef == 0) cout << "didnt find cube map image file" << endl;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureRef);

	// 减少接缝
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureRef;
}

float Utils::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

// 黄金材质 — 环境光、漫反射、镜面反射和光泽
float* Utils::goldAmbient() 
{
	static float a[4] = { 0.2473f, 0.1995f, 0.0745f, 1 }; 
	return (float*)a;
}
float* Utils::goldDiffuse() 
{
	static float a[4] = { 0.7516f, 0.6065f, 0.2265f, 1 }; 
	return (float*)a;
}
float* Utils::goldSpecular() 
{
	static float a[4] = { 0.6283f, 0.5559f, 0.3661f, 1 }; 
	return (float*)a;
}
float Utils::goldShininess() 
{ 
	return 51.2f; 
}

// 白银材质 — 环境光、漫反射、镜面反射和光泽
float* Utils::silverAmbient() 
{
	static float a[4] = { 0.1923f, 0.1923f, 0.1923f, 1 }; 
	return (float*)a;
}
float* Utils::silverDiffuse()
{
	static float a[4] = { 0.5075f, 0.5075f, 0.5075f, 1 }; 
	return (float*)a;
}
float* Utils::silverSpecular() 
{
	static float a[4] = { 0.5083f, 0.5083f, 0.5083f, 1 }; 
	return (float*)a;
}
float Utils::silverShininess() 
{ 
	return 51.2f; 
}

// 青铜材质 — 环境光、漫反射、镜面反射和光泽
float* Utils::bronzeAmbient() 
{
	static float a[4] = { 0.2125f, 0.1275f, 0.0540f, 1 }; 
	return (float*)a;
}
float* Utils::bronzeDiffuse() 
{
	static float a[4] = { 0.7140f, 0.4284f, 0.1814f, 1 }; 
	return (float*)a;
}
float* Utils::bronzeSpecular() {
	static float a[4] = { 0.3936f, 0.2719f, 0.1667f, 1 }; 
	return (float*)a;
}
float Utils::bronzeShininess() 
{ 
	return 25.6f; 
}
