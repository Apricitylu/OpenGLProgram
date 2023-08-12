#version 430

layout (quads, equal_spacing, ccw) in;

uniform mat4 mvp_matrix;

layout (binding = 0) uniform sampler2D tex_color;

out vec2 tes_out;           // �Ա�����ʽ�����������������鱻һ��������

void main (void)
{
	// ������ϸ�����񶥵��[0...1]ӳ�䵽��Ҫ�Ķ���[-0.5...+0.5]
	vec4 tessellatedPoint = vec4(gl_TessCoord.x - 0.5, 0.0, gl_TessCoord.y - 0.5, 1.0);

	// ��ֱ����ת�� Yֵ���Խ�����ϸ�����񶥵�ӳ�䵽��������
	// ���϶���������(0,0)����������������(0,0)
	vec2 tc = vec2(gl_TessCoord.x, 1.0 - gl_TessCoord.y);

	// ͼ���ǻҶ�ͼ�������κ�һ����ɫ������R��G��B����������Ϊ�߶�ƫ����
	tessellatedPoint.y += (texture(tex_color, tc).r) / 40.0;     // ����ɫֵ�ȱ�����СӦ����Yֵ

	// ���߶���ͼ�����ĵ�ת�����Ӿ��ռ�
	gl_Position = mvp_matrix * tessellatedPoint;
	tes_out = tc;
}