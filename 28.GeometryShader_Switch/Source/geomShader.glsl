#version 430

layout (triangles) in;

in vec3 varyingNormal[];        // ���Զ�����ɫ��������
in vec3 varyingLightDir[];
in vec3 varyingHalfVector[];
in vec3 varyingVertPos[];

out vec3 varyingNormalG;         // �������դ��ɫ��Ȼ��Ƭ����ɫ��
out vec3 varyingLightDirG;
out vec3 varyingHalfVectorG;
out vec3 varyingVertPosG;

layout (line_strip, max_vertices=2) out;	//�ı����ͼԪ���ͼ�������

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

float sLen = 0.04; 

void main (void)
{
    vec3 op0 = varyingVertPos[0];                                 // ԭʼ�����ζ���
    vec3 op1 = varyingVertPos[1];
    vec3 op2 = varyingVertPos[2];
    vec3 ep0 = varyingVertPos[0] + varyingNormal[0]*sLen;         // ƫ�������ζ���
    vec3 ep1 = varyingVertPos[1] + varyingNormal[1]*sLen;
    vec3 ep2 = varyingVertPos[2] + varyingNormal[2]*sLen;

    // �������С�߶ε��µ�
    vec3 newPoint1 = (op0 + op1 + op2)/3.0;              // ԭʼ����ʼ����
    vec3 newPoint2 = (ep0 + ep1 + ep2)/3.0;              // ������

    gl_Position = proj_matrix * vec4(newPoint1, 1.0);
    varyingVertPosG = newPoint1;
    varyingLightDirG = light.position - newPoint1;
    varyingNormalG = varyingNormal[0];
    EmitVertex();

    gl_Position = proj_matrix * vec4(newPoint2, 1.0);
    varyingVertPosG = newPoint2;
    varyingLightDirG = light.position - newPoint2;
    varyingNormalG = varyingNormal[1];
    EmitVertex();

    EndPrimitive();
}
