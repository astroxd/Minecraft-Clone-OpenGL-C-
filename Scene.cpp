#include "Scene.h";

Scene::Scene() {
}

void Scene::setMesh() {
	//Chunk.setData(vertices);
}


void Scene::setShader(Shader& shader) {
	world.setShader(shader);
	//world.window = window;
	//world.second_context = second_context;
}

void Scene::setCamera(Camera* camera) {
	world.setCamera(camera, window);
}

void Scene::render(Shader& shader) {
	//chunk.render(shader);
	world.update();
	world.render(shader);
	//chunk.Draw(shader);
}