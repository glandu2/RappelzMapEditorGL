#ifndef GLSTATICMESH_H
#define GLSTATICMESH_H

#include "GLMesh.h"
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>

#include "GL/glew.h"

class GLStaticMesh : public GLMesh
{
public:
	struct VertexAttribute {
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 texCoord;
	};

public:
	GLStaticMesh();

	void loadFromNx3(const char* filename);

	virtual RenderType getRenderType() { return RT_Triangles; }
	virtual IndexSize getIndexSize() { return IS_Int; }
	virtual int getIndicesCount() { return indices.size(); }

	virtual bool loadToGpu();
	virtual void unloadFromGpu();
	virtual void select();
	virtual void unselect();


	bool hasNormals;
	std::vector<VertexAttribute> vertexAttributes;
	std::vector<unsigned short> indices;
	RenderType renderType;

private:
	unsigned int vertexVboId;
	unsigned int indexVboId;
};

#endif // GLSTATICMESH_H
