#version 130

in vec3 vertexPosition_modelspace;

void main(void)
{
    gl_Position.xyz = vertexPosition_modelspace;
    gl_Position.w = 1.0;
}
