#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shaderClass.h"
#include "Input.h"


// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SPRINT = 8.5f;
const float SENSITIVITY = 0.1f;

class Camera {
public:
	// camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;



	Camera() {
		std::cout << "Camera Created" << std::endl;
	}

	// constructor with vectors
	Camera(int width, int height, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

	// constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);


	// returns the view matrix calculated using Euler Angles and the LookAt Matrix
	inline glm::mat4 GetViewMatrix() const
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void SetCamMatrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform);

	void Inputs(const float deltaTime);
	void KeyboardInputs(const float deltaTime);
	void MouseInputs();

	void ProcessMovement(CameraMovement direction, const float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);



	std::string GetCameraPosition() const;
	std::string GetCameraOrientation() const;

private:
	// calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();
private:
	// euler Angles
	float m_Yaw;
	float m_Pitch;
	// camera options
	float m_MovementSpeed;
	float m_MouseSensitivity;

	// Stores the width and height of the window
	int m_Width;
	int m_Height;
	bool m_firstClick = true;
	float m_lastMouseX;
	float m_lastMouseY;


	//Test to mimic a menu opened
	bool m_menuOpen = false;



};



#endif;
