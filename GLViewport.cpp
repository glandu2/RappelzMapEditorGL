#define GLM_FORCE_RADIAN

#include "GLViewport.h"
#include <QTimer>
#include <QKeyEvent>
#include "GLCamera.h"
#include "GLRenderer.h"
#include "GLStaticMesh.h"
#include "Terrain.h"
#include "GLProgram.h"


#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

GLViewport::GLViewport(QWidget *parent) :
	QGLWidget(parent),
	updateTimer(0)
{
	updateTimer = new QTimer(this);
	connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateGL()));
	updateTimer->start(1000 / 60);

	setFocusPolicy(Qt::ClickFocus);

	mouseSensivity = 0.01;
	speed = 400;
	moveCamera = false;
	moveForward = false;
	moveBackward = false;
	moveLeftward = false;
	moveRightward = false;
	moveDownward = false;
	moveUpward = false;
	speedBoost = false;
	useOrtho = false;
}

GLViewport::~GLViewport() {
	delete updateTimer;
}

void GLViewport::setCameraX(float x) {
	glm::vec3 position = camera->getPos();
	position.x = x;

	camera->setPos(position);
}

void GLViewport::setCameraY(float y) {
	glm::vec3 position = camera->getPos();
	position.y = y;

	camera->setPos(position);
}

void GLViewport::setCameraZ(float z) {
	glm::vec3 position = camera->getPos();
	position.z = z;

	camera->setPos(position);
}


void GLViewport::setCameraPitch(float pitch) {
	cameraPitch = glm::clamp(pitch, float(-M_PI_2), float(M_PI_2));

	camera->setRot(glm::quat(glm::vec3(0.0f, cameraPitch, cameraYaw)));
}

void GLViewport::setCameraYaw(float yaw) {
	cameraYaw = glm::mod(yaw, float(M_PI*2));

	camera->setRot(glm::quat(glm::vec3(0.0f, cameraPitch, cameraYaw)));
}

void GLViewport::keyPressEvent(QKeyEvent *keyEvent) {
	keyStateChanged(keyEvent, true);
}

void GLViewport::keyReleaseEvent(QKeyEvent *keyEvent) {
	keyStateChanged(keyEvent, false);
}

void GLViewport::keyStateChanged(QKeyEvent *keyEvent, bool pressed) {
	switch(keyEvent->key()) {
		case Qt::Key_E:
		case Qt::Key_Up:
			moveForward = pressed;
			break;

		case Qt::Key_D:
		case Qt::Key_Down:
			moveBackward = pressed;
			break;

		case Qt::Key_F:
		case Qt::Key_Right:
			moveRightward = pressed;
			break;

		case Qt::Key_S:
		case Qt::Key_Left:
			moveLeftward = pressed;
			break;

		case Qt::Key_C:
			moveDownward = pressed;
			break;

		case Qt::Key_Space:
			moveUpward = pressed;
			break;

		case Qt::Key_Shift:
			speedBoost = pressed;
			break;

		case Qt::Key_5:
		case Qt::Key_O:
			if(pressed) {
				useOrtho = !useOrtho;
				resizeGL(width(), height());
			}
			break;
	}
}

void GLViewport::updateCameraPos(float delta) {
	/*static int slowdownFPSUpdate = 0;
	slowdownFPSUpdate++;
	if(slowdownFPSUpdate >= 100) {
		parentWidget()->parentWidget()->setWindowTitle(QString("FPS: %1").arg(1/delta));
		slowdownFPSUpdate = 0;
	}*/
	float adjustedCameraSpeed = speed * delta;

	if(speedBoost)
		adjustedCameraSpeed *= 10;

	if(moveForward) camera->translate(glm::vec3(adjustedCameraSpeed, 0, 0));
	if(moveBackward) camera->translate(glm::vec3(-adjustedCameraSpeed, 0, 0));
	if(moveLeftward) camera->translate(glm::vec3(0,adjustedCameraSpeed, 0));
	if(moveRightward) camera->translate(glm::vec3(0,-adjustedCameraSpeed, 0));
	if(moveUpward) camera->translate(glm::vec3(0,0,adjustedCameraSpeed));
	if(moveDownward) camera->translate(glm::vec3(0,0,-adjustedCameraSpeed));

	if(moveForward || moveBackward || moveLeftward || moveRightward || moveUpward || moveDownward) {
		glm::vec3 position = camera->getPos();
		emit cameraXChanged(position.x);
		emit cameraYChanged(position.y);
		emit cameraZChanged(position.z);
	}
}

