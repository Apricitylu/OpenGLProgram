#version 430

in vec3 varyingNormal;
in vec3 varyingLightDir;
in vec3 varyingVertPos;
in vec3 varyingHalfVector;
// 与Phong着色相同，但添加此输入顶点属性
in vec3 originalVertex;
in vec2 tc;      // 输入插值过的材质坐标
in vec3 varyingTangent;      // 视觉空间顶点切向量

out vec4 fragColor;

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

layout (binding=0) uniform sampler2D samp;     // 纹理
layout (binding=1) uniform sampler2D samp2;    // 法线贴图

vec3 calcNewNormal()
{
    vec3 normal = normalize(varyingNormal);
    vec3 tangent = normalize(varyingTangent);
    tangent = normalize(tangent - dot(tangent, normal) * normal); // 切向量垂直于法向量
    vec3 bitangent = cross(tangent, normal);
	mat3 tbn = mat3(tangent, bitangent, normal);       // 用来变换到相机空间的TBN矩阵
    vec3 retrievedNormal = texture(samp2,tc).xyz;
    retrievedNormal = retrievedNormal * 2.0 - 1.0;     // 从RGB空间转换
    vec3 newNormal = tbn * retrievedNormal;
    newNormal = normalize(newNormal); 
    return newNormal;
}

void main(void)
{ 
	// 正规化光照向量、法向量、视觉向量
	vec3 L = normalize(varyingLightDir); 
	vec3 N = calcNewNormal();; 
	vec3 V = normalize(-varyingVertPos); 
	vec3 H = normalize(varyingHalfVector); 

	// 计算光照与平面法向量间的角度
	float cosTheta = dot(L,N);
	// 计算法向量N与角平分线向量H之间的角度
	float cosPhi = dot(H,N); 

	vec4 texel = texture(samp, tc);

	// 计算ADS分量(按像素)，并合并以构建输出颜色
	// 角平分线向量H已经在顶点着色器中计算过，并在光栅器中进行过插值
	fragColor = globalAmbient + 
    texel * (light.ambient + light.diffuse * max(cosTheta,0.0) 
    + light.specular * pow(max(cosPhi,0.0), material.shininess));
}