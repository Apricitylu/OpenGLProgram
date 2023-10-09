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

	// ���Ӿ��ռ��д������������ľ�����ǵ��������������ĳ��ȣ���Ϊ��������Ӿ��ռ��е�(0,0,0)λ��
	float dist = length(vertEyeSpacePos.xyz);
	float fogFactor = clamp(((fogEnd - dist) / (fogEnd - fogStart)), 0.0, 1.0);
	color = mix(fogColor, varyingColor, fogFactor);
}