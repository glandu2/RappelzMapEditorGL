#ifndef GLVIEWPORT_H
#define GLVIEWPORT_H

#include <GL/glew.h>
#include <QGLWidget>
#include <glm/mat4x4.hpp>

class GLCamera;
class GLMesh;
class GLRenderer;
class GLProgram;

class GLViewport : public QGLWidget
{
	Q_OBJECT

public:
	explicit GLViewport(QWidget *parent = 0);
	~GLViewport();

	virtual void initializeGL();
	virtual void resizeGL(int width, int height);
	virtual void paintGL();

public slots:
	virtual void updateGL();

protected:
	virtual void keyPressEvent(QKeyEvent *keyEvent);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

private:
	QTimer *updateTimer;
	GLuint VertexArrayID;
	GLuint vertexbuffer;

	GLCamera* camera;
	GLMesh* mesh;
	GLRenderer* renderer;
	GLProgram* program;
	unsigned int cameraMatrixUniform;
	unsigned int projectionMatrixUniform;

	glm::mat4x4 projectionMatrix;

	float cameraYaw;
	float cameraPitch;

	bool moveCamera;
	int lastMouseX;
	int lastMouseY;
	float mouseSensivity;
};

#endif // GLVIEWPORT_H
