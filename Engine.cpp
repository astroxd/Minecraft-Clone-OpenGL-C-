#include "Engine.h"

Engine::Engine() {
	std::cout << "Engine Created" << std::endl;

	Window::GetInstance();
	camera = Camera(Window::GetInstance().getWidth(), Window::GetInstance().getHeight(), glm::vec3(0.0f, 18.0f, 0.0f));
	ShaderProgram.init("chunk.vert", "chunk.frag");
	VoxelMarkerProgram.init("cube.vert", "cube.frag");
	scene.window = Window::GetInstance().GetWindow();
	scene.second_context = Window::GetInstance().GetSecondContext();
	scene.setShader(ShaderProgram);
	scene.setCamera(&camera);
	//scene.setMesh();

	glfwSetInputMode(Window::GetInstance().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	run();

}



void Engine::run() {
	while (!glfwWindowShouldClose(Window::GetInstance().GetWindow())) {

		currentTime = glfwGetTime();
		timeDiff = currentTime - prevTime;

		counter++;
		if (timeDiff >= 1.0 / 30.0) {
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string ms = std::to_string((timeDiff / counter) * 1000);
			std::string newTitle = "Title " + FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(Window::GetInstance().GetWindow(), (newTitle + "  " + camera.getCameraPosition() + "  " + camera.getCameraOrientation()).c_str());
			prevTime = currentTime;
			counter = 0;
		}


		glClearColor(0.5f, 0.5f, 1.0f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ShaderProgram.Activate();


		camera.Inputs(Window::GetInstance().GetWindow(), timeDiff);
		camera.MouseInput(Window::GetInstance().GetWindow());
		camera.Matrix(45.0f, 0.1f, 6000.0f, ShaderProgram, "camMatrix");

		VoxelMarkerProgram.Activate();
		camera.Matrix(45.0f, 0.1f, 6000.0f, VoxelMarkerProgram, "camMatrix");

		scene.render(ShaderProgram, VoxelMarkerProgram);


		if (glfwGetKey(Window::GetInstance().GetWindow(), GLFW_KEY_F) == GLFW_PRESS)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		if (glfwGetKey(Window::GetInstance().GetWindow(), GLFW_KEY_G) == GLFW_PRESS)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		glfwSwapBuffers(Window::GetInstance().GetWindow());
		glfwSwapBuffers(Window::GetInstance().GetSecondContext());
		glfwPollEvents();
	}

	std::cout << "Closing" << std::endl;

	ShaderProgram.Delete();
	VoxelMarkerProgram.Delete();
	glfwDestroyWindow(Window::GetInstance().GetWindow());
	glfwTerminate();
}