void GLViewport::mouseMoveEvent(QMouseEvent *event) {
	if(moveCamera) {
		int dx = event->x() - lastMouseX;
		int dy = event->y() - lastMouseY;

		cameraYaw = glm::mod(cameraYaw - dx * mouseSensivity, float(M_PI*2));
		cameraPitch = glm::clamp(cameraPitch + dy * mouseSensivity, float(-M_PI_2), float(M_PI_2));

		camera->setRot(glm::quat(glm::vec3(0.0f, cameraPitch, cameraYaw)));

		lastMouseX = event->x();
		lastMouseY = event->y();


		emit cameraPitchChanged(cameraPitch);
		emit cameraYawChanged(cameraYaw);
	}
}

void GLViewport::mousePressEvent(QMouseEvent *event) {
	if(event->button() == Qt::RightButton) {
		lastMouseX = event->x();
		lastMouseY = event->y();
		moveCamera = true;
	}
}

void GLViewport::mouseReleaseEvent(QMouseEvent *event) {
	if(event->button() == Qt::RightButton) {
		moveCamera = false;
	}
}


void GLViewport::updateGL() {
	glDraw(); //extra call to unneeded context change
	//paintGL();
}

void GLAPIENTRY onOpenglError(GLenum source,
				   GLenum type,
				   GLuint id,
				   GLenum severity,
				   GLsizei length,
				   const GLchar* message,
				   void* userParam)
{
	const char* sourceStr = "Unknown";
	const char* typeStr = "Unknown";
	const char* severityStr = "Unknown";
	switch(source) {
		case GL_DEBUG_SOURCE_API: sourceStr = "API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: sourceStr = "WINDOW_SYSTEM"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceStr = "SHADER_COMPILER"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY: sourceStr = "THIRD_PARTY"; break;
		case GL_DEBUG_SOURCE_APPLICATION: sourceStr = "APPLICATION"; break;
		case GL_DEBUG_SOURCE_OTHER: sourceStr = "OTHER"; break;
	}
	switch(type) {
		case GL_DEBUG_TYPE_ERROR: typeStr = "ERROR"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeStr = "DEPRECATED_BEHAVIOR"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: typeStr = "UNDEFINED_BEHAVIOR"; break;
		case GL_DEBUG_TYPE_PORTABILITY: typeStr = "PORTABILITY"; break;
		case GL_DEBUG_TYPE_PERFORMANCE: typeStr = "PERFORMANCE"; break;
		case GL_DEBUG_TYPE_OTHER: typeStr = "OTHER"; break;
	}
	switch(severity){
		case GL_DEBUG_SEVERITY_LOW: severityStr = "LOW"; break;
		case GL_DEBUG_SEVERITY_MEDIUM: severityStr = "MEDIUM"; break;
		case GL_DEBUG_SEVERITY_HIGH: severityStr = "HIGH"; break;
	}
	printf("OPENGL ERROR: source: %s(%d), type: %s(%d), id: %d, severity: %s(%d), length: %d, message: %s\n",
		   sourceStr, source, typeStr, type, id, severityStr, severity, length, message);
}

void GLViewport::initializeGL() {
	glewInit();
	glEnable(GL_DEBUG_OUTPUT);

#ifndef _WIN32
	glDebugMessageCallback(&onOpenglError, 0);
#endif

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	//wireframe
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_ALPHA_TEST);
	//glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DITHER);

	cameraPitch = cameraYaw = 0;

	camera = new GLCamera;
	program = new GLProgram;
	renderer = new GLRenderer(camera, this);

	program->loadShaders("vertex.glsl", "fragment.glsl");
	program->loadToGpu();

	glUseProgram(program->getId());

	projectionMatrixUniform = glGetUniformLocation(program->getId(), "projectionMatrix");
	cameraMatrixUniform = glGetUniformLocation(program->getId(), "cameraMatrix");

	resizeGL(width(), height());

	emit initialized();
}

void GLViewport::resizeGL(int width, int height) {
	static const glm::mat4 coordTranform(	//change screen coords: forward is X+, up is Z+, left is Y+
				 0.0f, 0.0f, -1.0f, 0.0f,
				-1.0f, 0.0f,  0.0f, 0.0f,
				 0.0f, 1.0f,  0.0f, 0.0f,
				 0.0f, 0.0f,  0.0f, 1.0f
				);

	if(height == 0)
		return;

	glViewport(0, 0, width, height);

	if(!useOrtho)
		projectionMatrix = glm::perspective(float(40.0*M_PI/180), float(width)/height, 1.0f, 327680.0f) * coordTranform;
	else {
		float w = 16128;
		float h = 16128;

		w *= float(width)/height;
		projectionMatrix = glm::ortho(-w/2, w/2, -h/2, h/2, 0.1f, 327680.0f) * coordTranform;
	}
}

void GLViewport::paintGL() {
	if(frameTime.isValid())
		updateCameraPos(frameTime.restart()/1000.0f);
	else
		frameTime.start();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	glm::mat4x4 cameraMatrix = camera->getTransformMatrix();
	glUniformMatrix4fv(cameraMatrixUniform, 1, GL_FALSE, &cameraMatrix[0][0]);

	renderer->render();
}
