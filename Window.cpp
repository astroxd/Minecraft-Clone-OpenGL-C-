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
	glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);

#ifdef __APPLE__
	glViewport(0, 0, getWidth() * 2, getHeight() * 2); // Due to mac glitch resolution needs to be twice as big
#else
	glViewport(0, 0, getWidth(), getHeight());
#endif

	glfwSetWindowAspectRatio(m_Window, 16, 9);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CW);

}

void Window::framebuffer_size_callback(GLFWwindow* window, GLint width, GLint height) {
#ifdef __APPLE__
	// Make sure the viewport matches the new window dimensions;
	std::cout << "Width: " << (width / 2) << " Height: " << (height / 2) << std::endl;
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
#else
	// Make sure the viewport matches the new window dimensions;
	std::cout << "Width: " << width << " Height: " << height << std::endl;
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
#endif
}