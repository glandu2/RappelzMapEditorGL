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
	updateTimer->start(1000 / 30);

	setFocusPolicy(Qt::ClickFocus);

	mouseSensivity = 0.01;
	speed = 50*60;
	moveCamera = false;
	moveForward = false;
	moveBackward = false;
	moveLeftward = false;
	moveRightward = false;
	moveDownward = false;
	moveUpward = false;
}

GLViewport::~GLViewport() {
	delete updateTimer;
}

void GLViewport::keyPressEvent(QKeyEvent *keyEvent) {
	keyStateChanged(keyEvent, true);
}

void GLViewport::keyReleaseEvent(QKeyEvent *keyEvent) {
	keyStateChanged(keyEvent, false);
}

void GLViewport::keyStateChanged(QKeyEvent *keyEvent, bool pressed) {
	switch(keyEvent->key()) {
		case Qt::Key_Escape:
			close();
			break;

		case Qt::Key_Z:
		case Qt::Key_Up:
			moveForward = pressed;
			break;

		case Qt::Key_S:
		case Qt::Key_Down:
			moveBackward = pressed;
			break;

		case Qt::Key_D:
		case Qt::Key_Right:
			moveRightward = pressed;
			break;

		case Qt::Key_Q:
		case Qt::Key_Left:
			moveLeftward = pressed;
			break;

		case Qt::Key_C:
			moveDownward = pressed;
			break;

		case Qt::Key_Space:
			moveUpward = pressed;
			break;

	}
}

void GLViewport::updateCameraPos(float delta) {
	static int slowdownFPSUpdate = 0;
	slowdownFPSUpdate++;
	if(slowdownFPSUpdate >= 100) {
		parentWidget()->parentWidget()->setWindowTitle(QString("FPS: %1").arg(1/delta));
		slowdownFPSUpdate = 0;
	}
	float adjustedCameraSpeed = speed * delta;

	if(moveForward) camera->translate(glm::vec3(adjustedCameraSpeed, 0, 0));
	if(moveBackward) camera->translate(glm::vec3(-adjustedCameraSpeed, 0, 0));
	if(moveLeftward) camera->translate(glm::vec3(0,adjustedCameraSpeed, 0));
	if(moveRightward) camera->translate(glm::vec3(0,-adjustedCameraSpeed, 0));
	if(moveUpward) camera->translate(glm::vec3(0,0,adjustedCameraSpeed));
	if(moveDownward) camera->translate(glm::vec3(0,0,-adjustedCameraSpeed));
}

void GLViewport::mouseMoveEvent(QMouseEvent *event) {
	if(moveCamera) {
		int dx = event->x() - lastMouseX;
		int dy = event->y() - lastMouseY;

		cameraYaw -= dx * mouseSensivity;

		float lastPitch = cameraPitch;
		cameraPitch = lastPitch + dy * mouseSensivity;

		if(cameraPitch < -M_PI_2 || cameraPitch > M_PI_2) {
			cameraPitch = lastPitch;
		}

		camera->setRot(glm::quat(glm::vec3(0.0f, cameraPitch, cameraYaw)));

		lastMouseX = event->x();
		lastMouseY = event->y();
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

	glDebugMessageCallback(&onOpenglError, 0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	//wireframe
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_ALPHA_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_DITHER);

	cameraPitch = cameraYaw = 0;

	camera = new GLCamera;
	Terrain *terrain = new Terrain;
	mesh = terrain;
	program = new GLProgram;
	renderer = new GLRenderer(camera, this);

	/*GLMesh::VertexAttribute vertex;
	vertex.normal = glm::vec3(0.0);
	vertex.texCoord = glm::vec2(0.0);

	vertex.pos = glm::vec3(-1.0f, -1.0f, 0.0f);
	mesh->vertexAttributes.push_back(vertex);

	vertex.pos = glm::vec3(1.0f, -1.0f, 0.0f);
	mesh->vertexAttributes.push_back(vertex);

	vertex.pos = glm::vec3(0.0f,  1.0f, 0.0f);
	mesh->vertexAttributes.push_back(vertex);

	mesh->indices.push_back(0);
	mesh->indices.push_back(1);
	mesh->indices.push_back(2);*/

	//mesh->loadFromNx3("test.nx3");
	terrain->loadFromNfm("m009_004.nfm");
	mesh->loadToGpu();

	program->loadShaders("vertex.glsl", "fragment.glsl");
	program->loadToGpu();

	glUseProgram(program->getId());

	projectionMatrixUniform = glGetUniformLocation(program->getId(), "projectionMatrix");
	cameraMatrixUniform = glGetUniformLocation(program->getId(), "cameraMatrix");

	renderer->addMesh(mesh);

	resizeGL(width(), height());
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
	projectionMatrix = glm::perspective(float(45.0*M_PI/180), float(width)/height, 1.0f, 327680.0f) * coordTranform;
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
