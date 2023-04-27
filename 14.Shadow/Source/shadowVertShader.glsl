#version 430
layout (location=0) in vec3 vertPos;
uniform mat4 shadowMVP;
out vec4 varyingColor;

void main(void)
{ 
    gl_Position = shadowMVP * vec4(vertPos, 1.0);
    varyingColor = vec4(vertPos,1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}