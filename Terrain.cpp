#include "Terrain.h"
#include "GLTexture.h"
#include "GLProgram.h"
#include "TerrainBlendMap.h"
#include "TerrainTextureArray.h"
#include <algorithm>

#include <stdio.h>
#include <string.h>

#define VBO_BUFFER_OFFSET(x) (reinterpret_cast<void*>(x))

#include <GL/glew.h>

Terrain::Terrain() : textureLayers(new TerrainTextureArray), blendMap(new TerrainBlendMap), shaderProgram(new GLProgram)
{
}

Terrain::~Terrain() {

}

struct TextureInfo {
	int index;
	float worldSize;
	const char* name;
};

bool Terrain::loadFromNfm(const char* filename) {
	NFM_HEADER_V22 header;

	needReload = true;

	if(texturesList.size() == 0) {
		if(!loadTexturesList("terraintextureinfo.cfg"))
			return false;
	}

	shaderProgram->loadShaders("vertex.glsl", "fragment.glsl");

	std::vector<unsigned short> usedTextures;
	std::vector<std::string> usedTexturesName;

	vertices.clear();
	indices.clear();
	usedTexturesSize.clear();
	blendMap->blendMapAlpha.clear();
	blendMap->blendMapIndices.clear();

	FILE* file = fopen(filename, "rb");
	if(!file) {
		printf("Cant open file %s\n", filename);
		return false;
	}

	fread(&header, 1, sizeof(header), file);
	if(strcmp(header.szSign, "nFlavor Map")) {
		printf("Not a NFM map\n");
		fclose(file);
		return false;
	}

	if(header.dwVersion != 22)
		printf("Warning: map is version %d\n", header.dwVersion);

	fseek(file, header.dwTerrainSegmentOffset, SEEK_SET);
	fread(&segments, 1, sizeof(segments), file);
	fclose(file);

	for(int segmentY = 0; segmentY < 64; segmentY++) {
		for(int tileY = 0; tileY < 6; tileY++) {
			for(int segmentX = 0; segmentX < 64; segmentX++) {
				for(int tileX = 0; tileX < 6; tileX++) {
					vertices.push_back(glm::vec3(segmentX*6*42 + tileX*42, segmentY*6*42 + tileY*42, segments[segmentY][segmentX].vertices[tileY][tileX].height));
				}
			}
		}
	}

	for(int segmentY = 0; segmentY < 64; segmentY++) {
		for(int segmentX = 0; segmentX < 64; segmentX++) {
			for(int texLayer = 0; texLayer < 3; texLayer++) {
				for(int i = 0; i < texturesList.size(); i++) {
					if(texturesList[i].id == segments[segmentY][segmentX].tile[texLayer] &&
							std::find(usedTextures.begin(), usedTextures.end(), i) == usedTextures.end())
					{
						usedTextures.push_back(i);
					}
				}
			}
		}
	}

	assert(usedTextures.size() < 256 && "Too many used texture in map !");

	for(size_t i = 0; i < usedTextures.size(); i++) {
		usedTexturesName.push_back(texturesList[usedTextures[i]].filename);
		usedTexturesSize.push_back((64.0f*6.0f) / texturesList[usedTextures[i]].tileSize);
	}

	textureLayers->loadDDS(usedTexturesName);

	for(int blendMapY = 0; blendMapY < BLENDMAP_SIZE_INDICES; blendMapY++) {
		for(int blendMapX = 0; blendMapX < BLENDMAP_SIZE_INDICES; blendMapX++) {
			int segmentX = blendMapX;
			int segmentY = blendMapY;

			Segment* segment = &segments[segmentY][segmentX];

			TerrainBlendMap::BlendIndexElement blendIndexElement;
			memset(&blendIndexElement, 0, sizeof(blendIndexElement));


			for(size_t i = 0; i < usedTextures.size(); i++) {
				if(segment->tile[0] == texturesList[usedTextures.at(i)].id) {
					blendIndexElement.texIndices[0] = i;
				}

				if(segment->tile[1] == texturesList[usedTextures.at(i)].id) {
					blendIndexElement.texIndices[1] = i;
				}

				if(segment->tile[2] == texturesList[usedTextures.at(i)].id) {
					blendIndexElement.texIndices[2] = i;
				}
			}
			blendMap->blendMapIndices.push_back(blendIndexElement);
		}
	}


	for(int blendMapY = 0; blendMapY < BLENDMAP_SIZE_ALPHA; blendMapY++) {
		for(int blendMapX = 0; blendMapX < BLENDMAP_SIZE_ALPHA; blendMapX++) {
			int segmentX = blendMapX/(4*6);
			int segmentY = blendMapY/(4*6);

			Segment* segment = &segments[segmentY][segmentX];

			int tileX = (blendMapX/4) % 6;
			int tileY = (blendMapY/4) % 6;
			int posInTileX = blendMapX % 4;
			int posInTileY = blendMapY % 4;

			Vertex* vertex = &segment->vertices[tileY][tileX];

			TerrainBlendMap::BlendAlphaElement blendAlphaElement;
			memset(&blendAlphaElement, 0, sizeof(blendAlphaElement));

			blendAlphaElement.alpha1 = ((vertex->textureBlend[0] >> (2*posInTileX + 8*posInTileY)) & 0x3) * 85;
			blendAlphaElement.alpha2 = ((vertex->textureBlend[1] >> (2*posInTileX + 8*posInTileY)) & 0x3) * 85;

			blendMap->blendMapAlpha.push_back(blendAlphaElement);
		}
	}

	for(int y = 0; y < 64*6-1; y++) {
		for(int x = 0; x < 64*6-1; x++) {
			unsigned int v00, v01, v10, v11;

			v00 = y*64*6 + x;
			v01 = y*64*6 + x+1;
			v10 = (y+1)*64*6 + x;
			v11 = (y+1)*64*6 + x+1;

			indices.push_back(v00);
			indices.push_back(v01);
			indices.push_back(v10);

			indices.push_back(v10);
			indices.push_back(v01);
			indices.push_back(v11);
		}
	}

	return true;
}


