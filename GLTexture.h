#ifndef TEXTURE_H
#define TEXTURE_H

#include "GLObject.h"

class GLTexture : public GLObject
{
public:
	GLTexture();

	bool loadDDS(const char* filename);

	virtual bool loadToGpu();
	virtual void unloadFromGpu();

private:
	unsigned char* imgData;
	int width;
	int height;
	int channels;
};

#endif // TEXTURE_H
