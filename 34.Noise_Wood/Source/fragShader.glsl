﻿#version 430
in vec3 varyingNormal;
in vec3 varyingLightDir;
in vec3 varyingVertPos;
in vec3 varyingHalfVector;
out vec4 fragColor;

in vec3 originalPosition;         // 接受原始模型坐标，用作3D纹理坐标
layout (binding=0) uniform sampler3D s;

// 与顶点着色器相同的统一变量
// 但并不直接在当前片段着色器使用

struct PositionalLight
{ 
	vec4 ambient; 
	vec4 diffuse; 
	vec4 specular; 
	vec3 position;
};

struct Material
{ 
	vec4 ambient; 
	vec4 diffuse; 
	vec4 specular; 
	float shininess;
};

uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform Material material;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;

void main(void)
{ 
	// 正规化光照向量、法向量、视觉向量
	vec3 L = normalize(varyingLightDir); 
	vec3 N = normalize(varyingNormal); 
	vec3 V = normalize(-varyingVertPos); 
	vec3 H = normalize(varyingHalfVector); 

	// 计算光照与平面法向量间的角度
	float cosTheta = dot(L,N);
	// 计算法向量N与角平分线向量H之间的角度
	float cosPhi = dot(H,N); 

	// 计算ADS分量(按像素)，并合并以构建输出颜色
	// 角平分线向量H已经在顶点着色器中计算过，并在光栅器中进行过插值
	vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz; 
	vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(cosTheta,0.0); 
	vec3 specular = light.specular.xyz * material.specular.xyz * pow(max(cosPhi,0.0), material.shininess*3.0); // 最后乘以3.0作为改善镜面高光的微调

	vec4 lightColor = vec4((ambient + diffuse + specular), 1.0);
	vec4 texColor = texture(s, originalPosition/2.0 + 0.5); // 顶点范围为[−1,+1]，纹理坐标范围为[0,1]

	fragColor = 0.3 * texColor + lightColor * 0.7;
}