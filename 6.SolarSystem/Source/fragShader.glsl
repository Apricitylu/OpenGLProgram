#version 430

out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

in vec4 varyingColor;

in vec3 vertEyeSpacePos;
out vec4 fragColor;

void main(void)
{ 
	vec4 fogColor = vec4(0.7, 0.8, 0.9, 1.0);
	float fogStart = 4.0;
	float fogEnd = 10.0;

	// 在视觉空间中从摄像机到顶点的距离就是到这个顶点的向量的长度，因为摄像机在视觉空间中的(0,0,0)位置
	float dist = length(vertEyeSpacePos.xyz);
	float fogFactor = clamp(((fogEnd - dist) / (fogEnd - fogStart)), 0.0, 1.0);
	color = mix(fogColor, varyingColor, fogFactor);
}