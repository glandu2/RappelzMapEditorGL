#include "TerrainBlendMap.h"
#include "GL/glew.h"

#define BLENDMAP_SIZE (64*6*4)

TerrainBlendMap::TerrainBlendMap() : imgData(0), width(0), height(0), channels(0) {
}

TerrainBlendMap::~TerrainBlendMap() {
}

bool TerrainBlendMap::loadToGpu() {
	if(glId == 0)
		glGenTextures(1, &glId);

	glBindTexture(GL_TEXTURE_2D, glId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_MIPMAP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glTexImage2D(GL_TEXTURE_2D, 0,
				 GL_RGBA8UI, BLENDMAP_SIZE, BLENDMAP_SIZE, 0,
				 GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, &imgData[0]);


	return glId != 0;
}

void TerrainBlendMap::unloadFromGpu() {
	if(glId)
		glDeleteTextures(1, &glId);

	glId = 0;
}

void TerrainBlendMap::select() {
	glBindTexture(GL_TEXTURE_2D, glId);
}

void TerrainBlendMap::unselect() {
	glBindTexture(GL_TEXTURE_2D, 0);
}
