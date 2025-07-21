#include "Scene.h";
Scene::Scene() {
	std::cout << "Scene Created" << std::endl;
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

void Scene::render(Shader& shader, Shader& voxelMarkerShader) {
	//chunk.render(shader);

	world.update();
	world.render(shader);
	world.voxelMarker.render(voxelMarkerShader, world.voxelHandler.voxelWorldPos);

	/*if (world.voxelHandler.voxelId) {
		world.voxelMarker.render(voxelMarkerShader, world.voxelHandler.voxelWorldPos);
	}*/
	//chunk.Draw(shader);

}