#include "TerrainTextureArray.h"
#include "GL/glew.h"
#include <stdio.h>
#include <string.h>
#include <string>

#define BLENDMAP_SIZE (64*6*4)

TerrainTextureArray::TerrainTextureArray() : width(0), height(0) {
}

TerrainTextureArray::~TerrainTextureArray() {
}

#ifndef MAKEFOURCC
	#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
				((unsigned int)(char)(ch0) | ((unsigned int)(char)(ch1) << 8) |   \
				((unsigned int)(char)(ch2) << 16) | ((unsigned int)(char)(ch3) << 24 ))
#endif //defined(MAKEFOURCC)

/*
 * FOURCC codes for DX compressed-texture pixel formats
 */
#define FOURCC_DXT1  (MAKEFOURCC('D','X','T','1'))
#define FOURCC_DXT2  (MAKEFOURCC('D','X','T','2'))
#define FOURCC_DXT3  (MAKEFOURCC('D','X','T','3'))
#define FOURCC_DXT4  (MAKEFOURCC('D','X','T','4'))
#define FOURCC_DXT5  (MAKEFOURCC('D','X','T','5'))

struct DDS_IMAGE_DATA
{
	GLsizei  width;
	GLsizei  height;
	GLint    components;
	GLenum   format;
	int      numMipMaps;
	GLubyte *pixels;
};

struct DDS_PIXELFORMAT {
  unsigned int dwSize;
  unsigned int dwFlags;
  unsigned int dwFourCC;
  unsigned int dwRGBBitCount;
  unsigned int dwRBitMask;
  unsigned int dwGBitMask;
  unsigned int dwBBitMask;
  unsigned int dwABitMask;
};

struct DDS_HEADER {
  unsigned int           dwSize;
  unsigned int           dwFlags;
  unsigned int           dwHeight;
  unsigned int           dwWidth;
  unsigned int           dwLinearSize;
  unsigned int           dwDepth;
  unsigned int           dwMipMapCount;
  unsigned int           dwReserved1[11];
  DDS_PIXELFORMAT ddpfPixelFormat;
  unsigned int           dwCaps;
  unsigned int           dwCaps2;
  unsigned int           dwCaps3;
  unsigned int           dwCaps4;
  unsigned int           dwReserved2;
};

bool loadDDSTextureFile( const char *filename, DDS_IMAGE_DATA* pDDSImageData)
{
	DDS_HEADER ddsd;
	char filecode[4];
	FILE *pFile;
	int factor;
	int bufferSize;
	std::string absoluteDir = "textures/" + std::string(filename);

	// Open the file
	pFile = fopen( absoluteDir.c_str(), "rb" );

	if( pFile == NULL )
	{
		printf("loadDDSTextureFile couldn't find, or failed to load \"%s\"", filename);
		return false;
	}

	// Verify the file is a true .dds file
	fread(filecode, 1, 4, pFile);

	if( strncmp( filecode, "DDS ", 4 ) != 0 )
	{
		printf("The file \"%s\" doesn't appear to be a valid .dds file!", filename);
		fclose(pFile);
		return false;
	}

	// Get the surface descriptor
	fread(&ddsd, sizeof(ddsd), 1, pFile );

	memset(pDDSImageData, 0, sizeof(DDS_IMAGE_DATA));

	//
	// This .dds loader supports the loading of compressed formats DXT1, DXT3
	// and DXT5.
	//

	switch( ddsd.ddpfPixelFormat.dwFourCC )
	{
		case FOURCC_DXT1:
			// DXT1's compression ratio is 8:1
			pDDSImageData->format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			factor = 2;
			break;

		case FOURCC_DXT3:
			// DXT3's compression ratio is 4:1
			pDDSImageData->format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			factor = 4;
			break;

		case FOURCC_DXT5:
			// DXT5's compression ratio is 4:1
			pDDSImageData->format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			factor = 4;
			break;

		default:

			printf("The file \"%s\" doesn't appear to be compressed "
					"using DXT1, DXT3, or DXT5!", filename );
			return false;
	}

	//
	// How big will the buffer need to be to load all of the pixel data
	// including mip-maps?
	//

	if( ddsd.dwLinearSize == 0 )
	{
		printf("dwLinearSize is 0!");
		return false;
	}

	if( ddsd.dwMipMapCount > 1 )
		bufferSize = ddsd.dwLinearSize * factor;
	else
		bufferSize = ddsd.dwLinearSize;

	pDDSImageData->pixels = new unsigned char[bufferSize];

	fread(pDDSImageData->pixels, 1, bufferSize, pFile);

	// Close the file
	fclose(pFile);

	pDDSImageData->width      = ddsd.dwWidth;
	pDDSImageData->height     = ddsd.dwHeight;
	pDDSImageData->numMipMaps = ddsd.dwMipMapCount;

	if( ddsd.ddpfPixelFormat.dwFourCC == FOURCC_DXT1)
		pDDSImageData->components = 3;
	else
		pDDSImageData->components = 4;

	return pDDSImageData;
}

bool TerrainTextureArray::loadDDS(const std::vector<const char *>& filenames)
{
	DDS_IMAGE_DATA DDSImageData;

	for(size_t i = 0; i < filenames.size(); i++) {
		if(!loadDDSTextureFile(filenames[i], &DDSImageData))
			return false;

		if(imgData.size() == 0) {
			height = DDSImageData.height;
			width  = DDSImageData.width;
			components = DDSImageData.components;
			format = DDSImageData.format;
			numMipMaps = DDSImageData.numMipMaps;
		}

		imgData.push_back(DDSImageData.pixels);
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

	//glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, format, width, height, imgData.size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	//glCompressedTexImage3D(GL_TEXTURE_2D_ARRAY, )

	layerWidth = width;
	layerHeight = height;
	for(int i = 0; i < numMipMaps; i++) {
		glTexImage3D(GL_TEXTURE_2D_ARRAY, i, format, layerWidth, layerHeight, imgData.size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		layerWidth  /= 2;
		layerHeight /= 2;
	}

	while(glGetError() != GL_NO_ERROR);

	for(size_t curTex = 0; curTex < imgData.size(); curTex++) {
		layerWidth = width;
		layerHeight = height;
		offset = 0;

		for( int i = 0; i < numMipMaps; ++i )
		{
			if( layerWidth  == 0 ) layerWidth  = 1;
			if( layerHeight == 0 ) layerHeight = 1;

			size = ((layerWidth+3)/4) * ((layerHeight+3)/4) * blockSize;

			glCompressedTexSubImage3D(GL_TEXTURE_2D_ARRAY,
									  i,
									  0, 0, curTex,
									  layerWidth,
									  layerHeight,
									  1,
									  format,
									  size,
									  imgData[curTex] + offset );

			if(glGetError() != GL_NO_ERROR) {
				printf("\n");
			}

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
