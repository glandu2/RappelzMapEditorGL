#include "GLRenderer.h"
#include "GLMesh.h"
#include <GL/glew.h>
#include "GLViewport.h"
#include "GLCamera.h"

GLRenderer::GLRenderer(GLCamera* camera, GLViewport *viewport) : camera(camera), viewport(viewport) {
}

void GLRenderer::addMesh(GLMesh* mesh) {
	meshes.push_back(mesh);
}

void GLRenderer::render() {
	for(size_t i = 0; i < meshes.size(); i++) {
		GLMesh* mesh = meshes[i];

		mesh->select();

		if(mesh->getIndicesCount() > 0)
			glDrawElements(mesh->getRenderType(), mesh->getIndicesCount(), mesh->getIndexSize(), reinterpret_cast<const GLvoid*>(mesh->getIndicesOffset()));
	}
}
