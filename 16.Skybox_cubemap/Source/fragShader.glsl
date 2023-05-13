#version 430

// noperspective in vec2 tc; //����OpenGL��͸��У��
in vec2 tc;      // �����ֵ���Ĳ�������

out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

layout (binding=0) uniform sampler2D samp;


void main(void)
{ 
	color = texture(samp, tc);
}