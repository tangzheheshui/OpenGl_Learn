#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Camera
{
public:
	Camera();
	~Camera();
	void SetCamera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup);
	//欧拉角
	void SetCamera(glm::vec3 position,float pitch,float yaw,glm::vec3 worldup);
	void SetCamera(glm::vec3 position, float x, float y, float z, float w, glm::vec3 worldup);
	//四元数
	glm::mat4 GetViewMatrix();
	
private:
	glm::vec3 _Position;
	glm::vec3 _Forward;
	glm::vec3 _Right;
	glm::vec3 _Up;
	glm::vec3 _Worldup;

};

