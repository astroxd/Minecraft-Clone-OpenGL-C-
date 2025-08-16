#ifndef WORLD_H
#define WORLD_H

#include "ShaderManager.h"
#include "Chunk.h"
#include "VoxelHandler.h"
#include "VoxelMarker.h"

#include <deque>
#include <thread>
#include <mutex>
#include "ChunkBorder.h"
#include "TextureAtlas.h"

//struct AllocationMetrics {
//	uint32_t TotalAllocated = 0;
//	uint32_t TotalFreed = 0;
//
//	uint32_t CurrentUsage() { return TotalAllocated - TotalFreed; }
//};
//
//
//static AllocationMetrics s_AllocationMetrics;
//
//static void PrintMemoryUsage() {
//	std::cout << "Memory usage: " << s_AllocationMetrics.CurrentUsage() << std::endl;
//}


class World {
public:


	ChunkUnorderedMap<ChunkCoord, std::shared_ptr<Chunk>> chunks = {};

	Camera* camera;
	TextureAtlas* texture;

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraOrientation;

	FastNoiseLite* noise;

	VoxelHandler* voxelHandler;
	VoxelMarker* voxelMarker;

	ChunkBorder ChunkBorder;


	std::deque<ChunkCoord> ChunkLoadList;
	std::deque<ChunkCoord> ChunkRenderList;
	std::deque<ChunkCoord> ChunkUnloadList;

	World() = default;
	World(Camera* camera);

	~World();
	void setCamera(Camera* camera);


	void update();
	void render();

	void updateChunks();
	void loadChunks();
	void deleteChunks();


	void UpdateChunkThread();

private:
	std::thread chunkThread;
	std::mutex chunkMutex;
	bool isThreadRunning = false;

	void LoadTexture();

	bool ChunkExists(const ChunkCoord chunkCoord);
};



#endif;
