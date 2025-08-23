#include "Engine.h"
#include "ShaderManager.h"

Engine::Engine() {

	LOG_INFO("Engine Created");

	ShaderManager::AddShader("ShaderProgram", std::make_shared<Shader>("chunk.vert", "chunk.frag"));
	ShaderManager::AddShader("VoxelMarkerProgram", std::make_shared<Shader>("cube.vert", "cube.frag"));

	camera = new Camera(Window::GetInstance().getWidth(), Window::GetInstance().getHeight(), glm::vec3(0.0f, 20.0f, 0.0f));
	scene = new Scene(camera);

	glfwSetInputMode(Window::GetInstance().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(Window::GetInstance().GetWindow(), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init("#version 460");


	run();

}

Engine::~Engine()
{
	delete camera;
	delete scene;
}

void Engine::run() {
	while (!glfwWindowShouldClose(Window::GetInstance().GetWindow())) {
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Debug");
		ImGui::Text("This is some useful text.");


		//ImGui::ShowDemoWindow(); // Show demo window! :)



		currentTime = glfwGetTime();
		timeDiff = currentTime - prevTime;
		ImGui::Text("FPS: %.1f	ms: %.3f", FPS, ms);
		counter++;
		if (timeDiff >= 1.0 / 30.0) {
			FPS = (1.0 / timeDiff) * counter;
			ms = (timeDiff / counter) * 1000;

			glfwSetWindowTitle(Window::GetInstance().GetWindow(), (camera->GetCameraPosition() + "  " + camera->GetCameraOrientation()).c_str());
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

		scene->render();


		if (Input::isKeyPressed(Key::F))
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		if (Input::isKeyPressed(Key::G))
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}


		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(Window::GetInstance().GetWindow());
		glfwSwapBuffers(Window::GetInstance().GetSecondContext());
		glfwPollEvents();
	}

	LOG_INFO("Closing");

	ShaderManager::GetShader("ShaderProgram").Delete();
	ShaderManager::GetShader("VoxelMarkerProgram").Delete();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();


	Window::GetInstance().DestroyWindow();

	glfwTerminate();
}