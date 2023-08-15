#version 430


uniform mat4 mvp_matrix;

layout (binding = 0) uniform sampler2D tex_height;

layout (vertices = 4) out;

in vec2 tc[ ];
out vec2 tcs_out[ ];

void main(void)
{ 
    float subdivisions = 16.0;        // 基于高度图中细节密度的可调整的常量
    if (gl_InvocationID == 0)
    { 
        vec4 p0 = mvp_matrix * gl_in[0].gl_Position;       // 屏幕空间中控制点的位置
        vec4 p1 = mvp_matrix * gl_in[1].gl_Position;
        vec4 p2 = mvp_matrix * gl_in[2].gl_Position;
        p0 = p0 / p0.w;
        p1 = p1 / p1.w;
        p2 = p2 / p2.w;
        float width = length(p2.xy - p0.xy) * subdivisions + 1.0;     // 曲面细分网格的感知"宽度"
        float height = length(p1.xy - p0.xy) * subdivisions + 1.0;    // 曲面细分网格的感知"高度"
        gl_TessLevelOuter[0] = height;           // 基于感知的边长设置曲面细分级别
        gl_TessLevelOuter[1] = width;
        gl_TessLevelOuter[2] = height;
        gl_TessLevelOuter[3] = width;
        gl_TessLevelInner[0] = width;
        gl_TessLevelInner[1] = height;
    }
    //  像以前一样将纹理坐标和控制点发送给TES
    tcs_out[gl_InvocationID] = tc[gl_InvocationID];
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}