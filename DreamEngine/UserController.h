#pragma once

#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glfw3.h>

class UserController
{
public:
	UserController();
	~UserController();

	void computeMatricesFromInputs(GLFWwindow* window);
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

private:
	// Initial position : on +Z
	glm::vec3 position = glm::vec3(0, 0, 5);
	// Initial horizontal angle : toward -Z
	float horizontalAngle = 3.14f;
	// Initial vertical angle : none
	float verticalAngle = 0.0f;
	// Initial Field of View
	float initialFoV = 45.0f;

	float speed = 20.0f; // 3 units / second
	float mouseSpeed = 0.005f;

	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

};

