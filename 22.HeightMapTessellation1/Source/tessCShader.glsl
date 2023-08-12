#version 430

in vec2 texCoord[]; 

out vec2 texCoord_TCSout[];  // 以标量形式从顶点着色器传来的纹理坐标输出，以数组形式被接收，然后被发送给评估着色器

uniform mat4 mvp_matrix;

layout (binding = 0) uniform sampler2D tex_color;
layout (vertices = 1) out;     // 每个补丁有16个控制点

void main(void)
{ 
    int TL = 32;                  // 曲面细分级别都被设置为这个值
    if (gl_InvocationID == 0)     //曲面细分级别只需要指定一次，因此该步骤仅在第0次调用期间完成
    { 
        gl_TessLevelOuter[0] = TL; 
        gl_TessLevelOuter[1] = TL;
        gl_TessLevelOuter[2] = TL; 
        gl_TessLevelOuter[3] = TL;
        gl_TessLevelInner[0] = TL; 
        gl_TessLevelInner[1] = TL;
    }
}