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

layout (triangle_strip, max_vertices=9) out;//��ʱҪ���9������

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

vec3 lightDir[], halfVector[], newPoints[];
float sLen = 0.04;     // sLen ��"��峤��"��С�������ĸ߶�

void setOutputValues(int p, vec3 norm)
{
    varyingNormalG = norm;
    varyingLightDirG = lightDir[p];
    varyingVertPosG = newPoints[p];
    varyingHalfVectorG = halfVector[p];
    gl_Position = proj_matrix * vec4(newPoints[p], 1.0);

    EmitVertex();
}

void makeNewTriangle(int p1, int p2)
{ 
    // Ϊ������������ɱ��淨����
    vec3 c1 = normalize(newPoints[p1].xyz - newPoints[3].xyz);
    vec3 c2 = normalize(newPoints[p2].xyz - newPoints[3].xyz);
    vec3 norm = cross(c1,c2);

    // ���ɲ�����3������
    setOutputValues(p1, norm);
    setOutputValues(p2, norm);
    setOutputValues(3, norm);

    EndPrimitive();
}

void main(void)
{ 
    // �����������ζ������ԭʼ���淨����
    vec3 sp0 = varyingVertPos[0] + varyingNormal[0]*sLen;
    vec3 sp1 = varyingVertPos[1] + varyingNormal[1]*sLen;
    vec3 sp2 = varyingVertPos[2] + varyingNormal[2]*sLen;

    // �������С���������µ�
    newPoints[0] = varyingVertPos[0];
    newPoints[1] = varyingVertPos[1];
    newPoints[2] = varyingVertPos[2];
    newPoints[3] = (sp0 + sp1 + sp2)/3.0; // ����

    // ����Ӷ��㵽���յķ���
    lightDir[0] = light.position - newPoints[0];
    lightDir[1] = light.position - newPoints[1];
    lightDir[2] = light.position - newPoints[2];
    lightDir[3] = light.position - newPoints[3];

    // ����Ӷ��㵽���յķ���
    halfVector[0] = (lightDir[0] + (-newPoints[0])).xyz;
    halfVector[1] = (lightDir[1] + (-newPoints[1])).xyz;
    halfVector[2] = (lightDir[2] + (-newPoints[2])).xyz;
    halfVector[3] = (lightDir[3] + (-newPoints[3])).xyz;

    // ����3�������Σ������С�������ı���
    makeNewTriangle(0,1);      // ����������Զ�Ǽ���
    makeNewTriangle(1,2);
    makeNewTriangle(2,0);
}