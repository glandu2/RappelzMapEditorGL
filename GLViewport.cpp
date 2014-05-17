#define GLM_FORCE_RADIAN

#include "GLViewport.h"
#include <QTimer>
#include <QKeyEvent>
#include "GLCamera.h"
#include "GLRenderer.h"
#include "GLMesh.h"
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
	moveCamera = false;
}

GLViewport::~GLViewport() {
	delete updateTimer;
}

void GLViewport::keyPressEvent(QKeyEvent *keyEvent) {
	switch(keyEvent->key()) {
		case Qt::Key_Escape:
			close();
			break;

		case Qt::Key_Up:
			camera->translate(glm::vec3(0.1, 0.0, 0));
			break;

		case Qt::Key_Down:
			camera->translate(glm::vec3(-0.1, 0.0, 0));
			break;

		case Qt::Key_Right:
			camera->translate(glm::vec3(0, -0.1, 0.0));
			break;

		case Qt::Key_Left:
			camera->translate(glm::vec3(0, 0.1, 0.0));
			break;

	}
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
	glDraw();
}

void GLViewport::initializeGL() {
	glewInit();

	cameraPitch = cameraYaw = 0;

	camera = new GLCamera;
	mesh = new GLMesh;
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

	mesh->loadFromNx3("test.nx3");
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
	projectionMatrix = glm::perspective(float(45.0*M_PI/180), float(width)/height, 0.1f, 5000.0f) * coordTranform;
}

void GLViewport::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	glm::mat4x4 cameraMatrix = camera->getTransformMatrix();
	glUniformMatrix4fv(cameraMatrixUniform, 1, GL_FALSE, &cameraMatrix[0][0]);

	renderer->render();
}
