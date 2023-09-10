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

vec4 newPoints[];
vec3 lightDir[], halfVector[], vertPos[];
float sLen = 0.04;     // sLen ��"��峤��"��С�������ĸ߶�

void setOutputValues(int p, vec3 norm)
{
    varyingNormalG = norm;
    varyingLightDirG = lightDir[p];
    varyingVertPosG = vertPos[p];
    varyingHalfVectorG = halfVector[p];
    gl_Position = newPoints[p];

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
    vec4 sp0 = gl_in[0].gl_Position + normalize(vec4(varyingNormal[0], 1.0)) * sLen;
    vec4 sp1 = gl_in[1].gl_Position + normalize(vec4(varyingNormal[1], 1.0)) * sLen;
    vec4 sp2 = gl_in[2].gl_Position + normalize(vec4(varyingNormal[2], 1.0)) * sLen;

    // �������С���������µ�
    newPoints[0] = gl_in[0].gl_Position;
    newPoints[1] = gl_in[1].gl_Position;
    newPoints[2] = gl_in[2].gl_Position;
    newPoints[3] = (sp0 + sp1 + sp2) / 3.0; // ����

    vertPos[0] = varyingVertPos[0];
    vertPos[1] = varyingVertPos[1];
    vertPos[2] = varyingVertPos[2];
    vertPos[3] = (vertPos[0] + varyingNormal[0] * sLen + vertPos[1] + varyingNormal[1] * sLen + vertPos[2] + varyingNormal[2] * sLen) / 3;

    // ����Ӷ��㵽���յķ���
    lightDir[0] = light.position - vertPos[0];
    lightDir[1] = light.position - vertPos[1];
    lightDir[2] = light.position - vertPos[2];
    lightDir[3] = light.position - vertPos[3];

    // ����Ӷ��㵽���յķ���
    halfVector[0] = (lightDir[0] + (-vertPos[0])).xyz;
    halfVector[1] = (lightDir[1] + (-vertPos[1])).xyz;
    halfVector[2] = (lightDir[2] + (-vertPos[2])).xyz;
    halfVector[3] = (lightDir[3] + (-vertPos[3])).xyz;

    // ����3�������Σ������С�������ı���
    makeNewTriangle(0,1);      // ����������Զ�Ǽ���
    makeNewTriangle(1,2);
    makeNewTriangle(2,0);
}