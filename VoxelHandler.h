#ifndef VOXEL_HANDLER_H
#define VOXEL_HANDLER_H

#include "Chunk.h"
#include <chrono>

class VoxelHandler {
public:

	VoxelHandler() = default;
	VoxelHandler(Camera* camera);

	void UpdateChunks(ChunkUnorderedMap<ChunkCoord, std::shared_ptr<Chunk>>* chunks);

	void RayCast();

	inline int GetVoxelId() const {
		return m_VoxelId;
	}
	inline glm::vec3 GetVoxelWorldPos() const {
		return m_VoxelWorldPos;
	}

private:
	const int MAX_RAY_DIST = 6;
	const int DELAY_DESTROY_BLOCK = 100;
	const int DELAY_PLACE_BLOCK = 100;
	std::chrono::milliseconds m_LastDestroyed;
	std::chrono::milliseconds m_LastPlaced;


	Camera* m_Camera;
	ChunkUnorderedMap<ChunkCoord, std::shared_ptr<Chunk>>* m_Chunks;

	int m_VoxelId = 0;

	ChunkCoord m_ChunkCoord;
	glm::ivec3 m_VoxelLocalPosition;
	glm::vec3 m_VoxelNormal;
	glm::vec3 m_VoxelWorldPos;

private:

	void RayCasting();
	int GetHitVoxelId(glm::vec3 voxelWorldPos);

	void DestroyVoxel();
	void PlaceVoxel();
	void RebuildAdjacentChunk(AdjacentChunkPos pos);

	void Input();

};

#endif;
