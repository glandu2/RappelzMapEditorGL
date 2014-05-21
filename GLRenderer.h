#ifndef GLRENDERER_H
#define GLRENDERER_H

#include <vector>
#include "glm/mat4x4.hpp"

class GLViewport;
class GLCamera;
class GLMesh;

class GLRenderer
{
public:
	GLRenderer(GLCamera* camera, GLViewport* viewport);

	void addMesh(GLMesh *mesh);

	void render();

private:
	GLCamera* camera;
	GLViewport* viewport;

	std::vector<GLMesh*> meshes;
	glm::mat4 modelMatrix;
};

#endif // GLRENDERER_H
