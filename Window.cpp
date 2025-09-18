#include "Window.h"
#include "Log.h"
#include "Input.h"
#include "Events/EventManager.h"

//Window* Window::s_Instance = new Window();
Window* Window::s_Instance = nullptr;
std::mutex Window::s_Mutex;

unsigned int Window::s_Width;
unsigned int Window::s_Height;

Window::Window()
	: m_Handler([this](const TestEvent& e) {OnEvent(e); })
{
	LOG_INFO("Window created");
	s_Width = 1280;
	s_Height = 720;
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

	Events::Subscribe(m_Handler);


	glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);
	glfwSetScrollCallback(m_Window, scroll_callback);






#ifdef __APPLE__
	glViewport(0, 0, getWidth() * 2, getHeight() * 2); // Due to mac glitch resolution needs to be twice as big
#else
	glViewport(0, 0, getWidth(), getHeight());
#endif

	glfwSetWindowAspectRatio(m_Window, 16, 9);

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
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
	s_Width = width;
	s_Height = height;

	Events::TriggerEvent(TestEvent(3));




#endif
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	Input::setScrollWheel(yoffset);
}

void Window::OnEvent(const TestEvent& e) {
	LOG_WARN("EVENTTTT, {0}", e.value);
}