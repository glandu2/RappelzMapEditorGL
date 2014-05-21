#ifndef TERRAINBLENDMAP_H
#define TERRAINBLENDMAP_H

#include "GLObject.h"
#include <vector>
#include <glm/vec4.hpp>

#define BLENDMAP_SIZE (64*6*4)

class TerrainBlendMap : public GLObject
{
public:
	struct BlendElement {
		unsigned char texIndices[3];
		unsigned char alpha1 : 4;
		unsigned char alpha2 : 4;
	};

	static_assert(sizeof(BlendElement) == 4, "Wrong struct size");
public:
	TerrainBlendMap();
	virtual ~TerrainBlendMap();

	virtual bool loadToGpu();
	virtual void unloadFromGpu();
	virtual void select();
	virtual void unselect();

	std::vector<BlendElement> imgData;

private:
	int width;
	int height;
	int channels;
};

#endif // TERRAINBLENDMAP_H
