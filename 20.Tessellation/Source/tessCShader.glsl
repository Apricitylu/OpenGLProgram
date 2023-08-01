#version 430

uniform mat4 mvp_matrix;

//用来指定从顶点着色器传递给控制着色器（以及“输出”给评估着色器）的每个“补丁”的顶点数
layout (vertices = 1) out;

void main(void)
{ 
	//外层级别
	gl_TessLevelOuter[0] = 6;
	gl_TessLevelOuter[1] = 6;
	gl_TessLevelOuter[2] = 6;
	gl_TessLevelOuter[3] = 6;
	//内层级别
	gl_TessLevelInner[0] = 6;
	gl_TessLevelInner[1] = 12;
}