char easytolower(char in){
  if(in >= 'A' && in <= 'Z')
	return in - ('Z'-'z');
  return in;
}

bool Terrain::loadTexturesList(const char* terraintextureinfo) {
	FILE* file;
	char buffer[4096];
	char *p;
	int currentDetail = 2;

	file = fopen(terraintextureinfo, "rb");
	if(!file)
		return false;


	texturesList.clear();

	while(fgets(buffer, 4096, file)) {
		p = strpbrk(buffer, "\r\n");
		if(p)
			*p = 0;

		p = buffer;
		while(*p == ' ' || *p == '\t')
			p++;

		if(*p == '\0' || *p == ';')
			continue;

		if(!strncmp(p, "DETAIL=", 7)) {
			p += 7;
			currentDetail = atoi(p);
		} else if(!strncmp(p, "TEXTURE=", 8)) {
			p += 8;
			char *sep = strchr(p, ',');
			if(sep && *sep && *(sep+1)) {
				*sep = 0;
				sep++;

				TextureInfo texInfo;
				texInfo.id = atoi(p);
				texInfo.filename = std::string(sep);
				texInfo.tileSize = currentDetail;

				std::transform(texInfo.filename.begin(), texInfo.filename.end(), texInfo.filename.begin(), &easytolower);

				texturesList.push_back(texInfo);
			}
		}
	}

	return texturesList.size() != 0;
}

bool Terrain::loadToGpu() {
	unloadFromGpu();

	needReload = true;
	// Create the VAO
	if(glId == 0)
		glGenVertexArrays(1, &glId);

	glBindVertexArray(glId);

	// Create the buffer for the vertices atttributes
	if(vertexVboId == 0)
		glGenBuffers(1, &vertexVboId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexVboId);

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, VBO_BUFFER_OFFSET(0));

	if(indexVboId == 0)
		glGenBuffers(1, &indexVboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	shaderProgram->loadToGpu();
	blendMap->loadToGpu();
	textureLayers->loadToGpu();

	return true;
}

void Terrain::unloadFromGpu() {
	textureLayers->unloadFromGpu();
	blendMap->unloadFromGpu();
	shaderProgram->unloadFromGpu();

	if(glId)
		glDeleteVertexArrays(1, &glId);
	glId = 0;

	if(vertexVboId)
		glDeleteBuffers(1, &vertexVboId);
	vertexVboId = 0;

	if(indexVboId)
		glDeleteBuffers(1, &indexVboId);
	indexVboId = 0;
}

void Terrain::select(int batch) {
	if(indices.size() == 0)
		return;

	glUseProgram(shaderProgram->getId());

	glBindVertexArray(glId);

	bindTextureUnit(GL_TEXTURE_2D, 0, blendMap->blendMapIndexTexture, "blendMapIndices");
	bindTextureUnit(GL_TEXTURE_2D, 1, blendMap->blendMapAlphaTexture, "blendMapAlpha");
	bindTextureUnit(GL_TEXTURE_2D_ARRAY, 2, textureLayers->getId(), "terrainTextures");
	glUniform1fv(glGetUniformLocation(shaderProgram->getId(), "terrainTexturesSize"), usedTexturesSize.size(), &usedTexturesSize[0]);

	needReload = false;
}

void Terrain::bindTextureUnit(int textureType, int texUnit, unsigned int texId, const char* uniformName) {
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(textureType, texId);
	unsigned int blendMapUniform = glGetUniformLocation(shaderProgram->getId(), uniformName);
	glUniform1i(blendMapUniform, texUnit);
}

void Terrain::unselect() {
	glBindVertexArray(0);
}
