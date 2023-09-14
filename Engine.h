#ifndef ENGINE_H
#define ENGINE_H

#include "Window.h"
#include "Camera.h"
#include "Scene.h"
//#include <glm/glm.hpp>

class Engine {

public:

	Window window;
	Camera camera = Camera(window.width, window.height, glm::vec3(0.0f, 0.0f, 2.0f));
	Shader ShaderProgram;
	Scene scene;

	Engine();
	void run();

private:
	
	double prevTime = 0.0;
	double currentTime = 0.0;
	double timeDiff;
	unsigned int counter = 0;

};

#endif;
