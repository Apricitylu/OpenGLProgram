#version 430

layout (location=0) in vec3 position;
layout (location=1) in vec2 texCoord;
layout (location=2) in vec3 vertNormal;
layout (location=3) in vec3 vertTangent;
out vec3 varyingNormal;      // �Ӿ��ռ䶥�㷨����
out vec3 varyingLightDir;    // ָ���Դ������
out vec3 varyingVertPos;     // �Ӿ��ռ��еĶ���λ��
out vec3 varyingHalfVector;  // ��ƽ�������� H ��Ϊ��������
out vec3 originalVertex;    // ��Phong��ɫ��ͬ�������Ӵ������������
out vec2 tc;        // ���������������դ��ɫ�����ڲ�ֵ
out vec3 varyingTangent;      // �Ӿ��ռ䶥��������

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
uniform mat4 norm_matrix;   // �����任������

void main(void)
{ 
    // �������λ�á����շ���ͷ���������դ���Խ��в�ֵ
    varyingVertPos = (mv_matrix * vec4(position,1.0)).xyz; 
    varyingLightDir = light.position - varyingVertPos; 
    varyingNormal = (norm_matrix * vec4(vertNormal,1.0)).xyz; 
    varyingHalfVector = (varyingLightDir + (-varyingVertPos)).xyz; 
    varyingTangent = (norm_matrix * vec4(vertTangent,1.0)).xyz; 

    // ����ԭʼ���㣬�����Խ��в�ֵ
    originalVertex = position;

    gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);

    tc = texCoord;
}