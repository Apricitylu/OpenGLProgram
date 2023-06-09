#version 430
in vec3 varyingNormal;
in vec3 varyingLightDir;
in vec3 varyingVertPos;
in vec3 varyingHalfVector;
out vec4 fragColor;
// ��Phong��ɫ��ͬ������Ӵ����붥������
in vec3 originalVertex;


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

void main(void)
{ 
	// ���滯�������������������Ӿ�����
	vec3 L = normalize(varyingLightDir); 
	vec3 N = normalize(varyingNormal); 
	vec3 V = normalize(-varyingVertPos); 
	vec3 H = normalize(varyingHalfVector); 

	// ������´��������Ҵ���ķ�����
	float a = 0.25;  // a ����͹��ĸ߶�
	float b = 100.0; // b ����͹��Ŀ��
	float x = originalVertex.x;
	float y = originalVertex.y;
	float z = originalVertex.z;
	N.x = varyingNormal.x + a*sin(b*x);   // ʹ�����Һ������Ҵ��뷨����
	N.y = varyingNormal.y + a*sin(b*y);
	N.z = varyingNormal.z + a*sin(b*z);
	N = normalize(N);

	// ���������ƽ�淨������ĽǶ�
	float cosTheta = dot(L,N);
	// ���㷨����N���ƽ��������H֮��ĽǶ�
	float cosPhi = dot(H,N); 

	// ����ADS����(������)�����ϲ��Թ��������ɫ
	// ��ƽ��������H�Ѿ��ڶ�����ɫ���м���������ڹ�դ���н��й���ֵ
	vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz; 
	vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(cosTheta,0.0); 
	vec3 specular = light.specular.xyz * material.specular.xyz * pow(max(cosPhi,0.0), material.shininess*3.0); // ������3.0��Ϊ���ƾ���߹��΢��

	fragColor = vec4((ambient + diffuse + specular), 1.0);
}