#version 430

uniform mat4 mvp_matrix;

//����ָ���Ӷ�����ɫ�����ݸ�������ɫ�����Լ����������������ɫ������ÿ�����������Ķ�����
layout (vertices = 1) out;

void main(void)
{ 
	//��㼶��
	gl_TessLevelOuter[0] = 6;
	gl_TessLevelOuter[1] = 6;
	gl_TessLevelOuter[2] = 6;
	gl_TessLevelOuter[3] = 6;
	//�ڲ㼶��
	gl_TessLevelInner[0] = 6;
	gl_TessLevelInner[1] = 12;
}