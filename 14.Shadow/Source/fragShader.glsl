#version 430
in vec3 varyingNormal;
in vec3 varyingLightDir;
in vec3 varyingVertPos;
in vec3 varyingHalfVector;
in vec4 shadow_coord;
out vec4 fragColor;

// �붥����ɫ����ͬ��ͳһ����
// ������ֱ���ڵ�ǰƬ����ɫ��ʹ��

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

layout (binding=0) uniform sampler2DShadow shTex;

void main(void)
{ 
	// ���滯�������������������Ӿ�����
	vec3 L = normalize(varyingLightDir); 
	vec3 N = normalize(varyingNormal); 
	vec3 V = normalize(-varyingVertPos); 
	vec3 H = normalize(varyingHalfVector); 

	// ���������ƽ�淨������ĽǶ�
	float cosTheta = dot(L,N);
	// ���㷨����N���ƽ��������H֮��ĽǶ�
	float cosPhi = dot(H,N); 

	// ����ADS����(������)�����ϲ��Թ��������ɫ
	// ��ƽ��������H�Ѿ��ڶ�����ɫ���м���������ڹ�դ���н��й���ֵ
	vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz; 
	vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(cosTheta,0.0); 
	vec3 specular = light.specular.xyz * material.specular.xyz * pow(max(cosPhi,0.0), material.shininess*3.0); // ������3.0��Ϊ���ƾ���߹��΢��

	float notlnShadow = textureProj(shTex, shadow_coord);//��Ӱ�����м�����ȷ��ֵ
	if(notlnShadow == 1.0)
	{
		fragColor = vec4((ambient + diffuse + specular), 1.0);
	}
	else
	{
		fragColor = vec4((ambient), 1.0);
	}
}