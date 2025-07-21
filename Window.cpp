#include "Window.h"

Window* Window::s_Instance = new Window();
std::mutex Window::s_Mutex;

Window::Window() {
	std::cout << "Creating Window" << std::endl;
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	m_Window = glfwCreateWindow(getWidth(), getHeight(), "Test", NULL, NULL);
	if (m_Window == NULL) {
		std::cout << "Failed to create WIndow" << std::endl;
		glfwTerminate();
	}

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	m_SecondContext = glfwCreateWindow(getWidth(), getHeight(), "Test", NULL, m_Window);
	if (m_SecondContext == NULL) {
		std::cout << "Failed to create secondo Window" << std::endl;
		glfwTerminate();
	}


	glfwMakeContextCurrent(m_Window);

	gladLoadGL();
	glViewport(0, 0, getWidth(), getHeight());

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CW);
	std::cout << "Window Created" << std::endl;

}
