#version 130
#extension GL_EXT_texture_array : enable

out vec4 color;
in vec2 texCoord;

uniform sampler2D blendMap;
uniform sampler2DArray terrainTextures;

void main() {
    vec4 blendData = clamp(texture2D(blendMap, texCoord.st), vec4(0.0), vec4(1.0, 1.0, 1.0, 1.0));
    int texIndices[3];
    vec3 texel[3];
    float alphaBlend[2];

    texIndices[0] = int(blendData.x*255 + 0.5);
    texIndices[1] = int(blendData.y*255 + 0.5);
    texIndices[2] = int(blendData.z*255 + 0.5);

    int i;
    for(i = 0; i < 3; ++i)
        texel[i] = texture2DArray(terrainTextures, vec3(texCoord.st*64*6/2, texIndices[i])).xyz;

    alphaBlend[0] = clamp((mod(blendData.a*255, 16)) / 3.0, 0, 1);
    alphaBlend[1] = clamp((blendData.a*255/16) / 3.0, 0, 1);

    vec3 finalColor;
    finalColor = texel[0] * (1 - alphaBlend[0]) + texel[1] * alphaBlend[0];
    finalColor =  finalColor   * (1 - alphaBlend[1]) + texel[2] * alphaBlend[1];
    color = vec4(finalColor, 1.0);
    //color = vec4(alphaBlend[0], alphaBlend[1], 1.0 - alphaBlend[0] - alphaBlend[1], 1.0);
}
