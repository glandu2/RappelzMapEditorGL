#version 130

out vec4 color;
in vec2 texCoord;

uniform sampler2D blendMapIndices;
uniform sampler2D blendMapAlpha;
uniform sampler2DArray terrainTextures;
uniform float terrainTexturesSize[256];

void main() {
    vec3 blendIndexData = clamp(texture(blendMapIndices, texCoord.st), vec4(0.0), vec4(1.0, 1.0, 1.0, 1.0)).rgb;
    vec2 blendAlphaData = clamp(texture(blendMapAlpha, texCoord.st), vec4(0.0), vec4(1.0, 1.0, 1.0, 1.0)).rg;
    float texIndices[3];
    vec3 texel[3];
    float alphaBlend[2];

    texIndices[0] = blendIndexData.r*255;
    texIndices[1] = blendIndexData.g*255;
    texIndices[2] = blendIndexData.b*255;

    int i;
    for(i = 0; i < 3; ++i)
        texel[i] = texture(terrainTextures, vec3(texCoord.st*terrainTexturesSize[int(texIndices[i]+0.5)], texIndices[i])).xyz;

    /*alphaBlend[0] = clamp((mod(blendAlphaData*255, 16)) / 3.0, 0, 1);
    alphaBlend[1] = clamp((blendAlphaData*255/16) / 3.0, 0, 1);*/

    alphaBlend[0] = blendAlphaData.x;
    alphaBlend[1] = blendAlphaData.y;

    vec3 finalColor;
    finalColor = texel[0] * (1 - alphaBlend[0]) + texel[1] * alphaBlend[0];
    finalColor =  finalColor   * (1 - alphaBlend[1]) + texel[2] * alphaBlend[1];
    color = vec4(finalColor, 1.0);
    //color = vec4(terrainTexturesSize[int(texIndices[0]+0.5)]/384.0, terrainTexturesSize[int(texIndices[1]+0.5)]/384.0, terrainTexturesSize[int(texIndices[2]+0.5)]/384.0, 1.0);
    //color = texture2DArray(terrainTextures, vec3(texCoord.st*64*6/2, 0.0));
    //color = vec4(terrainTexturesSize[int(texIndices[0]+0.5)]/(64*6), texture2D(blendMapIndices, texCoord.st).rg*8, 1.0);
    //color = vec4(texture2D(blendMapIndices, texCoord.st).rgb, 1.0);
    //color = vec4(texture2D(blendMapAlpha, texCoord.st).rgb, 1.0);
    //color = vec4(texCoord.s, texCoord.t, 1 - texCoord.s - texCoord.t, 1.0);
}
