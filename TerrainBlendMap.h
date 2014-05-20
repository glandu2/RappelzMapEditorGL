#ifndef TERRAINBLENDMAP_H
#define TERRAINBLENDMAP_H

#include "GLObject.h"
#include <vector>
#include <glm/vec4.hpp>

class TerrainBlendMap : public GLObject
{
public:
	TerrainBlendMap();
	virtual ~TerrainBlendMap();

	virtual bool loadToGpu();
	virtual void unloadFromGpu();
	virtual void select();
	virtual void unselect();

	std::vector<glm::vec4> imgData;

private:
	int width;
	int height;
	int channels;
};

#endif // TERRAINBLENDMAP_H
