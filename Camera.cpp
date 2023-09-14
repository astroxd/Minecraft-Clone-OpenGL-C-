#include"Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{

	std::cout << "Camera Created" << std::endl;
	std::cout << width << " - " << height << std::endl;

	Camera::width = width;
	Camera::height = height;
	Position = position;
	
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform)
{
	// Initializes matrices since otherwise they will be the null matrix
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right position
	view = glm::lookAt(Position, Position + Orientation, Up);
	// Adds perspective to the scene
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	// Exports the camera matrix to the Vertex Shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
	//glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	//glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
}

glm::mat4 Camera::getProj() {
	glm::mat4 projection = glm::mat4(1.0f);
	return projection = glm::perspective(glm::radians(45.0f), (float)1280 / 720, 0.1f, 6000.0f);
}

void Camera::Inputs(GLFWwindow* window, float deltaTime)
{	
	// Handles key inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += speed * Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position -= speed * glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position -= speed * Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += speed * glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Position += speed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Position += speed * -Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = 0.4f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		speed = 0.1f;
	}


	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientation;
		}

		// Rotates the Orientation left and right
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}
}

std::string Camera::getCameraPosition() {
	return std::to_string(Position.x) + "(" + std::to_string(std::floor(Position.x / 16)) + ")X  " 
		+ std::to_string(Position.y) + "(" + std::to_string(std::floor(Position.y / 50)) + ")Y  "
		+ std::to_string(Position.z) + "(" + std::to_string(std::floor(Position.z / 16)) + ")Z  ";
		
}

std::string Camera::getCameraOrientation() {
	return std::to_string(std::round(Orientation.x)) + "X  " 
		+ std::to_string(std::round(Orientation.y)) + "Y  " 
		+ std::to_string(std::round(Orientation.z)) + "Z  ";
}

Frustum Camera::createFrustumFromCamera(float aspect, float fovDeg, float nearPlane, float farPlane) {
	Frustum     frustum;
    const float halfVSide = farPlane * tanf(fovDeg * .5f);
    const float halfHSide = halfVSide * aspect;
    const glm::vec3 frontMultFar = farPlane * Orientation;

    frustum.nearFace = { Position + nearPlane * Orientation, Orientation };
    frustum.farFace = { Position + frontMultFar, -Orientation };
    frustum.rightFace = { Position,
                            glm::cross(frontMultFar - glm::normalize(glm::cross(Orientation, Up)) * halfHSide, Up) };
    frustum.leftFace = { Position,
                            glm::cross(Up,frontMultFar + glm::normalize(glm::cross(Orientation, Up)) * halfHSide) };
    frustum.topFace = { Position,
                            glm::cross(glm::normalize(glm::cross(Orientation, Up)), frontMultFar - Up * halfVSide) };
    frustum.bottomFace = { Position,
                            glm::cross(frontMultFar + Up * halfVSide, glm::normalize(glm::cross(Orientation, Up))) };

    return frustum;
}


bool Camera::isOnFrustum(glm::vec3 chunkCenter) {
	const float CHUNKSPHERERADIUS = 8 * sqrt(3);
	glm::vec3 sphereVec = chunkCenter - Position;

	float sz = glm::dot(sphereVec, Orientation);

	if (sz < 0.1 - CHUNKSPHERERADIUS || sz > 6000.0 + CHUNKSPHERERADIUS) return false;


	float sy = glm::dot(sphereVec, Up);
	float dist = factor_y * CHUNKSPHERERADIUS + sz * tan_y;
	if (sy < -dist || sy > dist) return false;

	float sx = glm::dot(sphereVec, glm::normalize(glm::cross(Orientation, Up)));
	float distX = factor_x * CHUNKSPHERERADIUS + sz * tan_x;
	if (sx < -distX || sx > distX) return false;

	return true;

}