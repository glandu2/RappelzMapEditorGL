#include "Model.h"
#include <IBtrfParser.h>
#include <ITmlFile.h>
#include <ITmlBlock.h>
#include <IBtrfRootBlock.h>
#include <IBtrfBlock.h>
#include <stddef.h>

#include "GLStaticMesh.h"
#include "GLTexture.h"
#include "GLProgram.h"

Model::Model() : mesh(0), shaderProgram(0)
{
}

void Model::loadFromNx3(const char* filename) {
	/*unload();

	ITmlFile* tmlfile = createTmlFile();
	tmlfile->loadNx3();
	IBtrfParser* btrfParser = createBtrfParser(tmlfile);
	IBtrfRootBlock* rootBlock;

	printf("Reading nx3 file %s\n", filename);
	rootBlock = btrfParser->readFile(filename);
	if(rootBlock == nullptr) {
		printf("Failed to parse file\n");
		return;
	}

	mesh = new GLMesh;

	IBtrfBlock *meshHeaderBlock, *meshArrayBlock, *newMesh;

	meshHeaderBlock = rootBlock->getBlockByGuid(tmlfile->getTemplateByName("nx3_new_mesh_header")->getTemplateGuid());
	meshArrayBlock = meshHeaderBlock->getBlock(0);

	newMesh = meshArrayBlock->getBlock(0); //only one mesh
	printf("Mesh name: %s\n", newMesh->getBlock(0)->getDataString(0));
	printf("Mesh material id: %d\n", newMesh->getBlock(1)->getDataInt(0));
	printf("Mesh material id: %d\n", newMesh->getBlock(1)->getDataInt(0));

	IBtrfBlock *meshBlockArray, *meshBlock;

	meshBlockArray = newMesh->getBlock(3);
	meshBlock = meshBlockArray->getBlock(0);	//only one block

	printf("Mesh texture id: %d\n", meshBlock->getBlock(0)->getDataInt(0));

	if(meshBlock->getBlock(1)->getElementNumber() == 0) {
			printf("Empty mesh block (no frame), ignoring\n");
			return;
	}

	IBtrfBlock *meshData;

	meshData = meshBlock->getBlock(1)->getBlock(0); //only one frame

	float *vertexArray, *normalArray, *texCoordArray;
	int vertexArrayLen, normalArrayLen, texCoordArrayLen, vertexCount;

	vertexArray = meshData->getBlock(1)->getDataFloatPtr();
	normalArray = meshData->getBlock(2)->getDataFloatPtr();
	texCoordArray = meshData->getBlock(3)->getDataFloatPtr();

	vertexArrayLen = meshData->getBlock(1)->getElementNumber();
	normalArrayLen = meshData->getBlock(2)->getElementNumber();
	texCoordArrayLen = meshData->getBlock(3)->getElementNumber();

	if(vertexArrayLen != normalArrayLen || vertexArrayLen % 3 != 0 || texCoordArrayLen % 2 != 0 || vertexArrayLen / 3 != texCoordArrayLen / 2) {
		printf("Invalid mesh data array sizes: vertexArray: %d, normalArray: %d, texCoordArray: %d\n",
			   vertexArrayLen, normalArrayLen, texCoordArrayLen);
		return;
	}

	vertexCount = vertexArrayLen / 3; //3 floats per vertex

	printf("Mesh with %d vertices\n", vertexCount);

	unsigned short *indexArray = reinterpret_cast<unsigned short*>(meshBlock->getBlock(2)->getDataShortPtr());
	int indexArrayLen = meshBlock->getBlock(2)->getElementNumber();

	vertexAttributes.clear();
	for(int i = 0; i < vertexCount; i++) {
		VertexAttribute vertexAttribute;

		vertexAttribute.pos = glm::vec3(vertexArray[i * 3], vertexArray[i * 3 + 1], vertexArray[i * 3 + 2]);
		vertexAttribute.normal = glm::vec3(normalArray[i * 3], normalArray[i * 3 + 1], normalArray[i * 3 + 2]);
		vertexAttribute.texCoord = glm::vec2(texCoordArray[i * 2], texCoordArray[i * 2 + 1]);

		vertexAttributes.push_back(vertexAttribute);
	}

	indices.reserve(indexArrayLen);
	indices.assign(indexArray, indexArray + indexArrayLen);

	printf("Done loading %s\n", filename);*/
}

void Model::unload() {
	if(mesh) {
		mesh->unloadFromGpu();
		delete mesh;
	}
	for(int i = 0; i < textures.size(); i++) {
		textures[i]->unloadFromGpu();
		delete textures[i];
	}
	textures.clear();

	if(shaderProgram) {
		shaderProgram->unloadFromGpu();
		delete shaderProgram;
	}
}
