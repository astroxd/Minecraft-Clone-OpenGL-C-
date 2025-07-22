#ifndef INPUT_H
#define INPUT_H

#include "Window.h"

#include "KeyCodes.h"
#include "MouseCodes.h"

class Input {
public:

	inline static bool isKeyPressed(const KeyCode key) {
		auto window = Window::GetInstance().GetWindow();
		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		return state == GLFW_PRESS;
	}

	inline static bool isMouseButtonPressed(const MouseCode button) {
		auto window = Window::GetInstance().GetWindow();
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}

	inline static glm::vec2 getMousePosition() {
		auto window = Window::GetInstance().GetWindow();
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}
};

#endif;