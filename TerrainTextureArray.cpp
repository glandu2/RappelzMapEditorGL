#include "TerrainTextureArray.h"
#include "GL/glew.h"
#include <stdio.h>
#include <string.h>
#include <string>

extern "C" {
#include "SOIL/src/SOIL.h"
#include "SOIL/src/image_DXT.h"
#include "SOIL/src/image_helper.h"
}

#define BLENDMAP_SIZE (64*6*4)

TerrainTextureArray::TerrainTextureArray() : width(0), height(0) {
}

TerrainTextureArray::~TerrainTextureArray() {
}

struct ImageInfo {
	unsigned char* data;
	int width;
	int height;
	int channel;
};

bool TerrainTextureArray::loadDDS(const std::vector<std::string>& filenames)
{
	format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
	width = 1;
	height = 1;

	std::vector<ImageInfo> images;

	imgData.clear();

	for(size_t i = 0; i < filenames.size(); i++) {
		std::string fullName = "textures/" + filenames[i];
		ImageInfo imgInfo;

		imgInfo.data = SOIL_load_image(fullName.c_str(), &imgInfo.width, &imgInfo.height, &imgInfo.channel, 0);

		if(!imgInfo.data) {
			printf("Faild to load texture %s\n", fullName.c_str());
			continue;
		}

		if(width < imgInfo.width)
			width = imgInfo.width;

		if(height < imgInfo.height)
			height = imgInfo.height;

		images.push_back(imgInfo);
	}

	numMipMaps = 1;
	for(int mipmap = 1; mipmap < width; mipmap *= 2)
		numMipMaps++;

	for(size_t i = 0; i < images.size(); i++) {
		std::vector<unsigned char> data;
		unsigned char *image;
		unsigned char *dds;
		int ddsSize;
		int imgWidth, imgHeight, imgChannels;


		image = images[i].data;
		imgWidth = images[i].width;
		imgHeight = images[i].height;
		imgChannels = images[i].channel;

		if(height != imgHeight || width != imgWidth) {
			unsigned char *resampled = (unsigned char*) malloc(imgChannels*width*height);
			up_scale_image(image, imgWidth, imgHeight, imgChannels, resampled, width, height);
			SOIL_free_image_data(image);
			image = resampled;
			imgWidth = width;
			imgHeight = height;
		}

		unsigned char *resampled = new unsigned char[imgChannels*imgWidth*imgHeight];
		int mipmapWidth = imgWidth;
		int mipmapHeight = imgHeight;
		int mipmapLevel = 0;
		while((1 << mipmapLevel) <= imgWidth && (1 << mipmapLevel) <= imgHeight)
		{
			if(mipmapLevel > 0) {
				mipmap_image(image, imgWidth, imgHeight, imgChannels, resampled, (1 << mipmapLevel), (1 << mipmapLevel) );
				dds = convert_image_to_DXT1(resampled, mipmapWidth, mipmapHeight, imgChannels, &ddsSize );
			} else {
				dds = convert_image_to_DXT1(image, imgWidth, imgHeight, imgChannels, &ddsSize );
			}

			assert(dds);

			data.insert(data.end(), dds, dds + ddsSize);
			SOIL_free_image_data(dds);

			++mipmapLevel;
			mipmapWidth /= 2;
			mipmapHeight /= 2;
		}
		delete[] resampled;
		SOIL_free_image_data(image);

		imgData.push_back(data);
	}

	return true;
}

bool TerrainTextureArray::loadToGpu() {
	if(glId == 0)
		glGenTextures(1, &glId);

	glBindTexture(GL_TEXTURE_2D_ARRAY, glId);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int layerWidth;
	int layerHeight;
	int size, offset;
	int blockSize;

	if( format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT )
		blockSize = 8;
	else
		blockSize = 16;

	layerWidth = width;
	layerHeight = height;
	for(int i = 0; i < numMipMaps; i++) {
		glTexImage3D(GL_TEXTURE_2D_ARRAY, i, format, layerWidth, layerHeight, imgData.size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		layerWidth  /= 2;
		layerHeight /= 2;
	}

	for(size_t curTex = 0; curTex < imgData.size(); curTex++) {
		layerWidth = width;
		layerHeight = height;
		offset = 0;

		for(int i = 0; i < numMipMaps; ++i)
		{
			if( layerWidth  == 0 ) layerWidth  = 1;
			if( layerHeight == 0 ) layerHeight = 1;

			size = ((layerWidth+3)/4) * ((layerHeight+3)/4) * blockSize;

			assert(offset + size <= (int)imgData[curTex].size());

			glCompressedTexSubImage3D(GL_TEXTURE_2D_ARRAY,
									  i,
									  0, 0, curTex,
									  layerWidth,
									  layerHeight,
									  1,
									  format,
									  size,
									  &imgData[curTex][offset]);

			offset += size;

			// Half the image size for the next mip-map level...
			layerWidth  /= 2;
			layerHeight /= 2;
		}
	}

	return glId != 0;
}

void TerrainTextureArray::unloadFromGpu() {
	if(glId)
		glDeleteTextures(1, &glId);

	glId = 0;
}

void TerrainTextureArray::select() {
	glBindTexture(GL_TEXTURE_2D_ARRAY, glId);
}

void TerrainTextureArray::unselect() {
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}
