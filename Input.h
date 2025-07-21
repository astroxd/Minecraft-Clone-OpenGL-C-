#ifndef INPUT_H
#define INPUT_H

#include "Window.h"

#include "KeyCodes.h"
#include "MouseCodes.h"

class Input {
public:

	inline static bool isKeyPressed(const KeyCode key, GLFWwindow* window) {
		auto windowa = Window::GetInstance().GetWindow();
		auto state = glfwGetKey(windowa, static_cast<int32_t>(key));
		return state == GLFW_PRESS;
	}

	inline static bool isMouseButtonPressed(const MouseCode button, GLFWwindow* window) {

		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}

	inline static glm::vec2 getMousePosition(GLFWwindow* window) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}
};

#endif;