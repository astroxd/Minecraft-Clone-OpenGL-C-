#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <mutex>




#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
	inline unsigned int getWidth() const { return m_Width; }
	inline unsigned int getHeight() const { return m_Height; }

	inline void DestroyWindow() { glfwDestroyWindow(m_Window); }

private:
	Window();

private:
	static Window* s_Instance;
	static std::mutex s_Mutex;

	const unsigned int m_Width = 1280;
	const unsigned int m_Height = 720;

	GLFWwindow* m_Window;
	GLFWwindow* m_SecondContext;

	// window resize callback
	static void framebuffer_size_callback(GLFWwindow* window, GLint width, GLint height);
};

#endif;
