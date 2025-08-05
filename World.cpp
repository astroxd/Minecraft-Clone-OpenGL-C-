#include "World.h"
#include "Log.h"

#include <cmath>

using namespace std::chrono_literals;

World::World() {
	LOG_INFO("World Created");
	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noise.SetSeed(122422);
	chunks.reserve(24 * 24);


	//SPAWN A SINGLE THREAD FOR HANDLING WORLD CREATION
	chunkThread = std::thread(&World::UpdateChunkThread, this);
	isThreadRunning = true;
};


World::~World() {
	isThreadRunning = false;
	chunkThread.join();

}


void World::setCamera(Camera* camera) {
	World::camera = camera;
	voxelHandler.init(camera);
}

void World::render() {
	//std::unique_lock<std::mutex> lock(chunkMutex);
	auto it = chunks.begin();

	while (it != chunks.end()) {
		if (it->second->isBuilt) {

			it->second->Render(camera);
		}
		++it;
	}
	//lock.unlock();
}




void World::updateChunks() {
	std::unique_lock<std::mutex> lock(chunkMutex);

	int x = (static_cast<int>(camera->Position.x) / CHUNK_W);
	int y = (static_cast<int>(camera->Position.y) / CHUNK_H);
	int z = (static_cast<int>(camera->Position.z) / CHUNK_D);

	int distance = 12;
	if (ChunkLoadList.empty()) {


		for (int xx = -distance; xx < distance; xx++)
		{
			for (int zz = -distance; zz < distance; zz++)
			{

				ChunkCoord coord = Chunk::GetChunkCoordFromWorldCoord(x + xx, z + zz);

				if (ChunkExists(coord)) {

					if (chunks[coord]->isLoaded) continue;

					if (chunks[coord]->isBuilt && !chunks[coord]->forceRebuid) {
						continue;
					}

					if (!chunks[coord]->isMeshed || chunks[coord]->forceRebuid) {
						ChunkLoadList.push_back(coord);
						chunks[coord]->isLoaded = true;

					}
				}
				else {
					ChunkLoadList.push_back(coord);
					chunks[coord] = std::make_shared<Chunk>(coord, glm::vec3(x + xx, 0, z + zz), &noise);
					chunks[coord]->isLoaded = true;

				}
			}
		}
	}
	LOG_ERROR("LOAD LIST {0}", ChunkLoadList.size());


	lock.unlock();

	loadChunks();
	deleteChunks();

}

void World::UpdateChunkThread() {
	while (isThreadRunning) {

		std::unique_lock<std::mutex> lock(chunkMutex);

		for (auto& chunkCoord : ChunkLoadList) {
			if (chunks.at(chunkCoord) == nullptr) {
				ChunkLoadList.pop_back();
				continue;
			}
			if (chunks[chunkCoord]->isMeshed) {
				ChunkRenderList.push_back(chunkCoord);
				ChunkLoadList.pop_back();
			}
		}

		for (auto& chunkCoord : ChunkLoadList)
		{
			if (chunks.at(chunkCoord) == nullptr) {
				ChunkLoadList.pop_back();
				continue;
			}
			if (chunks[chunkCoord]->isMeshing) continue;

			if (chunks[chunkCoord]->forceRebuid) {

				//chunks[chunkCoord]->isBuilt = false;
				//chunks[chunkCoord]->isMeshed = false;
			}



			const std::pair<int, int> xPos = Chunk::GetChunkCoordFromWorldCoord(chunkCoord.first + 1, chunkCoord.second);
			const std::pair<int, int> xNeg = Chunk::GetChunkCoordFromWorldCoord(chunkCoord.first - 1, chunkCoord.second);
			const std::pair<int, int> zPos = Chunk::GetChunkCoordFromWorldCoord(chunkCoord.first, chunkCoord.second + 1);
			const std::pair<int, int> zNeg = Chunk::GetChunkCoordFromWorldCoord(chunkCoord.first, chunkCoord.second - 1);


			if (!chunks[chunkCoord]->forceRebuid) {
				//SHOULD REBUILD ADJACENT CHUNKS
				//isBuilt = false is commented so the chunk is still rendered when rebuilding rather than 
				//disappearing and reappearing 
				if (ChunkExists(xPos)) {
					chunks[xPos]->forceRebuid = true;
					chunks[xPos]->Reset();
				}
				if (ChunkExists(xNeg)) {
					chunks[xNeg]->forceRebuid = true;
					chunks[xNeg]->Reset();
				}
				if (ChunkExists(zPos)) {
					chunks[zPos]->forceRebuid = true;
					chunks[zPos]->Reset();
				}
				if (ChunkExists(zNeg)) {
					chunks[zNeg]->forceRebuid = true;
					chunks[zNeg]->Reset();
				}

			}

			chunks[chunkCoord]->forceRebuid = false;


			chunks[chunkCoord]->isMeshing = true;
			chunks[chunkCoord]->SetWorldChunks(&chunks);

			lock.unlock();
			chunks[chunkCoord]->GenerateChunk();
			chunks[chunkCoord]->isMeshed = true;
			lock.lock();
		}

		lock.unlock();
		//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

void World::loadChunks()
{
	LOG_WARN("RENDER LIST: {0}", ChunkRenderList.size());

	std::unique_lock<std::mutex> lock(chunkMutex);
	for (auto& chunkCoord : ChunkRenderList)
	{
		if (chunks[chunkCoord]->isMeshed) {


			ChunkRenderList.pop_back();

			chunks[chunkCoord]->setVAO();
			chunks[chunkCoord]->isBuilt = true;
		}
	}
	lock.unlock();
}

void World::deleteChunks() {

	int x = (static_cast<int>(camera->Position.x) / CHUNK_W);
	int y = (static_cast<int>(camera->Position.y) / CHUNK_H);
	int z = (static_cast<int>(camera->Position.z) / CHUNK_D);

	//Distance Should be greater than loading distance to prevent bugs
	int distance = 16;

	std::unique_lock<std::mutex> lock(chunkMutex);

	ChunkUnloadList.clear();
	for (auto it = chunks.begin(); it != chunks.end(); it++) {
		if (abs(it->second->GetChunkCoord().first - x) > distance || abs(it->second->GetChunkCoord().second - z) > distance) {
			ChunkUnloadList.push_back(it->second->GetChunkCoord());
		}
	}

	for (auto& coord : ChunkUnloadList) {
		LOG_INFO(chunks[coord].use_count());
		//chunks[coord]->~Chunk();
		chunks.erase(coord);
	}
	lock.unlock();
}


void World::update() {
	updateChunks();

	if (m_cameraPosition != camera->Position || m_cameraOrientation != camera->Front) {
		voxelHandler.update(&chunks);

	}
	voxelHandler.input();

	m_cameraPosition = camera->Position;
	m_cameraOrientation = camera->Front;
}



bool World::ChunkExists(const ChunkCoord chunkCoord) {
	return chunks.find(chunkCoord) != chunks.end();
}
