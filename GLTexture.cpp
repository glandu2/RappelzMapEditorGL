#include "GLTexture.h"
#include "SOIL/src/SOIL.h"
#include <stdio.h>
#include <GL/glew.h>

GLTexture::GLTexture() : imgData(0), width(0), height(0), channels(0) {
}

GLTexture::~GLTexture() {
	if(imgData)
		SOIL_free_image_data(imgData);
}

bool GLTexture::loadDDS(const char* filename) {
	imgData = SOIL_load_image(filename, &width, &height, &channels, 0);

	if(!imgData) {
		printf("Failed to load file %s: %s\n", filename, SOIL_last_result());
		return false;
	}

	return true;
}

void GLTexture::unload() {
	unloadFromGpu();

	if(imgData) {
		SOIL_free_image_data(imgData);
		imgData = 0;
		width = height = channels = 0;
	}
}

bool GLTexture::loadToGpu() {
	glId = SOIL_create_OGL_texture(imgData, width, height, channels, glId, SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_INVERT_Y);

	return glId != 0;
}

void GLTexture::unloadFromGpu() {
	if(glId)
		glDeleteTextures(1, &glId);

	glId = 0;
}

void GLTexture::select() {
	glBindTexture(GL_TEXTURE_2D, glId);
}

void GLTexture::unselect() {
	glBindTexture(GL_TEXTURE_2D, 0);
}
