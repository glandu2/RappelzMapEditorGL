#ifndef GLCAMERA_H
#define GLCAMERA_H

#define GLM_FORCE_RADIANS
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class GLCamera
{
public:
	GLCamera();

	void setPos(glm::vec3 pos) { position = pos; }
	void setRot(glm::quat rot) { rotation = rot; }

	glm::vec3 getPos() { return position; }
	glm::quat getRot() { return rotation; }

	void translate(glm::vec3 tr);

	glm::mat4 getTransformMatrix();

private:
	glm::vec3 position;
	glm::quat rotation;
};

#endif // GLCAMERA_H
