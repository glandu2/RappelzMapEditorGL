#include "TerrainBlendMap.h"
#include "GL/glew.h"

TerrainBlendMap::TerrainBlendMap() : width(0), height(0), channels(0) {
}

TerrainBlendMap::~TerrainBlendMap() {
}

bool TerrainBlendMap::loadToGpu() {
	if(blendMapIndexTexture == 0)
		glGenTextures(1, &blendMapIndexTexture);

	glBindTexture(GL_TEXTURE_2D, blendMapIndexTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glTexImage2D(GL_TEXTURE_2D, 0,
				 GL_RGB8, BLENDMAP_SIZE_INDICES, BLENDMAP_SIZE_INDICES, 0,
				 GL_RGB, GL_UNSIGNED_BYTE, &blendMapIndices[0]);

	if(blendMapAlphaTexture == 0)
		glGenTextures(1, &blendMapAlphaTexture);

	glBindTexture(GL_TEXTURE_2D, blendMapAlphaTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glTexImage2D(GL_TEXTURE_2D, 0,
				 GL_RG8, BLENDMAP_SIZE_ALPHA, BLENDMAP_SIZE_ALPHA, 0,
				 GL_RG, GL_UNSIGNED_BYTE, &blendMapAlpha[0]);

	return blendMapIndexTexture != 0 && blendMapAlphaTexture != 0;
}

void TerrainBlendMap::unloadFromGpu() {
	if(blendMapIndexTexture)
		glDeleteTextures(1, &blendMapIndexTexture);

	if(blendMapAlphaTexture)
		glDeleteTextures(1, &blendMapAlphaTexture);

	blendMapIndexTexture = 0;
	blendMapAlphaTexture = 0;
}

void TerrainBlendMap::select() {
	//glBindTexture(GL_TEXTURE_2D, blendMapIndexTexture);
	glBindTexture(GL_TEXTURE_2D, blendMapAlphaTexture);
}

void TerrainBlendMap::unselect() {
	glBindTexture(GL_TEXTURE_2D, 0);
}
