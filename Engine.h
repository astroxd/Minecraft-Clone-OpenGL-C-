#ifndef ENGINE_H
#define ENGINE_H

#include "Window.h"
#include "Camera.h"
#include "Scene.h"
#include "Log.h"





class Engine {

public:

	Camera* camera;
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
