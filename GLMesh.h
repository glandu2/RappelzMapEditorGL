#ifndef GLMESH_H
#define GLMESH_H

#include "GLObject.h"
#include "GL/glew.h"

class GLMesh : public GLObject
{
public:
	enum RenderType {
		RT_Points = GL_POINTS,
		RT_LineStrip = GL_LINE_STRIP,
		RT_LineLoop = GL_LINE_LOOP,
		RT_Lines = GL_LINES,
		RT_LineStripAdjacency = GL_LINE_STRIP_ADJACENCY,
		RT_LinesAdjacency = GL_LINES_ADJACENCY,
		RT_TriangleStrip = GL_TRIANGLE_STRIP,
		RT_TriangleFan = GL_TRIANGLE_FAN,
		RT_Triangles = GL_TRIANGLES,
		RT_TriangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY,
		RT_TrianglesAdjacency = GL_TRIANGLES_ADJACENCY,
		RT_Patches = GL_PATCHES
	};

	enum IndexSize {
		IS_Byte = GL_UNSIGNED_BYTE,
		IS_Short = GL_UNSIGNED_SHORT,
		IS_Int = GL_UNSIGNED_INT
	};

public:
	virtual RenderType getRenderType(int batch = 0) = 0;
	virtual IndexSize getIndexSize(int batch = 0) = 0;
	virtual int getIndicesCount(int batch = 0) = 0;
	virtual int getIndicesOffset(int batch = 0) { return 0; }
	virtual int getBatchCount() { return 1; }
	virtual void select(int batch = 0) = 0;
	virtual unsigned int getModelMatrix() { return 0xFFFFFFFF; }
	virtual bool needSelect() { return true; }
};

#endif // GLMESH_H
