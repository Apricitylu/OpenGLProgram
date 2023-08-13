#version 430


uniform mat4 mvp_matrix;

layout (binding = 0) uniform sampler2D tex_height;

layout (vertices = 4) out;

in vec2 tc[ ];
out vec2 tcs_out[ ];

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

    tcs_out[gl_InvocationID] = tc[gl_InvocationID];
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}