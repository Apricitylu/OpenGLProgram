#version 430

layout (quads, equal_spacing, ccw) in;

uniform mat4 mvp_matrix;

layout (binding = 0) uniform sampler2D tex_color;

out vec2 tes_out;           // 以标量形式传来的纹理坐标数组被一个个传出

void main (void)
{
	// 将曲面细分网格顶点从[0...1]映射到想要的顶点[-0.5...+0.5]
	vec4 tessellatedPoint = vec4(gl_TessCoord.x - 0.5, 0.0, gl_TessCoord.y - 0.5, 1.0);

	// 垂直“翻转” Y值，以将曲面细分网格顶点映射到纹理坐标
	// 左上顶点坐标是(0,0)，左下纹理坐标是(0,0)
	vec2 tc = vec2(gl_TessCoord.x, 1.0 - gl_TessCoord.y);

	// 图像是灰度图，所以任何一个颜色分量（R、G或B）都可以作为高度偏移量
	tessellatedPoint.y += (texture(tex_color, tc).r) / 40.0;     // 将颜色值等比例缩小应用于Y值

	// 将高度贴图提升的点转换到视觉空间
	gl_Position = mvp_matrix * tessellatedPoint;
	tes_out = tc;
}