#version 430

out vec4 fragColor;

in vec3 originalPosition;         // 接受原始模型坐标，用作3D纹理坐标
layout (binding=0) uniform sampler3D s;

uniform float d;

void main(void)
{
	fragColor = texture(s, vec3(originalPosition.x, originalPosition.y, d));         // 常量替代了tc.z
}