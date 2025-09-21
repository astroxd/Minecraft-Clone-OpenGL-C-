#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <mutex>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include "Events/EventManager.h"
#include "Events/WindowEvent.h"

//Singleton implementation of Window
class Window {
public:
	// Deleting the copy constructor to prevent copies
	Window(const Window& obj) = delete;

	static Window& GetInstance() {
		if (s_Instance == nullptr) {
			std::lock_guard<std::mutex> lock(s_Mutex);
			if (s_Instance == nullptr) {
				s_Instance = new Window();
			}
		}
		return *s_Instance;
	};

	inline GLFWwindow* GetWindow() const { return m_Window; }
	inline GLFWwindow* GetSecondContext() const { return m_SecondContext; }
	inline unsigned int getWidth() const { return s_Width; }
	inline unsigned int getHeight() const { return s_Height; }
	inline glm::vec2 getWindowSize() const { return glm::vec2(s_Width, s_Height); }

	inline void DestroyWindow() { glfwDestroyWindow(m_Window); }


private:
	Window();

private:
	static Window* s_Instance;
	static std::mutex s_Mutex;

	static unsigned int s_Width;
	static unsigned int s_Height;

	GLFWwindow* m_Window;
	GLFWwindow* m_SecondContext;

	// window resize callback
	static void framebuffer_size_callback(GLFWwindow* window, GLint width, GLint height);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

};

#endif;
