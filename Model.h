#ifndef MODEL_H
#define MODEL_H

#include <vector>

class GLStaticMesh;
class GLTexture;
class GLProgram;

class Model
{
public:
	Model();

	void loadFromNx3(const char* filename);
	void unload();

protected:
	void loadMesh();

private:
	GLStaticMesh* mesh;
	std::vector<GLTexture*> textures;
	GLProgram* shaderProgram;
};

#endif // MODEL_H
