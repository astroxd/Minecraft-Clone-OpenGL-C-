#include "World.h"
#include <cmath>

#include <thread>
#include <future>

#include "Log.h"

using namespace std::chrono_literals;

void World::setCamera(Camera* camera) {
	World::camera = camera;
	voxelHandler.init(camera);
}

void World::buildChunks() {

	for (int x = 0; x < 4; x++)
	{
		for (int z = 0; z < 4; z++)
		{
			for (int y = 0; y < 1; y++)
			{
				ChunkCoord coord = MyChunk::getChunkCoordFromWorldCoord(x, z);
				chunks[coord] = std::make_shared<MyChunk>(coord, glm::vec3(x, y, z), &noise);
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

void World::render() {
	auto it = chunks.begin();

	while (it != chunks.end()) {
		it->second->render(camera);
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
	ChunkUnorderedMap<ChunkCoord, std::shared_ptr<MyChunk>>* chunks)
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
	ChunkLoadList.clear();

	int distance = 12;

	for (int xx = -distance; xx < distance; xx++)
	{
		for (int zz = -distance; zz < distance; zz++)
		{
			//if (std::abs(xx) + std::abs(zz) > distance) continue;

			ChunkCoord coord = MyChunk::getChunkCoordFromWorldCoord(x + xx, z + zz);

			if (ChunkExists(coord)) {


				if (chunks[coord]->isBuilt && !chunks[coord]->forceRebuid) {
					chunks[coord]->isVisible = true;
					continue;
				}

				if (!chunks[coord]->isMeshed || chunks[coord]->forceRebuid) {
					ChunkLoadList.push_back(coord);
				}
			}
			else {
				ChunkLoadList.push_back(coord);
				chunks[coord] = std::make_shared<MyChunk>(coord, glm::vec3(x + xx, 0, z + zz), &noise);
			}
		}
	}

	loadChunks();


	//deleteChunks();
}

std::mutex mtx;

void testThread(std::vector<ChunkCoord> ChunkLoadList,
	ChunkUnorderedMap<ChunkCoord, std::shared_ptr<MyChunk>>* chunks,
	std::vector<ChunkCoord>* ChunkRenderList) {
	//std::lock_guard<std::mutex> lock(mtx);

	const int MAXCHUNKPERFRAME = 6;
	int counter = 0;

	ChunkRenderList->clear();

	//for (auto& chunkCoord : ChunkLoadList) 
	for (size_t i = 0; i < ChunkLoadList.size(); i++)
	{
		if (counter > MAXCHUNKPERFRAME) return;

		auto& chunkCoord = ChunkLoadList.at(i);
		ChunkLoadList.erase(ChunkLoadList.begin() + i);

		(*chunks)[chunkCoord]->setWorldChunks(chunks);
		(*chunks)[chunkCoord]->isBuilt = false;
		(*chunks)[chunkCoord]->isVisible = false;
		(*chunks)[chunkCoord]->isMeshed = true;


		if (!(*chunks)[chunkCoord]->forceRebuid) {
			//SHOULD REBUILD ADJACENT CHUNKS
			if (chunks->find({ chunkCoord.first + 1, chunkCoord.second }) != chunks->end()) {
				(*chunks)[{ chunkCoord.first + 1, chunkCoord.second }]->forceRebuid = true;
			}
			if (chunks->find({ chunkCoord.first - 1, chunkCoord.second }) != chunks->end()) {
				(*chunks)[{ chunkCoord.first - 1, chunkCoord.second }]->forceRebuid = true;

			}
			if (chunks->find({ chunkCoord.first, chunkCoord.second + 1 }) != chunks->end()) {
				(*chunks)[{ chunkCoord.first, chunkCoord.second + 1 }]->forceRebuid = true;

			}
			if (chunks->find({ chunkCoord.first, chunkCoord.second - 1 }) != chunks->end()) {
				(*chunks)[{ chunkCoord.first, chunkCoord.second - 1 }]->forceRebuid = true;
			}

		}

		(*chunks)[chunkCoord]->forceRebuid = false;
		counter++;
		ChunkRenderList->push_back(chunkCoord);

	}
}

void World::loadChunks()
{
	const int MAXCHUNKPERFRAME = 10;
	int counter = 0;

	//LOG_WARN("Load Queue: {0}", ChunkLoadList.size());
	std::thread t(testThread, ChunkLoadList, &chunks, &ChunkRenderList);

	if (t.joinable()) {

		t.join();


		//LOG_WARN("join");
		//LOG_WARN("RENDER LIST: {0}", ChunkRenderList.size());
		//for (auto& chunkCoord : ChunkRenderList) 
		for (size_t i = 0; i < ChunkRenderList.size(); i++)
		{
			//if (counter > MAXCHUNKPERFRAME) return;
			auto& chunkCoord = ChunkRenderList.at(i);
			//ChunkRenderList.erase(ChunkRenderList.begin() + i);

			chunks[chunkCoord]->setVAO();
			chunks[chunkCoord]->isBuilt = true;
			chunks[chunkCoord]->isVisible = true;

			//counter++;
		}

	}
	deleteChunks();
}

void deleteThread(ChunkUnorderedMap<ChunkCoord, std::shared_ptr<MyChunk>>* chunks,
	std::vector<ChunkCoord>* ChunkUnloadList,
	ChunkUnorderedMap<ChunkCoord, std::shared_ptr<MyChunk>>* unsafeChunks
) {
	//std::lock_guard<std::mutex> lock(mtx);

	ChunkUnloadList->clear();
	*unsafeChunks = *chunks;

	auto it = unsafeChunks->begin();

	while (it != unsafeChunks->end()) {
		if (!it->second->isVisible && !it->second->forceRebuid) {
			ChunkUnloadList->push_back(it->first);
		}
		else {
			it->second->isVisible = false;
		}
		++it;
	}

	for (auto& chunkCoord : *ChunkUnloadList) {
		unsafeChunks->erase(chunkCoord);
	}

}

void World::deleteChunks() {
	std::thread t(deleteThread, &chunks, &ChunkUnloadList, &unsafeChunks);
	if (t.joinable()) {
		t.join();
		chunks = unsafeChunks;
	}

	/*for (auto& chunkCoord : ChunkUnloadList) {
		chunks.erase(chunkCoord);
	}*/
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
