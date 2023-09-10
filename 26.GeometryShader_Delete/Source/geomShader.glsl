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

layout (triangle_strip, max_vertices=3) out;

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

void main (void)
{
    //���ж��㣬����ÿ3��ͼԪ�еĵ�һ��ͼԪ�Ķ��㱻����֮�⣬�������ݡ�
    if (mod(gl_PrimitiveIDIn, 3) != 0)
    {
        for (int i=0; i<3; i++)
        { 
            gl_Position = gl_in[i].gl_Position + 0.000001;//���Ӹ�ֵû��Ч����
            varyingNormalG = varyingNormal[i];
            varyingLightDirG = varyingLightDir[i];
            varyingHalfVectorG = varyingHalfVector[i];
            varyingVertPosG = varyingVertPos[i];
            EmitVertex();
        }
    }
    EndPrimitive();
}
