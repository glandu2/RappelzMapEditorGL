#ifndef TERRAIN_H
#define TERRAIN_H

#include "GLMesh.h"
#include <vector>
#include "glm/vec3.hpp"
#include <string>

class GLStaticMesh;
class GLTexture;
class GLProgram;
class TerrainBlendMap;
class TerrainTextureArray;

class Terrain : public GLMesh
{
public:
	Terrain();
	~Terrain();

	bool loadFromNfm(const char* filename);
	bool loadTexturesList(const char* terraintextureinfo);

	virtual bool loadToGpu();
	virtual void unloadFromGpu();
	virtual void select(int batch = 0);
	virtual void unselect();


	virtual RenderType getRenderType(int batch = 0) { return RT_Triangles; }
	virtual IndexSize getIndexSize(int batch = 0) { return IS_Int; }
	virtual int getIndicesCount(int batch = 0) { return indices.size(); }

	virtual bool needSelect() { return needReload; }

protected:
	void bindTextureUnit(int textureType, int texUnit, unsigned int texId, const char* uniformName);

private:
#pragma pack(1)
	struct Vertex {
		float height;
		unsigned int textureBlend[2];
		int val[2];
		char val2[3];
	};

	struct Segment {
		int version;
		unsigned short tile[3];
		struct Vertex vertices[6][6];
	};

	struct NFM_HEADER_V22
	{
	  char szSign[16];
	  unsigned int dwVersion;
	  unsigned int dwMapPropertiesOffset;
	  unsigned int dwTerrainSegmentOffset;
	  unsigned int dwPropOffset;
	  unsigned int dwVectorAttrOffset;
	  unsigned int dwWaterOffset;
	  unsigned int dwGrassColonyOffset;
	  unsigned int dwEventAreaOffset;
	  int nTileCountPerSegment;
	  int nSegmentCountPerMap;
	  float fTileLength;
	};

	struct TextureInfo {
		int id;
		int tileSize;
		std::string filename;
	};
#pragma pack(0)

private:
	Segment segments[64][64];

	std::vector<TextureInfo> texturesList;

	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
	TerrainTextureArray* textureLayers;
	std::vector<float> usedTexturesSize;
	TerrainBlendMap* blendMap;
	GLProgram* shaderProgram;

	unsigned int vertexVboId;
	unsigned int indexVboId;

	bool needReload;
};

#endif // TERRAIN_H
