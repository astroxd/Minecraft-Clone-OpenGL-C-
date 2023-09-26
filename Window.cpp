#include "Window.h"



Window::Window() {

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	window = glfwCreateWindow(width, height, "Test", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create WIndow" << std::endl;
		glfwTerminate();
	}

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	second_context = glfwCreateWindow(width, height, "Test", NULL, window);
	if (second_context == NULL) {
		std::cout << "Failed to create secondo Window" << std::endl;
		glfwTerminate();
	}


	glfwMakeContextCurrent(window);
	
	gladLoadGL();
	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CW);
	std::cout << "Window Created" << std::endl;
	
}
