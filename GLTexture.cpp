#include "GLTexture.h"
#include "Simple-OpenGL-Image-Library/src/SOIL.h"
#include <stdio.h>
#include <GL/glew.h>

GLTexture::GLTexture()
{
}

bool GLTexture::loadDDS(const char* filename) {
	imgData = SOIL_load_image(filename, &width, &height, &channels, 0);

	if(!imgData) {
		printf("Failed to load file %s: %s\n", filename, SOIL_last_result());
		return false;
	}

	return true;
}

bool GLTexture::loadToGpu() {
	unloadFromGpu();

	glId = SOIL_create_OGL_texture(imgData, width, height, channels, 0, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_INVERT_Y);

	return glId != 0;
}

void GLTexture::unloadFromGpu() {
	if(glId)
		glDeleteTextures(1, &glId);

	glId = 0;
}
