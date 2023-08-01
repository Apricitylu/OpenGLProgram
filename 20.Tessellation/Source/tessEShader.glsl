#version 430

uniform mat4 mvp_matrix;

//ָʾ����ϸ����ȥ����������һ������Σ����ı��Ρ����ж����λ��
//ָ����ϸ���߶Σ������ڲ����ⲿ��������ȵĳ��ȣ��Ժ����ǽ��������Ȳ��ȵ�ϸ�ֵ�Ӧ�ó�������
//��ccw������ָ����������ϸ�����񶥵�Ĳ���˳���ڵ�ǰ����£�����ʱ�룩��
layout (quads, equal_spacing, ccw) in;

void main (void)
{ 
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;
	gl_Position = mvp_matrix * vec4(u, 0, v, 1);
}