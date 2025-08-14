#include "World.h"
#include "Log.h"

#include <cmath>

using namespace std::chrono_literals;

//void* operator new(size_t size) {
//	s_AllocationMetrics.TotalAllocated += size;
//	return malloc(size);
//}
//
//void operator delete(void* memory, size_t size) {
//	s_AllocationMetrics.TotalFreed += size;
//	free(memory);
//}


World::World(Camera* camera)
	: camera(camera)
{
	LOG_INFO("World Created");

	noise = new FastNoiseLite(122422);
	noise->SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	chunks.reserve(24 * 24);

	voxelHandler = new VoxelHandler(camera);
	voxelMarker = new VoxelMarker(voxelHandler);
	ChunkBorder.Init(camera);

	LoadTexture();

	//TODO replace with threadPool
	//SPAWN A SINGLE THREAD FOR HANDLING WORLD CREATION
	chunkThread = std::thread(&World::UpdateChunkThread, this);
	isThreadRunning = true;
};


World::~World() {
	isThreadRunning = false;
	chunkThread.join();

	delete noise;
	delete voxelMarker;
	delete voxelHandler;
	delete texture;

}


void World::setCamera(Camera* camera) {
	World::camera = camera;
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
	ChunkBorder.Render();
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
					chunks[coord] = std::make_shared<Chunk>(coord, glm::vec3(x + xx, 0, z + zz), noise);
					chunks[coord]->isLoaded = true;

				}
			}
		}
	}
	//LOG_ERROR("LOAD LIST {0}", ChunkLoadList.size());


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



			const ChunkCoord xPos = Chunk::GetChunkCoordFromWorldCoord(chunkCoord.x + 1, chunkCoord.y);
			const ChunkCoord xNeg = Chunk::GetChunkCoordFromWorldCoord(chunkCoord.x - 1, chunkCoord.y);
			const ChunkCoord zPos = Chunk::GetChunkCoordFromWorldCoord(chunkCoord.x, chunkCoord.y + 1);
			const ChunkCoord zNeg = Chunk::GetChunkCoordFromWorldCoord(chunkCoord.x, chunkCoord.y - 1);


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
	//LOG_WARN("RENDER LIST: {0}", ChunkRenderList.size());

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
		if (abs(it->second->GetChunkCoord().x - x) > distance || abs(it->second->GetChunkCoord().y - z) > distance) {
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

	if (m_cameraPosition != camera->Position) {
		voxelHandler->UpdateChunks(&chunks);
	}

	voxelHandler->RayCast();

	m_cameraPosition = camera->Position;
	m_cameraOrientation = camera->Front;

	ChunkBorder.Update();
}


void World::LoadTexture() {
	texture = new Texture("atlas.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	Shader shader = ShaderManager::GetShader("ShaderProgram");
	shader.Activate();
	shader.SetInt("tex0", 0);
	texture->Bind();
}


bool World::ChunkExists(const ChunkCoord chunkCoord) {
	return chunks.find(chunkCoord) != chunks.end();
}
