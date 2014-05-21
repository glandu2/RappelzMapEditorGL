#version 130

uniform mat4x4 projectionMatrix;
uniform mat4x4 cameraMatrix;
//uniform mat4x4 modelMatrix;

in vec3 vertexPosition_modelspace;
out vec2 texCoord;

void main(void)
{
    gl_Position = projectionMatrix * cameraMatrix * /*modelMatrix * */vec4(vertexPosition_modelspace.x, vertexPosition_modelspace.y, vertexPosition_modelspace.z, 1.0);
    texCoord = vec2(vertexPosition_modelspace.x/16128.0, vertexPosition_modelspace.y/16128.0);
}
