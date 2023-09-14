#include "Engine.h"

Engine::Engine() {

	ShaderProgram.init("chunk.vert", "chunk.frag");
	scene.window = window.window;
	scene.second_context = window.second_context;
	scene.setShader(ShaderProgram);
	scene.setCamera(&camera);
	//scene.setMesh();

	run();

}

void Engine::run() {
	while (!glfwWindowShouldClose(window.window)) {

		currentTime = glfwGetTime();
		timeDiff = currentTime - prevTime;
		
		counter++;
		if (timeDiff >= 1.0 / 30.0) {
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string ms = std::to_string((timeDiff / counter) * 1000);
			std::string newTitle = "Title " + FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window.window, (newTitle + "  " + camera.getCameraPosition() + "  " + camera.getCameraOrientation()).c_str());
			prevTime = currentTime;
			counter = 0;
		}


		glClearColor(0.5f, 0.5f, 1.0f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		ShaderProgram.Activate();


		camera.Inputs(window.window, timeDiff);
		camera.Matrix(45.0f, 0.1f, 6000.0f, ShaderProgram, "camMatrix");
		
		scene.render(ShaderProgram);


		if (glfwGetKey(window.window, GLFW_KEY_F) == GLFW_PRESS)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		if (glfwGetKey(window.window, GLFW_KEY_G) == GLFW_PRESS)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		glfwSwapBuffers(window.window);
		glfwSwapBuffers(window.second_context);
		glfwPollEvents();
	}

	std::cout << "Closing" << std::endl;
	
	ShaderProgram.Delete();

	glfwDestroyWindow(window.window);
	glfwTerminate();
}