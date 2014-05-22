#ifndef TERRAINBLENDMAP_H
#define TERRAINBLENDMAP_H

#include "GLObject.h"
#include <vector>
#include <glm/vec4.hpp>

#define BLENDMAP_SIZE_INDICES (64)
#define BLENDMAP_SIZE_ALPHA (64*6*4)

class TerrainBlendMap : public GLObject
{
public:
	struct BlendIndexElement {
		unsigned char texIndices[3];
	};

	struct BlendAlphaElement {
		unsigned char alpha1;
		unsigned char alpha2;
	};

public:
	TerrainBlendMap();
	virtual ~TerrainBlendMap();

	virtual bool loadToGpu();
	virtual void unloadFromGpu();
	virtual void select();
	virtual void unselect();

	std::vector<BlendIndexElement> blendMapIndices;
	std::vector<BlendAlphaElement> blendMapAlpha;

	unsigned int blendMapIndexTexture; //texture used as index array:  no filtering
	unsigned int blendMapAlphaTexture; //texture used for alpha blanding: use filtering

private:
	int width;
	int height;
	int channels;


};

#endif // TERRAINBLENDMAP_H
