#version 130

out vec4 color;
in vec2 texCoord;

void main() {
    //discard;
    color = vec4(1.0, 0.0, 0.0, 1.0);
    //color = vec4(texCoord.x,texCoord.y, 1 - texCoord.x - texCoord.y, 1.0);
}
