#version 430

layout (triangles) in;

in vec3 varyingNormal[];        // 来自顶点着色器的输入
in vec3 varyingLightDir[];
in vec3 varyingHalfVector[];
in vec3 varyingVertPos[];

out vec3 varyingNormalG;         // 输出给光栅着色器然后到片段着色器
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
uniform mat4 norm_matrix;   // 用来变换法向量

void main (void)
{
    //所有顶点，除了每3个图元中的第一个图元的顶点被忽略之外，都被传递。
    if (mod(gl_PrimitiveIDIn, 3) != 0)
    {
        for (int i=0; i<3; i++)
        { 
            gl_Position = gl_in[i].gl_Position + 0.000001;//不加个值没有效果？
            varyingNormalG = varyingNormal[i];
            varyingLightDirG = varyingLightDir[i];
            varyingHalfVectorG = varyingHalfVector[i];
            varyingVertPosG = varyingVertPos[i];
            EmitVertex();
        }
    }
    EndPrimitive();
}
