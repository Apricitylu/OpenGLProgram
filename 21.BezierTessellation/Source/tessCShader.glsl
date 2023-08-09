#version 430

in vec2 texCoord[]; 

out vec2 texCoord_TCSout[];  // �Ա�����ʽ�Ӷ�����ɫ�����������������������������ʽ�����գ�Ȼ�󱻷��͸�������ɫ��

uniform mat4 mvp_matrix;

layout (binding = 0) uniform sampler2D tex_color;
layout (vertices = 16) out;     // ÿ��������16�����Ƶ�

void main(void)
{ 
    int TL = 32;                  // ����ϸ�ּ��𶼱�����Ϊ���ֵ
    if (gl_InvocationID == 0)
    { 
        gl_TessLevelOuter[0] = TL; 
        gl_TessLevelOuter[1] = TL;
        gl_TessLevelOuter[2] = TL; 
        gl_TessLevelOuter[3] = TL;
        gl_TessLevelInner[0] = TL; 
        gl_TessLevelInner[1] = TL;
    }

    // ������Ϳ��Ƶ㴫�ݸ�TES
    texCoord_TCSout[gl_InvocationID] = texCoord[gl_InvocationID];

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}