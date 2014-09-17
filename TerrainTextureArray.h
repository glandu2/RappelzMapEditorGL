#ifndef TERRAINTEXTUREARRAY_H
#define TERRAINTEXTUREARRAY_H

#include "GLObject.h"
#include <vector>
#include <glm/vec4.hpp>
#include <string>

class TerrainTextureArray : public GLObject
{
public:
public:
	TerrainTextureArray();
	virtual ~TerrainTextureArray();

	bool loadDDS(const std::vector<std::string> &filenames);

	virtual bool loadToGpu();
	virtual void unloadFromGpu();
	virtual void select();
	virtual void unselect();

	std::vector< std::vector<unsigned char> > imgData;
	int width;
	int height;
	unsigned int format;
	int numMipMaps;
};

#endif // TERRAINTEXTUREARRAY_H
