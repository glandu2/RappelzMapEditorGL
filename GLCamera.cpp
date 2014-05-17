#include "GLCamera.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/geometric.hpp>

GLCamera::GLCamera()
{
	position = glm::vec3(0.0);
	rotation = glm::quat(1.0, 0.0, 0.0, 0.0);
}

void GLCamera::translate(glm::vec3 tr) {
	position += glm::vec3(rotation * glm::vec4(tr, 1.0));
}

glm::mat4 GLCamera::getTransformMatrix() {
	return glm::inverse(glm::translate(glm::mat4(1.0), position) * glm::mat4_cast(rotation));
}
