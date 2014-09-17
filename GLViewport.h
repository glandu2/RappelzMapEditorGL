#ifndef GLVIEWPORT_H
#define GLVIEWPORT_H

#include <GL/glew.h>
#include <QGLWidget>
#include <glm/mat4x4.hpp>
#include <QElapsedTimer>

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

	glm::mat4& getViewportMatrix() { return projectionMatrix; }
	GLRenderer* getRenderer() { return renderer; }

	void setSpeed(float speed) { this->speed = speed; }

public slots:
	virtual void updateGL();
	void setCameraX(float x);
	void setCameraY(float y);
	void setCameraZ(float z);

	void setCameraPitch(float pitch);
	void setCameraYaw(float yaw);

signals:
	void initialized();
	void cameraXChanged(float x);
	void cameraYChanged(float y);
	void cameraZChanged(float z);

	void cameraPitchChanged(float pitch);
	void cameraYawChanged(float yaw);

protected:
	virtual void keyPressEvent(QKeyEvent *keyEvent);
	virtual void keyReleaseEvent(QKeyEvent *keyEvent);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

	void keyStateChanged(QKeyEvent *keyEvent, bool pressed);
	void updateCameraPos(float delta);

private:
	QTimer *updateTimer;
	QElapsedTimer frameTime;
	GLuint VertexArrayID;
	GLuint vertexbuffer;

	GLCamera* camera;
//	GLMesh* mesh;
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

	float speed;
	bool moveForward;
	bool moveBackward;
	bool moveLeftward;
	bool moveRightward;
	bool moveDownward;
	bool moveUpward;
	bool speedBoost;
	bool useOrtho;
};

#endif // GLVIEWPORT_H
