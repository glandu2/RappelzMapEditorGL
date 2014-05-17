#ifndef GLRENDERER_H
#define GLRENDERER_H

#include <vector>

class GLViewport;
class GLCamera;
class GLMesh;

class GLRenderer
{
public:
	GLRenderer(GLCamera* camera, GLViewport* viewport);

	void addMesh(GLMesh* mesh);

	void render();

private:
	GLCamera* camera;
	GLViewport* viewport;

	std::vector<GLMesh*> meshes;
};

#endif // GLRENDERER_H
