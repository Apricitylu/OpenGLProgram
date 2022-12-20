#version 430

layout (location=0) in vec3 pos;
layout (location=1) in vec2 texCoord;

layout (binding=0) uniform sampler2D samp;     // 顶点，着色器中未使用

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

//noperspective out vec2 tc; //禁用OpenGL的透视校正
out vec2 tc;        // 纹理坐标输出到光栅着色器用于插值


void main(void)
{ 
    gl_Position = proj_matrix * mv_matrix * vec4(pos, 1.0);
    tc = texCoord;
}