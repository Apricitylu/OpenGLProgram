#version 430
layout (location=0) in vec3 vertPos;
layout (location=1) in vec3 vertNormal;
out vec3 varyingNormal;      // 视觉空间顶点法向量
out vec3 varyingLightDir;    // 指向光源的向量
out vec3 varyingVertPos;     // 视觉空间中的顶点位置
out vec3 varyingHalfVector;  // 角平分线向量 H 作为新增的输出
out vec4 shadow_coord;


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
uniform mat4 shadowMVP2;

void main(void)
{ 
    // 输出顶点位置、光照方向和法向量到光栅器以进行插值
    varyingVertPos = (mv_matrix * vec4(vertPos,1.0)).xyz; 
    varyingLightDir = light.position - varyingVertPos; 
    varyingNormal = (norm_matrix * vec4(vertNormal,1.0)).xyz; 
    varyingHalfVector = (varyingLightDir + (-varyingVertPos)).xyz; 
    shadow_coord = shadowMVP2 * vec4(vertPos, 1.0);
    gl_Position = proj_matrix * mv_matrix * vec4(vertPos,1.0);
}