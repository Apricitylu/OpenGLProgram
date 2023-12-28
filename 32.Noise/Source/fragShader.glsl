#version 430

out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

in vec3 originalPosition;         // 接受原始模型坐标，用作3D纹理坐标

layout (binding=0) uniform sampler3D s;

void main(void)
{
	color = texture(s, originalPosition/2.0 + 0.5); // 顶点范围为[−1,+1]，纹理坐标范围为[0,1]
}