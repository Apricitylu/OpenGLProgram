#version 430

uniform mat4 mvp_matrix;

//指示曲面细分器去生成排列在一个大矩形（“四边形”）中顶点的位置
//指定了细分线段（包括内部和外部）具有相等的长度（稍后我们将看到长度不等的细分的应用场景）。
//“ccw”参数指定生成曲面细分网格顶点的缠绕顺序（在当前情况下，是逆时针）。
layout (quads, equal_spacing, ccw) in;

void main (void)
{ 
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;
	gl_Position = mvp_matrix * vec4(u, 0, v, 1);
}