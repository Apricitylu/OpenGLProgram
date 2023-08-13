#version 430

uniform mat4 mvp_matrix;

layout (binding = 0) uniform sampler2D tex_height;

out vec2 tc;

void main(void)
{
	// 4����������
	vec2 patchTexCoords[] = vec2[](vec2(0,0), vec2(1,0), vec2(0,1), vec2(1,1));

	// ���ڵ�ǰ���ĸ�ʵ�����������ƫ����
	int x = gl_InstanceID % 64;
	int y = gl_InstanceID / 64;

	// �������걻�����64�������У�����һ����[0..1]����תY������
	tc = vec2((x + patchTexCoords[gl_VertexID].x) / 64.0, (63 - y + patchTexCoords[gl_VertexID].y) / 64.0);

	// ����λ�ú�����������ͬ��ֻ������ȡֵ��Χ��-0.5��+0.5
	gl_Position = vec4(tc.x - 0.5, 0.0, (1.0 - tc.y) - 0.5, 1.0);         // ���ҽ�Y�����귭ת����
}
