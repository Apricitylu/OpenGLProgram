#version 430

layout (binding=0) uniform sampler2D samp;

in vec3 varyingNormal;
in vec3 varyingVertPos;

out vec4 fragColor;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;

layout (binding = 0) uniform samplerCube tex_map;


void main(void)
{
	vec3 r = -reflect(normalize(-varyingVertPos), normalize(varyingNormal));//计算反射向量
	fragColor = texture(tex_map, r);
}