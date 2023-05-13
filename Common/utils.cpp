
#include "utils.h"


//��GLSL����ʧ��ʱ����ʾOpenGL��־����
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

//��GLSL����ʧ��ʱ����ʾOpenGL��־����
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

//���OpenGL�����־�����Ƿ���OpenGL����
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

	//���ж�����ɫ������ҪĿ�궼�ǽ����㷢�͸�����
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

	//���ö༶��Զ������ͼ��Mipmapping��
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	// ��������ø������Թ���
	if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) //�Կ��Ƿ�֧��AF
	{
		GLfloat anisoSetting = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting);//��ȡ�������̶�
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting);//Ӧ���ڼ����������
	}

	return textureID;
}

GLuint Utils::loadCubeMap(const char* mapDir) {
	GLuint textureRef;

	// ����6������ͼ���ļ�xp��xn��yp��yn��zp��zn����JPG��ʽͼ��
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

	// ���ٽӷ�
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureRef;
}

float Utils::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

// �ƽ���� �� �����⡢�����䡢���淴��͹���
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

// �������� �� �����⡢�����䡢���淴��͹���
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

// ��ͭ���� �� �����⡢�����䡢���淴��͹���
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
