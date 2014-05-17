#ifndef GLPROGRAM_H
#define GLPROGRAM_H

#include "GLObject.h"
#include <string>

class GLProgram : public GLObject
{
public:
	GLProgram();

	bool loadShaders(const char * vertex_file_path, const char * fragment_file_path);

	virtual bool loadToGpu();
	virtual void unloadFromGpu();
	virtual void select();
	virtual void unselect();

private:
	std::string vertexShaderName;
	std::string vertexShader;
	std::string fragmentShaderName;
	std::string fragmentShader;
};

#endif // GLPROGRAM_H
