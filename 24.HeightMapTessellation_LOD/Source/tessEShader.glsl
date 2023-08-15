#version 430

//从等间距变为分数间距，通过使相邻补丁实例的网格几何体更相似，达成了即使它们的细节级别不同，也可以减少此影响的目的。
layout (quads, fractional_even_spacing) in;

uniform mat4 mvp_matrix;

layout (binding = 0) uniform sampler2D tex_height;

in vec2 tcs_out[];
out vec2 tes_out;

void main (void)
{
	// 将纹理坐标映射到传入的控制点指定的子网格上
	vec2 tc = vec2(tcs_out[0].x + (gl_TessCoord.x) / 64.0, tcs_out[0].y + (1.0 - gl_TessCoord.y) / 64.0);

	// 将细分网格映射到传入的控制点指定的子网格上
	vec4 tessellatedPoint = vec4(gl_in[0].gl_Position.x + gl_TessCoord.x / 64.0, 0.0, gl_in[0].gl_Position.z + gl_TessCoord.y / 64.0, 1.0);

	// 将高度图的高度增加给顶点
	tessellatedPoint.y += (texture(tex_height, tc).r) / 40.0;
	gl_Position = mvp_matrix * tessellatedPoint;
	tes_out = tc;
}