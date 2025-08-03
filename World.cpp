#include "World.h"
#include <cmath>

//#include <future>

#include "Log.h"


using namespace std::chrono_literals;

World::World() {
	LOG_INFO("World Created");
	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noise.SetSeed(122422);
	chunks.reserve(13 * 13);


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

//void World::buildChunks() {
//	for (int x = 0; x < 4; x++)
//	{
//		for (int z = 0; z < 4; z++)
//		{
//			for (int y = 0; y < 1; y++)
//			{
//				ChunkCoord coord = MyChunk::getChunkCoordFromWorldCoord(x, z);
//				chunks[coord] = std::make_shared<MyChunk>(coord, glm::vec3(x, y, z), &noise);
//			}
//		}
//	}
//
//	auto it = chunks.begin();
//	while (it != chunks.end()) {
//		it->second->setWorldChunks(&chunks);
//		it->second->setVAO();
//		++it;
//	}
//}



void World::render() {
	//std::unique_lock<std::mutex> lock(chunkMutex);
	auto it = chunks.begin();

	while (it != chunks.end()) {
		if (it->second->isBuilt) {

			it->second->render(camera);
		}
		++it;
	}
	//lock.unlock();
}




void World::updateChunks() {
	std::unique_lock<std::mutex> lock(chunkMutex);

	//ChunkLoadList.clear();

	//MAYBE FLOOR INSTEAD OF STATIC CAST
	int x = (static_cast<int>(camera->Position.x) / CHUNK_W);
	int y = (static_cast<int>(camera->Position.y) / CHUNK_H);
	int z = (static_cast<int>(camera->Position.z) / CHUNK_D);

	int distance = 12;
	if (ChunkLoadList.empty()) {


		for (int xx = -distance; xx < distance; xx++)
		{
			for (int zz = -distance; zz < distance; zz++)
			{
				//if (std::abs(xx) + std::abs(zz) > distance) continue;

				ChunkCoord coord = MyChunk::getChunkCoordFromWorldCoord(x + xx, z + zz);

				if (ChunkExists(coord)) {

					if (chunks[coord]->isLoaded) continue;

					if (chunks[coord]->isBuilt && !chunks[coord]->forceRebuid) {
						//chunks[coord]->isVisible = true;
						continue;
					}


					/*	if (!chunks[coord]->isMeshed) {
							ChunkLoadList.push_back(coord);
						}*/
					if (!chunks[coord]->isMeshed || chunks[coord]->forceRebuid) {
						ChunkLoadList.push_back(coord);
						chunks[coord]->isLoaded = true;

					}
				}
				else {
					ChunkLoadList.push_back(coord);
					chunks[coord] = std::make_shared<MyChunk>(coord, glm::vec3(x + xx, 0, z + zz), &noise);
					chunks[coord]->isLoaded = true;

				}
			}
		}
	}
	LOG_ERROR("LOAD LIST {0}", ChunkLoadList.size());


	lock.unlock();


	//ThreadInsert();
	//loadChunks();
	//deleteChunks();

	loadChunks();
	deleteChunks();

}

void World::UpdateChunkThread() {
	while (isThreadRunning) {




		std::unique_lock<std::mutex> lock(chunkMutex);
		const int MAXCHUNKPERFRAME = 6;
		int counter = 0;

		const auto SIZE = ChunkLoadList.size();

		//LOG_WARN("Load LIST: {0}", ChunkLoadList.size());
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
			//if (counter > MAXCHUNKPERFRAME) break;
			//std::cout << "Load Queue IN THREAD: " << ChunkLoadList.size() << " " << SIZE << std::endl;

			//auto& chunkCoord = ChunkLoadList->at(i);
			//ChunkLoadList->erase(ChunkLoadList->begin() + i);

			//ChunkLoadList.pop_back();
			if (chunks[chunkCoord]->forceRebuid) {

				//chunks[chunkCoord]->isBuilt = false;
				//chunks[chunkCoord]->isMeshed = false;
			}
			//chunks[chunkCoord]->isVisible = false;





			if (!chunks[chunkCoord]->forceRebuid) {
				//SHOULD REBUILD ADJACENT CHUNKS
				//isBuilt = false is commented so the chunk is still rendered when rebuilding rather than 
				//disappearing and reappearing 
				if (chunks.find({ chunkCoord.first + 1, chunkCoord.second }) != chunks.end()) {
					chunks[{ chunkCoord.first + 1, chunkCoord.second }]->forceRebuid = true;
					chunks[{ chunkCoord.first + 1, chunkCoord.second }]->isMeshed = false;
					chunks[{ chunkCoord.first + 1, chunkCoord.second }]->isLoaded = false;
					//chunks[{ chunkCoord.first + 1, chunkCoord.second }]->isBuilt = false;
					chunks[{ chunkCoord.first + 1, chunkCoord.second }]->isMeshing = false;
				}
				if (chunks.find({ chunkCoord.first - 1, chunkCoord.second }) != chunks.end()) {
					chunks[{ chunkCoord.first - 1, chunkCoord.second }]->forceRebuid = true;
					chunks[{ chunkCoord.first - 1, chunkCoord.second }]->isMeshed = false;
					chunks[{ chunkCoord.first - 1, chunkCoord.second }]->isLoaded = false;
					//chunks[{ chunkCoord.first - 1, chunkCoord.second }]->isBuilt = false;
					chunks[{ chunkCoord.first - 1, chunkCoord.second }]->isMeshing = false;

				}
				if (chunks.find({ chunkCoord.first, chunkCoord.second + 1 }) != chunks.end()) {
					chunks[{ chunkCoord.first, chunkCoord.second + 1 }]->forceRebuid = true;
					chunks[{ chunkCoord.first, chunkCoord.second + 1 }]->isMeshed = false;
					chunks[{ chunkCoord.first, chunkCoord.second + 1 }]->isLoaded = false;
					//chunks[{ chunkCoord.first, chunkCoord.second + 1 }]->isBuilt = false;
					chunks[{ chunkCoord.first, chunkCoord.second + 1 }]->isMeshing = false;

				}
				if (chunks.find({ chunkCoord.first, chunkCoord.second - 1 }) != chunks.end()) {
					chunks[{ chunkCoord.first, chunkCoord.second - 1 }]->forceRebuid = true;
					chunks[{ chunkCoord.first, chunkCoord.second - 1 }]->isMeshed = false;
					chunks[{ chunkCoord.first, chunkCoord.second - 1 }]->isLoaded = false;
					//chunks[{ chunkCoord.first, chunkCoord.second - 1 }]->isBuilt = false;
					chunks[{ chunkCoord.first, chunkCoord.second - 1 }]->isMeshing = false;
				}

			}

			chunks[chunkCoord]->forceRebuid = false;
			counter++;
			//if (std::find(ChunkRenderList.begin(), ChunkRenderList.end(), chunkCoord) == ChunkRenderList.end()) {

			//ChunkRenderList.push_back(chunkCoord);
			//}

			//!? FA LAGGAREEEEEEEEE
			//! PER NON LAGGARE IL CHUNK DEVE ESSERE CREATO FUORI DAL LOCK

			chunks[chunkCoord]->isMeshing = true;
			chunks[chunkCoord]->setWorldChunks(&chunks);

			lock.unlock();
			chunks[chunkCoord]->generateChunk();
			chunks[chunkCoord]->isMeshed = true;

			lock.lock();
		}






		//*chunks = *unsafeChunks;
		lock.unlock();
		//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}




void testThread(std::deque<ChunkCoord>* ChunkLoadList,
	ChunkUnorderedMap<ChunkCoord, std::shared_ptr<MyChunk>>* chunks,
	std::deque<ChunkCoord>* ChunkRenderList,
	Camera* camera,
	FastNoiseLite* noise,
	ChunkUnorderedMap<ChunkCoord, std::shared_ptr<MyChunk>>* unsafeChunks) {

	////std::cout << "BEFORE LOCK" << std::endl;
	//std::unique_lock<std::mutex> lock(mtx);

	////*unsafeChunks = *chunks;

	//const int MAXCHUNKPERFRAME = 6;
	//int counter = 0;

	////ChunkRenderList->clear();
	//const auto SIZE = ChunkLoadList->size();

	//LOG_WARN("Load LIST: {0}", ChunkLoadList->size());


	//for (auto& chunkCoord : *ChunkLoadList)
	//{
	//	//if (counter > MAXCHUNKPERFRAME) return;
	//	std::cout << "Load Queue IN THREAD: " << ChunkLoadList->size() << " " << SIZE << std::endl;

	//	//auto& chunkCoord = ChunkLoadList->at(i);
	//	//ChunkLoadList->erase(ChunkLoadList->begin() + i);
	//	ChunkLoadList->pop_back();
	//	std::cout << "UNSAFE SIZE: " << unsafeChunks->size() << std::endl;
	//	(*chunks)[chunkCoord]->setWorldChunks(chunks);
	//	(*chunks)[chunkCoord]->isBuilt = false;
	//	(*chunks)[chunkCoord]->isVisible = false;
	//	(*chunks)[chunkCoord]->isMeshed = true;


	//	if (!(*chunks)[chunkCoord]->forceRebuid) {
	//		//SHOULD REBUILD ADJACENT CHUNKS
	//		if (chunks->find({ chunkCoord.first + 1, chunkCoord.second }) != chunks->end()) {
	//			(*chunks)[{ chunkCoord.first + 1, chunkCoord.second }]->forceRebuid = true;
	//			(*chunks)[{ chunkCoord.first + 1, chunkCoord.second }]->isMeshed = false;
	//		}
	//		if (chunks->find({ chunkCoord.first - 1, chunkCoord.second }) != chunks->end()) {
	//			(*chunks)[{ chunkCoord.first - 1, chunkCoord.second }]->forceRebuid = true;
	//			(*chunks)[{ chunkCoord.first - 1, chunkCoord.second }]->isMeshed = false;

	//		}
	//		if (chunks->find({ chunkCoord.first, chunkCoord.second + 1 }) != chunks->end()) {
	//			(*chunks)[{ chunkCoord.first, chunkCoord.second + 1 }]->forceRebuid = true;
	//			(*chunks)[{ chunkCoord.first, chunkCoord.second + 1 }]->isMeshed = false;

	//		}
	//		if (chunks->find({ chunkCoord.first, chunkCoord.second - 1 }) != chunks->end()) {
	//			(*chunks)[{ chunkCoord.first, chunkCoord.second - 1 }]->forceRebuid = true;
	//			(*chunks)[{ chunkCoord.first, chunkCoord.second - 1 }]->isMeshed = false;
	//		}

	//	}

	//	(*chunks)[chunkCoord]->forceRebuid = false;
	//	counter++;
	//	if (std::find(ChunkRenderList->begin(), ChunkRenderList->end(), chunkCoord) == ChunkRenderList->end()) {

	//		ChunkRenderList->push_back(chunkCoord);
	//	}

	//}
	////*chunks = *unsafeChunks;
	//lock.unlock();
}



void World::ThreadInsert() {

	if (!isThreadRunning) {
		isThreadRunning = true;
		//LOG_ERROR("RUNNING THREAD");
		LOG_ERROR("CHUNK BEFORE THREAD {0}", chunks.size());
		//chunkThread = std::thread(testThread, &ChunkLoadList, &chunks, &ChunkRenderList, camera, &noise, &unsafeChunks);

	}
	//LOG_ERROR("OUTSIDE THREAD");
	return;
}





void World::loadChunks()
{


	const int MAXCHUNKPERFRAME = 6;
	int counter = 0;


	LOG_WARN("RENDER LIST: {0}", ChunkRenderList.size());
	std::unique_lock<std::mutex> lock(chunkMutex);
	for (auto& chunkCoord : ChunkRenderList)
	{
		if (chunks[chunkCoord]->isMeshed) {


			ChunkRenderList.pop_back();

			chunks[chunkCoord]->setVAO();
			chunks[chunkCoord]->isBuilt = true;
			//chunks[chunkCoord]->isVisible = true;
			counter++;
		}
	}
	lock.unlock();
	//}


	//deleteChunks();
}

void deleteThread(ChunkUnorderedMap<ChunkCoord, std::shared_ptr<MyChunk>>* chunks,
	std::vector<ChunkCoord>* ChunkUnloadList,
	ChunkUnorderedMap<ChunkCoord, std::shared_ptr<MyChunk>>* unsafeChunks
) {
	//std::unique_lock<std::mutex> lock(mtx);

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

	int x = (static_cast<int>(camera->Position.x) / CHUNK_W);
	int y = (static_cast<int>(camera->Position.y) / CHUNK_H);
	int z = (static_cast<int>(camera->Position.z) / CHUNK_D);

	//Distance Should be greated than loading distance to prevent bugs
	int distance = 16;

	std::unique_lock<std::mutex> lock(chunkMutex);

	ChunkUnloadList.clear();
	for (auto it = chunks.begin(); it != chunks.end(); it++) {
		if (abs(it->second->coord.first - x) > distance || abs(it->second->coord.second - z) > distance) {
			ChunkUnloadList.push_back(it->second->coord);
		}
	}

	for (auto& coord : ChunkUnloadList) {
		LOG_INFO(chunks[coord].use_count());
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
