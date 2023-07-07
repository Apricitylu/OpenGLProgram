#version 430

in vec3 varyingNormal;
in vec3 varyingLightDir;
in vec3 varyingVertPos;
in vec3 varyingHalfVector;
// ��Phong��ɫ��ͬ������Ӵ����붥������
in vec3 originalVertex;
in vec2 tc;      // �����ֵ���Ĳ�������

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

layout (binding=0) uniform sampler2D samp;

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

	vec4 texel = texture(samp, tc);

	// ����ADS����(������)�����ϲ��Թ��������ɫ
	// ��ƽ��������H�Ѿ��ڶ�����ɫ���м���������ڹ�դ���н��й���ֵ
	fragColor = globalAmbient + 
    texel * (light.ambient + light.diffuse * max(cosTheta,0.0) 
    + light.specular * pow(max(cosPhi,0.0), material.shininess));
}