#version 130

uniform mat4x4 projectionMatrix;
uniform mat4x4 cameraMatrix;

in vec3 vertexPosition_modelspace;

void main(void)
{
    gl_Position = projectionMatrix * cameraMatrix * vec4(vertexPosition_modelspace.x, vertexPosition_modelspace.y, vertexPosition_modelspace.z, 1.0);
}
