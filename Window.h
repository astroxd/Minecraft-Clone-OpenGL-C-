#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


class Window {
	public:
		const unsigned int width = 1280;
		const unsigned int height = 720;

		GLFWwindow* window;
		GLFWwindow* second_context;

		Window();
};

#endif;
