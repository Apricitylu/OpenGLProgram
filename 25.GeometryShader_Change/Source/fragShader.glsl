#version 430
in vec3 varyingNormalG;
in vec3 varyingLightDirG;
in vec3 varyingVertPosG;
in vec3 varyingHalfVectorG;
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

void main(void)
{ 
	// 正规化光照向量、法向量、视觉向量
	vec3 L = normalize(varyingLightDirG); 
	vec3 N = normalize(varyingNormalG); 
	vec3 V = normalize(-varyingVertPosG); 
	vec3 H = normalize(varyingHalfVectorG); 

	// 计算光照与平面法向量间的角度
	float cosTheta = dot(L,N);
	// 计算法向量N与角平分线向量H之间的角度
	float cosPhi = dot(H,N); 

	// 计算ADS分量(按像素)，并合并以构建输出颜色
	// 角平分线向量H已经在顶点着色器中计算过，并在光栅器中进行过插值
	vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz; 
	vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(cosTheta,0.0); 
	vec3 specular = light.specular.xyz * material.specular.xyz * pow(max(cosPhi,0.0), material.shininess*3.0); // 最后乘以3.0作为改善镜面高光的微调

	fragColor = vec4((ambient + diffuse + specular), 1.0);
}