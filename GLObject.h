#ifndef GLOBJECT_H
#define GLOBJECT_H

class GLObject {
public:
	GLObject() : glId(0) {}
	virtual ~GLObject() { /*unloadFromGpu(); */}

	virtual unsigned int getId() { if(glId == 0) loadToGpu(); return glId; }

	//Load/Unload from GPU
	virtual bool loadToGpu() = 0;
	virtual void unloadFromGpu() = 0;

protected:
	unsigned int glId;
};

#endif // GLOBJECT_H
