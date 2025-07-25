#include "Engine.h"
#include "ShaderManager.h"

Engine::Engine() {

	LOG_INFO("Engine Created");

	camera = new Camera(Window::GetInstance().getWidth(), Window::GetInstance().getHeight(), glm::vec3(0.0f, 18.0f, 0.0f));
	scene.setCamera(camera);

	ShaderManager::AddShader("ShaderProgram", std::make_shared<Shader>("chunk.vert", "chunk.frag"));
	ShaderManager::AddShader("VoxelMarkerProgram", std::make_shared<Shader>("cube.vert", "cube.frag"));
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
			glfwSetWindowTitle(Window::GetInstance().GetWindow(), (newTitle + "  " + camera->GetCameraPosition() + "  " + camera->GetCameraOrientation()).c_str());
			prevTime = currentTime;
			counter = 0;
		}


		glClearColor(0.5f, 0.5f, 1.0f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ShaderManager::GetShader("ShaderProgram").Activate();

		camera->Inputs(timeDiff);
		camera->SetCamMatrix(45.0f, 0.1f, 6000.0f, ShaderManager::GetShader("ShaderProgram"), "camMatrix");

		ShaderManager::GetShader("VoxelMarkerProgram").Activate();
		camera->SetCamMatrix(45.0f, 0.1f, 6000.0f, ShaderManager::GetShader("VoxelMarkerProgram"), "camMatrix");

		scene.render();


		if (Input::isKeyPressed(Key::F))
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		if (Input::isKeyPressed(Key::G))
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		glfwSwapBuffers(Window::GetInstance().GetWindow());
		glfwSwapBuffers(Window::GetInstance().GetSecondContext());
		glfwPollEvents();
	}

	LOG_INFO("Closing");

	//ShaderProgram.Delete();
	//VoxelMarkerProgram.Delete();
	ShaderManager::GetShader("ShaderProgram").Delete();
	ShaderManager::GetShader("VoxelMarkerProgram").Delete();

	Window::GetInstance().DestroyWindow();

	glfwTerminate();
}