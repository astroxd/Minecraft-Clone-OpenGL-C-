#include "Window.h"
#include "Log.h"

//Window* Window::s_Instance = new Window();
Window* Window::s_Instance = nullptr;
std::mutex Window::s_Mutex;

Window::Window() {
	LOG_INFO("Window created");
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	m_Window = glfwCreateWindow(getWidth(), getHeight(), "Test", NULL, NULL);
	if (m_Window == NULL) {
		LOG_ERROR("Failed to create Window");
		glfwTerminate();
	}

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	m_SecondContext = glfwCreateWindow(getWidth(), getHeight(), "Test", NULL, m_Window);
	if (m_SecondContext == NULL) {
		LOG_ERROR("Failed to create secondo Window");
		glfwTerminate();
	}


	glfwMakeContextCurrent(m_Window);

	gladLoadGL();
	glViewport(0, 0, getWidth(), getHeight());

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CW);

}
