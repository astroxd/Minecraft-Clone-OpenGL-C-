#include"Camera.h"

// constructor with vectors
Camera::Camera(int width, int height, glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: m_Width(width), m_Height(height), Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(SPEED), m_MouseSensitivity(SENSITIVITY)
{
	std::cout << "Costruttore camera" << std::endl;
	Position = position;
	WorldUp = up;

	m_Yaw = yaw;
	m_Pitch = pitch;

	m_lastMouseX = m_Width / 2.0f;
	m_lastMouseY = m_Height / 2.0f;

	updateCameraVectors();
}

// constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	: Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(SPEED), m_MouseSensitivity(SENSITIVITY)
{
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);

	m_Yaw = yaw;
	m_Pitch = pitch;

	updateCameraVectors();
}

void Camera::SetCamMatrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform) {
	glm::mat4 projection = glm::perspective(glm::radians(FOVdeg), (float)m_Width / (float)m_Height, nearPlane, farPlane);
	shader.SetMat4(uniform, projection * GetViewMatrix());
}


void Camera::Inputs(const float deltaTime) {
	KeyboardInputs(deltaTime);
	MouseInputs();
}

void Camera::KeyboardInputs(const float deltaTime) {

	if (Input::isKeyPressed(Key::W)) ProcessMovement(FORWARD, deltaTime);
	if (Input::isKeyPressed(Key::A)) ProcessMovement(LEFT, deltaTime);
	if (Input::isKeyPressed(Key::S)) ProcessMovement(BACKWARD, deltaTime);
	if (Input::isKeyPressed(Key::D)) ProcessMovement(RIGHT, deltaTime);
	if (Input::isKeyPressed(Key::Space)) ProcessMovement(UP, deltaTime);
	if (Input::isKeyPressed(Key::LeftShift)) ProcessMovement(DOWN, deltaTime);

	m_MovementSpeed = Input::isKeyPressed(Key::LeftControl) ? SPRINT : SPEED;

	if (Input::isKeyPressed(Key::E)) {

		glfwSetInputMode(Window::GetInstance().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_firstClick = true;
		m_menuOpen = true;
	}
	if (Input::isKeyPressed(Key::Escape)) {
		glfwSetInputMode(Window::GetInstance().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (m_menuOpen) glfwSetCursorPos(Window::GetInstance().GetWindow(), m_lastMouseX, m_lastMouseY);
		m_menuOpen = false;
	}
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessMovement(CameraMovement direction, const float deltaTime)
{
	if (m_menuOpen) return;
	float velocity = m_MovementSpeed * deltaTime;

	switch (direction) {
	case FORWARD:
		Position += Front * velocity;
		break;
	case LEFT:
		Position -= Right * velocity;
		break;
	case BACKWARD:
		Position -= Front * velocity;
		break;
	case RIGHT:
		Position += Right * velocity;
		break;
	case UP:
		Position += WorldUp * velocity;
		break;
	case DOWN:
		Position -= WorldUp * velocity;
		break;
	default:
		std::cout << "Invalid Camera Movement" << std::endl;
	}

	//LOCK Y position	
	//Position.y = 18.0f;
}


void Camera::MouseInputs() {
	GLFWwindow* window = Window::GetInstance().GetWindow();

	glm::vec2 mousePosition = Input::getMousePosition();

	float xpos = mousePosition.x;
	float ypos = mousePosition.y;

	//if mouse is still don't do anything
	if (m_lastMouseX == xpos && m_lastMouseY == ypos) return;


	if (m_firstClick)
	{
		m_lastMouseX = xpos;
		m_lastMouseY = ypos;
		m_firstClick = false;
		glfwSetCursorPos(window, (m_Width / 2), (m_Height / 2));
	}

	//MenuOpen check is here to prevent mouse jumping when closing menu
	if (m_menuOpen) return;

	float xoffset = xpos - m_lastMouseX;
	float yoffset = m_lastMouseY - ypos; // reversed since y-coordinates go from bottom to top

	m_lastMouseX = xpos;
	m_lastMouseY = ypos;

	ProcessMouseMovement(xoffset, yoffset);
}


// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= m_MouseSensitivity;
	yoffset *= m_MouseSensitivity;

	m_Yaw += xoffset;
	m_Pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;
	}

	// update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}




std::string Camera::GetCameraPosition() const {
	return std::to_string(Position.x) + "(" + std::to_string(std::floor(Position.x / 16)) + ")X  "
		+ std::to_string(Position.y) + "(" + std::to_string(std::floor(Position.y / 50)) + ")Y  "
		+ std::to_string(Position.z) + "(" + std::to_string(std::floor(Position.z / 16)) + ")Z  ";

}

std::string Camera::GetCameraOrientation() const {
	return std::to_string(std::round(Front.x)) + "X  "
		+ std::to_string(std::round(Front.y)) + "Y  "
		+ std::to_string(std::round(Front.z)) + "Z  ";
}

// calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors()
{
	// calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	Front = glm::normalize(front);
	// also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}