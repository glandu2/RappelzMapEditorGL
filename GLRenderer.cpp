#include "GLRenderer.h"
#include "GLMesh.h"
#include <GL/glew.h>

GLRenderer::GLRenderer(GLCamera* camera, GLViewport *viewport) : camera(camera), viewport(viewport) {
}

void GLRenderer::addMesh(GLMesh* mesh) {
	meshes.push_back(mesh);
}

void GLRenderer::render() {
	for(size_t i = 0; i < meshes.size(); i++) {
		GLMesh* mesh = meshes[i];

		mesh->select();

		glDrawElements(GL_TRIANGLES, mesh->getIndicesCount(), GL_UNSIGNED_SHORT, reinterpret_cast<const GLvoid*>(0));
	}
}
