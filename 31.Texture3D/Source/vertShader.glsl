#version 430

layout (location=0) in vec3 position;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

out vec3 originalPosition;               // 原始模型顶点将被用于纹理坐标

layout (binding=0) uniform sampler3D s;


void main(void)
{ 
    originalPosition = position;          // 将原始模型坐标传递，用作3D纹理坐标
    
    gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);
}