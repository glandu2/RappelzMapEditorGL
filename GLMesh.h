#ifndef GLMESH_H
#define GLMESH_H

#include "GLObject.h"
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>

class GLMesh : public GLObject
{
public:
	struct VertexAttribute {
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 texCoord;
	};

public:
	GLMesh();

	void loadFromNx3(const char* filename);

	int getIndicesCount() { return indices.size(); }

	virtual bool loadToGpu();
	virtual void unloadFromGpu();
	virtual void select();
	virtual void unselect();


	bool hasNormals;
	std::vector<VertexAttribute> vertexAttributes;
	std::vector<unsigned short> indices;

private:
	unsigned int vertexVboId;
	unsigned int indexVboId;
};

#endif // GLMESH_H
