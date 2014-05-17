#ifndef TEXTURE_H
#define TEXTURE_H

#include "GLObject.h"

class GLTexture : public GLObject
{
public:
	GLTexture();
	virtual ~GLTexture();

	bool loadDDS(const char* filename);
	void unload();

	virtual bool loadToGpu();
	virtual void unloadFromGpu();
	virtual void select();
	virtual void unselect();

private:
	unsigned char* imgData;
	int width;
	int height;
	int channels;
};

#endif // TEXTURE_H
