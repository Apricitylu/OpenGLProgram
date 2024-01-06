#version 430

layout (location=0) in vec3 position;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

out vec3 originalPosition;               // ԭʼģ�Ͷ��㽫��������������

layout (binding=0) uniform sampler3D s;


void main(void)
{ 
    originalPosition = position;          // ��ԭʼģ�����괫�ݣ�����3D��������
    
    gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);
}