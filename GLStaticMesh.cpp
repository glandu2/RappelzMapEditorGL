#include "GLStaticMesh.h"
#include <IBtrfParser.h>
#include <ITmlFile.h>
#include <ITmlBlock.h>
#include <IBtrfRootBlock.h>
#include <IBtrfBlock.h>
#include <stddef.h>

#define VBO_BUFFER_OFFSET(x) (reinterpret_cast<void*>(x))

#include <GL/glew.h>

GLStaticMesh::GLStaticMesh() : renderType(RT_Triangles), vertexVboId(0), indexVboId(0) {
}

void GLStaticMesh::loadFromNx3(const char* filename) {
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

	printf("Done loading %s\n", filename);
}

bool GLStaticMesh::loadToGpu() {
	// Create the VAO
	if(glId == 0)
		glGenVertexArrays(1, &glId);

	glBindVertexArray(glId);

	// Create the buffer for the vertices atttributes
	if(vertexVboId == 0)
		glGenBuffers(1, &vertexVboId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexVboId);

	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexAttribute) * vertexAttributes.size(), &vertexAttributes[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), VBO_BUFFER_OFFSET(offsetof(VertexAttribute, pos)));
/*
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), VBO_BUFFER_OFFSET(offsetof(VertexAttribute, normal)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), VBO_BUFFER_OFFSET(offsetof(VertexAttribute, texCoord)));
*/

	if(indexVboId == 0)
		glGenBuffers(1, &indexVboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	return true;
}

void GLStaticMesh::unloadFromGpu() {
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

void GLStaticMesh::select() {
	glBindVertexArray(glId);
}

void GLStaticMesh::unselect() {
	glBindVertexArray(0);
}
