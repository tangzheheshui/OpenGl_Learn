#include "Camera.h"


Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::SetCamera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup)
{
	_Position = position;
	_Worldup = worldup;
	_Forward = glm::normalize(target - position);
	_Right = glm::normalize(glm::cross(_Forward, _Worldup));
	_Up = glm::cross(_Forward, _Right);
}

void Camera::SetCamera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup)
{
	_Position = position;
	_Worldup = worldup;
	float b = glm::cos(glm::radians(pitch));
	float a = glm::sin(glm::radians(yaw));
	a = ((float)((int)((a + 0.005) * 100))) / 100;
	_Forward.x = glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));
	_Forward.y = glm::sin(glm::radians(pitch));
	_Forward.z = glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw));

	_Forward = glm::normalize(_Forward);
	_Right = glm::normalize(glm::cross(_Forward, _Worldup));
	_Up = glm::normalize(glm::cross(_Forward, _Right));
}

void Camera::SetCamera(glm::vec3 position, float x, float y, float z, float w, glm::vec3 worldup)
{
	_Position = position;
	_Worldup = worldup;
	_Forward.x = x;
	_Forward.y = y;
	_Forward.z = z;
	_Forward = glm::normalize(_Forward);
	//_Right = glm::normalize(glm::cross(_Forward, _Worldup));
	//_Up = glm::normalize(glm::cross(_Forward, _Right));
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(_Position, _Position + _Forward, _Up);
}