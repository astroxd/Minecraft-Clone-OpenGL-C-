#include "Scene.h";
#include "Log.h";
#include "ShaderManager.h"

Scene::Scene() {
	LOG_INFO("Scene Created");
}

void Scene::setMesh() {
	//Chunk.setData(vertices);
}

void Scene::setCamera(Camera* camera) {
	world.setCamera(camera);
}

void Scene::render() {
	//chunk.render(shader);

	world.update();
	world.render();
	world.voxelMarker.render(world.voxelHandler.voxelWorldPos);

	/*if (world.voxelHandler.voxelId) {
		world.voxelMarker.render(voxelMarkerShader, world.voxelHandler.voxelWorldPos);
	}*/
	//chunk.Draw(shader);

}