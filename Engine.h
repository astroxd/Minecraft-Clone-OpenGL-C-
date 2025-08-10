#ifndef ENGINE_H
#define ENGINE_H

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Window.h"
#include "Camera.h"
#include "Scene.h"
#include "Log.h"

class Engine {

public:

	Camera* camera;
	Scene* scene;

	Engine();
	~Engine();
	void run();

private:

	double prevTime = 0.0;
	double currentTime = 0.0;
	double timeDiff;
	unsigned int counter = 0;
	double FPS;
	double ms;

};

#endif;
