#include "World.h"
#include <cmath>

#include <thread>
#include <future>

using namespace std::chrono_literals;

void World::setShader(Shader& shader) {
	World::shader = shader;
	//buildChunks();
}

void World::setCamera(Camera* camera, GLFWwindow* window) {
	World::camera = camera;
	World::window = window;
	voxelHandler.init(camera);
}


void World::buildChunks() {

	for (int x = 0; x < 4; x++)
	{
		for (int z = 0; z < 4; z++)
		{
			for (int y = 0; y < 1; y++)
			{
				ChunkCoord coord = MyChunk::getChunkCoordFromWorldCoord(x,z);
				chunks[coord] = std::make_unique<MyChunk>(coord, glm::vec3(x, y, z), &noise);
			}
		}
	}

	auto it = chunks.begin();
	while (it != chunks.end()) {
		it->second->setWorldChunks(&chunks);
		it->second->setVAO();
		++it;
	}
}

void World::render(Shader& shader) {
	auto it = chunks.begin();

	while (it != chunks.end()) {
		it->second->render(shader, camera);
		++it;
	}
}



/*void showChunk(std::pair<int, int> chunkCoord, std::map<std::pair<int, int>, std::unique_ptr<MyChunk>>* chunks, Shader* shader) {
	(*chunks)[chunkCoord]->setVAO();
	(*chunks)[chunkCoord]->render(*shader);
}*/

//std::function<void(std::pair<int, int>, std::map<std::pair<int, int>, std::unique_ptr<MyChunk>>*, Shader*)> func
bool buildNewChunks(
	ChunkCoord newChunksCoords,
	ChunkUnorderedMap<ChunkCoord, std::unique_ptr<MyChunk>>*chunks)
{
	
	(*chunks)[newChunksCoords]->isVisible = true;
	(*chunks)[newChunksCoords]->setWorldChunks(chunks);
	return true;
}



void World::updateChunks() {
	//MAYBE FLOOR INSTEAD OF STATIC CAST
	int x = (static_cast<int>(camera->Position.x) / CHUNK_W);
	int y = (static_cast<int>(camera->Position.y) / CHUNK_H);
	int z = (static_cast<int>(camera->Position.z) / CHUNK_D);

	std::vector<ChunkCoord> newChunksCoords;


	int distance = 6;

	for (int xx = -distance; xx < distance; xx++)
	{
		for (int zz = -distance; zz < distance; zz++)
		{
			if (std::abs(xx) + std::abs(zz) > distance) continue;

			ChunkCoord coord = MyChunk::getChunkCoordFromWorldCoord( x + xx, z + zz );

			if (chunks.count(coord) > 0 && chunks[coord]->isBuilt) {
				chunks[coord]->isVisible = true;
				continue;
			}

			newChunksCoords.push_back(coord);

			chunks[coord] = std::make_unique<MyChunk>(coord, glm::vec3(x + xx, 0, z + zz), &noise);
			if (chunks.count(MyChunk::getChunkCoordFromWorldCoord( x + xx + 1 ,z + zz )) > 0) {
				//newChunksCoords.push_back({ xCoord + 1, zCoord });
			}
			if (chunks.count(MyChunk::getChunkCoordFromWorldCoord( x + xx - 1 ,z + zz )) > 0) {
				//newChunksCoords.push_back({ xCoord - 1, zCoord });
			}
			if (chunks.count(MyChunk::getChunkCoordFromWorldCoord( x + xx, z + zz + 1 )) > 0) {
				//newChunksCoords.push_back({ xCoord, zCoord + 1 });
			}
			if (chunks.count(MyChunk::getChunkCoordFromWorldCoord( x + xx, z + zz - 1 )) > 0) {
				//newChunksCoords.push_back({ xCoord, zCoord - 1 });
			}
		}
	}
	const int MAXCHUNKPERFRAME = 5;
	int counter = 0;

	for (auto& chunkCoord : newChunksCoords) {
		//if (counter > MAXCHUNKPERFRAME) return;
		
		std::future<bool> fut = std::async(std::launch::async, buildNewChunks, chunkCoord, &chunks);
		//std::chrono::milliseconds span(100);
		//while (fut.wait_for(span) == std::future_status::timeout) {}
		if (fut.get()) {
			chunks[chunkCoord]->setVAO();
			chunks[chunkCoord]->isBuilt = true;
		}
		//glfwMakeContextCurrent(window);
		
		//counter++;

	}

	//! THREAD VERSION
	/*for (auto& pair : newChunksCoords) {
		std::thread t(buildNewChunks, pair, &chunks, &shader);
		if (t.joinable()) {
			t.join();
			chunks[pair]->setVAO();
			//chunks[pair]->render(shader);
		}
	}*/


	deleteChunks();
}




void World::deleteChunks() {

	std::vector<ChunkCoord> unloadedChunks = {};

	auto it = chunks.begin();

	while (it != chunks.end()) {
		if (!it->second->isVisible) {
			unloadedChunks.push_back(it->first);
		}
		else {
			it->second->isVisible = false;
		}
		++it;
	}


	for (auto& chunkCoord : unloadedChunks) {
		chunks.erase(chunkCoord);
	}


}

void World::update() {
	updateChunks();
	
	if (m_cameraPosition != camera->Position || m_cameraOrientation != camera->Orientation) {
		voxelHandler.update(&chunks);
		
	}
	voxelHandler.input(window);
	m_cameraPosition = camera->Position;
	m_cameraOrientation = camera->Orientation;


	

	
}




